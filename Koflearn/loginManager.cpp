#include "loginManager.h"
// program_interface 를 통해서 접근하려는 모든 
// Manager 클래스들이 필요한 헤더 파일이 include 되어 있어야 함. 
// why? 순환참조 방지로 IKoflearnPlatManager 에서 Manager 클래스들을 include 하지 않고
//      전방선언 처리했으므로
#include "memberManager.h"
#include "sessionManager.h"

#include <string>
using namespace std;

// 생성자에서 인터페이스 타입의 의존성을 주입받음
LoginManager::LoginManager(IKoflearnPlatManager* program) 
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
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
            loginMember = program_interface->getMemberManager().searchMember(email);
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
                    program_interface->getSessionManager().setIs_login(true);
                    program_interface->getSessionManager().setLoginUser(loginMember);

                    if (loginMember->getIsManager() == "true") {
                        program_interface->getSessionManager().setIs_admin(true);
                    }
                    cout << "[Enter] 를 눌러 메인 페이지로 이동" << endl;
                    while (getchar() != '\n');

                    isContinue = false;
                }
            }
            break;
        case 2: 
            joinMember = program_interface->getMemberManager().inputMember();
            if (joinMember != nullptr)  {
                cout << "회원 가입이 완료되었습니다." << endl;
                program_interface->getSessionManager().setIs_login(true);
                program_interface->getSessionManager().setLoginUser(joinMember);
                member = program_interface->getSessionManager().getLoginUser();
                if (member->getIsManager() == "true") {
                    program_interface->getSessionManager().setIs_admin(true);
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