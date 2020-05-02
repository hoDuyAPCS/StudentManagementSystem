#include "SMS_header.h"
userList::userList(int t) {
	ifstream read;
	if (t == 0) read.open("Login\\Login_type0.csv");
	else {
		read.open("Login\\Login_type1_2.csv");
		type = 1; //change type
	}
	if (!read.is_open()) return;
	else {
		user*tmp;
		user*cur = head;
		string dummy;//dummy variable to check if file is still readable
		while (getline(read, dummy, ',')) {
			tmp = new user;
			tmp->username = dummy;
			getline(read, tmp->password, ',');
			getline(read, tmp->fullname, ',');
			getline(read, tmp->email, ',');
			getline(read, tmp->phoneNum, ',');
			getline(read, tmp->class_, ','); //class for lecturers: LEC, class for staffs: STF
			getline(read, tmp->flag);
			if (head == NULL) {
				head = tmp;
				cur = head;
			}
			else {
				cur->next = tmp;
				cur = cur->next;
			}
		}
	}
	read.close();
}
void userList::changePasw(user*& a) {
	//not sure that this case can happen
	//but hey! it's good to be safe than sorry
	if (a == NULL)return;
	bool pass = false;
	string newPsw;
	//uninitialized password
	while (!pass) {
		system("CLS");
		if (a->flag == "0") {
			cout << "You need to change to a new password!" << endl;
		}
		cout << "Please enter a new password!: ";
		getline(cin, newPsw);
		cout << "Please re enter the password: ";
		string reEnter; getline(cin, reEnter);
		if (reEnter != newPsw) cout << "re confirm is not exact!" << endl;
		else {
			if (newPsw.size() <= 8) cout << "The password's length need to be over 8 characters!" << endl;
			else {
				pass = true;
				cout << "Change password successful!!" << endl;
			}
		}
		system("pause");
	}
	a->password = newPsw;
	a->flag = "1";//set flag
	update(head);
}
userList::~userList() {
	//put this here just in case
	if (head == NULL)return;
	//destructor: update the file
	ofstream write;
	if (type == 0) write.open("Login\\Login_type0.csv");
	else write.open("Login\\Login_type1_2.csv");
	user*cur = head;
	while (cur != NULL) {
		write << cur->username << ",";
		write << cur->password << ",";
		write << cur->fullname << ",";
		write << cur->email << ",";
		write << cur->phoneNum << ",";
		write << cur->class_ << ",";
		write << cur->flag << endl;
		cur = cur->next;
	}
	write.close();
	//release the memory
	user*step = head;
	while (step != NULL) {
		user*tmp = cur;
		step = step->next; //move to next user memory block
		delete tmp;
	}
}
student* studentList::find(string name) {
	student*cur = head;
	while (cur != NULL) {
		if (cur->name == name) return cur;
		else cur = cur->next;
	}
	return NULL;
}

/*void tmpFunc() {
	system("CLS");
	cout << "THIS IS AN EMPTY FUNCTION!" << endl;
	system("pause");
}*/
void spacing(int n = 10) {
	for (int i = 0; i < n; i++) cout << " ";
}
void Tupper(string &a) {
	//change to upper case
	int n = a.size();
	for (int i = 0; i < n; i++) {
		if (int(a[i]) > 97 && int(a[i]) <= 122) a[i] -= 32;
	}
}
void Tlower(string&name) {
	//make all the all the uppercase characters become lowercase characters
	int n = name.size();
	for (int i = 0; i < n; i++) {
		if (int(name[i]) >= 65 && int(name[i]) <= 90)name[i] += 32;
	}
}
string genEmail(string name) {
	string email;//generate email
	email += name[0];//get first char from name
	int n = 0;//-1 if can't find a certain char
	int m;//starting point of last name
	while (n != -1) {
		n = name.find(" ", n);//search for space char
		if (n != -1) {
			n++; m = n;//move n and m
			email += name[n];//get the first char after space
		}
	}
	//m is now at the position of first character of last name
	email.append(name, m + 1);//append the rest of the last name to email
	Tlower(email);//lower all uppercases
	email += "@apcs.vn";//add tail
	return email;//return the result
}
bool newUserStd(string &name, string &id, string &mobile, string &cls) {
	string dummy;//test if file is empty
	ifstream read("Login\\Login_type0.csv");
	getline(read, dummy); read.close();//read the first line
	ofstream writeF;
	//file is empty
	if (dummy.size() == 0) writeF.open("Login\\Login_type0.csv");
	else writeF.open("Login\\Login_type0.csv", std::ios_base::app);
	if (!writeF.is_open())return false;
	string e, out; e = genEmail(name);//generate email from name
	out = id + "," + id + "," + name + "," + e + "," + mobile + "," + cls + "," + "0";//flag not set
	writeF << out << endl;
	writeF.close();
	return true;
}
void addClsF(string ncls) {
	string link = "Classes\\" + ncls;
	const char*c = link.c_str();
	_mkdir(c);//make folder for class
	string make;
	make ="Classes\\" + ncls + "\\Score";
	const char*a = make.c_str();
	_mkdir(a);//make subfolder
	make ="Classes\\" + ncls + "\\Schedule";
	const char*b = make.c_str();
	_mkdir(b);//make subfolder
	ofstream list("Classes\\" + ncls + "\\" + "listCourses.csv"); list.close();//course list of the class
	return;
}
void addCrs_Cls(string &cls, string &crs) {
	ifstream find("Classes\\" + cls + "\\listCourses.csv"); if (!find.is_open())return;
	string course;
	while (getline(find, course))if (course == crs) { 
		find.close();
		return;
	}
	//course already exist
	ofstream newScr("Classes\\" + cls + "\\Score\\" + crs + ".csv"); newScr.close();
	ofstream newSch("Classes\\" + cls + "\\Schedule\\" + crs + ".csv"); newSch.close();
	ofstream newCrs("Classes\\" + cls + "\\listCourses.csv", std::ios_base::app);
	newCrs << crs << endl; newCrs.close();
}

