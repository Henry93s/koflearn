#ifndef _LOGIN_MANAGER_H_
#define _LOGIN_MANAGER_H_


// koflearnPlatManager.h 주석 사유
// loginManager 에서 싱글톤 객체를 가져오는 getInstance 를 사용함으로써 koflearnPlatManager.h 에도 koflearnPlatManager.h 를 include 할 경우
// 계속 circular include 하는 "순환 참조" 현상이 발생한다. 
// koflearnPlatManager 에서는 싱글톤 객체 주소만 받아오면 되므로 
// koflearnPlatManager.h 를 include 대신 koflearnPlatManager 클래스 자체를 "전방 선언" 으로 작성한다.
//#include "koflearnPlatManager.h"
#include <iostream>
using namespace std;

// KoflearnPlatManager 전방 선언
class KoflearnPlatManager;

class LoginManager {
public:
	LoginManager();              
	~LoginManager();

	// Koflearn 싱글톤 객체에 있는 membermanager 서비스(member 정보 추적)를 사용하기 위해
	// 싱글톤 객체 인스턴스를 가져옴
	// 싱글톤 객체를 가져오는 getInstance 를 사용함으로써 koflearnPlatManager.h 에도 loginManager.h 를 include 할 경우
	// 계속 circular include 하는 "순환 참조" 현상이 발생한다. 따라서 loginManager 자체 멤버 변수를 사용할 일은 없으므로
	// koflearnPlatManager.h 에서 loginManager.h 를 include 하는 대신 loginManager 클래스 자체를 "전방 선언" 으로 작성한다.
	KoflearnPlatManager* getInstance() const;
	void displayMenu();

};



#endif // !_LOGIN_MANAGER_H_
