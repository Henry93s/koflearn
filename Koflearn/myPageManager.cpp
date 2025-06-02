#include "myPageManager.h"
#include <string>
// program_interface 를 통해서 접근하려는 모든 
// Manager 클래스들이 필요한 헤더 파일이 include 되어 있어야 함. 
// why? 순환참조 방지로 IKoflearnPlatManager 에서 Manager 클래스들을 include 하지 않고
//      전방선언 처리했으므로
#include "enrollManager.h"
#include "sessionManager.h"
#include "memberManager.h"
#include "lectureManager.h"


// 생성자에서 인터페이스 타입 의존성을 주입받음
MyPageManager::MyPageManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }
}
MyPageManager::~MyPageManager() {}

void MyPageManager::myStudentLecturePrint() {
    // 코드 정리 : student 가 수강하는 강의 list 를 찾는 부분은 enrollManager 클래스 책임으로 진행
    unsigned long long primaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    vector<Lecture*>& lectureList = program_interface->getEnrollManager().findStudentLectureAllList(primaryKey);

    if (lectureList.size() == 0) {
        cout << endl;
        cout << "현재 수강 중인 강좌가 없습니다 !" << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "    key      |            Title                 |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        for (const auto& i : lectureList) {
            i->displayInfo();
        }
        cout << endl;
        int ch;
        while (1) {
            cout << "1. 뒤로 가기" << endl;
            cout << "2. 수강 종료하기" << endl;
            cin >> ch;
            if (cin.fail()) {
                cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
                // 스트림의 오류 상태를 초기화
                cin.clear();
                cout << "[Enter] 를 눌러 다시 입력하기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            if (ch == 1) {
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            else if (ch == 2) {
                unsigned long long primaryKey;
                cout << "수강 종료할 강좌의 key 를 입력하세요 : ";
                cin >> primaryKey;
                program_interface->getLectureManager().exitLecture(primaryKey);
                break;
            }
            else {
                continue;
            }
        }
    }
}

void MyPageManager::myInstructorLecturePrint() {
    // 코드 정리 : Instructor 가 수강하는 강의 list 를 찾는 부분은 enrollManager 클래스 책임으로 진행
    unsigned long long primaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    vector<Lecture*>& lectureList = program_interface->getEnrollManager().findInstructorLectureAllList(primaryKey);
    unsigned long long delete_primaryKey = 0;

    if (lectureList.size() == 0) {
        cout << endl;
        cout << "현재 진행하시는 강좌가 없습니다 !" << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "    key      |            Title                 |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        for (const auto& i : lectureList) {
            i->displayInfo();
        }
        cout << endl;
        int ch;
        bool is_deleted = false;
        while (1) {
            cout << "1. 뒤로 가기" << endl;
            cout << "2. 강의 진행 종료하기" << endl;
            cin >> ch;
            if (cin.fail()) {
                cout << "잘못된 입력입니다. 숫자를 입력해주세요." << endl;
                // 스트림의 오류 상태를 초기화
                cin.clear();
                cout << "[Enter] 를 눌러 다시 입력하기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (ch == 1) {
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            }
            else if (ch == 2) {
                cout << "강의 진행을 종료할 강좌의 key 를 입력하세요 : ";
                cin >> delete_primaryKey;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                is_deleted = program_interface->getLectureManager().deleteLectureProcess(delete_primaryKey);
                if (is_deleted == true) {
                    break;
                }
            }
            else {
                continue;
            }
        }
    }
}

void MyPageManager::displayMenu() {
    int ch;
    bool isContinue = true;
    bool is_delete = false;

    string email, password;
    Member* member = program_interface->getSessionManager().getLoginUser();

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "               Koflearn My Page                  " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. 수강 중인 강의 보기 및 수강 종료                     " << endl;
        cout << "  2. 진행하는 강의 보기 및 강의 진행 종료                          " << endl;
        cout << "  3. 패스워드 수정                            " << endl;
        cout << "  4. 회원 탈퇴                       " << endl;
        cout << "  5. 메인 메뉴로 돌아가기                " << endl;
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
        case 1:
            this->myStudentLecturePrint();
            break;
        case 2:
            this->myInstructorLecturePrint();
            break;
        case 3:
            program_interface->getMemberManager().modifyMember(member->getPrimaryKey());
            break;
        case 4:
            is_delete = program_interface->getMemberManager().deleteUserProcess(member->getPrimaryKey());
            if (is_delete == true) {
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