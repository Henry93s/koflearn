#ifndef _KOFLEARN_MANAGER_
#define _KOFLEARN_MANAGER_
#include "memberManager.h"
#include "lectureManager.h"
#include "loginManager.h"
#include "myPageManager.h"
#include "enrollManager.h"
#include "sessionManager.h"
#include "IKoflearnPlatManager.h"
#include <iostream>

// 인터페이스를 상속받은 구현체
class KoflearnPlatManager : public IKoflearnPlatManager {
private:
	// 실제 manager 객체들 선언
	// KoflearnPlatManager 메인 컨트롤러 Manager 클래스의 객체가 만들어 질 때
	// 이미 실제 manager 객체들이 KoflearnPlatManager() 생성자의 초기화 리스트에서 
	// 'this' 포인터를 사용하여 각 매니저의 생성자에 주입 !!
	// 해당 객체들은 KoflearnPlatManager 메인 컨트롤러와 수명 주기를 함께한다.
	MemberManager memberManager;
	LectureManager lectureManager;
	LoginManager loginManager;
	EnrollManager enrollManager;
	MyPageManager myPageManager;
	SessionManager sessionManager;

public:
	KoflearnPlatManager();

	// 메인 컨트롤러 Manager 클래스에 있는 특정 멤버 변수에 대한 reference value 반환을 위함
	MemberManager& getMemberManager() override;
	LectureManager& getLectureManager() override;
	LoginManager& getLoginManager() override;
	MyPageManager& getMyPageManager() override;
	EnrollManager& getEnrollManager() override;
	SessionManager& getSessionManager() override;

	// main 에서 KoflearnPlatManager 를 한 번 생성하므로 displayMenu() 멤버 함수는 
	// 종합 컨트롤러 Manager(인터페이스 구현체) 에서 선언 !
	void displayMenu(IKoflearnPlatManager* program);
};

#endif // !_KOFLEARN_MANAGER_
