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
    bool isDisplay = false;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Main page                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        if (program->getSessionManager().getIs_login() == true) {
            string welcomeText = program->getEnrollManager().makeWelcomeText();
            cout << program->getSessionManager().getLoginUser()->getNickName() + welcomeText << endl << endl;
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

        // 메뉴에서 숫자 명령어를 받으려고 할 때 영문자 등을 입력했을 때 
        // 무한 깜빡임 현상 해결
        if (cin.fail()) {
            cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
            // 스트림의 오류 상태를 초기화
            cin.clear();
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            // 버퍼의 최대 크기, '\n'은 버퍼를 비울 때까지 찾을 문자
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        // 버퍼의 최대 크기, '\n'은 버퍼를 비울 때까지 찾을 문자
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                // 로그인 화면 이동
                program->getLoginManager().displayMenu();
            }
            break;
        case 2:
            if (program->getSessionManager().getIs_login() == true) {
                // displayAllLecture 처음 조회 시 최대 50 개의 강의만 출력됨
                // 이후 조회는 검색을 통해 진행해야함
                isDisplay = program->getLectureManager().displayAllLecture();
                // 전체 강의가 한 개라도 있을 때만 search Lecture 단계로 진행된다.
                if (isDisplay == true) {
                    program->getEnrollManager().searchAndStudentEnrollLecture();
                }
            }
            else {
                cout << "로그인 후 강의 조회 및 신청이 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 3:
            if (program->getSessionManager().getIs_login() == true) {
                lecture = program->getLectureManager().inputLecture();
                if (lecture != nullptr) {
                    cout << "강의를 신규 등록하였습니다." << endl;
                }
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                cout << "로그인 후 강의 등록이 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 4:
            if (program->getSessionManager().getIs_login() == true && program->getSessionManager().getIs_admin() == true) {
                program->getMemberManager().displayMenu();
            }
            else {
                cout << "멤버(회원) 통합 관리 시스템은 관리자만 접근 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 5:
            if (program->getSessionManager().getIs_login() == true && program->getSessionManager().getIs_admin() == true) {
                program->getLectureManager().displayMenu();
            }
            else {
                cout << "강의(제품) 통합 관리 시스템은 관리자만 접근 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
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