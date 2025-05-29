// koflearnPlatManager.h 에 myPageManager.h 가 포함되어 있음
#include "koflearnPlatManager.h"

#include <string>

MyPageManager::MyPageManager() {}
MyPageManager::~MyPageManager() {}

KoflearnPlatManager* MyPageManager::getInstance() const {
	return KoflearnPlatManager::getInstance();
}

void MyPageManager::myStudentLecturePrint() {
    int cnt = 0;
    for (const auto& i : getInstance()->getEnrollManager().studentLectureList) {
        Lecture* lecture = nullptr;
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        if (i.first->getNickName() == getInstance()->getLoginUser()->getNickName()) {
            lecture = i.second;
            lecture->displayInfo();
            cnt++;
        }
    }
    if (cnt == 0) {
        cout << endl;
        cout << "현재 수강 중인 강좌가 없습니다 !" << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
    }
    else {
        cout << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
    }
}

void MyPageManager::myInstructorLecturePrint() {
    int cnt = 0;
    for (const auto& i : getInstance()->getEnrollManager().instructorLectureList) {
        Lecture* lecture = nullptr;
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        if (i.first == getInstance()->getLoginUser()) {
            lecture = i.second;
            lecture->displayInfo();
            cnt++;
        }
    }
    if (cnt == 0) {
        cout << endl;
        cout << "현재 진행하시는 강좌가 없습니다 !" << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
    }
    else {
        cout << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
    }
}

void MyPageManager::displayMenu() {
    int ch;
    bool isContinue = true;

    string email, password;
    KoflearnPlatManager* program = getInstance();
    // mypage 는 이미 로그인한 유저만 들어올 수 있으므로 getLoginUser 로 바로 받음
    Member* loginUser = program->getLoginUser();

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn My Page                  " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. 수강 중인 강의 보기                     " << endl;
        cout << "  2. 진행하는 강의 보기                            " << endl;
        cout << "  3. 메인 메뉴로 돌아가기                       " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;
        cin.ignore();

        switch (ch) {
        case 1:
            this->myStudentLecturePrint();
            break;
        case 2:
            this->myInstructorLecturePrint();
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