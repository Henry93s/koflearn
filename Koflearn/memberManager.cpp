#include "member.h"
#include "memberManager.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// 생성자에서 인터페이스 타입의 의존성을 주입받음
MemberManager::MemberManager(IKoflearnPlatManager* program)
    : program_interface(program)
{ 
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }
    ifstream file;
    file.open("memberList.txt");
    char* endptr;
    if (!file.fail()) {
        while (!file.eof()) {
            vector<string> row = parseCSV(file, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr, 10);
                
                Member* member = new Member(primaryKey, row[1], row[2], row[3], row[4], row[5]);
                memberList.insert({ primaryKey, member });
            }
        }
    }
    file.close();
}

MemberManager::~MemberManager()
{
    ofstream file;
    file.open("memberList.txt");
    if (!file.fail()) {
        for (const auto& v : memberList) {
            Member* member = v.second;
            file << member->getPrimaryKey() << ", " 
                << member->getNickName() << ", " 
                << member->getEmail() << ", "
                << member->getPassword() << ", "
                << member->getPhoneNumber() << ", "
                << member->getIsManager() << "\n";
        }
    }
    file.close();
}

Member* MemberManager::inputMember()
{
    string nickname, email, password, phoneNumber;
    string rePassword;

    int isDuplicationNickName = 0;
    while (1) {
        cout << "닉네임 (2자 이상) : ";
        getline(cin, nickname, '\n');
        if (nickname.compare("F") == 0) {
            cout << "'F' 키를 입력했으므로 회원가입을 중단합니다." << endl;
            return nullptr;
        }

        else if (nickname.length() < 2) {
            cout << "닉네임의 길이는 2자리 이상이어야 합니다." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }

        isDuplicationNickName = this->nickNameDuplicationCheck(nickname);
        if (isDuplicationNickName == 1) {
            cout << "중복된 닉네임입니다. 다시 입력해주세요." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }
        else if (isDuplicationNickName == 0) { break; }
    }

    int isDuplicationEmail = 0;
    while (1) {
        cout << "이메일(ID) : ";
        getline(cin, email, '\n');
        if (email.compare("F") == 0) {
            cout << "'F' 키를 입력했으므로 회원가입을 중단합니다." << endl;
            return nullptr;
        }

        isDuplicationEmail = this->emailDuplicationCheck(email);
        if (isDuplicationEmail == 1) {
            cout << "이미 가입한 이메일입니다. 다시 입력해주세요." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
        }
        else if(isDuplicationEmail == 0) { break; }
    }
    
    int isSamePassword = 0;
    while (1) {
        cout << "패스워드 : ";
        getline(cin, password, '\n');
        if (password == "F") {
            cout << "'F' 키를 입력했으므로 회원가입을 중단합니다." << endl;
            return nullptr;
        }
        else if (password.length() < 8) {
            cout << "패스워드의 길이는 8자리 이상이어야 합니다." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }
        cout << "패스워드 확인 : ";
        getline(cin, rePassword, '\n');
        if (password.compare(rePassword) != 0) {
            cout << "패스워드 입력 값이 일치하지 않습니다. 다시 입력해주세요." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }
        else {
            break;
        }
    }
    
    int isDuplicationPhone = 0;
    while (1) {
        cout << "휴대폰 번호 : ";
        getline(cin, phoneNumber, '\n');
        if (phoneNumber.compare("F") == 0) {
            cout << "'F' 키를 입력했으므로 회원가입을 중단합니다." << endl;
            return nullptr;
        }

        isDuplicationPhone = this->phoneDuplicationCheck(phoneNumber);
        if (isDuplicationPhone == 1) {
            cout << "중복된 휴대폰 번호입니다. 다시 입력해주세요." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }
        else if (isDuplicationPhone == 0) { break; }
    }

    string managerPassKey;
    string isManager = "false";
    while (1) {
        cout << "관리자일 경우 관리자 보안 키를 입력해주세요 : ";
        getline(cin, managerPassKey, '\n');
        if (managerPassKey.compare("A") == 0) {
            cout << "'A' 키를 입력했으므로 일반 회원으로 가입을 계속 진행합니다." << endl;
            break;
        }
        if (managerPassKey.compare(getManagerKey()) != 0) {
            cout << "관리자 키 값이 일치하지 않습니다. 다시 입력해주세요." << endl;
            cout << "일반 회원으로 가입을 계속 진행하실 경우 'A' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }
        else if (managerPassKey.compare(getManagerKey()) == 0) {
            isManager = "true";
            break; 
        }
    }
    
    unsigned long long primaryKey = this->makePrimaryKey();

    Member* member = new Member(primaryKey, nickname, email,
                                password, phoneNumber, isManager);
    memberList.insert({ primaryKey, member });
    return member;
}

Member* MemberManager::searchMember(unsigned long long primaryKey)
{
    Member* ret = nullptr;
    for (const auto& i : memberList) {
        if (i.first == primaryKey) {
            return i.second;
        }
    }
    return nullptr;
}

// login 관련으로 이메일로 멤버 찾기 추가
Member* MemberManager::searchMember(string email)
{
    Member* ret = nullptr;
    for (const auto& i : memberList) {
        if (i.second->getEmail() == email) {
            ret = i.second;
        }
    }
    return ret;
}

// 닉네임 중복 검사 함수 정의
int MemberManager::nickNameDuplicationCheck(string nickName)
{
    for (auto& i : memberList) {
        if (i.second->getNickName().compare(nickName) == 0) {
            return 1;
        }
    }
    return 0;
}

// 이메일 중복 검사 함수 정의
int MemberManager::emailDuplicationCheck(string email)
{
    for (auto& i : memberList) {
        if (i.second->getEmail().compare(email) == 0) {
            return 1;
        }
    }
    return 0;
}

// 휴대폰 번호 중복 검사 함수 정의
int MemberManager::phoneDuplicationCheck(string phoneNumber)
{
    for (auto& i : memberList) {
        if (i.second->getPhoneNumber().compare(phoneNumber) == 0) {
            return 1;
        }
    }
    return 0;
}

void MemberManager::displayAllMembers() const {
    if (memberList.empty()) {
        cout << "등록된 멤버가 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');

        return;
    }

    cout << "    key      |            Email(ID)          |   nickName   |    Phone Number    |   isManager   |" << endl;
    for (const auto& member : memberList) {
        member.second->displayInfo(); // 단순 멤버 객체(Member) read 책임은 Member 클래스가 맡음
    }
    cout << endl << endl;
}

void MemberManager::deleteMember(unsigned long long primaryKey)
{
    // erase : primaryKey 인덱스에 value 가 있으면 pair 를 지우고, value 가 없으면 그냥 넘어감
    memberList.erase(primaryKey);
}

void MemberManager::modifyMember(unsigned long long primaryKey)
{
    Member* member = searchMember(primaryKey);
    if (member != nullptr) {
        cout << "    key     |            Email(ID)          |   nickName   |    Phone Number    |   isManager   |" << endl;
        cout << setw(11) << setfill('0') << right << member->getPrimaryKey() << " | " << left;
        cout << setw(29) << setfill(' ') << member->getEmail() << " | ";
        cout << setw(12) << setfill(' ') << member->getNickName() << " | ";
        cout << setw(18) << setfill(' ') << member->getPhoneNumber() << " | ";
        cout << setw(13) << setfill(' ') << member->getIsManager() << " | ";
        cout << endl;

        string password;
        string rePassword;

        while (1) {
            cout << "패스워드 : ";
            getline(cin, password, '\n');
            if (password == "F") {
                cout << "'F' 키를 입력했으므로 수정을 중단합니다." << endl;
                return;
            }
            else if (password.length() < 8) {
                cout << "패스워드의 길이는 8자리 이상이어야 합니다." << endl;
                cout << "수정을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
                continue;
            }
            cout << "패스워드 확인 : ";
            getline(cin, rePassword, '\n');
            if (password.compare(rePassword) != 0) {
                cout << "패스워드 입력 값이 일치하지 않습니다. 다시 입력해주세요." << endl;
                cout << "수정을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
                continue;
            }
            else {
                cout << "수정이 정상적으로 완료되었습니다." << endl;
                break;
            }
        }

        member->setPassword(password);
        memberList[primaryKey] = member;
    }
    else {
        cout << "입력한 primaryKey 로 조회되는 멤버가 없습니다." << endl;
    }
}

void MemberManager::addMember(Member* member)
{
    memberList.insert({ member->getPrimaryKey(), member });
}

unsigned long long MemberManager::makePrimaryKey()
{
    if (memberList.size() == 0) {
        // member 의 고유 key 값은 1 부터 시작 *(최대 10,000,000,000 100 억)
        // lecture 의 고유 key 값은 10,000,000,001 부터 시작
        return 1;
    }
    else {
        auto elem = memberList.end();
        unsigned long long primaryKey = (--elem)->first;
        return ++primaryKey;
    }
}

string MemberManager::getManagerKey() {
    ifstream file;
    file.open("managerKey.txt");
    if (!file.fail()) {
        while (!file.eof()) {
            vector<string> row = parseCSV(file, '\n');
            return row[0];
        }
    }
    file.close();
    return "";
}

vector<string> MemberManager::parseCSV(istream& file, char delimiter)
{
    stringstream ss;
    vector<string> row;
    string t = " \n\r\t";

    while (!file.eof()) {
        char c = file.get();
        if (c == delimiter || c == '\r' || c == '\n') {
            if (file.peek() == '\n') file.get();
            string s = ss.str();
            s.erase(0, s.find_first_not_of(t));
            s.erase(s.find_last_not_of(t) + 1);
            row.push_back(s);
            ss.str("");
            if (c != delimiter) break;
        }
        else {
            ss << c;
        }
    }
    return row;
}

map<unsigned long long, Member*> MemberManager::getMemberList() {
    return this->memberList;
}

void MemberManager::displayMenu()
{
    int ch;
    unsigned long long key;
    bool isContinue = true;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Member Manager                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Koflearn 멤버(회원) 리스트 출력                     " << endl;
        cout << "  2. 멤버 추가                            " << endl;
        cout << "  3. 멤버 삭제                           " << endl;
        cout << "  4. 멤버 수정                           " << endl;
        cout << "  5. 메인 메뉴로 돌아가기                       " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;
        while (getchar() != '\n');


        switch (ch) {
        case 1: default:
            displayAllMembers();
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 2:
            inputMember();
            cout << "회원가입이 완료되었습니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 3:
            displayAllMembers();
            cout << "   멤버 primaryKey 입력 : ";
            cin >> key;
            while (getchar() != '\n');

            deleteMember(key);
            cout << "멤버 삭제 작업이 종료되었습니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 4:
            displayAllMembers();
            cout << "   멤버 primaryKey 입력 : ";
            cin >> key;
            while (getchar() != '\n');

            modifyMember(key);
            cout << "멤버 수정 작업이 종료되었습니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 5:
            isContinue = false;
            break;
        }
    }
    return;
}