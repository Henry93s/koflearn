#ifndef IKOFLEARN_PLAT_MANAGER_H
#define IKOFLEARN_PLAT_MANAGER_H

#include <iostream>
using namespace std;

// 순환 참조 방지로 각 매니저 클래스들을 [전방 선언]
/* 전방 선언 사용 CASE
 헤더 파일에서 다른 클래스의 전체 정의가 필요 없고 
 단순히 포인터나 참조 타입으로만 사용할 경우, #include 대신 전방 선언을 사용
*/
class MemberManager;
class LectureManager;
class LoginManager;
class EnrollManager;
class MyPageManager;
class SessionManager;

// KoflearnPlatManager 의 인터페이스 정의
class IKoflearnPlatManager {
public:
	virtual ~IKoflearnPlatManager() = 0;

	// 모든 매니저(컨트롤러) 에 대한 가상 getter 순수 함수를 포함
	virtual MemberManager& getMemberManager() = 0;
	virtual LectureManager& getLectureManager() = 0;
	virtual LoginManager& getLoginManager() = 0;
	virtual EnrollManager& getEnrollManager() = 0;
	virtual MyPageManager& getMyPageManager() = 0;
	// SessionManager 헤더파일 & 클래스 추가
	virtual SessionManager& getSessionManager() = 0;
};

#endif