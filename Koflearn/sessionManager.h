#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "IKoflearnPlatManager.h"
// program_interface �� ���ؼ� �����Ϸ��� ��� 
// Manager Ŭ�������� �ʿ��� ��� ������ include �Ǿ� �־�� ��. 
// why? ��ȯ���� ������ IKoflearnPlatManager ���� Manager Ŭ�������� include ���� �ʰ�
//      ���漱�� ó�������Ƿ�
#include "member.h"
#include <iostream>
using namespace std;

class SessionManager {
private:
    // Manager �� �������̽��� �����ϵ��� �������̽� ������ ����
    IKoflearnPlatManager* program_interface;

public:
    // �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
    SessionManager(IKoflearnPlatManager* program);
    ~SessionManager();

    // ��ü ���α׷����� �α��� ������ ���� ������ �޸��ϱ� ����(�α��� ���� ����)
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