#ifndef _MEMBER_H
#define _MEMBER_H
#include "koflearnPlatform.h"
#include <iostream>
using namespace std;

class Member : public KoflearnPlatform {
public:
	// ������ ����
	Member(unsigned long long primaryKey, string nickName, string email
		, string password, string phoneNumber, string isManager);

	// getter Method
	// unsigned long long getPrimaryKey(); KoflearnPlatform
	string getNickName();
	string getEmail();
	string getPassword();
	string getPhoneNumber();
	string getIsManager();
	void displayInfo() override;

	// setter Method
	// void setPrimaryKey(unsigned long long primaryKey); KoflearnPlatform
	void setNickName(string nickName);
	void setEmail(string email);
	void setPassword(string password);
	void setPhoneNumber(string phoneNumber);
	void setIsManager(string isManager);

private:
	string nickName; // Member ���� �г���
	string email; // Member ���� �̸��� (id ����)
	string password; // �н�����
	string phoneNumber; // Member ���� �޴��� ��ȣ
	string isManager; // ������ ���� ���� ( true, false )
};

#endif // !_MEMBER_H