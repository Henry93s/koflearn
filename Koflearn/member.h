#ifndef _MEMBER_H
#define _MEMBER_H
#include "koflearnPlatform.h"
#include <iostream>
using namespace std;

class Member : public KoflearnPlatform {
public:
	// 생성자 선언
	Member(unsigned long long primaryKey, string nickName, string email
		, string password, string phoneNumber, int isManager);

	// getter Method
	// unsigned long long getPrimaryKey(); KoflearnPlatform
	string getNickName();
	string getEmail(); 
	string getPassword();
	string getPhoneNumber();
	int getIsManager();
	void displayInfo() override;
	
	// setter Method
	// void setPrimaryKey(unsigned long long primaryKey); KoflearnPlatform
	void setNickName(string nickName);
	void setEmail(string email);
	void setPassword(string password);
	void setPhoneNumber(string phoneNumber);
	void setIsManager(int isManager);

private:
	string nickName; // Member 고유 닉네임
	string email; // Member 고유 이메일 (id 역할)
	string password; // 패스워드
	string phoneNumber; // Member 고유 휴대폰 번호
	int isManager; // 관리자 여부 구분 ( 1: true, 0: false )
};

#endif // !_MEMBER_H
