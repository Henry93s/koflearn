#include "koflearnPlatManager.h"

// 소스 코드 파일(cpp)에서 singleton 인스턴스 초기화
KoflearnPlatManager* KoflearnPlatManager::instance = nullptr;

KoflearnPlatManager* KoflearnPlatManager::getInstance() {
    if (!instance)
        instance = new KoflearnPlatManager();
    return instance;
}

MemberManager& KoflearnPlatManager::getMemberManager() {
    return this->memberManager;
}

LectureManager& KoflearnPlatManager::getLectureManager() {
    return this->lectureManager;
}

bool KoflearnPlatManager::getIs_login() {
    return this->is_login;
}

void KoflearnPlatManager::setIs_login(bool boolean) {
    this->is_login = boolean;
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
        cout << "  2. 모든 강의 보기                        " << endl;
        cout << "  3. 신청한 강의 보기                         " << endl;
        cout << "  4. 강의 등록하기                     " << endl;
        cout << "  5. Koflearn 멤버(회원) 관리 프로그램                           " << endl;
        cout << "  6. Koflearn 강의(제품) 관리 프로그램                           " << endl;
        cout << "  7. 종료                              " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;
        cin.ignore();

        switch (ch) {
        case 0:
            if (this->getIs_login() == true) {
                cout << "마이페이지 테스트" << endl;
                cout << "[Enter] 를 눌러 메인 페이지로 돌아가기" << endl;
                while (getchar() != '\n');
            }
            break;
        case 1:
            if (this->getIs_login() == true) {
                // 로그아웃
                this->setIs_login(false);
                this->loginUser = nullptr;
                cout << "정상적으로 로그아웃 되었습니다. " << endl;
                cout << "[Enter] 를 눌러 메인 페이지로 돌아가기" << endl;
                while (getchar() != '\n');
            }
            else {
                // 로그인 화면 이동
                this->loginManager.displayMenu();
            }
            break;
        case 2:
            // all lecture(); -> Order(강의 구매) 클래스 의 all_displaymenu 로 진입 -> 강의 수강 신청 가능 메소드 추가 예정
            break;
        case 3:
            // my lecture(); (로그인 시 접근 가능 메뉴) -> order 클래스의 my_displaymenu 로 진입
            break;
        case 4:
            // upload lecture(); (로그인 시 접근 가능 메뉴) -> order 클래스의 have_displaymenu 로 진입
            break;
        case 5:
            this->memberManager.displayMenu();
            break;
        case 6:
            this->lectureManager.displayMenu();
            break;
        case 7:
            isContinue = false;
            break;
        default:
            break;
        }
    }

    // ! 싱글톤 인스턴스를 동적으로 할당했다면, 프로그램 종료 전에 반드시 delete !
    // getInstance() 내부에서 new KoflearnPlatManager()를 사용했으므로, 여기서 delete 해야 합니다.
    delete this;
}