bool Login(string &usrn, string &psw) {
	//check if user is student our lecturer
	int t = 0;//type
	bool exitCode = false;
	int op;//operations
    //first character contains an alphabet =>user is lecturer or academic staff
	if (int(usrn[0]) >= 65) t = 1;
	userList A(t);//pass the type to open the correct log file
	//if log file is broken
	if (A.head == NULL) {
		cout << "ERROR: CANT FIND LOG FILE"<<endl;
		system("pause");
		return false;
	}
	user*cur = A.head;
	while (cur != NULL) {
		if (usrn == cur->username && psw == cur->password) {
			//flag=="0" if haven't change password the first time
			if (cur->flag == "0") {
				A.changePasw(cur);
				return false; // re-login
			}
			else {
				//show menu
				if (cur->class_ == "LEC") t = 1; // lecturer
				if (cur->class_ == "STF") t = 2; // staff
				while (!exitCode) {
					system("CLS");
					//view info
					cout << "Welcome " << cur->fullname << " to the Student management system! <S.M.S>" << endl;
					cout << "*Infomation of user: " << endl;
					cout << "Email: " << cur->email << "   Phone number: " << cur->phoneNum << endl;
					cout << "Position: ";
					if (t == 0) {//student
						cout << "Student of class: " << cur->class_ << endl;
						cout << "------------------------------" << endl;
						cout << endl << "Please choose an operation" << endl;
						cout << "1.Check-in" << endl;
						cout << "2.View check-in result" << endl;
						cout << "3.View score of course" << endl;
						cout << "4.View schedule" << endl;
						cout << "5.Change Password" << endl;
						cout << "6.LOG OUT" << endl;
						cout << "Operation: "; cin >> op; cin.get(); system("CLS");//clear the screen
						switch (op)
						{
						case 1: checkIn(cur->fullname,cur->class_);
							break;
						case 2: viewCKI(cur->fullname, cur->class_);
							break;
						case 3: viewScr(cur->username,cur->class_);
							break;
						case 4: viewSchedule(cur->class_);
							break;
						case 5: A.changePasw(cur);
							break;
						case 6: exitCode = true;
							break;
						default: break;
						}
					}
					else if (t == 2) {
						cout << "Academic Staff" << endl;
						cout << endl << "Please choose an operation" << endl;
						cout << "1.Open Menu for editing student" << endl;
						cout << "2.Open Menu for editing classes" << endl;
						cout << "3.Open Menu for editing course" << endl;
						cout << "4.Open Menu for editing course's schedule" << endl;
						cout << "5.Open Menu for editing Score" << endl;
						cout << "6.Open Menu for editing Attendance" << endl;
						cout << "7.Change password" << endl;
						cout << "8.LOG OUT" << endl;
						cout << "Operation: "; cin >> op; cin.get();
						switch (op)
						{
						case 1: MenuStd();
							break;
						case 2: MenuCls();
							break;
						case 3: MenuCourse();
							break;
						case 4: MenuCrse_Schedl();
							break;
						case 5: MenuScore();
							break;
						case 6: MenuAttd();
							break;
						case 7:  A.changePasw(cur);
							break;
						case 8: exitCode = true;
							break;
						default: break;
						}
					}
					else if (t == 1) {
						cout << "Lecturer" << endl;
						cout << endl << "Please choose an operation" << endl;
						cout << "1.Import scoreboard" << endl;
						cout << "2.Edit grade" << endl;
						cout << "3.View scoreboard" << endl;
						cout << "4.Change password" << endl;
						cout << "5.LOG OUT" << endl;
						cout << "Operation: "; cin >> op; cin.get();
						switch (op)
						{
						case 1: import_Score();
							break;
						case 2: edit_Score(cur->fullname);
							break;
						case 3: view_ScoreBrd(cur->fullname);
							break;
						case 4:  A.changePasw(cur);
							break;
						case 5: exitCode = true;
							break;
						default: break;
						}
					}
				}
				return true;
			}
		}
		else cur = cur->next;
	}
	//end while loop cur==NULL: cant find user
	cout << "Wrong password or username" << endl;
	system("pause");
	return false;
}

//function of staff
void MenuStd() {
	bool exitCode = false;
	int operation = 0;
	while (!exitCode) {
		system("CLS");
		cout << "*This is the menu for editing student!*";
		cout << endl << "Please choose an operation" << endl;
		cout << "1.Import students" << endl;
		cout << "2.Add new student" << endl;
		cout << "3.Edit a student(or remove)" << endl;
		cout << "4.Change the class of a student" << endl;
		cout << "5.Back" << endl;
		cout << "Operation: "; cin >> operation; cin.get();
		switch (operation)
		{
		case 1: importStd();
			break;
		case 2: addNewStd();
			break;
		case 3: editStd();
			break;
		case 4: changeCls();
			break;
		case 5:  exitCode = true;
			break;
		default: break;
		}
	}
}
void MenuCourse() {
	bool exitCode = false;
	int operation = 0;
	while (!exitCode) {
		system("CLS");
		cout << "*This is the menu for editing course*";
		cout << endl << "Please choose an operation" << endl;
		cout << "1.Import course" << endl;
		cout << "2.Add new course" << endl;
		cout << "3.Edit a course( or remove it )" << endl;
		cout << "4.View list of couses" << endl;
		cout << "5.Back" << endl;
		cout << "Operation: "; cin >> operation; cin.get();
		switch (operation)
		{
		case 1: importcourse();
			break;
		case 2: addCourse();
			break;
		case 3: editCourse();
			break;
		case 4: viewCourseList();
			break;
		case 5:  exitCode = true;
			break;
		default: break;
		}
	}
}
void MenuCrse_Schedl() {
	bool exitCode = false;
	int operation = 0;
	while (!exitCode) {
		system("CLS");
		cout << "*This is the menu for editing course's schedule*";
		cout << endl << "Please choose an operation" << endl;
		cout << "1.Import course's schedules" << endl;
		cout << "2.Add new course's schedules" << endl;
		cout << "3.Edit a course's schedules(or remove)" << endl;
		cout << "4.View list of course's schedules" << endl;
		cout << "5.Back" << endl;
		cout << "Operation: "; cin >> operation; cin.get();
		switch (operation)
		{
		case 1: importSchdl();
			break;
		case 2: addSchedule();
			break;
		case 3: editSchedule();
			break;
		case 4: viewAllSchedule();
			break;
		case 5:  exitCode = true;
			break;
		default: break;
		}
	}
}
void MenuScore() {
	bool exitCode = false;
	int operation = 0;
	while (!exitCode) {
		system("CLS");
		cout << "*This is the menu for editing Score*";
		cout << endl << "Please choose an operation" << endl;
		cout << "1.Search and view" << endl;
		cout << "2.Export" << endl;
		cout << "3.Back" << endl;
		cout << "Operation: "; cin >> operation; cin.get();
		switch (operation)
		{
		case 1: searchScrBrd();
			break;
		case 2: exportScrBrd();
			break;
		case 3:  exitCode = true;
			break;
		default: break;
		}
	}
}
void MenuAttd() {
	bool exitCode = false;
	int operation = 0;
	while (!exitCode) {
		system("CLS");
		cout << "*This is the menu for editing Attendance*";
		cout << endl << "Please choose an operation" << endl;
		cout << "1.Search and view" << endl;
		cout << "2.Export" << endl;
		cout << "3.Back" << endl;
		cout << "Operation: "; cin >> operation; cin.get();
		switch (operation)
		{
		case 1: searchAtt();
			break;
		case 2: exportAtt();
			break;
		case 3:  exitCode = true;
			break;
		default: break;
		}
	}
}
void MenuCls() {
	bool exitCode = false;
	int operation = 0;
	while (!exitCode) {
		system("CLS");
		cout << "*This is the menu for editing Classes*";
		cout << endl << "Please choose an operation" << endl;
		cout << "1.Add new empty class" << endl;
		cout << "2.View list of class" << endl;
		cout << "3.View list of student in a class" << endl;
		cout << "4.Back" << endl;
		cout << "Operation: "; cin >> operation; cin.get();
		switch (operation)
		{
		case 1: addNewClass();
			break;
		case 2: viewAllClass();
			break;
		case 3: viewStdList();
			break;
		case 4:  exitCode = true;
			break;
		default: break;
		}
	}
}

//function support
bool checkLgth(int n, int min = 5) {
	if (n < min) {
		cout << "Insufficient length!" << endl;
		system("pause");
		return false;
	}
	else return true;
}

