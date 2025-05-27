#ifndef _MEMBER_H
#define _MEMBER_H

#include <iostream>
using namespace std;

class Member {
public:
	// 생성자 선언
	Member(int primaryKey, string nickName, string email
		, string password, string phoneNumber, int isManager);

	// getter Method
	int getPrimaryKey();
	string getNickName();
	string getEmail(); 
	string getPassword();
	string getPhoneNumber();
	int getIsManager();

	// setter Method
	void setPrimaryKey(int primaryKey);
	void setNickName(string nickName);
	void setEmail(string email);
	void setPassword(string password);
	void setPhoneNumber(string phoneNumber);
	void setIsManager(int isManager);

private:
	int primaryKey; // Member 고유 key (1 부터 1 씩 증가)
	string nickName; // Member 고유 닉네임
	string email; // Member 고유 이메일 (최우선 구분, id 역할)
	string password; // 패스워드
	string phoneNumber; // Member 고유 휴대폰 번호
	int isManager; // 관리자 여부 구분 ( 1: true, 0: false )
};

#endif // !_MEMBER_H
