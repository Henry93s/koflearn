// koflearnPlatManager.h 에 loginManager.h 포함
#include "koflearnPlatManager.h"

#include <string>

LoginManager::LoginManager() {}
LoginManager::~LoginManager() {}

KoflearnPlatManager* LoginManager::getInstance() const{
	return KoflearnPlatManager::getInstance();
}

void LoginManager::displayMenu()
{
    int ch;
    bool isContinue = true;
    
    string email, password;
    Member* loginMember = nullptr;
    Member* member = nullptr;
    KoflearnPlatManager* program = getInstance();
    Member* joinMember = nullptr;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn Login                  " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. 로그인                     " << endl;
        cout << "  2. 회원 가입                            " << endl;
        cout << "  3. 메인 메뉴로 돌아가기                       " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;
        cin.ignore();

        switch (ch) {
        case 1:
            cout << "ID : ";
            getline(cin, email, '\n');
            loginMember = program->getMemberManager().searchMember(email);
            if (loginMember == nullptr) {
                cout << "가입되지 않은 이메일입니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');
            }
            else {
                cout << "비밀번호 : ";
                getline(cin, password, '\n');
                if (password != loginMember->getPassword()) {
                    cout << "패스워드를 다시 확인해주세요." << endl;
                    cout << "[Enter] 를 눌러 뒤로가기" << endl;
                    while (getchar() != '\n');
                }
                else {
                    cout << loginMember->getNickName() + " 님 환영합니다." << endl;
                    program->setIs_login(true);
                    program->setLoginUser(loginMember);

                    if (loginMember->getIsManager() == "true") {
                        program->setIs_admin(true);
                    }
                    cout << "[Enter] 를 눌러 메인 페이지로 이동" << endl;
                    while (getchar() != '\n');
                    isContinue = false;
                }
            }
            break;
        case 2: 
            joinMember = program->getMemberManager().inputMember();
            if (joinMember != nullptr)  {
                cout << "회원 가입이 완료되었습니다." << endl;
                program->setIs_login(true);
                program->setLoginUser(joinMember);
                member = program->getLoginUser();
                if (member->getIsManager() == "true") {
                    program->setIs_admin(true);
                }
                cout << "[Enter] 를 눌러 메인 페이지로 자동 로그인됩니다." << endl;
                while (getchar() != '\n');
                isContinue = false;
            }
            else {
                cout << "회원 가입이 취소되었습니다." << endl;
                cout << "[Enter] 를 눌러 메인 페이지로 돌아가기" << endl;
                while (getchar() != '\n');
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