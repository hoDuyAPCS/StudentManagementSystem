#ifndef SMS
#define SMS
#include <iostream>
using namespace std;
#include <fstream>
#include <string>
#include <cstring>
#include <direct.h>
#include <ctime>
#pragma warning(disable:4996)

struct user {
	string username;
	string password;
	string fullname;
	string email;
	string phoneNum;
	string class_;
	string flag; // flag == 0 password uninitialized. Must change password to continue
	user*next = NULL;
};
struct userList {
	user* head = NULL;
	int type = 0;
	//constructor to get all input in the beginning
	userList(int t); //type
					 //function to change pass word and set flag to 1
	void changePasw(user*&a);
	//destructor to update the file and release the memory
	~userList();
	void update(user*head) {
		ofstream write;
		if (type == 0)write.open("Login\\Login_type0.csv");
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
	}
	user* find(string &name) {
		user*cur = head;
		while (cur != NULL) {
			if (cur->fullname == name) return cur;
			else cur = cur->next;
		}
		return NULL;
	}
};

struct cls {
	string name;
	cls*next = NULL;
};
struct classList {
	cls*head = NULL;
	classList() {
		ifstream read("Classes\\ListOfClasses.csv");
		string dummy;//dummy variable
		cls*cur = head; cls*n;
		while (getline(read, dummy)) {
			if (dummy.size() == 0)return;//file is empty
			n = new cls;
			n->name = dummy;
			if (head == NULL) {
				head = n;
				cur = head;
			}
			else {
				cur->next = n;
				cur = cur->next;
			}
		}
		read.close();
	}
	bool find(string &nme) {
		if (head == NULL)return false;
		cls*get = head;
		while (get->name != nme) {
			get = get->next;
			if (get == NULL)return false;
		}
		return true;
	}
	~classList() {
		if (head == NULL)return;
		while (head != NULL) {
			cls*tmp = head;
			head = head->next;
			delete tmp;
		}
	}
};

struct student {
	string name;
	string ID;
	string email;
	string mobileP;
	student*next = NULL;
};
struct studentList {
	student*head = NULL;
	string link;
	studentList(string path) {
		link = path; //for destructor to find and update file
		ifstream read(path);
		student* add;
		student* cur = head;
		string dummy;
		if (!read.is_open()) return;
		else {
			while (getline(read, dummy, ',')) {
				add = new student;
				add->name = dummy;
				getline(read, add->ID, ',');
				getline(read, add->email, ',');
				getline(read, add->mobileP);
				if (head == NULL) {
					head = add;
					cur = head;
				}
				else {
					cur->next = add;
					cur = cur->next;
				}
			}
		}
		read.close();
	}
	student* find(string name);
	~studentList() {
		//destructor to release memory and update file
		if (head == NULL)return;
		ofstream write(link);
		student*cur = head;
		while (cur != NULL) {
			write << cur->name << ",";
			write << cur->ID << ",";
			write << cur->email << ",";
			write << cur->mobileP << endl;
			cur = cur->next;
		}
		//release memory
		while (head != NULL) {
			student*tmp = head;
			head = head->next;
			delete tmp;
		}
		write.close();
	}
};

struct course {
	string ccode;
	string aca;
	string semester;
	string cname;
	string lec;
	string start;
	string end;
	course *next = NULL;
};
struct courseList {
	course*first = NULL;
	courseList() {
		course * cur = first;
		ifstream fin;
		fin.open("Courses\\ListOfCourses.csv");
		if (!fin.is_open()) {
			cout << "CANT FIND SOURCE" << endl;
			system("pause");
		}
		else {
			string dummy;
			while (getline(fin, dummy, ',')) {
				course *p = new course;
				p->ccode = dummy;
				getline(fin, p->aca, ',');
				getline(fin, p->semester, ',');
				getline(fin, p->cname, ',');
				getline(fin, p->lec, ',');
				getline(fin, p->start, ',');
				getline(fin, p->end);
				if (first == NULL) {
					first = p;
					cur = first;
				}
				else {
					cur->next = p;
					cur = p;
				}
			}
		}
		fin.close();
	}
	~courseList() {
		//update
		if (first == NULL) return;
		ofstream fout;
		fout.open("Courses\\ListOfCourses.csv");
		course*curr = first;
		while (curr != NULL) {
			fout << curr->ccode << ",";
			fout << curr->aca << ",";
			fout << curr->semester << ",";
			fout << curr->cname << ",";
			fout << curr->lec << ",";
			fout << curr->start << ",";
			fout << curr->end << endl;
			curr = curr->next;
		}
		fout.close();
		//release memory
		while (first != NULL) {
			course*remove = first;
			first = first->next;
			delete remove;
		}
	}
	course* find(string name) {
		course*cur = first;
		while (cur != NULL) {
			if (cur->ccode==name) return cur;
			else cur = cur->next;
		}
		return NULL;
	}
	string findCrs(string &lecturer) {
		course*cur = first;
		while (cur != NULL) {
			if (cur->lec == lecturer) return cur->ccode;
			else cur = cur->next;
		}
		string emptystring = "";
		return emptystring;
	}
};

