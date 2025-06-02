#include "lecture.h"
#include "lectureManager.h"
#include "IKoflearnPlatManager.h"
// program_interface �� ���ؼ� �����Ϸ��� ��� 
// Manager Ŭ�������� �ʿ��� ��� ������ include �Ǿ� �־�� ��. 
// why? ��ȯ���� ������ IKoflearnPlatManager ���� Manager Ŭ�������� include ���� �ʰ�
//      ���漱�� ó�������Ƿ�
#include "sessionManager.h"
#include "enrollManager.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono> // �ð� ���� ���(searching cout ��¿� ������ �ο� ����)
#include <thread> // ������ ���� ���(searching cout ��¿� ������ �ο� ����)
using namespace std;

// �����ڿ��� �������̽� Ÿ���� �������� ���Թ���
LectureManager::LectureManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "����: MyPageManager�� ��ȿ�� IKoflearnPlatManager�� ���Ե��� �ʾҽ��ϴ�!\n";
    }

    ifstream file;
    file.open("lectureList.csv");
    char* endptr;
    if (!file.fail()) {
        while (!file.eof()) {
            vector<string> row = parseCSV(file, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr, 10);
                int price = atoi(row[3].c_str());
                int enrolledStudentsCount = atoi(row[4].c_str());
                int durationHours = atoi(row[5].c_str());
                Lecture* lecture = new Lecture(primaryKey, row[1], row[2], price, enrolledStudentsCount, durationHours, row[6]);
                lectureList.insert({ primaryKey, lecture });
            }
        }
    }
    file.close();
}

LectureManager::~LectureManager() {
    ofstream file;
    file.open("lectureList.csv");
    if (!file.fail()) {
        for (const auto& v : lectureList) {
            Lecture* lecture = v.second;
            file << lecture->getPrimaryKey() << ", "
                << lecture->getLectureTitle() << ", "
                << lecture->getInstructorName() << ", "
                << lecture->getPrice() << ", "
                << lecture->getEnrolledStudentsCount() << ", "
                << lecture->getDurationHours() << ", "
                << lecture->getDifficultyLevel() << "\n";
        }
    }
    file.close();
}

Lecture* LectureManager::inputLecture() {
    string lectureTitle, instructorName, difficultyLevel;
    int price, enrolledStudentsCount, durationHours;

    unsigned long long primaryKey = makePrimaryKey();
    if (primaryKey == -1) {
        cout << "���� �ִ� ���뷮�� �ʰ��Ͽ����ϴ�. (99,999,999,999)" << endl;
        return nullptr;
    }

    // �� ����� ���� ������ �ִ� 9������ ���� ����.
    // �����̳� ��ü ��ȯ���� �� �ӽ� ��ü �̽��� ��۸� �����͵� �� �����Ƿ� ���� �� �ޱ�
    map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    for (const auto& i : instructorLectureList) {
        Lecture* lecture = nullptr;
        if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
            if (i.second.size() > 9) {
                cout << "�� ����� ���� �ִ� ������ 9�������� �����մϴ�." << endl;
                return nullptr;
            }
        }
    }

    cout << "���� �� : ";
    getline(cin, lectureTitle, '\n');
    cout << "���� �� : ";
    cout << program_interface->getSessionManager().getLoginUser()->getNickName() << endl;
    instructorName = program_interface->getSessionManager().getLoginUser()->getNickName();
    cout << "���� : ";
    cin >> price;
    enrolledStudentsCount = 0; // �⺻ ������ ���� 0 ���� ����
    cout << "���� �ð� : ";
    cin >> durationHours;
    int selectLevel = 2;
    cout << "���� ���̵� (1 : ����, 2 : ����, 3 : �����) : ";
    cin >> selectLevel;
    switch (selectLevel)
    {
    case 1:
        difficultyLevel = "����";
        break;
    case 2:
        difficultyLevel = "����";
        break;
    case 3:
        difficultyLevel = "�����";
        break;
    default:
        difficultyLevel = "����";
        break;
    }


    Lecture* lecture = new Lecture(primaryKey, lectureTitle, instructorName, price,
        enrolledStudentsCount, durationHours, difficultyLevel);

    lectureList.insert({ primaryKey, lecture });
    // ���Ǹ� ������� �� "�� ���� ����" ����Ʈ�� ����ϱ� ���� instructor(������) ���� ����Ʈ�� ������ �߰�
    Member* member = program_interface->getSessionManager().getLoginUser();

    // Ư�� member privateKey �� key �� vector �� ���� �� ��� �����ϱ� !
    unsigned long long instructorKey = member->getPrimaryKey();

    // ���� Ű�� map ���� vector ã��
    auto it = instructorLectureList.find(instructorKey);

    if (it != instructorLectureList.end()) {
        // �����ڰ� �̹� ������ �� �ش� �������� ���� ����Ʈ�� �� ���Ǹ� �߰��Ѵ�.
        // push_back : ������ �� ���� �׸� �߰�
        it->second.push_back(lecture);
    }
    else {
        // �����ڰ� �������� ������ �� ��Ʈ�� ���� (���ο� ���͸� ����� ���� �߰�)
        instructorLectureList.insert({ instructorKey, {lecture} });
    }

    return lecture;
}

