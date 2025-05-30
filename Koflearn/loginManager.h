#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_

#include "IKoflearnPlatManager.h"
#include <iostream>
using namespace std;

class LoginManager {
private:
	// Manager 가 인터페이스에 의존하도록 인터페이스 포인터 선언
	IKoflearnPlatManager* program_interface;

public:
	// 생성자에서 인터페이스 타입 의존성을 주입받음
	LoginManager(IKoflearnPlatManager* program);
	~LoginManager();

	void displayMenu();
};

#endif // !_LOGIN_MANAGER_H_
