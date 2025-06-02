#include "member.h"
#include "sessionManager.h"

// 생성자에서 인터페이스 타입의 의존성을 주입받음
SessionManager::SessionManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }
}
SessionManager::~SessionManager() {}

Member* SessionManager::getLoginUser() {
    return this->loginUser;
}

void SessionManager::setLoginUser(Member* member) {
    this->loginUser = member;
}

bool SessionManager::getIs_login() {
    return this->is_login;
}

void SessionManager::setIs_login(bool boolean) {
    this->is_login = boolean;
}

bool SessionManager::getIs_admin() {
    return this->is_admin;
}

void SessionManager::setIs_admin(bool boolean) {
    this->is_admin = boolean;
}