Lecture* LectureManager::searchLecture(unsigned long long primaryKey) {
    // lecture = lectureList[primaryKey] �� lecture ��ü �����͸� ã�� ��
    // ��ü �����Ͱ� ���� �� nullptr �� �ش� primary key �� �ε����� "���� ��" �� ����� �� ���� !
    // => ���� lecture�� ã�� ����� ��ü lectureList �� ��ȸ�ϸ鼭 ����� first �� primarykey�� ������ lecture �� ��ȯ�ϵ��� ��
    for (const auto& i : lectureList) {
        if (i.first == primaryKey) {
            return i.second;
        }
    }
    return nullptr;
}

bool LectureManager::searchLectureList(string text) {
    bool is_size = false;
    char* endptr;
    unsigned long long searchKey = strtoull(text.c_str(), &endptr, 10);

    cout << endl;
    // 1. text �� ���ڿ��� ��� -> Lecture �� primaryKey �θ� ��ȸ
    if (*endptr == '\0' && !text.empty()) {
        cout << "searching PrimaryKey .";
        for (auto i = 0; i < 5;i++) {
            cout << " .";
            this_thread::sleep_for(chrono::milliseconds(350));
            cout.flush(); // ���ۿ� �ִ� ������ �ٷ� ȭ�鿡 �Ѹ����ν�
                          // ������ thread chrono timer �� ���缭 �ϳ��� ��Ÿ���� ���μ��� ȿ�� �ο�
        }
        cout << endl;

        // ���� �����͸� ã�ƺ��� ������ ���ͷ����Ͱ� end() �� ��ȯ�ϴ� �� üũ�Ѵ�.
        auto it = this->lectureList.find(searchKey);
        if (it != lectureList.end()) {
            // ��µǴ� Lecture �� ���� �� is_size = true
            is_size = true;
            cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
            it->second->displayInfo();
        }
    } // 2. text �� ���ڰ� �ϳ��� ���Ե� ���ڿ��̿��� ��� 
      //    -> Lecture �� title �Ǵ� instructorName ���� ��ȸ
    else {
        cout << "searching title or instructorName .";
        for (auto i = 0; i < 5;i++) {
            cout << " .";
            this_thread::sleep_for(chrono::milliseconds(350));
            cout.flush(); // ���ۿ� �ִ� ������ �ٷ� ȭ�鿡 �Ѹ����ν�
                          // ������ thread chrono timer �� ���缭 �ϳ��� ��Ÿ���� ���μ��� ȿ�� �ο�
        }
        cout << endl;

        /* �κ� ���ڿ� ã�� �޼ҵ� ���� : find()
           �κ� ���ڿ��� ã���� �ش� �κ� ���ڿ��� �����ϴ� �ε���(��ġ)�� ��ȯ
           ã�� ���ϸ� string::npos Ư�� ���� ��ȯ�Ѵ�.
        */
        for (const auto& i : lectureList) {
            if (i.second->getLectureTitle().find(text) != string::npos
                || i.second->getInstructorName().find(text) != string::npos) {
                if (is_size == false) {
                    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
                    // ��µǴ� Lecture �� �ϳ��� ���� �� is_size = true
                    is_size = true;
                }
                i.second->displayInfo();
            }
        }
    }

    return is_size;
}

