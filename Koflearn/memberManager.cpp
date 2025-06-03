#include "member.h"
#include "memberManager.h"
#include "myPageManager.h"
#include "enrollManager.h"
#include "lectureManager.h"
#include "sessionManager.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono> // 시간 관련 기능(searching cout 출력에 딜레이 부여 위함)
#include <thread> // 스레드 관련 기능(searching cout 출력에 딜레이 부여 위함)

// 생성자에서 인터페이스 타입의 의존성을 주입받음
MemberManager::MemberManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }
    ifstream file;
    file.open("memberList.csv");
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
    file.open("memberList.csv");
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

    unsigned long long primaryKey = this->makePrimaryKey();
    if (primaryKey == -1) {
        cout << "회원 최대 수용량을 초과하였습니다. (9,999,999,999)" << endl;
        return nullptr;
    }

    int isDuplicationNickName = 0;
    while (1) {
        cout << "닉네임(이름) (한글 2자 이상) : ";
        getline(cin, nickname, '\n');
        if (nickname.compare("F") == 0) {
            cout << "'F' 키를 입력했으므로 회원가입을 중단합니다." << endl;
            return nullptr;
        }

        else if (nickname.length() < 6) {
            cout << "닉네임(이름)의 길이는 한글 기준 2자리 이상이어야 합니다." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }
        else if (nickname.length() > 15) {
            cout << "닉네임(이름)의 길이는 한글 기준 5자리 이하여야 합니다." << endl;
            cout << "회원가입을 중단하시려면 'F' 키를 누르고 [Enter] 를 입력해주세요. " << endl;
            continue;
        }


        isDuplicationNickName = this->nickNameDuplicationCheck(nickname);
        if (isDuplicationNickName == 1) {
            cout << "중복된 닉네임(이름)입니다. 다시 입력해주세요." << endl;
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
        else if (isDuplicationEmail == 0) { break; }
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
        cout << getManagerKey() << endl;
        cout << managerPassKey << endl;
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

// 닉네임(이름) 중복 검사 함수 정의
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

bool MemberManager::displayAllMembers() const {
    bool is_size = false;
    if (memberList.empty()) {
        cout << "등록된 멤버가 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return is_size;
    }

    cout << endl;
    int cnt = 0;
    for (const auto& member : memberList) {
        if (cnt == 0) {
            cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
        }
        member.second->displayInfo(); // 단순 멤버 객체(Member) read 책임은 Member 클래스가 맡음
        cnt++;
        is_size = true;
        // 처음 출력은 최대 50명 까지만 출력 이후 조회를 통해서 처리 가능
        if (cnt == 50) {
            break;
        }
    }
    cout << endl << endl;
    return is_size;
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
        cout << "    key     |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
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
        if (primaryKey == 9999999999) {
            return -1;
        }
        return ++primaryKey;
    }
}

string MemberManager::getManagerKey() {
    ifstream file;
    file.open("managerKey.csv");
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

// 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
// "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
// => "댕글링 포인터" 이슈 발생!!
// * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.
map<unsigned long long, Member*>& MemberManager::getMemberList() {
    return this->memberList;
}

bool MemberManager::deleteUserProcess(unsigned long long key) {
    string is_delete = "";

    cout << "탈퇴 시 수강하시는 강의, 강의하시는 강좌 모두 제거됩니다." << endl;
    cout << "정말 탈퇴하시겠습니까?" << endl;
    cout << "탈퇴하시려면 [I agree to withdraw from member] 의 [ ] 안 문구를 정확히 입력해주세요." << endl;
    getline(cin, is_delete, '\n');

    if (is_delete.compare("I agree to withdraw from member") == 0) {
        Member* member = this->searchMember(key);
        // 탈퇴자 관련 데이터 우선 제거(lectureList, instructorLectureList, studentLectureList)
        allDeletedUserData(key);

        // 탈퇴자 member 데이터 제거
        deleteMember(key);

        cout << "회원 탈퇴가 정상적으로 되었습니다." << endl;
        if (member->getPrimaryKey() == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
            cout << "로그인 상태가 해제되고 [Enter] 를 누르면 메인 페이지로 이동합니다." << endl;
            program_interface->getSessionManager().setIs_login(false);
            program_interface->getSessionManager().setIs_admin(false);
            program_interface->getSessionManager().setLoginUser(nullptr);
        }
        else {
            cout << "[Enter] 를 눌러 되돌아가기" << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
    else {
        cout << "문구를 정상적으로 입력하지 않아 회원 탈퇴를 진행할 수 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
}

void MemberManager::allDeletedUserData(unsigned long long primaryKey) {
    unsigned long long userPrimaryKey = primaryKey;
    Member* member = this->searchMember(primaryKey);
    if (member == nullptr) {
        return;
    }
    string userName = member->getNickName();
    cout << "userName " << userName << endl;

    auto& lectureList = program_interface->getLectureManager().getLectureList();
    auto& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 1. instructorLectureList 에서 instructor 제거
    for (auto it = instructorLectureList.begin(); it != instructorLectureList.end(); ) {
        if (it->first == userPrimaryKey) {
            // 현재 요소를 삭제하고 list 의 다음 이터레이터를 받음
            it = instructorLectureList.erase(it);
        }
        else {
            ++it; // next
        }
    }


    // 2. 회원이 수강자(Student)인 경우 -> 본인의 수강 리스트(studentLectureList)에서 수강자 수 감소 + 제거
    auto it = studentLectureList.find(primaryKey);
    // find ~ if(~end()) : 검출될 때(primaryKey 가 한 개일 때 주로 사용하는 로직)
    if (it != studentLectureList.end()) { 
        vector<Lecture*>& lectures = it->second;
        for (Lecture* lec : lectures) {
            if (lec) {
                int count = lec->getEnrolledStudentsCount();
                lec->setEnrolledStudentsCount(count - 1);
            }
        }
        studentLectureList.erase(it);
    }
    // 3. 모든 회원이 수강하고 있는 강의 리스트(studentLectureList)에서 삭제 회원이 진행하던 강좌 제거
    for (auto it = studentLectureList.begin(); it != studentLectureList.end();) {
        vector<Lecture*>& lectures = it->second;
        for (auto vecIt = lectures.begin(); vecIt != lectures.end();) {
            if ((*vecIt)->getInstructorName() == userName) {
                vecIt = lectures.erase(vecIt);
            }
            else {
                ++vecIt; // next lectures iterator
            }
        }

        // StudentLectureList 의 lectures 가 비어있게 될 경우 해당 map 엔트리 제거 
        if (lectures.empty()) {
            it = studentLectureList.erase(it);
        }
        else {
            ++it; // next iterator
        }
    }


    // 4. 해당 강사가 개설한 lectureList 제거
    for (auto it = lectureList.begin(); it != lectureList.end(); ) {
        Lecture* lec = it->second;
        if (lec && lec->getInstructorName() == userName) {
            // 현재 요소를 삭제하고 list 의 다음 이터레이터를 받음
            it = lectureList.erase(it);
        }
        else {
            ++it; // next
        }
    }

    return;
}

bool MemberManager::searchMemberList(string text) {
    bool is_size = false;
    char* endptr;
    unsigned long long searchKey = strtoull(text.c_str(), &endptr, 10);

    cout << endl;
    // 1. text 가 숫자엿을 경우 -> Member 의 primaryKey 로만 조회
    if (*endptr == '\0' && !text.empty()) {
        cout << "Member PrimaryKey 로 검색 중입니다 .";
        for (auto i = 0; i < 5;i++) {
            cout << " .";
            this_thread::sleep_for(chrono::milliseconds(250));
            cout.flush(); // 버퍼에 있는 내용을 바로 화면에 뿌림으로써
            // 점들이 thread chrono timer 에 맞춰서 하나씩 나타나는 프로세싱 효과 부여
        }
        cout << endl;

        // 먼저 데이터를 찾아보고 없으면 이터레이터가 end() 를 반환하는 걸 체크한다.
        auto it = this->memberList.find(searchKey);
        if (it != this->memberList.end()) {
            // 출력되는 Member 가 있을 때 is_size = true
            is_size = true;
            cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
            it->second->displayInfo();
        }
    }

    cout << endl << endl;

    // 2. text 가 숫자이거나 문자가 하나라도 포함된 문자열이였을 경우 
    //    -> Member 의 닉네임(이름) 또는 휴대폰번호 또는 ID(email) 으로 조회
    cout << "Member 의 닉네임(이름) 또는 휴대폰번호 또는 ID(email) 로 검색 중입니다 .";
    for (auto i = 0; i < 5;i++) {
        cout << " .";
        this_thread::sleep_for(chrono::milliseconds(250));
        cout.flush(); // 버퍼에 있는 내용을 바로 화면에 뿌림으로써
        // 점들이 thread chrono timer 에 맞춰서 하나씩 나타나는 프로세싱 효과 부여
    }
    cout << endl;

    /* 부분 문자열 찾기 메소드 적용 : find()
        부분 문자열을 찾으면 해당 부분 문자열이 시작하는 인덱스(위치)를 반환
        찾지 못하면 string::npos 특수 값을 반환한다.
    */
    for (const auto& i : this->memberList) {
        if (i.second->getEmail().find(text) != string::npos
            || i.second->getNickName().find(text) != string::npos
            || i.second->getPhoneNumber().find(text) != string::npos) {
            if (is_size == false) {
                cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
                // 출력되는 Member 가 하나라도 있을 때 is_size = true
                is_size = true;
            }
            i.second->displayInfo();
        }
    }

    return is_size;
}

void MemberManager::displayMenu()
{
    int ch, ch2;
    unsigned long long primaryKey = 0;
    bool isContinue = true;
    Member* member = nullptr;
    bool all_is_size = false;
    string text = "";
    bool is_size = false;
    bool is_deleted = false;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Member Manager                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Koflearn 멤버(회원) 리스트 조회(삭제, 패스워드 수정)     " << endl;
        cout << "  2. 멤버 추가                            " << endl;
        cout << "  3. 메인 메뉴로 돌아가기                       " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;

        // 메뉴에서 숫자 명령어를 받으려고 할 때 영문자 등을 입력했을 때 
        // 무한 깜빡임 현상 해결
        if (cin.fail()) {
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            // 스트림의 오류 상태를 초기화
            cin.clear();
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // 버퍼의 최대 크기, '\n'은 버퍼를 비울 때까지 찾을 문자
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // 버퍼의 최대 크기, '\n'은 버퍼를 비울 때까지 찾을 문자
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (ch) {
        case 1:
            cout << " 멤버 조회 메인 화면에서는 최대 50명의 회원만 조회됩니다. " << endl;
            cout << "  [ 특정 멤버 확인은 검색 기능을 활용하세요. ] " << endl;
            all_is_size = displayAllMembers();
            if (all_is_size == true) {
                // 수정 또는 삭제할 멤버 조회 text 를 입력받고 출력 처리
                cout << endl;
                cout << "   'primaryKey' 또는 '휴대폰 번호' 또는 '회원 닉네임(이름)' 또는 ID(email) 을 입력하세요" << endl;
                cout << "       primaryKey 조회는 정확히 일치해야하며,  '휴대폰 번호' 또는 '회원 닉네임(이름)' 또는 ID(email) 은 부분 조회가 가능합니다." << endl;
                cout << endl;
                cout << "-1 : 취소" << endl;
                cout << "검색 : ";
                getline(cin, text, '\n');
                if (text == "-1") {
                    continue;
                }
                // 조회된 강의가 있을 땐 true, 없을 땐 false 반환처리
                is_size = this->searchMemberList(text);
                if (is_size == true) {
                    cout << "조회된 회원 중 수정 또는 삭제할 회원 의 privateKey 를 입력하세요." << endl;
                    cout << "-1 : 취소" << endl;
                    cout << "입력 : ";
                    cin >> primaryKey;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (primaryKey == -1) {
                        continue;
                    }
                    member = this->searchMember(primaryKey);
                    if (member != nullptr) {
                        cout << "     1. 수정 하기        " << endl;
                        cout << "     2. 삭제 하기        " << endl;
                        cout << "     3. 메인 메뉴로 돌아가기        " << endl;
                        cout << "   기능을 선택하세요 : ";
                        cin >> ch2;

                        // 메뉴에서 숫자 명령어를 받으려고 할 때 영문자 등을 입력했을 때 
                        // 무한 깜빡임 현상 해결
                        if (cin.fail()) {
                            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
                            // 스트림의 오류 상태를 초기화
                            cin.clear();
                            cout << "[Enter] 를 눌러 뒤로가기" << endl;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            // 버퍼의 최대 크기, '\n'은 버퍼를 비울 때까지 찾을 문자
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        // 버퍼의 최대 크기, '\n'은 버퍼를 비울 때까지 찾을 문자
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        if (ch2 == 1) {
                            this->modifyMember(primaryKey);
                        }
                        else if (ch2 == 2) {
                            is_deleted = this->deleteUserProcess(primaryKey);
                            if (is_deleted == true) {
                                isContinue = false;
                            }
                        }
                        else {
                            continue;
                        }
                    }
                    else {
                        cout << "검색된 멤버가 없습니다." << endl;
                    }
                }
                else {
                    cout << "조회된 멤버가 없습니다." << endl;
                }
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 2:
            member = inputMember();
            if (member != nullptr) {
                cout << "회원가입이 완료되었습니다." << endl;
            }
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            break;
        case 3:
            isContinue = false;
            break;
        default:
            break;
        }
    }
    return;
}