//menu student of staff
void viewStdList() {
	system("CLS");
	string data;
	string className;
	//user input
	cout << "Please enter the name of the class you want to see the list student of: ";
	getline(cin, className);
	ifstream read("Classes\\" + className + ".csv");
	if (!read.is_open()) cout << "This class does not exist!" << endl;
	else {
		system("CLS");
		cout << "----------List student of class: "<<className<<"------------" << endl;
		while (getline(read, data, ',')) {
			cout << "Full name: " << data << endl;
			getline(read, data, ',');
			cout << "ID: " << data << endl;
			getline(read, data, ',');
			cout << "Email: " << data << endl;
			getline(read, data);
			cout << "Mobile phone: " << data << endl;
			cout << "-------------" << endl;
		}
	}
	system("pause");
}
void importStd() {
	system("CLS");
	string fileName;
	string dummy;
	string name, id, mp,e;
	//user input
	cout << "Please enter the name of the class whose student list you want to import: ";
	getline(cin, fileName);
	//check if file is in import
	ifstream read("Import\\" + fileName + "_Cls.csv");
	if (!read.is_open()) {
		cout << "Cant find the source. Please check your input and remember to add _Cls\n";
		system("pause");
		return;
	}
	//check if class exist
	Tupper(fileName);
	classList a;
	if (!a.find(fileName)) {
		cout << "Can't find class in the list: Please create a new empty class first\n";
		system("pause");
		return;
	}
	//append new student to empty class
	ofstream add;
	add.open("Classes\\" + fileName + ".csv",std::ios_base::app);
	while (getline(read, dummy, ',')) {
		name = dummy; e = genEmail(name);
		getline(read, id, ',');
		getline(read, mp);
		add << name << "," << id << "," << e << "," << mp << endl;
		newUserStd(name, id, mp, fileName);
	}
	cout << "-------------------" << endl;
	cout << "Import successful!" << endl;
	read.close(); add.close();
	system("pause");
}
void editStd() {
	system("CLS");
	bool exitCode = false;
	string name;
	string class_;
	int choice;
	string data;

	int n;//dummy variables
		  //user input
	cout << "--This is the function to Edit a student in a class--\n";
	cout << "Please enter the name of the student and his/her class:" << endl;
	cout << "Name: "; getline(cin, name);
	cout << "Class: "; getline(cin, class_); Tupper(class_);
	classList x;
	if (!x.find(class_)) {
		cout << "This class does NOT exist\n";
		system("pause"); return;
	}
	studentList A("Classes\\" + class_ + ".csv");
	if (A.head == NULL) {
		cout << "This class is EMPTY!\n";
		system("pause");
		return;
	}
	student* get = A.find(name);
	if (get == NULL) {
		cout << "There is no student with such name in this class " << endl;
		system("pause");
	}
	else {
		while (!exitCode) {
			system("CLS");
			cout << "-----Search result-----" << endl;
			cout << "Name: " << get->name << endl;
			cout << "ID: " << get->ID << endl;
			cout << "Email: " << get->email << endl;
			cout << "Mobile phone: " << get->mobileP << endl;
			cout << "------What changes would you like to make-------------" << endl;
			cout << "0.Name" << endl;
			cout << "1.ID" << endl;
			cout << "2.Email" << endl;
			cout << "3.Mobile phone" << endl;
			cout << "4.Remove this student" << endl;
			cout << "5.Back" << endl;
			cout << "Operation: "; cin >> choice; cin.get();
			switch (choice)
			{
			case 0:
				do {
					system("CLS");
					cout << "Enter new name (> 5 char): "; getline(cin, data);
					n = data.size();
				} while (!checkLgth(n));//check if the length is sufficient
				get->name = data;
				break;
			case 1:
				do {
					system("CLS");
					cout << "Enter new ID (> 6 char): "; getline(cin, data);
					n = data.size();
				} while (!checkLgth(n, 6));//check if the length is sufficient
				get->ID = data;
				break;
			case 2:
				do {
					system("CLS");
					cout << "Enter new email (> 8 char): "; getline(cin, data);
					n = data.size();
				} while (!checkLgth(n));//check if the length is sufficient
				get->email = data;
				break;
			case 3:  do {
				system("CLS");
				cout << "Enter new mobile phone number (> 9 char): "; getline(cin, data);
				n = data.size();
			} while (!checkLgth(n));//check if the length is sufficient
			get->mobileP = data;
			break;
			case 4: system("CLS");
				if (get == A.head) {
					A.head = A.head->next;
					//make empty if only one student
					if (A.head == NULL) {
						ofstream empty("Classes\\" + class_ + ".csv");
						empty << "";
						empty.close();
					}
					delete get;
				}
				else {
					student* cur = A.head;
					while (cur->next != get)cur = cur->next;
					cur->next = get->next;
					delete get;
				}
				exitCode = true;
				cout << "Operation successful!" << endl;
				system("pause");
				break;
			case 5: exitCode = true;
				break;
			default: break;
			}
		}
	}
}
void addNewStd() {
	bool pass = false; //check valid input
	string data;
	string name, id, mobileP,email;
	string dummy;//find class
	string result;//info of student
	system("CLS");
	cout << "Please choose a class to add the new student: "; getline(cin, data); Tupper(data);
	//check if the class exist
	ifstream read("Classes\\ListOfClasses.csv");
	while (getline(read, dummy)) {
			if (dummy == data) pass = true;//found the class
	}
	//if class not found
	if (pass == false) {
		cout << "Cant find the class. Create a new one" << endl;
		system("pause");
		read.close();
		return;
	}
	//open file to add new student
	ofstream write("Classes\\" + data + ".csv", std::ios_base::app);
	pass = false;//reset
	while (!pass) {
		system("CLS");
		cout << "**This is the function to add a new student to class: " << data << endl;
		cout << "Please enter the name of the new student: "; if (name.size() > 5) cout << name << endl; 
		else getline(cin, name);
		if (name.size() > 5) {
			//result = data + ",";
			cout << "Please enter the ID of the new student: "; if (id.size() > 6) cout << id << endl;
			else getline(cin, id);
			if (id.size() > 6) {
				if (email.size() <= 1) email = genEmail(name);
				cout << "Please enter the mobile phone number of the new student: "; getline(cin, mobileP);
					if (mobileP.size() > 7) {
						//result += data;
						pass = true;
						result = name + "," + id + "," + email + "," + mobileP;
						write << result << endl;
						write.close();
						cout << "Student added!!" << endl;
						if(newUserStd(name, id, mobileP, data))
						cout << "Create new account for student successful!\n";
						else cout << "Create new account failed!\n";
						system("pause");
					}
			}
		}
		if (pass == false) {
			cout << "INSUFFICIENT LENGTH!!" << endl;
			system("pause");
		}
	}
}