bool LectureManager::displayAllLecture() const {
    if (lectureList.empty()) {
        cout << "��ϵ� ���ǰ� �����ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cout << endl;
    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    int cnt = 0;
    for (const auto& lecture : lectureList) {
        lecture.second->displayInfo(); // �ܼ� ���� ��ü(Lecture()) read å���� Lecture Ŭ�������� ó��
        cnt++;
        // ó�� ����� �ִ� 50�� ������ ��� ���� ��ȸ�� ���ؼ� ó�� ����
        if (cnt == 50) {
            break;
        }
    }
    cout << endl << endl << endl;
    return true;
}

void LectureManager::deleteLecture(unsigned long long primaryKey) {
    // erase : primaryKey �ε����� value �� ������ pair �� �����, value �� ������ �׳� �Ѿ
    lectureList.erase(primaryKey);
}

void LectureManager::modifyLecture(unsigned long long primaryKey) {
    Lecture* lecture = searchLecture(primaryKey);
    if (lecture != nullptr) {
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        lecture->displayInfo();

        int op = -1;
        string lectureTitle;
        int price;
        int durationHours;
        int integerLevel = 2;
        string difficultyLevel = "����";

        while (1) {
            cout << "������ �׸��� �����ϼ���. \n(���� �� : 1, ���� : 2, ���� �ð� : 3, ���̵� : 4, [���� ����] : 5) : ";
            cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (op)
            {
            case 1:
                cout << "���� �� ���� : ";
                getline(cin, lectureTitle, '\n');
                lecture->setLectureTitle(lectureTitle);
                break;
            case 2:
                cout << "���� ���� : ";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                lecture->setPrice(price);
                break;
            case 3:
                cout << "���� �ð� ���� : ";
                cin >> durationHours;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                lecture->setDurationHours(durationHours);
                break;
            case 4:
                cout << "���̵� ����(1 : ����, 2: ����, 3 : �����) : ";
                cin >> integerLevel;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (integerLevel == 1) {
                    difficultyLevel = "����";
                }
                else if (integerLevel == 3) {
                    difficultyLevel = "�����";
                }
                else {
                    difficultyLevel = "����";
                }
                lecture->setDifficultyLevel(difficultyLevel);
                break;
            default:
                break;
            }
            if (op == 5) { break; }
        }
        lectureList[primaryKey] = lecture;
    }
    else {
        cout << "primaryKey �� ��ȸ�� ���ǰ� �����ϴ�." << endl;
    }
}

void LectureManager::addLecture(Lecture* lecture) {
    lectureList.insert({ lecture->getPrimaryKey(), lecture });
}

unsigned long long LectureManager::makePrimaryKey() {
    if (lectureList.size() == 0) {
        // member 1 ~ 10,000,000,000
        // lecture 10,000,000,001 ~ 99,999,999,999
        return 10000000001;
    }
    else {
        auto elem = lectureList.end();
        unsigned long long primaryKey = (--elem)->first;
        if (primaryKey == 99999999999) {
            return -1;
        }
        return ++primaryKey;
    }
}

vector<string> LectureManager::parseCSV(istream& file, char delimiter)
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
map<unsigned long long, Lecture*>& LectureManager::getLectureList() {
    return this->lectureList;
}

void LectureManager::displayMenu()
{
    int ch;
    unsigned long long key;
    bool isContinue = true;
    Lecture* lecture = nullptr;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Lecture Manager                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Koflearn ����(��ǰ) ����Ʈ ���                     " << endl;
        cout << "  2. ���� �߰�                            " << endl;
        cout << "  3. ���� ����                           " << endl;
        cout << "  4. ���� ����                           " << endl;
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
            displayAllLecture();
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 2:
            lecture = inputLecture();
            if (lecture != nullptr) {
                cout << "���� ����� �Ϸ�Ǿ����ϴ�." << endl;
            }
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 3:
            displayAllLecture();
            cout << "   ��� primaryKey �Է� : ";
            cin >> key;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            deleteLecture(key);
            cout << "���� ���� �۾��� ����Ǿ����ϴ�." << endl;
            cout << "[Enter] �� ���� �ڷΰ���" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        case 4:
            displayAllLecture();
            cout << "   ��� primaryKey �Է� : ";
            cin >> key;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            modifyLecture(key);
            cout << "���� ���� �۾��� ����Ǿ����ϴ�." << endl;
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