#include "koflearnPlatManager.h"
#include <sstream>

KoflearnPlatManager::KoflearnPlatManager()
    : // �ʱ�ȭ ����Ʈ���� 'this' �����͸� ����Ͽ� �� �Ŵ����� �����ڿ� ������ ����
    memberManager(this),
    lectureManager(this),
    loginManager(this),
    enrollManager(this),
    myPageManager(this),
    sessionManager(this)
{
};

MemberManager& KoflearnPlatManager::getMemberManager() {
    return this->memberManager;
}

LectureManager& KoflearnPlatManager::getLectureManager() {
    return this->lectureManager;
}

LoginManager& KoflearnPlatManager::getLoginManager() {
    return this->loginManager;
}

MyPageManager& KoflearnPlatManager::getMyPageManager() {
    return this->myPageManager;
}

EnrollManager& KoflearnPlatManager::getEnrollManager() {
    return this->enrollManager;
}

SessionManager& KoflearnPlatManager::getSessionManager() {
    return this->sessionManager;
}

void KoflearnPlatManager::displayMenu(IKoflearnPlatManager* program) {
    int ch;
    bool isContinue = true;
    Lecture* lecture = nullptr;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Main page                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        if (program->getSessionManager().getIs_login() == true) {
            string welcomeText = program->getEnrollManager().makeWelcomeText();
            cout << program->getSessionManager().getLoginUser()->getNickName() + welcomeText << endl << endl;
            cout << "  0. ����������                            " << endl;
            cout << "  1. �α׾ƿ�                              " << endl;
        }
        else {
            cout << "  1. �α��� / ȸ������                            " << endl;
        }
        cout << "  2. ���� ��ȸ / ��û �ϱ� [�α��� �ʿ�]                   " << endl;
        cout << "  3. ���� ��� �ϱ� [�α��� �ʿ�]                   " << endl;
        cout << "  4. Koflearn ���(ȸ��) ���� �ý��� [������ ���� �ʿ�]                           " << endl;
        cout << "  5. Koflearn ����(��ǰ) ���� �ý��� [������ ���� �ʿ�]                          " << endl;
        cout << "  6. ����                              " << endl;
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
        case 0:
            if (program->getSessionManager().getIs_login() == true) {
                this->myPageManager.displayMenu();
            }
            break;
        case 1:
            if (program->getSessionManager().getIs_login() == true) {
                // �α׾ƿ�
                program->getSessionManager().setIs_login(false);
                program->getSessionManager().setLoginUser(nullptr);
                program->getSessionManager().setIs_admin(false);

                cout << "���������� �α׾ƿ� �Ǿ����ϴ�. " << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                // �α��� ȭ�� �̵�
                program->getLoginManager().displayMenu();
            }
            break;
        case 2:
            if (program->getSessionManager().getIs_login() == true) {
                program->getLectureManager().displayAllLecture();
                program->getEnrollManager().searchAndStudentEnrollLecture();
            }
            else {
                cout << "�α��� �� ���� ��ȸ �� ��û�� �����մϴ�." << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 3:
            if (program->getSessionManager().getIs_login() == true) {
                lecture = program->getLectureManager().inputLecture();
                if (lecture != nullptr) {
                    cout << "���Ǹ� �ű� ����Ͽ����ϴ�." << endl;
                }
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                cout << "�α��� �� ���� ����� �����մϴ�." << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 4:
            if (program->getSessionManager().getIs_login() == true && program->getSessionManager().getIs_admin() == true) {
                program->getMemberManager().displayMenu();
            }
            else {
                cout << "���(ȸ��) ���� ���� �ý����� �����ڸ� ���� �����մϴ�." << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 5:
            if (program->getSessionManager().getIs_login() == true && program->getSessionManager().getIs_admin() == true) {
                program->getLectureManager().displayMenu();
            }
            else {
                cout << "����(��ǰ) ���� ���� �ý����� �����ڸ� ���� �����մϴ�." << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 6:
            isContinue = false;
            break;
        default:
            break;
        }
    }
}