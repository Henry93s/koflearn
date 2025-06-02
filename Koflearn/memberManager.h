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
    // Manager 가 인터페이스에 의존하도록 인터페이스 포인터 선언
    IKoflearnPlatManager* program_interface;

public:
    // 생성자에서 인터페이스 타입 의존성을 주입받음
    MemberManager(IKoflearnPlatManager* program);               // 생성자 선언
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

    // 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
    // "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
    // => "댕글링 포인터" 이슈 발생!!
    // * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.
    map<unsigned long long, Member*>& getMemberList();

private:
    map<unsigned long long, Member*> memberList;
};

#endif // ! _MEMBER_MANAGER_H_
