#include "loginManager.h"
// program_interface �� ���ؼ� �����Ϸ��� ��� 
// Manager Ŭ�������� �ʿ��� ��� ������ include �Ǿ� �־�� ��. 
// why? ��ȯ���� ������ IKoflearnPlatManager ���� Manager Ŭ�������� include ���� �ʰ�
//      ���漱�� ó�������Ƿ�
#include "memberManager.h"
#include "sessionManager.h"

#include <string>
using namespace std;

// �����ڿ��� �������̽� Ÿ���� �������� ���Թ���
LoginManager::LoginManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "����: MyPageManager�� ��ȿ�� IKoflearnPlatManager�� ���Ե��� �ʾҽ��ϴ�!\n";
    }
}
LoginManager::~LoginManager() {}

void LoginManager::displayMenu()
{
    int ch;
    bool isContinue = true;

    string email, password;
    Member* loginMember = nullptr;
    Member* member = nullptr;
    Member* joinMember = nullptr;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn Login                  " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. �α���                     " << endl;
        cout << "  2. ȸ�� ����                            " << endl;
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
            cout << "ID : ";
            getline(cin, email, '\n');
            loginMember = program_interface->getMemberManager().searchMember(email);
            if (loginMember == nullptr) {
                cout << "���Ե��� ���� �̸����Դϴ�." << endl;
                cout << "[Enter] �� ���� �ڷΰ���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                cout << "��й�ȣ : ";
                getline(cin, password, '\n');
                if (password != loginMember->getPassword()) {
                    cout << "�н����带 �ٽ� Ȯ�����ּ���." << endl;
                    cout << "[Enter] �� ���� �ڷΰ���" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                else {
                    cout << loginMember->getNickName() + " �� ȯ���մϴ�." << endl;
                    program_interface->getSessionManager().setIs_login(true);
                    program_interface->getSessionManager().setLoginUser(loginMember);

                    if (loginMember->getIsManager() == "true") {
                        program_interface->getSessionManager().setIs_admin(true);
                    }
                    cout << "[Enter] �� ���� ���� �������� �̵�" << endl;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    isContinue = false;
                }
            }
            break;
        case 2:
            joinMember = program_interface->getMemberManager().inputMember();
            if (joinMember != nullptr) {
                cout << "ȸ�� ������ �Ϸ�Ǿ����ϴ�." << endl;
                program_interface->getSessionManager().setIs_login(true);
                program_interface->getSessionManager().setLoginUser(joinMember);
                member = program_interface->getSessionManager().getLoginUser();
                if (member->getIsManager() == "true") {
                    program_interface->getSessionManager().setIs_admin(true);
                }
                cout << "[Enter] �� ���� ���� �������� �ڵ� �α��ε˴ϴ�." << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                isContinue = false;
            }
            else {
                cout << "ȸ�� ������ ��ҵǾ����ϴ�." << endl;
                cout << "[Enter] �� ���� ���� �������� ���ư���" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                isContinue = false;
            }
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