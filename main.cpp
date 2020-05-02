#include "SMS_header.h"
int main() {
	string usrn, psw;
	do {
		system("ClS");
		cout << "Please login!" << endl;
		cout << "username: ";
		getline(cin, usrn);
		cout << "password: ";
		getline(cin, psw);
	} while (!Login(usrn, psw));
	return 0;
}