struct day {
	day*next = NULL;
	string dayName;
	string mor1;
	string mor2;
	string aft1;
	string aft2;
};
struct schedule {
	day*first = NULL;
	string name;//to update file in destructor
	schedule(string clsName) {
		name = clsName;//to update file in destructor
		ifstream read("Schedule\\" + clsName + ".csv");
		if (!read.is_open()) {
			cout << "There is no such class or the class does not have a schedule" << endl;
			system("pause");
		}
		else {
			string dummy;
			day*d;
			day*cur = first;
			while (getline(read, dummy, ',')) {
				d = new day;
				d->dayName = dummy;
				getline(read, d->mor1, ',');
				getline(read, d->mor2, ',');
				getline(read, d->aft1, ',');
				getline(read, d->aft2);
				if (first == NULL) {
					first = d;
					cur = first;
				}
				else {
					cur->next = d;
					cur = cur->next;
				}
			}
		}
		read.close();
	}
	//update and realease memory
	~schedule() {
		if (first == NULL)return;
		ofstream write("Schedule\\" + name + ".csv");
		day*cur = first;
		while (cur != NULL) {
			write << cur->dayName << "," << cur->mor1<<","<<cur->mor2 << "," << cur->aft1 << "," << cur->aft2 << endl;
			cur = cur->next;
		}
		write.close();
		//release memory
		while (first != NULL) {
			day*tmp = first;
			first = first->next;
			delete tmp;
		}
	}
};

struct attd {
	string ID, name;
	string cki[10];
	attd*next = NULL;
};
struct attdL {
	attd*head = NULL;
	string link;//for destructor to update and delete memory
	attdL(string crs, string cls) {
		link = "Attendance\\" + crs + "\\" + cls + ".csv";
		ifstream read(link);
		if (!read.is_open())return;//head==NULL
		string dummy;//dummy var
		attd*cur = head; attd*n;
		//read all the data as linked list
		while(getline(read,dummy,',')){
			n = new attd;
			n->ID = dummy;
			getline(read, n->name, ',');
			for (int i = 0; i < 9; i++) getline(read, n->cki[i], ',');
			getline(read, n->cki[9]);
			if (head == NULL) {
				head = n;
				cur = head;
			}
			else {
				cur->next = n;
				cur = cur->next;
			}
		}
		read.close();
	}
	~attdL() {
		if (head == NULL)return;
		ofstream write(link);
		attd*cur = head;
		while (cur != NULL) {
			write << cur->ID << "," << cur->name << ",";
			for (int i = 0; i < 9; i++) write << cur->cki[i] << ",";
			write << cur->cki[9] << endl;
			cur = cur->next;
		}
		write.close();
		while (head != NULL) {
			attd*tmp = head;
			head = head->next;
			delete tmp;
		}
	}
	attd* find(string &name) {
		attd*cur = head;
		while (cur->name != name) {
			cur = cur->next;
			if (cur == NULL)break;
		}
		return cur;
	}
};

struct score {
	string name,id;
	string mid, fnal, lb, bnus;
	score*next=NULL;
};
struct scoreL {
	score*head=NULL;
	string link;//for destructor
	scoreL(string &crs, string &cls) {
		link = "Classes\\" + cls+"\\"+"Score\\"+crs+".csv";
		ifstream read(link);
		if (!read.is_open())return;
		string dummy;//dummy variable
		score*tmp; score*cur = head;
		while (getline(read, dummy, ',')) {
			tmp = new score;
			tmp->name = dummy;
			getline(read, tmp->id,',');
			getline(read, tmp->mid, ',');
			getline(read, tmp->fnal, ',');
			getline(read, tmp->lb, ',');
			getline(read, tmp->bnus);

			if (head == NULL) {
				head = tmp;
				cur = head;
			}
			else {
				cur->next = tmp;
				cur = cur->next;
			}
		}
		read.close();
	}
	~scoreL() {
		if (head == NULL)return;
		ofstream write(link);
		score*cur = head;
		while (cur != NULL) {
			write << cur->name <<","<<cur->id<< "," << cur->mid << "," << cur->fnal << "," << cur->lb << "," << cur->bnus << endl;
			cur = cur->next;
		}
		write.close();
		while (head != NULL) {
			score*tmp = head;
			head = head->next;
			delete tmp;
		}
	}
	score* find(string ID) {
		if (head == NULL)return NULL;
		score*cur = head;
		while (cur->id != ID) {
			cur = cur->next;
			if (cur == NULL) break;
		}
		return cur;
	}
};

struct node {
	string date;
	string month;
	int check[10];
	string name;
	string st;
	string ed;
	node *next;
	string id;
};
//login + main Menu
bool Login(string &usrn, string &psw);


//tmp function
//void tmpFunc();
bool checkLgth(int, int);//check if lenght is sufficient
void Tupper(string &a);//upper case
void Tlower(string&name);//lower case
string genEmail(string name);//generate email
bool newUserStd(string &name, string &id, string &mobile, string &cls);//add new to login
void addClsF(string ncls);//add folder to class
void addCrs_Cls(string &cls, string &crs);//add a course to class

//6 main Menu of Academic Staff
void MenuStd();
void MenuCourse();
void MenuCrse_Schedl();
void MenuScore();
void MenuAttd();
void MenuCls();

//Menu for class
void viewStdList();
void viewAllClass();
void addNewClass();

//Menu for students
void importStd();
void editStd();
void changeCls();
void addNewStd();

//Menu for courses
void addCourse();
void viewCourseList();
void editCourse();
void importcourse();

//Menu for schedules
void viewAllSchedule();
void addSchedule();
void editSchedule();
void moveCrsSch(day*&a);
void importSchdl();
void removeCrsSch(day*&a);

//menu for score
void searchScrBrd();
void exportScrBrd();

//menu for attendance
void searchAtt();
void exportAtt();

//Menu for student
void checkIn(string &name,string &cls);
void viewCKI(string &name,string &cls);
void viewScr(string &id,string &cls);
void viewSchedule(string &cls);

//Menu for lecturer
void import_Score();
void view_ScoreBrd(string &lecN);
void edit_Score(string &lecN);
#endif

