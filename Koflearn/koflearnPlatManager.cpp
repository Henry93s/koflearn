#include "koflearnPlatManager.h"
#include <sstream>

// 소스 코드 파일(cpp)에서 singleton 인스턴스 초기화
KoflearnPlatManager* KoflearnPlatManager::instance = nullptr;

KoflearnPlatManager* KoflearnPlatManager::getInstance() {
    if (!instance) {
        instance = new KoflearnPlatManager();
    }
    return instance;
}

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

Member* KoflearnPlatManager::getLoginUser() {
    return this->loginUser;
}

void KoflearnPlatManager::setLoginUser(Member* member) {
    this->loginUser = member;
}

bool KoflearnPlatManager::getIs_login() {
    return this->is_login;
}

void KoflearnPlatManager::setIs_login(bool boolean) {
    this->is_login = boolean;
}

bool KoflearnPlatManager::getIs_admin() {
    return this->is_admin;
}

void KoflearnPlatManager::setIs_admin(bool boolean) {
    this->is_admin = boolean;
}

void KoflearnPlatManager::displayMenu() {
    int ch;
    bool isContinue = true;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Main page                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        if (this->getIs_login() == true) {
            cout << this->loginUser->getNickName() + " 님 안녕하세요. 현재 수강중인 강의가 있어요 !" << endl << endl;
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
            if (this->getIs_login() == true) {
                this->myPageManager.displayMenu();
            }
            break;
        case 1:
            if (this->getIs_login() == true) {
                // 로그아웃
                this->setIs_login(false);
                this->loginUser = nullptr;
                this->setIs_admin(false);

                cout << "정상적으로 로그아웃 되었습니다. " << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');
            }
            else {
                // 로그인 화면 이동
                this->loginManager.displayMenu();
            }
            break;
        case 2:
            if (this->getIs_login() == true) {
                this->lectureManager.displayAllLecture();
                this->enrollManager.searchAndStudentEnrollLecture();
            }
            else {
                cout << "로그인 후 강의 조회 및 신청이 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');
            }
            break;
        case 3:
            if (this->getIs_login() == true) {
                this->lectureManager.inputLecture();
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
            if (this->getIs_login() == true && this->getIs_admin() == true) {
                this->memberManager.displayMenu();
            }
            else {
                cout << "멤버(회원) 통합 관리 시스템은 관리자만 접근 가능합니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                while (getchar() != '\n');
            }
            break;
        case 5:
            if (this->getIs_login() == true && this->getIs_admin() == true) {
                this->lectureManager.displayMenu();
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

    // ! 싱글톤 인스턴스를 동적으로 할당했다면, 프로그램 종료 전에 반드시 delete !
    // getInstance() 내부에서 new KoflearnPlatManager()를 사용했으므로, 여기서 delete !!!
    delete this;
}