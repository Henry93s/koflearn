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
#include <chrono> // �ð� ���� ���(searching cout ��¿� ������ �ο� ����)
#include <thread> // ������ ���� ���(searching cout ��¿� ������ �ο� ����)

// �����ڿ��� �������̽� Ÿ���� �������� ���Թ���
MemberManager::MemberManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "����: MyPageManager�� ��ȿ�� IKoflearnPlatManager�� ���Ե��� �ʾҽ��ϴ�!\n";
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
        cout << "ȸ�� �ִ� ���뷮�� �ʰ��Ͽ����ϴ�. (9,999,999,999)" << endl;
        return nullptr;
    }

    int isDuplicationNickName = 0;
    while (1) {
        cout << "�г���(�̸�) (�ѱ� 2�� �̻�) : ";
        getline(cin, nickname, '\n');
        if (nickname.compare("F") == 0) {
            cout << "'F' Ű�� �Է������Ƿ� ȸ�������� �ߴ��մϴ�." << endl;
            return nullptr;
        }

        else if (nickname.length() < 6) {
            cout << "�г���(�̸�)�� ���̴� �ѱ� ���� 2�ڸ� �̻��̾�� �մϴ�." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }
        else if (nickname.length() > 15) {
            cout << "�г���(�̸�)�� ���̴� �ѱ� ���� 5�ڸ� ���Ͽ��� �մϴ�." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }


        isDuplicationNickName = this->nickNameDuplicationCheck(nickname);
        if (isDuplicationNickName == 1) {
            cout << "�ߺ��� �г���(�̸�)�Դϴ�. �ٽ� �Է����ּ���." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }
        else if (isDuplicationNickName == 0) { break; }
    }

    int isDuplicationEmail = 0;
    while (1) {
        cout << "�̸���(ID) : ";
        getline(cin, email, '\n');
        if (email.compare("F") == 0) {
            cout << "'F' Ű�� �Է������Ƿ� ȸ�������� �ߴ��մϴ�." << endl;
            return nullptr;
        }

        isDuplicationEmail = this->emailDuplicationCheck(email);
        if (isDuplicationEmail == 1) {
            cout << "�̹� ������ �̸����Դϴ�. �ٽ� �Է����ּ���." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
        }
        else if (isDuplicationEmail == 0) { break; }
    }

    int isSamePassword = 0;
    while (1) {
        cout << "�н����� : ";
        getline(cin, password, '\n');
        if (password == "F") {
            cout << "'F' Ű�� �Է������Ƿ� ȸ�������� �ߴ��մϴ�." << endl;
            return nullptr;
        }
        else if (password.length() < 8) {
            cout << "�н������� ���̴� 8�ڸ� �̻��̾�� �մϴ�." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }
        cout << "�н����� Ȯ�� : ";
        getline(cin, rePassword, '\n');
        if (password.compare(rePassword) != 0) {
            cout << "�н����� �Է� ���� ��ġ���� �ʽ��ϴ�. �ٽ� �Է����ּ���." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }
        else {
            break;
        }
    }

    int isDuplicationPhone = 0;
    while (1) {
        cout << "�޴��� ��ȣ : ";
        getline(cin, phoneNumber, '\n');
        if (phoneNumber.compare("F") == 0) {
            cout << "'F' Ű�� �Է������Ƿ� ȸ�������� �ߴ��մϴ�." << endl;
            return nullptr;
        }

        isDuplicationPhone = this->phoneDuplicationCheck(phoneNumber);
        if (isDuplicationPhone == 1) {
            cout << "�ߺ��� �޴��� ��ȣ�Դϴ�. �ٽ� �Է����ּ���." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }
        else if (isDuplicationPhone == 0) { break; }
    }

    string managerPassKey;
    string isManager = "false";
    while (1) {
        cout << "�������� ��� ������ ���� Ű�� �Է����ּ��� : ";
        getline(cin, managerPassKey, '\n');
        if (managerPassKey.compare("A") == 0) {
            cout << "'A' Ű�� �Է������Ƿ� �Ϲ� ȸ������ ������ ��� �����մϴ�." << endl;
            break;
        }
        cout << getManagerKey() << endl;
        cout << managerPassKey << endl;
        if (managerPassKey.compare(getManagerKey()) != 0) {
            cout << "������ Ű ���� ��ġ���� �ʽ��ϴ�. �ٽ� �Է����ּ���." << endl;
            cout << "�Ϲ� ȸ������ ������ ��� �����Ͻ� ��� 'A' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
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

// login �������� �̸��Ϸ� ��� ã�� �߰�
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

// �г���(�̸�) �ߺ� �˻� �Լ� ����
int MemberManager::nickNameDuplicationCheck(string nickName)
{
    for (auto& i : memberList) {
        if (i.second->getNickName().compare(nickName) == 0) {
            return 1;
        }
    }
    return 0;
}

// �̸��� �ߺ� �˻� �Լ� ����
int MemberManager::emailDuplicationCheck(string email)
{
    for (auto& i : memberList) {
        if (i.second->getEmail().compare(email) == 0) {
            return 1;
        }
    }
    return 0;
}

// �޴��� ��ȣ �ߺ� �˻� �Լ� ����
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
        cout << "��ϵ� ����� �����ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return is_size;
    }

    cout << endl;
    int cnt = 0;
    for (const auto& member : memberList) {
        if (cnt == 0) {
            cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
        }
        member.second->displayInfo(); // �ܼ� ��� ��ü(Member) read å���� Member Ŭ������ ����
        cnt++;
        is_size = true;
        // ó�� ����� �ִ� 50�� ������ ��� ���� ��ȸ�� ���ؼ� ó�� ����
        if (cnt == 50) {
            break;
        }
    }
    cout << endl << endl;
    return is_size;
}

