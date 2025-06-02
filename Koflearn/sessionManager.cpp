#include "member.h"
#include "sessionManager.h"

// �����ڿ��� �������̽� Ÿ���� �������� ���Թ���
SessionManager::SessionManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "����: MyPageManager�� ��ȿ�� IKoflearnPlatManager�� ���Ե��� �ʾҽ��ϴ�!\n";
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