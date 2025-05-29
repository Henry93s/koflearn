// koflearnPlatManager.h 에 myPageManager.h 가 포함되어 있음
#include "koflearnPlatManager.h"

#include <string>

MyPageManager::MyPageManager() {}
MyPageManager::~MyPageManager() {}

KoflearnPlatManager* MyPageManager::getInstance() const {
	return KoflearnPlatManager::getInstance();
}

void MyPageManager::displayMenu() {
    int ch;
    bool isContinue = true;

    string email, password;
    Member* member = nullptr;
    KoflearnPlatManager* program = getInstance();

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn My Page                  " << endl;
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
           
        default:
            break;
        }
    }
    return;

}