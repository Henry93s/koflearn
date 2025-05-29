#include "member.h"

#include <iostream>
#include <iomanip>
using namespace std;

// member 자식 클래스 생성자에서 부모 클래스 생성자 포함
Member::Member(unsigned long long primaryKey, string nickName, string email
	, string password, string phoneNumber, string isManager)
	: KoflearnPlatform(primaryKey), nickName(nickName), email(email)
	, password(password), phoneNumber(phoneNumber), isManager(isManager)
{}

// getter 함수 구현
string Member::getNickName() {
	return this->nickName;
}
string Member::getEmail() {
	return this->email;
}
string Member::getPassword() {
	return this->password;
}
string Member::getPhoneNumber() {
	return this->phoneNumber;
}
string Member::getIsManager() {
	return this->isManager;
}
// override 
void Member::displayInfo() {
	cout << setw(11) << setfill('0') << right << getPrimaryKey() << "  | " << left;
	cout << setw(29) << setfill(' ') << this->email << " | ";
	cout << setw(12) << setfill(' ') << this->nickName << " | ";
	cout << setw(18) << setfill(' ') << this->phoneNumber << " | ";
	cout << setw(13) << setfill(' ') << this->isManager << " | ";
	cout << endl;
}


// setter 함수 구현
void Member::setNickName(string nickName) {
	this->nickName = nickName;
}
void Member::setEmail(string email) {
	this->email = email;
}
void Member::setPassword(string password) {
	this->password = password;
}
void Member::setPhoneNumber(string phoneNumber) {
	this->phoneNumber = phoneNumber;
}
void Member::setIsManager(string isManager) {
	this->isManager = isManager;
}
