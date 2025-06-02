#include "myPageManager.h"
#include <string>
// program_interface �� ���ؼ� �����Ϸ��� ��� 
// Manager Ŭ�������� �ʿ��� ��� ������ include �Ǿ� �־�� ��. 
// why? ��ȯ���� ������ IKoflearnPlatManager ���� Manager Ŭ�������� include ���� �ʰ�
//      ���漱�� ó�������Ƿ�
#include "enrollManager.h"
#include "sessionManager.h"
#include "memberManager.h"
#include "lectureManager.h"


// �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
MyPageManager::MyPageManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "����: MyPageManager�� ��ȿ�� IKoflearnPlatManager�� ���Ե��� �ʾҽ��ϴ�!\n";
    }
}
MyPageManager::~MyPageManager() {}

void MyPageManager::myStudentLecturePrint() {
    // �ڵ� ���� : student �� �����ϴ� ���� list �� ã�� �κ��� enrollManager Ŭ���� å������ ����
    unsigned long long primaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    vector<Lecture*>& lectureList = program_interface->getEnrollManager().findStudentLectureAllList(primaryKey);

    if (lectureList.size() == 0) {
        cout << endl;
        cout << "���� ���� ���� ���°� �����ϴ� !" << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        for (const auto& i : lectureList) {
            i->displayInfo();
        }
        cout << endl;
        int ch;
        while (1) {
            cout << "1. �ڷ� ����" << endl;
            cout << "2. ���� �����ϱ�" << endl;
            cin >> ch;
            if (cin.fail()) {
                cout << "�߸��� �Է��Դϴ�. ���ڸ� �Է����ּ���." << endl;
                // ��Ʈ���� ���� ���¸� �ʱ�ȭ
                cin.clear();
                cout << "[Enter] �� ���� �ٽ� �Է��ϱ�" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (ch == 1) {
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            else if (ch == 2) {
                unsigned long long primaryKey;
                cout << "���� ������ ������ key �� �Է��ϼ��� : ";
                cin >> primaryKey;
                exitLecture(primaryKey);
                break;
            }
            else {
                continue;
            }
        }
    }
}

void MyPageManager::exitLecture(unsigned long long primaryKey) {
    Lecture* lecture = program_interface->getLectureManager().searchLecture(primaryKey);
    unsigned long long myPrimaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // ȸ���� ���� ����Ʈ(studentLectureList)���� ������ �� ����
    auto it = studentLectureList.find(myPrimaryKey);
    if (lecture && it != studentLectureList.end()) {
        vector<Lecture*>& lectures = it->second;
        // ���Ϳ��� �ش� ���Ǹ� ã�� ������ �� ���� �� ����
        for (auto vecIt = lectures.begin(); vecIt != lectures.end(); ++vecIt) {
            if (*vecIt && (*vecIt)->getPrimaryKey() == primaryKey) {
                int count = (*vecIt)->getEnrolledStudentsCount();
                (*vecIt)->setEnrolledStudentsCount(count - 1);  // ������ �� ����

                lectures.erase(vecIt);  // ���� ���Ϳ��� ����
                break;  // �ϳ��� �����ϰ� ����(������ ȸ���� �ߺ� ���Ǹ� ������ �� �� ����)
            }
        }

        // �ش� �л��� ���� ����� ������� map������ ����
        if (lectures.empty()) {
            studentLectureList.erase(it);
        }

        cout << lecture->getLectureTitle() << " ���Ǹ� ���� ����ó���Ͽ����ϴ�." << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return;
}

void MyPageManager::myInstructorLecturePrint() {
    // �ڵ� ���� : Instructor �� �����ϴ� ���� list �� ã�� �κ��� enrollManager Ŭ���� å������ ����
    unsigned long long primaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    vector<Lecture*>& lectureList = program_interface->getEnrollManager().findInstructorLectureAllList(primaryKey);

    if (lectureList.size() == 0) {
        cout << endl;
        cout << "���� �����Ͻô� ���°� �����ϴ� !" << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        for (const auto& i : lectureList) {
            i->displayInfo();
        }
        cout << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool MyPageManager::selfDeleteID() {
    string is_delete = "";

    cout << "Ż�� �� �����Ͻô� ����, �����Ͻô� ���� ��� ���ŵ˴ϴ�." << endl;
    cout << "���� Ż���Ͻðڽ��ϱ�?" << endl;
    cout << "Ż���Ͻ÷��� [I agree to withdraw from member] �� [ ] �� ������ ��Ȯ�� �Է����ּ���." << endl;
    getline(cin, is_delete, '\n');

    if (is_delete.compare("I agree to withdraw from member") == 0) {
        Member* member = program_interface->getSessionManager().getLoginUser();
        // Ż���� ���� ������ �켱 ����(lectureList, instructorLectureList, studentLectureList)
        allDeletedUserData(member->getPrimaryKey());
        // Ż���� member ������ ����

        program_interface->getMemberManager().deleteMember(member->getPrimaryKey());

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

void MyPageManager::allDeletedUserData(unsigned long long primaryKey) {
    unsigned long long instructorPrimaryKey = primaryKey;
    Member* member = program_interface->getMemberManager().searchMember(primaryKey);
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

void MyPageManager::displayMenu() {
    int ch;
    bool isContinue = true;
    bool is_delete = false;

    string email, password;
    Member* member = program_interface->getSessionManager().getLoginUser();

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn My Page                  " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. ���� ���� ���� �� ���� ����                     " << endl;
        cout << "  2. �����ϴ� ���� ����                            " << endl;
        cout << "  3. �н����� ����                            " << endl;
        cout << "  4. ȸ�� Ż��                       " << endl;
        cout << "  5. ���� �޴��� ���ư���                " << endl;
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
            this->myStudentLecturePrint();
            break;
        case 2:
            this->myInstructorLecturePrint();
            break;
        case 3:
            program_interface->getMemberManager().modifyMember(member->getPrimaryKey());
            break;
        case 4:
            is_delete = this->selfDeleteID();
            if (is_delete == true) {
                program_interface->getSessionManager().setIs_login(false);
                program_interface->getSessionManager().setIs_admin(false);
                program_interface->getSessionManager().setLoginUser(nullptr);

                isContinue = false;
            }
            break;
        case 5:
            isContinue = false;
            break;
        default:
            break;
        }
    }
    return;

}