//menu class of staff
void viewAllClass() {
	system("CLS");
	string data;
	ifstream read("Classes\\ListOfClasses.csv");
	if (!read.is_open()) {
		cout << "ERROR: CANT FIND SOURCE!" << endl;
		system("pause");
	}
	else {
		cout << "-------List of Classes--------" << endl;
		while (getline(read, data)) cout << data << endl;
		system("pause");
	}
	read.close();
}
void addNewClass() {
	bool pass = false;//validate input
	string name;
	while (!pass) {
		system("CLS");
		cout << "Please enter the name of the new class: "; getline(cin, name);
		if (name.size() < 4) {
			cout << "insuficent length!" << endl;
			system("pause");
		}
		else pass = true;
	}
	//add to list of class
	Tupper(name);
	classList a;
	if (a.find(name)) {
		cout << "This class has already exist! \n";
		system("pause");
		return;
	}
	ofstream write;
	if (a.head == NULL)write.open("Classes\\ListOfClasses.csv");
	else write.open("Classes\\ListOfClasses.csv", std::ios_base::app);
	write << name << endl;
	write.close();
	//make a new csv file with the name
	ofstream makeNew("Classes\\" + name + ".csv");
	makeNew.close();
	//make new folder with subfolder
	addClsF(name);
	cout << "-----------------\n";
	cout << "New class added!" << endl;
	system("pause");
	return;
}
void changeCls() {
	system("CLS");
	string ocls;//old class name
	string name;//student name
	string class_;//new class name
	string data;
	cout << "--This is the function to CHANGE CLASS of a student--\n";
	cout << "Please enter the name of the student and his/her class:" << endl;
	cout << "Name: "; getline(cin, name);
	cout << "Class: "; getline(cin, class_); Tupper(class_); ocls = class_;
	classList x;
	if (!x.find(class_)) {
		cout << "This class does not exist! please create new class\n"; system("pause");
		return;
	}
	studentList A("Classes\\" + class_ + ".csv");
	//class is empty
	if (A.head == NULL) {
		cout << "This class is EMPTY!\n";
			system("pause"); return;
		}
	student* get = A.find(name);//find the student
	if (get == NULL) {
		cout << "There is no student with such name in this class" << endl;
		system("pause"); return;
	}
	else {
			cout << "Please enter the new class of his/her: "; getline(cin, class_); Tupper(class_);
			if (!x.find(class_)) {
				cout << "This class does not exist. Please create a new class!\n";
				system("pause"); return;
			}
			//class do exist
			ofstream write("Classes\\" + class_ + ".csv",std::ios_base::app);
			//disconect the student from the class
			if (get == A.head) {
				A.head = A.head->next;
				//if the student is the last in class, empty the class
				if (A.head == NULL) {
					ofstream empty("Classes\\" + ocls + ".csv");
					empty << "";
					empty.close();
				}
			}
			else {
				student*cur = A.head;
				while (cur->next != get) cur = cur->next;
				//believe me, cur wont reach NULL. Or so I thought
				cur->next = get->next;
			}
			//append the student to the new class
			write << get->name << ",";
			write << get->ID << ",";
			write << get->email << ",";
			write << get->mobileP << endl;
			write.close();
			//remove the student memory in old class
			delete get;
			cout << "Change the class successful!" << endl;
			userList a(0);
			user*get = a.find(name);
			get->class_ = class_;
			system("pause");
			return;
		}
}

//menu course of staff
void importcourse() {
	system("CLS");
	string source;
	string dta;
	cout << "Please enter the name of the file you want to import from: "; getline(cin, source);
	ifstream read("Import\\" + source + "_Crs.csv");
	if (!read.is_open()) {
		cout << "CAN'T FIND SOURCE" << endl;
		system("pause");
	}
	else {
		ofstream write;
		ifstream check("Courses\\ListOfCourses.csv");
		getline(check, dta); check.close();//check if course list if empty
		if (dta.size() <= 1) write.open("Courses\\ListOfCourses.csv");
		else write.open("Courses\\ListOfCourses.csv", std::ios_base::app);
		while (getline(read, dta)) {
			//read all data
			write << dta << endl;
		}
		write.close();
		cout << "Import successful!" << endl;
		system("pause");
	}
	read.close();
}
void viewCourseList() {
	system("CLS");
	string dta; //data
				//char data[200];
	cout << "THIS IS THE LIST OF ALL COURSES IN SCHOOL" << endl;
	ifstream read("Courses\\ListOfCourses.csv");
	while (getline(read, dta, ',')) {
		if (dta.size() <= 1) {
			cout << "-EMPTY\n"; system("pause");
			return;
		}
		cout << "Course Code: " << dta << endl;
		getline(read, dta, ',');
		cout << "Academic Year: " << dta << endl;
		getline(read, dta, ',');
		cout << "Semester: " << dta << endl;
		getline(read, dta, ',');
		cout << "Course Name: " << dta << endl;
		getline(read, dta, ',');
		cout << "Lecturer Username: " << dta << endl;
		getline(read, dta, ',');
		cout << "Starting date: " << dta << endl;
		getline(read, dta);
		cout << "Ending date: " << dta << endl;
		cout << "===============================================" << endl;
	}
	system("pause");
}
void editCourse() {
	courseList A; //take in data
	string name; //course code
	string data;// take data from user
	int choice;
	bool exitCode = false;
	system("CLS");
	cout << "Please enter the CODE name of the course you want to edit: "; getline(cin, name); Tupper(name);
	course*get = A.find(name);
	if (get == NULL) {
		cout << "Sorry, there is NO course with that code in the list!" << endl;
		system("pause");
	}
	else {
		while (!exitCode) {
			system("CLS");
			cout << "-------Search Result-------" << endl;
			cout << "Course code: " << get->ccode << endl;
			cout << "Academic year: " << get->aca << endl;
			cout << "Semester: " << get->semester << endl;
			cout << "Course name: " << get->cname << endl;
			cout << "Lecturer name: " << get->lec << endl;
			cout << "Starting date: " << get->start << endl;
			cout << "End date: " << get->end << endl;
			cout << "==============WHAT CHANGES WOULD YOU LIKE TO MAKE==============" << endl;
			cout << "0. Course code" << endl;
			cout << "1. Academic year" << endl;
			cout << "2. Semester" << endl;
			cout << "3. Course name" << endl;
			cout << "4. Lecturer  name" << endl;
			cout << "5. Starting date" << endl;
			cout << "6. End date" << endl;
			cout << "7. Remove this course" << endl;
			cout << "8. Back" << endl;
			cout << "OPERATION: "; cin >> choice; cin.get();
			switch (choice)
			{
			case 0: do {
				system("CLS");
				cout << "Please enter new course code ( >3 ): "; getline(cin, data); Tupper(data);
			} while (!checkLgth(data.size(), 3));
			get->ccode = data;
			break;
			case 1: do {
				system("CLS");
				cout << "Please enter new academic year ( >2 ): "; getline(cin, data);
			} while (!checkLgth(data.size(), 2));
			get->aca = data;
			break;
			case 2: do {
				system("CLS");
				cout << "Please enter new semester ( >0 ): "; getline(cin, data);
			} while (!checkLgth(data.size(), 0));
			get->semester = data;
			break;
			case 3: do {
				system("CLS");
				cout << "Please enter new course name ( >5 ): "; getline(cin, data);
			} while (!checkLgth(data.size()));
			get->cname = data;
			break;
			case 4: do {
				system("CLS");
				cout << "Please enter new lecturer name ( >3 ): "; getline(cin, data);
			} while (!checkLgth(data.size(), 3));
			get->lec = data;
			break;
			case 5: do {
				system("CLS");
				cout << "Please enter new starting date ( DD/MM ): "; getline(cin, data);
			} while (!checkLgth(data.size(), 3));
			get->start = data;
			break;
			case 6: do {
				system("CLS");
				cout << "Please enter new end date ( DD/MM ): "; getline(cin, data);
			} while (!checkLgth(data.size(), 3));
			get->end = data;
			break;
			case 7: system("CLS");
				if (get == A.first) {
					A.first = A.first->next;
					delete get;
				}
				else {
					course* cur = A.first;
					while (cur->next != get)cur = cur->next;
					cur->next = get->next;
					delete get;
				}
				exitCode = true;
				cout << "Operation successful!" << endl;
				system("pause");
				break;
			case 8: exitCode = true;
				break;
			default:
				break;
			}
		}
	}
}
void addCourse() {
	system("CLS"); bool pass = false;//check valid input
	course *input = new course;
	while (true) {
		system("CLS");
		cout << "***Please input the information for the new course***" << endl;
		cout << "Input the Course code: "; if (input->ccode.size() < 3) getline(cin, input->ccode);
		else cout << input->ccode << endl;
		if (input->ccode.size() >= 3) {
			cout << "Input the academic year: "; if (input->aca.size() < 7) getline(cin, input->aca);
			else cout << input->aca << endl;
			if (input->aca.size() >= 7) {
				cout << "Input the semester: "; if (input->semester.size() > 0) cout << input->semester << endl;
				else getline(cin, input->semester);
				if (input->semester.size() > 0) {
					cout << "Input the course name: "; if (input->cname.size() > 5) cout << input->cname << endl;
					else getline(cin, input->cname);
					if (input->cname.size() > 5) {
						cout << "Input the lecturer username: "; if (input->lec.size() > 3)cout << input->lec << endl;
						else getline(cin, input->lec);
						if (input->lec.size() > 3) {
							cout << "Input the Starting date(DD/MM): "; if (input->start.size() > 4)cout << input->start << endl;
							else getline(cin, input->start);
							if (input->start.size() > 4) {
								cout << "Input the Ending date(DD/MM): ";
								getline(cin, input->end);
								if (input->end.size() > 4) {
									//check if course list is empty
									ofstream fout;
									ifstream fin("Courses\\ListOfCourses.csv"); string d;//dummy variable
									getline(fin, d); fin.close();
									if (d.size() <= 0)fout.open("Courses\\ListOfCourses.csv");
									else fout.open("Courses\\ListOfCourses.csv", std::ios_base::app);
									fout << input->ccode << ",";
									fout << input->aca << ",";
									fout << input->semester << ",";
									fout << input->cname << ",";
									fout << input->lec << ",";
									fout << input->start << ",";
									fout << input->end << endl;
									delete input;
									fout.close();
									cout << "New course added" << endl;
									system("pause");
									return;
								}
							}
						}
					}
				}
			}
		}
		cout << "Insufficient length! Please re enter\n";
		system("pause");
	}
}

