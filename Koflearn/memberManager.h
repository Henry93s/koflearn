#ifndef _MEMBER_MANAGER_H_
#define _MEMBER_MANAGER_H_

#include "member.h"
#include "IKoflearnPlatManager.h"
#include <vector>
#include <map>
#include <iostream>
using namespace std;


class MemberManager {
private:
    // Manager �� �������̽��� �����ϵ��� �������̽� ������ ����
    IKoflearnPlatManager* program_interface;

public:
    // �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
    MemberManager(IKoflearnPlatManager* program);               // ������ ����
    ~MemberManager();              // �Ҹ��� ����

    Member* inputMember();
    void addMember(Member*);
    void deleteMember(unsigned long long key);
    void modifyMember(unsigned long long key);
    Member* searchMember(unsigned long long key);
    // login �������� �̸��Ϸ� ��� ã�� �Լ� �߰�(�����ε�)
    Member* searchMember(string email);
    int nickNameDuplicationCheck(string nickName); // �г��� �ߺ� �˻�
    int emailDuplicationCheck(string email); // �̸��� �ߺ� �˻�
    int phoneDuplicationCheck(string phoneNumber); // �޴��� ��ȣ �ߺ� �˻�
    void displayAllMembers() const;

    unsigned long long makePrimaryKey();
    string getManagerKey();
    vector<string> parseCSV(istream&, char);
    void displayMenu();

    // �����̳� ��ü�� ��� Ư�� ������ ���� �Լ����� ��ȯ�� ���� �Ҵ����� ��,
    // "�ӽ� ��ü" �� �����ǰ� ��ȯ ���� ; �� ���� ������ ������ �ӽ� �����̳� ��ü�� �Ҹ��.
    // => "��۸� ������" �̽� �߻�!!
    // * �ذ� : �����̳� ��ü�� ������ �Ҵ��Ͽ� ��ȯ�� ��, "�������� �ʰ�" "����" ���� ��ȯ�Ѵ�.
    map<unsigned long long, Member*>& getMemberList();

private:
    map<unsigned long long, Member*> memberList;
};

#endif // ! _MEMBER_MANAGER_H_
