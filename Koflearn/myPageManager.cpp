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
    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    for (const auto& i : getInstance()->getEnrollManager().studentLectureList) {
        Lecture* lecture = nullptr;
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
    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    for (const auto& i : getInstance()->getEnrollManager().instructorLectureList) {
        Lecture* lecture = nullptr;
        if (i.first->getNickName() == getInstance()->getLoginUser()->getNickName()) {
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

bool MyPageManager::selfDeleteID() {
    KoflearnPlatManager* program = getInstance();
    string is_delete = "";

    cout << "탈퇴 시 수강하시는 강의, 강의하시는 강좌 모두 제거됩니다." << endl;
    cout << "정말 탈퇴하시겠습니까?" << endl;
    cout << "탈퇴하시려면 [I agree to withdraw from member] 의 [ ] 안 문구를 정확히 입력해주세요." << endl;
    getline(cin, is_delete, '\n');
    cout << is_delete << endl;
    if (is_delete.compare("I agree to withdraw from member") == 0) {
        // 탈퇴자 관련 데이터 우선 제거(lectureList, instructorLectureList, studentLectureList)
        allDeletedUserData();
        // 탈퇴자 member 데이터 제거
        program->getMemberManager().deleteMember(program->getLoginUser()->getPrimaryKey());
        
        cout << "회원 탈퇴가 정상적으로 되었습니다." << endl;
        cout << "로그인 상태가 해제되고 [Enter] 를 누르면 메인 페이지로 이동합니다." << endl;
        while (getchar() != '\n');
        return true;
    }
    else {
        cout << "문구를 정상적으로 입력하지 않아 회원 탈퇴를 진행할 수 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
        return false;
    }
}

void MyPageManager::allDeletedUserData() {
    KoflearnPlatManager* program = getInstance();
    string instructorName = program->getLoginUser()->getNickName();

    // lectureList data 제거
    auto i = program->getLectureManager().lectureList.begin();
    while (i != program->getLectureManager().lectureList.end()) {
        if (i->second->getInstructorName().compare(instructorName) == 0) {
            // i(iterator) 를 삭제된 다음 위치로 update
            i = program->getLectureManager().lectureList.erase(i);
        }
        else {
            // 조건을 만족하지 않으면 다음 요소로 이동
            i++;
        }
    }

    // instructorLectureList data 제거
    auto i2 = program->getEnrollManager().instructorLectureList.begin();
    while (i2 != program->getEnrollManager().instructorLectureList.end()) {
        if (i2->first->getNickName().compare(instructorName) == 0) {
            // i(iterator) 를 삭제된 다음 위치로 update
            i2 = program->getEnrollManager().instructorLectureList.erase(i2);
        }
        else {
            // 조건을 만족하지 않으면 다음 요소로 이동
            i2++;
        }
    }

    // studentLectureList data 제거
    auto i3 = program->getEnrollManager().studentLectureList.begin();

    while (i3 != program->getEnrollManager().studentLectureList.end()) {
        if (i3->first->getNickName().compare(instructorName) == 0) {
            // i(iterator) 를 삭제된 다음 위치로 update
            i3 = program->getEnrollManager().studentLectureList.erase(i3);
        }
        else {
            // 조건을 만족하지 않으면 다음 요소로 이동
            i3++;
        }
    }

    return;
}

void MyPageManager::displayMenu() {
    int ch;
    bool isContinue = true;
    bool is_delete = false;

    string email, password;
    KoflearnPlatManager* program = getInstance();

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn My Page                  " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. 수강 중인 강의 보기                     " << endl;
        cout << "  2. 진행하는 강의 보기                            " << endl;
        cout << "  3. 패스워드 수정                            " << endl;
        cout << "  4. 회원 탈퇴                       " << endl;
        cout << "  5. 메인 메뉴로 돌아가기                " << endl;
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
            program->getMemberManager().modifyMember(program->getLoginUser()->getPrimaryKey());
            break;
        case 4:
            is_delete = this->selfDeleteID();
            if (is_delete == true) {
                program->setIs_login(false);
                program->setIs_admin(false);
                program->setLoginUser(nullptr);

                isContinue = false;
            }
            break;
        case 5:
            isContinue = false;
            break;
        default:
            break;
        }
    }
    return;

}