//menu schedule of staff
void viewAllSchedule() {
	system("CLS");
	string classN;
	cout << "Please enter the class name: "; getline(cin, classN);
	ifstream read("Schedule\\" + classN + ".csv");
	if (!read.is_open()) {
		cout << "This class does not have schedule or not exist" << endl;
		system("pause");
	}
	else {
		system("CLS");
		string data;
		cout << "----------Schedule of class: " << classN << "------------" << endl << endl;
		spacing(16); cout << "7:30-9:15"; spacing(6); cout << "9:30-11:10"; spacing(4); cout << "13:30-15:10"; spacing(4); cout << "15:30-17:10"; cout << endl << endl;
		while (getline(read, data, ',')) {
			cout.width(20);
			cout << left << data;
			for (int i = 0; i < 3; i++) {
				cout.width(15);
				getline(read, data, ',');
				if (data == "none") data = ' ';
				cout << left << data;
			}
			cout.width(15);
			getline(read, data);
			if (data == "none") data = ' ';
			cout << left << data << endl;
		}
		cout << "-----------------------------------"<<endl;
		system("pause");
	}
}
void importSchdl() {
	system("CLS");
	string cls;
	courseList a;//to check if course exist
	cout << "Please enter the name of the class to import schedule: "; getline(cin, cls); Tupper(cls);
	classList cll; if (!cll.find(cls)) {
		cout << "There are no such class in school, please add new class\n"; system("pause");
		return;
	}
	ifstream read("Import\\" + cls + "_Sch.csv");
	if (!read.is_open()) {
		cout << "CANT FIND THE SCHEDULE IN THE IMPORT FOLDER" << endl;
		system("pause");
	}
	else {
		string d, c;//day and course
		ofstream write("Schedule\\" + cls + ".csv");//update to system file
		while (getline(read, d, ',')) {
			write << d << ",";
			for (int i = 0; i < 3; i++) {
				getline(read, c, ','); 
				//if course does not exist: set it to "none"
				if (c != "none") {
					Tupper(c);
					if (a.find(c) == NULL) {
						cout << c << " course set to none\n";
						c = "none";
					}
					else addCrs_Cls(cls, c);
				}
				write << c << ",";
			}
			getline(read, c);
			if (c != "none") {
				Tupper(c);
				if (a.find(c) == NULL) { 
					cout << c << " course set to none\n";
					c = "none";
				}
				else addCrs_Cls(cls, c);
			}
			write << c << endl;
		}
		cout << "--------------------------\n";
		cout << "Import successful" << endl;
		system("pause");
		write.close();
	}
	read.close();
}
void addSchedule() {
	system("CLS");
	string clss,course;
	bool exitCode = false;
	cout << "Please enter the name of the class (with the schedule you want to add): "; getline(cin, clss);
	schedule Scls(clss);
	if (Scls.first == NULL) return;
	else {
		ifstream read("Schedule\\" + clss + ".csv");
		system("CLS");
		string data;
		//open view the schedule
		cout << "----------Schedule of class: " << clss << "------------" << endl << endl;
		spacing(16); cout << "7:30-9:15"; spacing(6); cout << "9:30-11:10"; spacing(4); cout << "13:30-15:10"; spacing(4); cout << "15:30-17:10"; cout << endl << endl;
		while (getline(read, data, ',')) {
			cout.width(20);
			cout << left << data;
			for (int i = 0; i < 3; i++) {
				cout.width(15);
				getline(read, data, ',');
				if (data == "none") data = ' ';
				cout << left << data;
			}
			cout.width(15);
			getline(read, data);
			if (data == "none") data = ' ';
			cout << left << data << endl;
		}
		read.close();
		cout << "================================\n";
		cout << "Please enter the name of the course to add to schedule: "; getline(cin, course);
		Tupper(course);//change input to upper cBase
		courseList a;
		if (a.first == NULL)return;
		if (a.find(course) == NULL) {
			cout << "Cant find the course in course list to add to schedule "<<endl;
			system("pause");
			return;
		}
		else {
			// a and b are used for comparision between user input and day (case insensitive)
			char a[10];
			char b[10];
			cout << "Please enter the day you want to add(monday,tuesday...): "; cin.getline(a, 10);
			//seach for day
			day*tmp = Scls.first;
			while (tmp != NULL) {
				//type conversion for string to array of char
				strcpy_s(b, tmp->dayName.c_str());
				if (_strcmpi(a, b) == 0) {
					if (tmp->mor1 != "none"&&tmp->mor2 != "none"&&tmp->aft1 != "none"&&tmp->aft2 != "none") {
						cout << "This day is full!" << endl;
						system("pause");
						return;
					}
					int time;
					while (!exitCode) {
						//system("CLS");
						cout << "Please enter the time in day (morning 1: enter 1 / morning 2: enter 2 /3/4): "; cin >> time;
						cin.get();
						switch (time)
						{
						case 1:
							if (tmp->mor1 == "none") {
								tmp->mor1 = course;
								exitCode = true;
							}
							break;
						case 2:
							if (tmp->mor2 == "none") {
								tmp->mor2 = course;
								exitCode = true;
							}
							break;
						case 3:
							if (tmp->aft1 == "none") {
								tmp->aft1 = course;
								exitCode = true;
							}
							break;
						case 4:
							if (tmp->aft2 == "none") {
								tmp->aft2 = course;
								exitCode = true;
							}
							break;
						default: exitCode = true;
							break;
						}
						if (exitCode == false) {
							cout << "Course collied!!" << endl;
							system("pause");
						}
					}
					cout << "New course schedule added" << endl;
					addCrs_Cls(clss, course);
					system("pause");
					return;
				}
				else tmp = tmp->next;
			}
			cout << "Cant find the day: "<<endl;
			system("pause");
			return;
		}
	}
}
void editSchedule() {
	system("CLS");
	string cls;
	bool exitCode=false;
	cout << "Please enter the name of the class with the schedule you want to edit: "; getline(cin, cls);
	schedule a(cls);
	if (a.first == NULL) return;
	day*cur;
	//view the schedule and edit directly
	while (!exitCode) {
		system("CLS");
		cur = a.first;
		cout << "----------Schedule of class: " << cls << "------------" << endl << endl;
		spacing(16); cout << "7:30-9:15"; spacing(6); cout << "9:30-11:10"; spacing(4); cout << "13:30-15:10"; spacing(4); cout << "15:30-17:10"; cout << endl << endl;
		while (cur != NULL) {
			cout.width(20);
			cout << left << cur->dayName;
			cout.width(15);
			if (cur->mor1 == "none") cout << left << " ";
			else cout << left << cur->mor1;
			cout.width(15);
			if (cur->mor2 == "none") cout << left << " ";
			else cout << left << cur->mor2;
			cout.width(15);
			if (cur->aft1 == "none") cout<<left << " ";
			else cout<<left << cur->aft1;
			cout.width(15);
			if (cur->aft2 == "none") cout<<left << " "<<endl;
			else cout<< left << cur->aft2<<endl;
			cur = cur->next;
		}
		cout << "==========================================================" << endl;
		cout << "What do you want to edit: "<<endl; int choice;
		cout << "1.Move a course schedule\n";
		cout << "2.Remove a course schedule\n";
		cout << "3.Nothing and return\n";
		cout << "Choice: "; cin >> choice; cin.get();
		switch (choice)
		{
		case 1:
			moveCrsSch(a.first);
			break;
		case 2:
			removeCrsSch(a.first);
			break;
		case 3: return;
		default:
			break;
		}
	}
}
void moveCrsSch(day*&head) {
	//system("CLS");
	bool valid = false;
	char d[10];
	char d_[10];//for comparison case insensitive
	cout << "MOVE SCHEDULE: Please enter the day: "; cin.getline(d, 10);
	day*cur = head;
	while (cur != NULL) {
		strcpy_s(d_, cur->dayName.c_str());
		if (_strcmpi(d_, d) == 0) break;
		else cur = cur->next;
	}
	if (cur == NULL)return;
	string crs;
	cout << "Please enter the time of the day (1,2,3,4): "; int time; cin >> time; cin.get();
	switch (time)
	{
	case 1:
		if (cur->mor1 == "none") {
			cout << "Error: emtpy" << endl;
			return;
		}
		else {
			crs = cur->mor1;
			cur->mor1 = "none";
		}
		break;
	case 2:
		if (cur->mor2 == "none") {
			cout << "Error: emtpy" << endl;
			return;
		}
		else {
			crs = cur->mor2;
			cur->mor2 = "none";
		}
		break;
	case 3:
		if (cur->aft1 == "none") {
			cout << "Error: emtpy" << endl;
			return;
		}
		else {
			crs = cur->aft1;
			cur->aft1 = "none";
		}
		break;
	case 4:
		if (cur->aft2 == "none") {
			cout << "Error: emtpy" << endl;
			return;
		}
		else {
			crs = cur->aft2;
			cur->aft2 = "none";
		}
		break;
	default: return;
		break;
	}
	cur = head;
	while (!valid) {
		//system("CLS");
		cout << "******" << endl;
		cout << "Where do you want to move it to: " << endl;
		cout << "Please enter the day: "; cin.getline(d, 10);
		cur = head;
		while (cur != NULL) {
			strcpy_s(d_, cur->dayName.c_str());
			if (_strcmpi(d,d_) == 0) break;
			else cur = cur->next;
		}
		if (cur == NULL)cout << "Please re-enter the day";
		else if (cur->mor1 != "none"&&cur->mor2 != "none"&&cur->aft1 != "none"&&cur->aft2 != "none") cout << "This day is full\n";
		else valid = true;
	}valid = false;
	while (!valid) {
		cout << "Please enter the time of the day (1,2,3,4): "; cin >> time; cin.get();
		switch (time)
		{
		case 1: if (cur->mor1 == "none") {
			cur->mor1 = crs; valid = true;
		}
				else cout << "Collied cases\n";
			break;
		case 2: if (cur->mor2 == "none") {
			cur->mor2 = crs;
			valid = true;
		}
				else cout << "Collied cases\n";
			break;
		case 3: if (cur->aft1 == "none") {
			cur->aft1 = crs;
			valid = true;
		}
				else cout << "Collied cases\n";
			break;
		case 4: if (cur->aft2 == "none") {
			cur->aft2 = crs;
			valid = true;
		}
				else cout << "Collied cases\n";
			break;
		default: cout << "Please enter again\n";
			system("pause");
			break;
		}
	}
		cout << "Moving course successful!" << endl;
		system("pause");
}
void removeCrsSch(day*&head) {
	char d[10];
	char d_[10];//for comparison case insensitive
	cout << "REMOVE A SCHEDULE: Please enter the day: "; cin.getline(d, 10);
	day*cur = head;
	while (cur != NULL) {
		strcpy_s(d_, cur->dayName.c_str());
		if (_strcmpi(d_, d) == 0) break;
		else cur = cur->next;
	}
	if (cur == NULL)return;
	string crs;
	cout << "Please enter the time of the day (1,2,3,4): "; int time; cin >> time; cin.get();
	switch (time)
	{
	case 1: cur->mor1 = "none";
	case 2: cur->mor2 = "none";
	case 3: cur->aft1 = "none";
	case 4: cur->aft2 = "none";
	default:
		break;
	}
	cout << "REMOVE SUCCESSFUL\n";
	system("pause");
}