void MemberManager::deleteMember(unsigned long long primaryKey)
{
    // erase : primaryKey �ε����� value �� ������ pair �� �����, value �� ������ �׳� �Ѿ
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
            cout << "�н����� : ";
            getline(cin, password, '\n');
            if (password == "F") {
                cout << "'F' Ű�� �Է������Ƿ� ������ �ߴ��մϴ�." << endl;
                return;
            }
            else if (password.length() < 8) {
                cout << "�н������� ���̴� 8�ڸ� �̻��̾�� �մϴ�." << endl;
                cout << "������ �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
                continue;
            }
            cout << "�н����� Ȯ�� : ";
            getline(cin, rePassword, '\n');
            if (password.compare(rePassword) != 0) {
                cout << "�н����� �Է� ���� ��ġ���� �ʽ��ϴ�. �ٽ� �Է����ּ���." << endl;
                cout << "������ �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
                continue;
            }
            else {
                cout << "������ ���������� �Ϸ�Ǿ����ϴ�." << endl;
                break;
            }
        }

        member->setPassword(password);
        memberList[primaryKey] = member;
    }
    else {
        cout << "�Է��� primaryKey �� ��ȸ�Ǵ� ����� �����ϴ�." << endl;
    }
}

void MemberManager::addMember(Member* member)
{
    memberList.insert({ member->getPrimaryKey(), member });
}

