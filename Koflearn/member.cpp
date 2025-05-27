#include "member.h"

#include <iostream>
using namespace std;

// member 자식 클래스 생성자에서 부모 클래스 생성자 포함
Member::Member(unsigned long long primaryKey, string nickName, string email
	, string password, string phoneNumber, int isManager)
	: KoflearnPlatform(primaryKey), nickName(nickName), email(email)
	, password(password), phoneNumber(phoneNumber), isManager(isManager)
{}

// getter 함수 구현
unsigned long long Member::getPrimaryKey() {
	return this->primaryKey;
}
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
int Member::getIsManager() {
	return this->isManager;
}


// setter 함수 구현
void Member::setPrimaryKey(unsigned long long primaryKey) {
	this->primaryKey = primaryKey;
}
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
void Member::setIsManager(int isManager) {
	this->isManager = isManager;
}
