#ifndef _MYPAGE_MANAGER_H_
#define _MYPAGE_MANAGER_H_
#include "IKoflearnPlatManager.h"
#include <iostream>
using namespace std;

class MyPageManager {
private:
	// Manager �� �������̽��� �����ϵ��� �������̽� ������ ����
	IKoflearnPlatManager* program_interface;

public:
	// �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
	MyPageManager(IKoflearnPlatManager* program);
	~MyPageManager();

	void displayMenu();
	void myStudentLecturePrint();
	void myInstructorLecturePrint();
	bool selfDeleteID();
	void allDeletedUserData(unsigned long long primaryKey);
	void exitLecture(unsigned long long primaryKey);
};

#endif // !_MYPAGE_MANAGER_H_