//menu for score of staff
void searchScrBrd() {
	system("CLS");
	cout << "Please enter the name of the class you want to see the score board of: "; string cls; getline(cin, cls); Tupper(cls);
	classList a; if (!a.find(cls)) {
		cout << "Sorry this class does not exist, please add new class\n"; system("pause"); return;
	}
	cout << "Please enter the name of the course: "; string crs; getline(cin, crs); Tupper(crs);
	system("CLS");
	ifstream read("Classes\\" + cls + "\\Score\\" + crs + ".csv");
	if (!read.is_open()) {
		cout << "Sorry this class don't have this course\n";
		system("pause");
		return;
	}
	else {
		system("CLS");
		cout << "**SEARCH RESULT**\n";
		cout << "Course: " << crs << endl;
		cout << "Class: " << cls << endl;
		cout << "Name--------------------ID--------mid--final--lab--bonus"<<endl;
		string data;
		while (getline(read, data, ',')) {
			//name
			cout.width(22);
			cout << left << data;
			//id
			getline(read, data, ',');
			cout.width(13);
			cout << left << data;
			//mid
			getline(read, data, ',');
			cout.width(6);
			cout << left << data;
			//final
			getline(read, data, ',');
			cout.width(6);
			cout << left << data;
			//lab
			getline(read, data, ',');
			cout.width(6);
			cout << left << data;
			//bonus
			getline(read, data);
			cout << data <<endl;
		}
		system("pause");
		read.close();
	}
}
void exportScrBrd() {
	system("CLS");
	cout << "Please enter the name of the class you want to export the score board of: "; string cls; getline(cin, cls); Tupper(cls);
	classList a; if (!a.find(cls)) {
		cout << "Sorry this class does not exist, please add new class\n"; system("pause"); return;
	}
	cout << "Please enter the name of the course: "; string crs; getline(cin, crs); Tupper(crs);
	system("CLS");
	ifstream read("Classes\\" + cls + "\\Score\\" + crs + ".csv");
	if (!read.is_open()) {
		cout << "Sorry this class don't have this course\n";
		system("pause");
		return;
	}
	else {
		string data;
		ofstream write("Export\\ScoreBoard#" + crs + "_" + cls + ".csv");
		while (getline(read, data)) write << data << endl;
		write.close();
		read.close();
		cout << "Export successful!\n";
		system("pause");
	}
}

