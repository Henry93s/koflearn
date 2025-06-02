#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_

#include "IKoflearnPlatManager.h"
#include <iostream>
using namespace std;

class LoginManager {
private:
	// Manager �� �������̽��� �����ϵ��� �������̽� ������ ����
	IKoflearnPlatManager* program_interface;

public:
	// �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
	LoginManager(IKoflearnPlatManager* program);
	~LoginManager();

	void displayMenu();
};

#endif // !_LOGIN_MANAGER_H_