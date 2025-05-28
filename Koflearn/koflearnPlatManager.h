#ifndef _KOFLEARN_MANAGER_
#define _KOFLEARN_MANAGER_
#include "memberManager.h"
#include "lectureManager.h"
#include "loginManager.h"

#include <iostream>

class KoflearnPlatManager {
private:
	static KoflearnPlatManager* instance;
	bool is_login = false;
	KoflearnPlatManager(){};

public:
	static KoflearnPlatManager* getInstance();

	MemberManager memberManager;
	LectureManager lectureManager;
	LoginManager loginManager;

	// 전체 프로그램에서 로그인 유무에 따라 동작을 달리하기 위함(로그인 세션 관리)
	Member* loginUser = nullptr;
	bool getIs_login();
	void setIs_login(bool isTrue);

	// 한 manager 클래스에서 다른 manager 클래스에 접근하도록 생성한 
	// manager 들에 대해 get 함수를 선언함

	MemberManager& getMemberManager();
	LectureManager& getLectureManager();

	void displayMenu();
};

#endif // !_KOFLEARN_MANAGER_
