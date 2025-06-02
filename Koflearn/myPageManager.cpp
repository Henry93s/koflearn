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
        cout << "    key      |            Title                 |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
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
                program_interface->getLectureManager().exitLecture(primaryKey);
                break;
            }
            else {
                continue;
            }
        }
    }
}

void MyPageManager::myInstructorLecturePrint() {
    // �ڵ� ���� : Instructor �� �����ϴ� ���� list �� ã�� �κ��� enrollManager Ŭ���� å������ ����
    unsigned long long primaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    vector<Lecture*>& lectureList = program_interface->getEnrollManager().findInstructorLectureAllList(primaryKey);
    unsigned long long delete_primaryKey = 0;

    if (lectureList.size() == 0) {
        cout << endl;
        cout << "���� �����Ͻô� ���°� �����ϴ� !" << endl;
        cout << "[Enter] �� ���� �ڷΰ���" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "    key      |            Title                 |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        for (const auto& i : lectureList) {
            i->displayInfo();
        }
        cout << endl;
        int ch;
        bool is_deleted = false;
        while (1) {
            cout << "1. �ڷ� ����" << endl;
            cout << "2. ���� ���� �����ϱ�" << endl;
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
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1) {
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            else if (ch == 2) {
                cout << "���� ������ ������ ������ key �� �Է��ϼ��� : ";
                cin >> delete_primaryKey;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                is_deleted = program_interface->getLectureManager().deleteLectureProcess(delete_primaryKey);
                if (is_deleted == true) {
                    break;
                }
            }
            else {
                continue;
            }
        }
    }
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
        cout << "  1. ���� ���� ���� ���� �� ���� ����                     " << endl;
        cout << "  2. �����ϴ� ���� ���� �� ���� ���� ����                          " << endl;
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
            is_delete = program_interface->getMemberManager().deleteUserProcess(member->getPrimaryKey());
            if (is_delete == true) {
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