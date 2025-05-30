#include "koflearnPlatManager.h"
#include <sstream>

KoflearnPlatManager::KoflearnPlatManager()
    : // 초기화 리스트에서 'this' 포인터를 사용하여 각 매니저의 생성자에 의존성 주입
    memberManager(this),
    lectureManager(this),
    loginManager(this),
    enrollManager(this),
    myPageManager(this),
    sessionManager(this)
{};

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

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Main page                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        if (program->getSessionManager().getIs_login() == true) {
            cout << program->getSessionManager().getLoginUser()->getNickName() + " 님 안녕하세요. 현재 수강중인 강의가 있어요 !" << endl << endl;
            cout << "  0. 마이페이지                            " << endl;
            cout << "  1. 로그아웃                              " << endl;
        }
        else {
            cout << "  1. 로그인 / 회원가입                            " << endl;
        }
        cout << "  2. 강의 조회 / 신청 하기 [로그인 필요]                   " << endl;
        cout << "  3. 강의 등록 하기 [로그인 필요]                   " << endl;
        cout << "  4. Koflearn 멤버(회원) 관리 시스템 [관리자 접근 필요]                           " << endl;
        cout << "  5. Koflearn 강의(제품) 관리 시스템 [관리자 접근 필요]                          " << endl;
        cout << "  6. 종료                              " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;
        cin.ignore();

        switch (ch) {
        case 0:
            if (program->getSessionManager().getIs_login() == true) {
                this->myPageManager.displayMenu();
            }
            break;
        case 1:
            if (program->getSessionManager().getIs_login() == true) {
                // 로그아웃
                program->getSessionManager().setIs_login(false);
                program->getSessionManager().setLoginUser(nullptr);
                program->getSessionManager().setIs_admin(false);

                cout << "정상적으로 로그아웃 되었습니다. " << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');

            }
            else {
                // 로그인 화면 이동
                program->getLoginManager().displayMenu();
            }
            break;
        case 2:
            if (program->getSessionManager().getIs_login() == true) {
                program->getLectureManager().displayAllLecture();
                program->getEnrollManager().searchAndStudentEnrollLecture();
            }
            else {
                cout << "로그인 후 강의 조회 및 신청이 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');

            }
            break;
        case 3:
            if (program->getSessionManager().getIs_login() == true) {
                program->getLectureManager().inputLecture();
                cout << "강의를 신규 등록하였습니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');

            }
            else {
                cout << "로그인 후 강의 등록이 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');

            }
            break;
        case 4:
            if (program->getSessionManager().getIs_login() == true && program->getSessionManager().getIs_admin() == true) {
                program->getMemberManager().displayMenu();
            }
            else {
                cout << "멤버(회원) 통합 관리 시스템은 관리자만 접근 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');

            }
            break;
        case 5:
            if (program->getSessionManager().getIs_login() == true && program->getSessionManager().getIs_admin() == true) {
                program->getLectureManager().displayMenu();
            }
            else {
                cout << "강의(제품) 통합 관리 시스템은 관리자만 접근 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');

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