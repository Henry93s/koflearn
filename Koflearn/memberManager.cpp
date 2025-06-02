#include "member.h"
#include "memberManager.h"
#include "myPageManager.h"
#include "enrollManager.h"
#include "lectureManager.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
using namespace std;

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
        cout << "�̸� (2�� �̻�) : ";
        getline(cin, nickname, '\n');
        if (nickname.compare("F") == 0) {
            cout << "'F' Ű�� �Է������Ƿ� ȸ�������� �ߴ��մϴ�." << endl;
            return nullptr;
        }

        else if (nickname.length() < 2) {
            cout << "�̸��� ���̴� 2�ڸ� �̻��̾�� �մϴ�." << endl;
            cout << "ȸ�������� �ߴ��Ͻ÷��� 'F' Ű�� ������ [Enter] �� �Է����ּ���. " << endl;
            continue;
        }

        isDuplicationNickName = this->nickNameDuplicationCheck(nickname);
        if (isDuplicationNickName == 1) {
            cout << "�ߺ��� �̸��Դϴ�. �ٽ� �Է����ּ���." << endl;
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

// �г��� �ߺ� �˻� �Լ� ����
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

void MemberManager::displayAllMembers() const {
    if (memberList.empty()) {
        cout << "��ϵ� ����� �����ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "    key      |            Email(ID)          |     Name     |    Phone Number    |   isManager   |" << endl;
    for (const auto& member : memberList) {
        member.second->displayInfo(); // �ܼ� ��� ��ü(Member) read å���� Member Ŭ������ ����
    }
    cout << endl << endl;
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
        cout << "�α��� ���°� �����ǰ� [Enter] �� ������ ���� �������� �̵��մϴ�." << endl;
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
    unsigned long long instructorPrimaryKey = primaryKey;
    Member* member = this->searchMember(primaryKey);
    if (member == nullptr) {
        return;
    }

    string name = member->getNickName();

    auto& lectureList = program_interface->getLectureManager().getLectureList();
    auto& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 1. �ش� ���簡 ������ ���� ����Ʈ ���� -> lectureList ����
    vector<Lecture*> lecturesToDelete;
    for (auto it = lectureList.begin(); it != lectureList.end(); ) {
        Lecture* lec = it->second;
        if (lec && lec->getInstructorName() == name) {
            lecturesToDelete.push_back(lec);
            it = lectureList.erase(it);
        }
        else {
            ++it;
        }
    }

    // 2. instructorLectureList ���� �ش� ���� �׸� ����
    instructorLectureList.erase(instructorPrimaryKey);

    // 3. ȸ���� ������(Student)�� ��� -> ������ ���� ����Ʈ(studentLectureList)���� ������ �� ���� + ����
    auto it = studentLectureList.find(primaryKey);
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

    return;
}

void MemberManager::displayMenu()
{
    int ch;
    unsigned long long key;
    bool isContinue = true;
    Member* member = nullptr;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Member Manager                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Koflearn ���(ȸ��) ����Ʈ ���                     " << endl;
        cout << "  2. ��� �߰�                            " << endl;
        cout << "  3. ��� ����                           " << endl;
        cout << "  4. ��� ����                           " << endl;
        cout << "  5. ���� �޴��� ���ư���                       " << endl;
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
        case 1: default:
            displayAllMembers();
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
            displayAllMembers();
            cout << "   ��� primaryKey �Է� : ";
            cin >> key;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            program_interface->getMemberManager().allDeletedUserData(key);
            deleteMember(key);
            cout << "��� ���� �۾��� ����Ǿ����ϴ�." << endl;
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 4:
            displayAllMembers();
            cout << "   ��� primaryKey �Է� : ";
            cin >> key;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            modifyMember(key);
            cout << "��� ���� �۾��� ����Ǿ����ϴ�." << endl;
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 5:
            isContinue = false;
            break;
        }
    }
    return;
}