//menu for attendance of staff
void searchAtt() {
	system("CLS");
	cout << "Please enter the name of the course: "; string crs; getline(cin, crs); Tupper(crs);
	cout << "Please enter the name of the class you want to see the attendance list of: "; string cls; getline(cin, cls); Tupper(cls);
	system("CLS");
	ifstream read("Attendance\\" + crs + "\\" +cls + ".csv");
	if (!read.is_open()) {
		cout << "Invalid input\n";
		system("pause");
		return;
	}
	else {
		string data;
		system("CLS");
		cout << "**SEARCH RESULT**\n";
		while (getline(read, data, ',')) {
			cout.width(20);
			cout << left << data;
			//9 weeks
			int i;
			for (i = 0; i < 9; i++) {
				getline(read, data, ',');
				if (data == "-1") {
					for (int j = i; j < 10; j++) {
						cout.width(5);
						cout << left << "N";
					}
					return;
				}
				else {
					cout.width(5);
					cout << left << data;
				}
			}
			//final week
			getline(read, data);
			if (data == "-1")cout <<left<< "N" << endl;
			else cout << left << data<<endl;
		}
		system("pause");
	}
	read.close();
}
void exportAtt() {
	system("CLS");
	cout << "Please enter the name of the course: "; string crs; getline(cin, crs); Tupper(crs);
	cout << "Please enter the name of the class with the attendance list you want to export: "; string cls; getline(cin, cls); Tupper(cls);
	system("CLS");
	ifstream read("Attendance\\" + crs + "\\" + cls + ".csv");
	if (!read.is_open()) {
		cout << "Invalid input\n";
		system("pause");
		return;
	}
	else {
		string data;
		ofstream write("AttendanceList#" + crs + "_" + cls + ".csv");
		while (getline(read, data)) write << data << endl;
		cout << "Export successful" << endl;
		system("pause");
		write.close();
	}
	read.close();
}

//menu for student
void checkIn(string &name,string &cls) {
	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout << "Today is: " << ltm->tm_mday << "/" << ltm->tm_mon + 1 << endl;
	int day = ltm->tm_mday;
	int month = ltm->tm_mon + 1;
	int tme;
	tme = ltm->tm_hour * 100 + ltm->tm_min;
	int choose;
	cout << "Which course do you want to check in: " << endl;
	cout << "1.CS162" << endl;
	cout << "2.PH212" << endl;
	cin >> choose;
	ifstream fin;
	string course;
	if (choose == 1) {
		course = "CS162";
		fin.open("Checkin\\" + cls + "_date_" + course + ".csv");
	}
	else if (choose == 2) {
		course = "PH212";
		fin.open("Checkin\\" + cls + "_date_" + course + ".csv");
	}
	if (!fin.is_open())
		cout << "can't open the file" << endl;
	int count = 0; int checkd = 0;
	node *head1 = NULL, *curr = NULL;
	int cday = 0, cmon = 0, st = 0, ed = 0;
	while (fin.good())
	{
		node *p = new node;
		getline(fin, p->date, '.');
		getline(fin, p->month, ',');
		getline(fin, p->st, ',');
		getline(fin, p->ed);
		if (p->date.length() != 0)
		{
			st = stoi(p->st);
			ed = stoi(p->ed);
			cday = stoi(p->date);
			cmon = stoi(p->month);
		}
		p->next = NULL;
		if (head1 == NULL)
		{
			head1 = p;
			curr = head1;
			count++;
			if (day == cday && month == cmon && tme >= st && tme <= ed)
				checkd = count;
		}
		else if (p->date.length() == 0)
			curr->next = NULL;
		else {
			count++;
			if (day == cday && month == cmon && tme >= st && tme <= ed)
				checkd = count;
			curr->next = p;
			curr = p;
		}

	}
	if (checkd == 0)
	{
		cout << "You Don't have this Course at this time" << endl;
		return;
	}
	fin.close();
	//open attd file
	node *att = new node, *head2 = NULL, *curr2 = NULL;
	fin.open("Checkin\\" + cls + "_att_" + course + ".csv");
	if (!fin.is_open()) {
		cout << "Can't open the file";
		return;
	}
	int check;
	string scheck;
	while (fin.good())
	{
		node *p = new node;
		getline(fin, p->id, ',');
		getline(fin, p->name, ',');
		for (int i = 0; i < 10; i++) {
			if (i == 9) {
				getline(fin, scheck, '\n');
				check = stoi(scheck);
				p->check[i] = check;
			}
			else {
				getline(fin, scheck, ',');
				check = stoi(scheck);
				p->check[i] = check;
			}
		}
		if (head2 == NULL)
		{

			head2 = p;
			curr2 = head2;
			if (name == p->name)
				att = curr2;

		}
		else if (p->name.length() == 0)
			curr2->next = NULL;
		else
		{
			curr2->next = p;
			curr2 = p;
			if (name == p->name)
				att = curr2;
		}
	}
	fin.close();
	if (att->check[checkd - 1] == 0)
	{
		cout << "Checked in" << endl;
		att->check[checkd - 1] = 1;
	}
	else { cout << "You Have already checked in" << endl; return; }
	//update the list
	ofstream fout;
	fout.open("Checkin\\" + cls + "_att_" + course + ".csv");
	curr2 = head2;
	while (curr2 != NULL)
	{
		fout << curr2->id << ",";
		fout << curr2->name << ",";
		for (int i = 0; i < 10; ++i)
		{
			if (i == 9)
			{
				char s = curr2->check[i] + '0';
				fout << s << endl;
			}
			else {
				char s = curr2->check[i] + '0';
				fout << s << ",";
			}

		}
		curr2 = curr2->next;
	}
	fout.close();
	//cleaning the memory
	curr = head1;
	curr2 = head2;
	while (curr != NULL)
	{
		node *remove = curr;
		curr = curr->next;
		delete remove;
	}
	while (curr2 != NULL)
	{
		node *remove = curr2;
		curr2 = curr2->next;
		delete remove;
	}
}
void viewCKI(string &name, string &cls) {
	system("CLS");
	cout << "Enter the course: "; string crs; getline(cin, crs); Tupper(crs);//uppercase the course
	attdL attendance(crs, cls);
	if (attendance.head == NULL) {
		cout << "Check in failed: course not found" << endl;
		system("pause");
		return;
	}
	attd*get = attendance.find(name);
	if (get == NULL) {
		cout << "Check in failed!: can't find the name" << endl;
		system("pause");
		return;
	}
	cout << "**ATTENDANCE RESULT FOR COURSE: " << crs << endl;
	cout.width(20);
	cout << left << get->name;
	cout <<"ID: "<< get->ID<<endl; 
	for (int i = 0; i < 10; i++) {
		cout.width(10);
		cout << left << "Week ";
		cout.width(5);cout<< left << i + 1 << " : ";
		cout << get->cki[i] << endl;
	}
	cout <<"------------------"<< endl;
	system("pause");
	return;
}
void viewScr(string &id, string &cls) {
	system("CLS");
	cout << "Please enter the course: "; string crs; getline(cin, crs); Tupper(crs);
	scoreL a(crs, cls);
	if (a.head == NULL) {
		cout << "This course does NOT exist\n"; system("pause"); return;
	}
	score*get = a.find(id);
	if (get == NULL) {
		cout << "You do not study in this course\n"; system("pause"); return;
	}
	cout << "YOUR SCORE FOR COURSE: " << crs<<endl;
	cout.width(10);
	cout <<left<< "MID: " << get->mid << endl;
	cout.width(10);
	cout <<left<< "FINAL: " << get->fnal << endl;
	cout.width(10);
	cout <<left<< "LAB: " << get->lb << endl;
	cout.width(10);
	cout <<left<< "BONUS: " << get->bnus << endl;
	system("pause");
}
void viewSchedule(string &cls) {
	system("CLS");
	ifstream read("Schedule\\" + cls + ".csv");
	if (!read.is_open()) {
		cout << "Your class does NOT have a schedule\n"; system("pause");
		return;
	}
	else {
		system("CLS");
		string data;
		cout << "----------Schedule of class: " << cls << "------------" << endl << endl;
		spacing(16); cout << "7:30-9:15"; spacing(6); cout << "9:30-11:10"; spacing(4); cout << "13:30-15:10"; spacing(4); cout << "15:30-17:10"; cout << endl << endl;
		while (getline(read, data, ',')) {
			cout.width(20);
			cout << left << data;
			for (int i = 0; i < 3; i++) {
				cout.width(15);
				getline(read, data, ',');
				if (data == "none") data = ' ';
				cout << left << data;
			}
			cout.width(15);
			getline(read, data);
			if (data == "none") data = ' ';
			cout << left << data << endl;
		}
		cout << "-----------------------------------" << endl;
		system("pause");
	}
}

