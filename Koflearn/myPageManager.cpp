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
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
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
                exitLecture(primaryKey);
                break;
            }
            else {
                continue;
            }
        }
    }
}

void MyPageManager::exitLecture(unsigned long long primaryKey) {
    Lecture* lecture = program_interface->getLectureManager().searchLecture(primaryKey);
    unsigned long long myPrimaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 회원의 수강 리스트(studentLectureList)에서 수강자 수 감소
    auto it = studentLectureList.find(myPrimaryKey);
    if (lecture && it != studentLectureList.end()) {
        vector<Lecture*>& lectures = it->second;
        // 벡터에서 해당 강의를 찾아 수강자 수 감소 및 제거
        for (auto vecIt = lectures.begin(); vecIt != lectures.end(); ++vecIt) {
            if (*vecIt && (*vecIt)->getPrimaryKey() == primaryKey) {
                int count = (*vecIt)->getEnrolledStudentsCount();
                (*vecIt)->setEnrolledStudentsCount(count - 1);  // 수강자 수 감소

                lectures.erase(vecIt);  // 강의 벡터에서 제거
                break;  // 하나만 삭제하고 종료(어차피 회원이 중복 강의를 수강할 수 는 없음)
            }
        }

        // 해당 학생의 수강 목록이 비었으면 map에서도 삭제
        if (lectures.empty()) {
            studentLectureList.erase(it);
        }

        cout << lecture->getLectureTitle() << " 강의를 수강 종료처리하였습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return;
}

void MyPageManager::myInstructorLecturePrint() {
    // 코드 정리 : Instructor 가 수강하는 강의 list 를 찾는 부분은 enrollManager 클래스 책임으로 진행
    unsigned long long primaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    vector<Lecture*>& lectureList = program_interface->getEnrollManager().findInstructorLectureAllList(primaryKey);

    if (lectureList.size() == 0) {
        cout << endl;
        cout << "현재 진행하시는 강좌가 없습니다 !" << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        for (const auto& i : lectureList) {
            i->displayInfo();
        }
        cout << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

bool MyPageManager::selfDeleteID() {
    string is_delete = "";

    cout << "탈퇴 시 수강하시는 강의, 강의하시는 강좌 모두 제거됩니다." << endl;
    cout << "정말 탈퇴하시겠습니까?" << endl;
    cout << "탈퇴하시려면 [I agree to withdraw from member] 의 [ ] 안 문구를 정확히 입력해주세요." << endl;
    getline(cin, is_delete, '\n');

    if (is_delete.compare("I agree to withdraw from member") == 0) {
        Member* member = program_interface->getSessionManager().getLoginUser();
        // 탈퇴자 관련 데이터 우선 제거(lectureList, instructorLectureList, studentLectureList)
        allDeletedUserData(member->getPrimaryKey());
        // 탈퇴자 member 데이터 제거
        
        program_interface->getMemberManager().deleteMember(member->getPrimaryKey());
        
        cout << "회원 탈퇴가 정상적으로 되었습니다." << endl;
        cout << "로그인 상태가 해제되고 [Enter] 를 누르면 메인 페이지로 이동합니다." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
    else {
        cout << "문구를 정상적으로 입력하지 않아 회원 탈퇴를 진행할 수 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
}

void MyPageManager::allDeletedUserData(unsigned long long primaryKey) {
    unsigned long long instructorPrimaryKey = primaryKey;
    Member* member = program_interface->getMemberManager().searchMember(primaryKey);
    if (member == nullptr) {
        return;
    }

    string name = member->getNickName();

    auto& lectureList = program_interface->getLectureManager().getLectureList();
    auto& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 1. 해당 강사가 개설한 강의 리스트 수집 -> lectureList 제거
    vector<Lecture*> lecturesToDelete;
    for (auto it = lectureList.begin(); it != lectureList.end(); ) {
        Lecture* lec = it->second;
        if (lec && lec->getInstructorName() == name) {
            lecturesToDelete.push_back(lec);
            it = lectureList.erase(it);
        }
        else {
            ++it;
        }
    }

    // 2. instructorLectureList 에서 해당 강사 항목 제거
    instructorLectureList.erase(instructorPrimaryKey);

    // 3. 회원이 수강자(Student)인 경우 -> 본인의 수강 리스트(studentLectureList)에서 수강자 수 감소 + 제거
    auto it = studentLectureList.find(primaryKey);
    if (it != studentLectureList.end()) {
        vector<Lecture*>& lectures = it->second;
        for (Lecture* lec : lectures) {
            if (lec) {
                int count = lec->getEnrolledStudentsCount();
                lec->setEnrolledStudentsCount(count - 1);
            }
        }
        studentLectureList.erase(it);
    }

    return;
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
        cout << "  1. 수강 중인 강의 및 수강 종료                     " << endl;
        cout << "  2. 진행하는 강의 보기                            " << endl;
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
            is_delete = this->selfDeleteID();
            if (is_delete == true) {
                program_interface->getSessionManager().setIs_login(false);
                program_interface->getSessionManager().setIs_admin(false);
                program_interface->getSessionManager().setLoginUser(nullptr);

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