unsigned long long MemberManager::makePrimaryKey()
{
    if (memberList.size() == 0) {
        // member �� ���� key ���� 1 ���� ���� *(�ִ� 10,000,000,000 100 ��)
        // lecture �� ���� key ���� 10,000,000,001 ���� ����
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

// �����̳� ��ü�� ��� Ư�� ������ ���� �Լ����� ��ȯ�� ���� �Ҵ����� ��,
// "�ӽ� ��ü" �� �����ǰ� ��ȯ ���� ; �� ���� ������ ������ �ӽ� �����̳� ��ü�� �Ҹ��.
// => "��۸� ������" �̽� �߻�!!
// * �ذ� : �����̳� ��ü�� ������ �Ҵ��Ͽ� ��ȯ�� ��, "�������� �ʰ�" "����" ���� ��ȯ�Ѵ�.
map<unsigned long long, Member*>& MemberManager::getMemberList() {
    return this->memberList;
}

bool MemberManager::deleteUserProcess(unsigned long long key) {
    string is_delete = "";

    cout << "Ż�� �� �����Ͻô� ����, �����Ͻô� ���� ��� ���ŵ˴ϴ�." << endl;
    cout << "���� Ż���Ͻðڽ��ϱ�?" << endl;
    cout << "Ż���Ͻ÷��� [I agree to withdraw from member] �� [ ] �� ������ ��Ȯ�� �Է����ּ���." << endl;
    getline(cin, is_delete, '\n');

    if (is_delete.compare("I agree to withdraw from member") == 0) {
        Member* member = this->searchMember(key);
        // Ż���� ���� ������ �켱 ����(lectureList, instructorLectureList, studentLectureList)
        allDeletedUserData(key);

        // Ż���� member ������ ����
        deleteMember(key);

        cout << "ȸ�� Ż�� ���������� �Ǿ����ϴ�." << endl;
        if (member->getPrimaryKey() == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
            cout << "�α��� ���°� �����ǰ� [Enter] �� ������ ���� �������� �̵��մϴ�." << endl;
            program_interface->getSessionManager().setIs_login(false);
            program_interface->getSessionManager().setIs_admin(false);
            program_interface->getSessionManager().setLoginUser(nullptr);
        }
        else {
            cout << "[Enter] �� ���� �ǵ��ư���" << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
    else {
        cout << "������ ���������� �Է����� �ʾ� ȸ�� Ż�� ������ �� �����ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
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

    // 1. instructorLectureList ���� instructor ����
    for (auto it = instructorLectureList.begin(); it != instructorLectureList.end(); ) {
        if (it->first == userPrimaryKey) {
            // ���� ��Ҹ� �����ϰ� list �� ���� ���ͷ����͸� ����
            it = instructorLectureList.erase(it);
        }
        else {
            ++it; // next
        }
    }


    // 2. ȸ���� ������(Student)�� ��� -> ������ ���� ����Ʈ(studentLectureList)���� ������ �� ���� + ����
    auto it = studentLectureList.find(primaryKey);
    // find ~ if(~end()) : ����� ��(primaryKey �� �� ���� �� �ַ� ����ϴ� ����)
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
    // 3. ��� ȸ���� �����ϰ� �ִ� ���� ����Ʈ(studentLectureList)���� ���� ȸ���� �����ϴ� ���� ����
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

        // StudentLectureList �� lectures �� ����ְ� �� ��� �ش� map ��Ʈ�� ���� 
        if (lectures.empty()) {
            it = studentLectureList.erase(it);
        }
        else {
            ++it; // next iterator
        }
    }


    // 4. �ش� ���簡 ������ lectureList ����
    for (auto it = lectureList.begin(); it != lectureList.end(); ) {
        Lecture* lec = it->second;
        if (lec && lec->getInstructorName() == userName) {
            // ���� ��Ҹ� �����ϰ� list �� ���� ���ͷ����͸� ����
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
    // 1. text �� ���ڿ��� ��� -> Member �� primaryKey �θ� ��ȸ
    if (*endptr == '\0' && !text.empty()) {
        cout << "Member PrimaryKey �� �˻� ���Դϴ� .";
        for (auto i = 0; i < 5;i++) {
            cout << " .";
            this_thread::sleep_for(chrono::milliseconds(250));
            cout.flush(); // ���ۿ� �ִ� ������ �ٷ� ȭ�鿡 �Ѹ����ν�
            // ������ thread chrono timer �� ���缭 �ϳ��� ��Ÿ���� ���μ��� ȿ�� �ο�
        }
        cout << endl;

        // ���� �����͸� ã�ƺ��� ������ ���ͷ����Ͱ� end() �� ��ȯ�ϴ� �� üũ�Ѵ�.
        auto it = this->memberList.find(searchKey);
        if (it != this->memberList.end()) {
            // ��µǴ� Member �� ���� �� is_size = true
            is_size = true;
            cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
            it->second->displayInfo();
        }
    }

    cout << endl << endl;

    // 2. text �� �����̰ų� ���ڰ� �ϳ��� ���Ե� ���ڿ��̿��� ��� 
    //    -> Member �� �г���(�̸�) �Ǵ� �޴�����ȣ �Ǵ� ID(email) ���� ��ȸ
    cout << "Member �� �г���(�̸�) �Ǵ� �޴�����ȣ �Ǵ� ID(email) �� �˻� ���Դϴ� .";
    for (auto i = 0; i < 5;i++) {
        cout << " .";
        this_thread::sleep_for(chrono::milliseconds(250));
        cout.flush(); // ���ۿ� �ִ� ������ �ٷ� ȭ�鿡 �Ѹ����ν�
        // ������ thread chrono timer �� ���缭 �ϳ��� ��Ÿ���� ���μ��� ȿ�� �ο�
    }
    cout << endl;

    /* �κ� ���ڿ� ã�� �޼ҵ� ���� : find()
        �κ� ���ڿ��� ã���� �ش� �κ� ���ڿ��� �����ϴ� �ε���(��ġ)�� ��ȯ
        ã�� ���ϸ� string::npos Ư�� ���� ��ȯ�Ѵ�.
    */
    for (const auto& i : this->memberList) {
        if (i.second->getEmail().find(text) != string::npos
            || i.second->getNickName().find(text) != string::npos
            || i.second->getPhoneNumber().find(text) != string::npos) {
            if (is_size == false) {
                cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
                // ��µǴ� Member �� �ϳ��� ���� �� is_size = true
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
        cout << "  1. Koflearn ���(ȸ��) ����Ʈ ��ȸ(����, �н����� ����)     " << endl;
        cout << "  2. ��� �߰�                            " << endl;
        cout << "  3. ���� �޴��� ���ư���                       " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " ����� �����ϼ��� : ";
        cin >> ch;

        // �޴����� ���� ��ɾ �������� �� �� ������ ���� �Է����� �� 
        // ���� ������ ���� �ذ�
        if (cin.fail()) {
            cout << "�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << endl;
            // ��Ʈ���� ���� ���¸� �ʱ�ȭ
            cin.clear();
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // ������ �ִ� ũ��, '\n'�� ���۸� ��� ������ ã�� ����
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // ������ �ִ� ũ��, '\n'�� ���۸� ��� ������ ã�� ����
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (ch) {
        case 1:
            cout << " ��� ��ȸ ���� ȭ�鿡���� �ִ� 50���� ȸ���� ��ȸ�˴ϴ�. " << endl;
            cout << "  [ Ư�� ��� Ȯ���� �˻� ����� Ȱ���ϼ���. ] " << endl;
            all_is_size = displayAllMembers();
            if (all_is_size == true) {
                // ���� �Ǵ� ������ ��� ��ȸ text �� �Է¹ް� ��� ó��
                cout << endl;
                cout << "   'primaryKey' �Ǵ� '�޴��� ��ȣ' �Ǵ� 'ȸ�� �г���(�̸�)' �Ǵ� ID(email) �� �Է��ϼ���" << endl;
                cout << "       primaryKey ��ȸ�� ��Ȯ�� ��ġ�ؾ��ϸ�,  '�޴��� ��ȣ' �Ǵ� 'ȸ�� �г���(�̸�)' �Ǵ� ID(email) �� �κ� ��ȸ�� �����մϴ�." << endl;
                cout << endl;
                cout << "-1 : ���" << endl;
                cout << "�˻� : ";
                getline(cin, text, '\n');
                if (text == "-1") {
                    continue;
                }
                // ��ȸ�� ���ǰ� ���� �� true, ���� �� false ��ȯó��
                is_size = this->searchMemberList(text);
                if (is_size == true) {
                    cout << "��ȸ�� ȸ�� �� ���� �Ǵ� ������ ȸ�� �� privateKey �� �Է��ϼ���." << endl;
                    cout << "-1 : ���" << endl;
                    cout << "�Է� : ";
                    cin >> primaryKey;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (primaryKey == -1) {
                        continue;
                    }
                    member = this->searchMember(primaryKey);
                    if (member != nullptr) {
                        cout << "     1. ���� �ϱ�        " << endl;
                        cout << "     2. ���� �ϱ�        " << endl;
                        cout << "     3. ���� �޴��� ���ư���        " << endl;
                        cout << "   ����� �����ϼ��� : ";
                        cin >> ch2;

                        // �޴����� ���� ��ɾ �������� �� �� ������ ���� �Է����� �� 
                        // ���� ������ ���� �ذ�
                        if (cin.fail()) {
                            cout << "�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << endl;
                            // ��Ʈ���� ���� ���¸� �ʱ�ȭ
                            cin.clear();
                            cout << "[Enter] �� ���� �ڷΰ���" << endl;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');

                            // ������ �ִ� ũ��, '\n'�� ���۸� ��� ������ ã�� ����
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            continue;
                        }
                        // ������ �ִ� ũ��, '\n'�� ���۸� ��� ������ ã�� ����
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
                        cout << "�˻��� ����� �����ϴ�." << endl;
                    }
                }
                else {
                    cout << "��ȸ�� ����� �����ϴ�." << endl;
                }
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 2:
            member = inputMember();
            if (member != nullptr) {
                cout << "ȸ�������� �Ϸ�Ǿ����ϴ�." << endl;
            }
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
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