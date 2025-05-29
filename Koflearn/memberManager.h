#ifndef _MEMBER_MANAGER_H_
#define _MEMBER_MANAGER_H_

#include <vector>
#include <map>
#include "member.h"
#include <iostream>
using namespace std;

// KoflearnPlatManager 전방 선언
class KoflearnPlatManager;

class MemberManager {
public:
    MemberManager();               // 생성자 선언
    ~MemberManager();              // 소멸자 선언

    Member* inputMember();
    void addMember(Member*);
    void deleteMember(unsigned long long key);
    void modifyMember(unsigned long long key);
    Member* searchMember(unsigned long long key);
    // login 관련으로 이메일로 멤버 찾기 함수 추가(오버로딩)
    Member* searchMember(string email);
    int nickNameDuplicationCheck(string nickName); // 닉네임 중복 검사
    int emailDuplicationCheck(string email); // 이메일 중복 검사
    int phoneDuplicationCheck(string phoneNumber); // 휴대폰 번호 중복 검사
    void displayAllMembers() const;

    unsigned long long makePrimaryKey();
    string getManagerKey();
    vector<string> parseCSV(istream&, char);
    void displayMenu();

    map<unsigned long long, Member*> getMemberList();

    KoflearnPlatManager* getInstance() const;

private:
    map<unsigned long long, Member*> memberList;
};

#endif // ! _MEMBER_MANAGER_H_

