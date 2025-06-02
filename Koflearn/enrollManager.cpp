#include "enrollManager.h"
// program_interface �� ���ؼ� �����Ϸ��� ��� 
// Manager Ŭ�������� �ʿ��� ��� ������ include �Ǿ� �־�� ��. 
// why? ��ȯ���� ������ IKoflearnPlatManager ���� Manager Ŭ�������� include ���� �ʰ�
//      ���漱�� ó�������Ƿ�
#include "lectureManager.h"
#include "sessionManager.h"
#include "memberManager.h"

#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// �����ڿ��� �������̽� Ÿ���� �������� ���Թ���
EnrollManager::EnrollManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "����: MyPageManager�� ��ȿ�� IKoflearnPlatManager�� ���Ե��� �ʾҽ��ϴ�!\n";
    }

    ifstream file1, file2;
    file1.open("studentLectureList.csv");
    char* endptr;
    char* endptr2;
    if (!file1.fail()) {
        while (!file1.eof()) {
            vector<string> row = parseCSV(file1, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr, 10);

                // memberList �� ������ ���� �����ϱ� ���� new �� �ƴ� memberList ���� �ּҸ� �����;� �Ѵ�.
                Member* member = program_interface->getMemberManager().searchMember(primaryKey);

                // 6 index ����
                // lectureList �� ������ ���� �����ϱ� ���� new �� �ƴ� lectureList ���� �ּҸ� �����;� �Ѵ�.
                unsigned long long primaryKey2 = strtoull(row[6].c_str(), &endptr2, 10);
                Lecture* lecture = program_interface->getLectureManager().searchLecture(primaryKey2);

                // map<unsigned long long, vector<Lecture*>>
                // map<unsigned long long, Lecture*> ������ ���
                /*
                �� �л��� �ϳ��� ���� �ۿ� �������� ���� : �� key �� �����ϱ� ������ �ߺ��ؼ�
                ���� key �� ������ �� ����.
                */
                studentLectureList.insert({ member->getPrimaryKey(), { lecture } });
            }
        }
    }
    file1.close();

    file2.open("instructorLectureList.csv");
    char* endptr3;
    char* endptr4;
    if (!file2.fail()) {
        while (!file2.eof()) {
            vector<string> row = parseCSV(file2, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr3, 10);

                // memberList �� ������ ���� �����ϱ� ���� new �� �ƴ� memberList ���� �ּҸ� �����;� �Ѵ�.
                Member* member = program_interface->getMemberManager().searchMember(primaryKey);

                // 6 index ����
                // lectureList �� ������ ���� �����ϱ� ���� new �� �ƴ� lectureList ���� �ּҸ� �����;� �Ѵ�.
                unsigned long long primaryKey2 = strtoull(row[6].c_str(), &endptr4, 10);
                Lecture* lecture = program_interface->getLectureManager().searchLecture(primaryKey2);

                instructorLectureList.insert({ member->getPrimaryKey(), { lecture } });
            }
        }
    }
    file2.close();
}
EnrollManager::~EnrollManager() {
    ofstream file1, file2;
    file1.open("studentLectureList.txt");
    file2.open("instructorLectureList.txt");

    if (!file1.fail()) {
        for (const auto& v : studentLectureList) {
            Member* member = program_interface->getMemberManager().searchMember(v.first);
            // map<unsigned long long, vector<Lecture*>> ���� vector<Lecture*> �� ���� ���
            // for �� ��ȯ �ʿ���
            for (Lecture* lecture : v.second) {
                file1 << member->getPrimaryKey() << ", "
                    << member->getNickName() << ", "
                    << member->getEmail() << ", "
                    << member->getPassword() << ", "
                    << member->getPhoneNumber() << ", "
                    << member->getIsManager() << ", "

                    << lecture->getPrimaryKey() << ", "
                    << lecture->getLectureTitle() << ", "
                    << lecture->getInstructorName() << ", "
                    << lecture->getPrice() << ", "
                    << lecture->getEnrolledStudentsCount() << ", "
                    << lecture->getDurationHours() << ", "
                    << lecture->getDifficultyLevel() << "\n";
            }
        }
    }
    if (!file2.fail()) {
        for (const auto& v : instructorLectureList) {
            Member* member = program_interface->getMemberManager().searchMember(v.first);
            // map<unsigned long long, vector<Lecture*>> ���� vector<Lecture*> �� ���� ���
            // for �� ��ȯ �ʿ���
            for (Lecture* lecture : v.second) {
                file2 << member->getPrimaryKey() << ", "
                    << member->getNickName() << ", "
                    << member->getEmail() << ", "
                    << member->getPassword() << ", "
                    << member->getPhoneNumber() << ", "
                    << member->getIsManager() << ", "

                    << lecture->getPrimaryKey() << ", "
                    << lecture->getLectureTitle() << ", "
                    << lecture->getInstructorName() << ", "
                    << lecture->getPrice() << ", "
                    << lecture->getEnrolledStudentsCount() << ", "
                    << lecture->getDurationHours() << ", "
                    << lecture->getDifficultyLevel() << "\n";
            }
        }
    }
    file1.close();
    file2.close();
}

