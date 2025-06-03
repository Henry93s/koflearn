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

                // map<unsigned long long, vector<Lecture*>> �����̳� ���
                // 
                /* map<unsigned long long, Lecture*> ������ ���
                �� �л��� �ϳ��� ���� �ۿ� �������� ���� : �� key �� �����ϱ� ������ �ߺ��ؼ�
                ���� key �� ������ �� ����.
                */

                // �ش� �л��� primaryKey�� �ʿ� �����ϴ��� Ȯ��
                auto it = studentLectureList.find(member->getPrimaryKey());

                if (it != studentLectureList.end()) {
                    // �̹� �����ϴ� ���, �ش� vector<Lecture*>�� ���ο� ���Ǹ� push_back
                    it->second.push_back(lecture);
                }
                else {
                    // �������� �ʴ� ���, ���ο� ��Ʈ���� �߰��ϰ� vector�� ���Ǹ� ����
                    studentLectureList.insert({ member->getPrimaryKey(), { lecture } });
                }
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

                // �ش� ������ primaryKey�� �ʿ� �����ϴ��� Ȯ��
                auto it = instructorLectureList.find(member->getPrimaryKey());

                if (it != instructorLectureList.end()) {
                    // �̹� �����ϴ� ���, �ش� vector<Lecture*>�� ���ο� ���Ǹ� push_back
                    it->second.push_back(lecture);
                }
                else {
                    // �������� �ʴ� ���, ���ο� ��Ʈ���� �߰��ϰ� vector�� ���Ǹ� ����
                    instructorLectureList.insert({ member->getPrimaryKey(), { lecture } });
                }
            }
        }
    }
    file2.close();
}
EnrollManager::~EnrollManager() {
    ofstream file1, file2;
    file1.open("studentLectureList.csv");
    file2.open("instructorLectureList.csv");

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
    string text = "";
    unsigned long long primaryKey = 0;
    Lecture* lecture = nullptr;
    bool is_size = false;
    map<unsigned long long, Lecture*>& searchLectureList = program_interface->getLectureManager().getLectureList();

    bool isContinue = true;
    bool isTwice = false;
    int ch;
    while (isContinue == true) {
        // isTwice : �ش� �޼ҵ� ���� �� displayAllLecture ���� ��ȸ�Ǵ� �׸���� ó���� ���
        //           �����ǰ� �������� �ܼ� text Ŭ���� ó���ؾ���.
        if (isTwice == true) {
            cout << "\033[2J\033[1;1H";
        }
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Search and Enroll                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " ���� ��ȸ ���� ȭ�鿡���� �ִ� 50���� ���Ǹ� ��ȸ�˴ϴ�. " << endl;
        cout << "  [ Ư�� ���� Ȯ���� �˻� ����� Ȱ���ϼ���. ] " << endl;
        cout << "  1. ���� �˻��ϱ�" << endl;
        cout << "  2. ���� �޴��� ���ư���" << endl;
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
        isTwice = true;

        switch (ch)
        {
        case 1: 
            // ������ ���� ��ȸ text �� �Է¹ް� ��� ó��
            cout << endl;
            cout << "   'primaryKey' �Ǵ� '���Ǹ�' �Ǵ� '���� �̸�' �� �Է��ϼ���" << endl;
            cout << "       primaryKey ��ȸ�� ��Ȯ�� ��ġ�ؾ��ϸ�, '���Ǹ�' �Ǵ� '���� �̸�' �� �κ� ��ȸ�� �����մϴ�." << endl;
            cout << endl;
            cout << "�˻� : ";
            getline(cin, text, '\n');
            // ��ȸ�� ���ǰ� ���� �� true, ���� �� false ��ȯó��
            is_size = program_interface->getLectureManager().searchLectureList(text);

            if (is_size == false) {
                cout << "��ȸ�� ���ǰ� �����ϴ�." << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                cout << "��ȸ�� ���� �� ������ ���� �� privateKey �� �Է��ϼ���" << endl;
                cout << "-1 : ���" << endl;
                cout << "�Է� : ";
                cin >> primaryKey;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (primaryKey == -1) {
                    continue;
                }
                // ���� ��û ó��
                lecture = program_interface->getLectureManager().searchLecture(primaryKey);
                if (lecture != nullptr) {
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
                    }
                }
                else {
                    cout << "��ȸ�� ���ǰ� �����ϴ�." << endl;
                }
                cout << endl << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 2:
            isContinue = false;
            break;
        default:
            break;
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