//menu for lecturer
void import_Score() {
	system("CLS");
	cout << "Please enter name of file: "; string file; getline(cin, file);
	ifstream read("Import\\" + file + "_Scr.csv");
	if (!read.is_open()) {
		cout << "Unable to find the file, remember to add _Scr\n"; system("pause"); 
		return;
	}
	string crs(file, 0, file.find("_"));//course
	string cls(file, file.find("_") + 1);//class
	if (crs.size() <= 1 || cls.size() <= 1) {
		//course_class
		cout << "Please rename the file, invalid name\n";
		system("pause"); return;
	}
	Tupper(crs); Tupper(cls);
	classList a; if (!a.find(cls)) {
		cout << "No such class found, please add new class\n"; system("pause");
		return;
	}
	courseList b; if (b.find(crs) == NULL) {
		cout << "No such course found, please add new course\n"; system("pause");
		return;
	}
	string data;
	ifstream check("Classes\\" + cls + "\\listCourses.csv");
	while(getline(check,data)) if (data==crs){
		ofstream write("Classes\\" + cls + "\\Score\\" + crs + ".csv");
		if (!write.is_open()) return;
		while (getline(read, data)) write << data << endl;
		cout << "--------------------\n";
		cout << "Import successful" << endl;
		system("pause");
		write.close();
		read.close();
		check.close();
		return;
	}
	cout << "Sorry, this class doesn't have this course\n";
	system("pause");
	read.close(); check.close();
	return;
}
void view_ScoreBrd(string &lecN) {
	system("CLS");
	cout << "Please enter the name of the class you want to see the score board of: "; string cls; getline(cin, cls); Tupper(cls);
	classList a; if (!a.find(cls)) {
		cout << "Sorry this class does not exist, please add new class\n"; system("pause"); return;
	}
	courseList b;
	string crs=b.findCrs(lecN);
	if (crs.size() <= 1) {
		cout << "You don't teach any course!\n"; system("pause"); return;
	}
	Tupper(crs);
	ifstream read("Classes\\" + cls + "\\Score\\" + crs + ".csv");
	if (!read.is_open()) {
		cout << "Sorry this class don't have this course\n";
		system("pause");
		return;
	}
	else {
		system("CLS");
		cout << "**SEARCH RESULT**\n";
		cout << "Course: " << crs << endl;
		cout << "Class: " << cls << endl;
		cout << "Name--------------------ID--------mid--final--lab--bonus" << endl;
		string data;
		while (getline(read, data, ',')) {
			//name
			cout.width(22);
			cout << left << data;
			//id
			getline(read, data, ',');
			cout.width(13);
			cout << left << data;
			//mid
			getline(read, data, ',');
			cout.width(6);
			cout << left << data;
			//final
			getline(read, data, ',');
			cout.width(6);
			cout << left << data;
			//lab
			getline(read, data, ',');
			cout.width(6);
			cout << left << data;
			//bonus
			getline(read, data);
			cout << data << endl;
		}
		system("pause");
		read.close();
	}
}
void edit_Score(string &lecN) {
	system("CLS");
	courseList b;
	string crs=b.findCrs(lecN);
	if (crs.size() <= 1) {
		cout << "You dont teach any course!\n"; system("pause"); return;
	}
	Tupper(crs);
	cout << "Please enter the class: "; string cls; getline(cin, cls); Tupper(cls);
	scoreL a(crs, cls);
	if (a.head == NULL) {
		cout << "Class do not exist, please add new class\n"; system("pause"); return;
	}
	cout << "Please enter the id of the student: "; string id; getline(cin, id);
	score* get = a.find(id);
	if (get == NULL) {
		cout << "This class does NOT have such student\n"; system("pause"); return;
	}
	system("CLS");
	cout << "**result**" << endl;
	cout << get->name <<"    ID: "<<get->id<< endl;
	cout << "Mid score:    " << get->mid<<endl;
	cout << "Final score:  " << get->fnal<<endl;
	cout << "Lab score:    " << get->lb<<endl;
	cout << "Bonus score:  " << get->bnus<<endl;
	cout << "------------------" << endl;
	cout << "What woud you like to change?" << endl;
	cout << "1.Mid\n";
	cout << "2.Final\n";
	cout << "3.Lab\n";
	cout << "4.Bonus\n";
	cout << "choice: "; int c; cin >> c; cin.get();
	cout << "-------------------" << endl;
	string scr;//score
	switch (c)
	{
	case 1: cout << "Please enter new mid score: "; getline(cin, scr);
		if (stoi(scr) < 0)return;
		else get->mid = scr;
		break;
	case 2: cout << "Please enter new final score: "; getline(cin, scr);
		if (stoi(scr) < 0)return;
		else get->fnal = scr;
		break;
	case 3: cout << "Please enter new lab score: "; getline(cin, scr);
		if (stoi(scr) < 0)return;
		else get->lb = scr;
		break;
	case 4: cout << "Please enter new bonus score: "; getline(cin, scr);
		if (stoi(scr) < 0)return;
		else get->bnus = scr;
		break;
	default: return;
		break;
	}
	cout << "Change score successful" << endl;
	system("pause");
}
