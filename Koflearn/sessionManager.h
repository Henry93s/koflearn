#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "IKoflearnPlatManager.h"
// program_interface 를 통해서 접근하려는 모든 
// Manager 클래스들이 필요한 헤더 파일이 include 되어 있어야 함. 
// why? 순환참조 방지로 IKoflearnPlatManager 에서 Manager 클래스들을 include 하지 않고
//      전방선언 처리했으므로
#include "member.h"
#include <iostream>
using namespace std;

class SessionManager {
private:
    // Manager 가 인터페이스에 의존하도록 인터페이스 포인터 선언
    IKoflearnPlatManager* program_interface;

public:
    // 생성자에서 인터페이스 타입 의존성을 주입받음
    SessionManager(IKoflearnPlatManager* program);
    ~SessionManager();

    // 전체 프로그램에서 로그인 유무에 따라 동작을 달리하기 위함(로그인 세션 관리)
    Member* getLoginUser();
    void setLoginUser(Member* member);
    bool getIs_login();
    void setIs_login(bool isTrue);
    bool getIs_admin();
    void setIs_admin(bool isTrue);

private:
    bool is_login = false;
    bool is_admin = false;
    Member* loginUser = nullptr;
};

#endif