vector<string> EnrollManager::parseCSV(istream& file, char delimiter)
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

void EnrollManager::searchAndStudentEnrollLecture() {
    unsigned long long privateKey = 0;
    Lecture* lecture = nullptr;
    map<unsigned long long, Lecture*>& lectureList = program_interface->getLectureManager().getLectureList();

    if (!lectureList.empty()) {
        cout << "������ ���� privateKey �� �Է��ϼ��� : ";
        cout << "('-1' : ���)" << endl;
        cin >> privateKey;
        if (privateKey == -1) { return; }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    lecture = program_interface->getLectureManager().searchLecture(privateKey);
    if (lecture == nullptr) {
        cout << "��ȸ�� ���ǰ� �����ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    else {
        bool is_duplication = false;
        Member* member = program_interface->getSessionManager().getLoginUser();
        is_duplication = this->isDuplicationOrSizeCheckStudentEnrollLecture(member, lecture);

        if (is_duplication == false) {
            cout << "���� ��û�� �Ϸ�Ǿ����ϴ�." << endl;
            unsigned long long studentKey = member->getPrimaryKey();

            // �߰��Ϸ��� ������ "������ ��" �� ���� ��Ŵ
            int temp = lecture->getEnrolledStudentsCount();
            temp++;
            // ���� data�� ������ �� ����
            lecture->setEnrolledStudentsCount(temp);

            // �л� Ű�� map ���� vector ã��
            auto it = this->studentLectureList.find(studentKey);
            if (it != this->studentLectureList.end()) {
                // �л��� �̹� ������ �� �ش� �л� ���� ����Ʈ�� �� ���� �߰�
                it->second.push_back(lecture);
            }
            else {
                // �л��� �������� ������, ���ο� ��Ʈ�� ���� (���ο� ���͸� ����� ���� �߰�)
                this->studentLectureList.insert({ studentKey, {lecture} });
            }

            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else if (is_duplication == true) {
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return;
}

bool EnrollManager::isDuplicationOrSizeCheckStudentEnrollLecture(Member* member, Lecture* lecture) {
    // �����̳� ��ü '��۸� ������' �̽� ������ ���� �� ���� �Ҵ�
    map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 1. ���� �л� key �� Map ���� �˻�
    auto it = studentLectureList.find(member->getPrimaryKey());

    if (it != studentLectureList.end()) {
        // 2. �л��� ã�Ҵٸ� �ش� �л��� ���� ����Ʈ(std::vector<Lecture*>)�� ������
        const vector<Lecture*>& lecturesOfStudent = it->second;

        // 3. (�л��� �����ϴ� ���� ������ 9�� �ʰ����� üũ)
        if (lecturesOfStudent.size() >= 9) {
            cout << "�ִ� ���� ��û�� ���Ǵ� 9�� �Դϴ�." << endl;
            return true; // �ʰ� �� �� �̻� ������ �� ����.
        }

        // 4. �� ����Ʈ ������ ���� ���ǰ� �̹� �ִ��� ��ȸ�� üũ
        for (const auto& i : lecturesOfStudent) {
            if (i && i->getPrimaryKey() == lecture->getPrimaryKey()) {
                cout << "�̹� ���� ��û�� �����Դϴ�." << endl;
                return true; // �ߺ� �߰�
            }
        }
    }
    return false; // �ߺ� �ƴ�
}

void EnrollManager::instructorEnrollLecture() {
    Lecture* lecture = nullptr;

    lecture = program_interface->getLectureManager().inputLecture();
    if (lecture != nullptr) {
        cout << "���� ����� �Ϸ�Ǿ����ϴ�." << endl;
        Member* member = program_interface->getSessionManager().getLoginUser();
        this->instructorLectureList.insert({ member->getPrimaryKey() , {lecture} });
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "���ǰ� ���������� �������� �ʾҽ��ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return;
}

string EnrollManager::makeWelcomeText() {
    string welcomeText = "�� �ȳ��ϼ���. ���� �� ���Ǹ� ������û�غ����� !";
    unsigned long long myPrimaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
    bool is_own = false;

    for (const auto& i : studentLectureList) {
        if (i.first == myPrimaryKey) {
            is_own = true;
        }
    }
    if (is_own) {
        welcomeText = "�� �ȳ��ϼ���. ���� �������� ���ǰ� �־�� !";
    }
    return welcomeText;
}

// �л��� �����ϴ� Ư�� �� ���� ã��
Lecture* EnrollManager::findStudentLectureFromList(Lecture* lecture) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // �����̳� ��ü ��ȯ���� �� �ӽ� ��ü �̽��� ��۸� �����͵� �� �����Ƿ� ���� �� �ޱ�
        map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
        for (const auto& i : studentLectureList) {
            if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
                for (const auto& i2 : i.second) {
                    if (i2->getPrimaryKey() == lecture->getPrimaryKey()) {
                        return lecture;
                    }
                }
            }
        }
    }
    return nullptr;
}
// ���簡 �����ϴ� Ư�� �� ���� ã��
Lecture* EnrollManager::findInstructorLectureFromList(Lecture* lecture) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // �����̳� ��ü ��ȯ���� �� �ӽ� ��ü �̽��� ��۸� �����͵� �� �����Ƿ� ���� �� �ޱ�
        map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
        for (const auto& i : instructorLectureList) {
            if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
                for (const auto& i2 : i.second) {
                    if (i2->getPrimaryKey() == lecture->getPrimaryKey()) {
                        return lecture;
                    }
                }
            }
        }
    }
    return nullptr;
}
// Ư�� �л��� �����ϴ� ��� ���� ã��
vector<Lecture*>& EnrollManager::findStudentLectureAllList(unsigned long long primaryKey) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // �����̳� ��ü ��ȯ���� �� �ӽ� ��ü �̽��� ��۸� �����͵� �� �����Ƿ� ���� �� �ޱ�
        map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
        auto it = studentLectureList.find(primaryKey);
        if (it != studentLectureList.end()) {
            return it->second;
        }
    }
    static vector<Lecture*> emptyLectureList;
    return emptyLectureList;
}
// Ư�� ���簡 �����ϴ� ��� ���� ã��
vector<Lecture*>& EnrollManager::findInstructorLectureAllList(unsigned long long primaryKey) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // �����̳� ��ü ��ȯ���� �� �ӽ� ��ü �̽��� ��۸� �����͵� �� �����Ƿ� ���� �� �ޱ�
        map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
        auto it = instructorLectureList.find(primaryKey);
        if (it != instructorLectureList.end()) {
            return it->second;
        }
    }
    static vector<Lecture*> emptyLectureList;
    return emptyLectureList;
}


// �����̳� ��ü�� ��� Ư�� ������ ���� �Լ����� ��ȯ�� ���� �Ҵ����� ��,
// "�ӽ� ��ü" �� �����ǰ� ��ȯ ���� ; �� ���� ������ ������ �ӽ� �����̳� ��ü�� �Ҹ��.
// => "��۸� ������" �̽� �߻�!!
// * �ذ� : �����̳� ��ü�� ������ �Ҵ��Ͽ� ��ȯ�� ��, "�������� �ʰ�" "����" ���� ��ȯ�Ѵ�.
map<unsigned long long, vector<Lecture*>>& EnrollManager::getStudentLectureList() {
    return this->studentLectureList;
}
map<unsigned long long, vector<Lecture*>>& EnrollManager::getInstructorLectureList() {
    return this->instructorLectureList;
}