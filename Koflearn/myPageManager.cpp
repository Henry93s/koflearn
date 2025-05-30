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
    int cnt = 0;

    // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
    map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
    cout << studentLectureList.size() << endl;
    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    for (const auto& i : studentLectureList) {
        if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
            for (const auto& i2 : i.second) {
                i2->displayInfo();
                cnt++;
            }
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
    // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
    map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();

    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    for (const auto& i : instructorLectureList) {
        Lecture* lecture = nullptr;
        if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
            for (const auto& i2 : i.second) {
                i2->displayInfo();
                cnt++;
            }
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
        Member* member = program_interface->getSessionManager().getLoginUser();
        program_interface->getMemberManager().deleteMember(member->getPrimaryKey());
        
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
    unsigned long long instructorPrimaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();

    // 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
    // "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
    // => "댕글링 포인터" 이슈 발생!!
    // * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.


    // lectureList data 제거
    auto& lectureList = program_interface->getLectureManager().getLectureList();
    auto i = lectureList.begin();
    while (i != lectureList.end()) {
        if (i->first == instructorPrimaryKey) {
            // i(iterator) 를 삭제된 다음 위치로 update
            i = lectureList.erase(i);
        }
        else {
            // 조건을 만족하지 않으면 다음 요소로 이동
            i++;
        }
    }

    // instructorLectureList data 제거
    auto& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    auto i2 = instructorLectureList.begin();
    while (i2 != instructorLectureList.end()) {
        if (i2->first == instructorPrimaryKey) {
            // i(iterator) 를 삭제된 다음 위치로 update
            i2 = instructorLectureList.erase(i2);
        }
        else {
            // 조건을 만족하지 않으면 다음 요소로 이동
            i2++;
        }
    }

    // studentLectureList data 제거
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
    auto i3 = studentLectureList.begin();
    while (i3 != studentLectureList.end()) {
        vector<Lecture*>& lecturesOfStudent = i3->second;

        // std::remove_if는 실제 삭제는 하지 않고, 삭제될 요소들을 벡터의 끝으로 이동시키고 새 end 이터레이터를 반환합니다.
        auto new_end = remove_if(lecturesOfStudent.begin(), lecturesOfStudent.end(),
            [instructorPrimaryKey](Lecture* lecture_ptr) {
                return lecture_ptr && lecture_ptr->getPrimaryKey() == instructorPrimaryKey;
            });
        // 실제로 벡터에서 요소 제거
        lecturesOfStudent.erase(new_end, lecturesOfStudent.end());

        // 강좌 삭제 후, 만약 학생의 강의 리스트(vector)가 비어 있다면 해당 학생의 맵 엔트리도 삭제
        if (lecturesOfStudent.empty()) {
            // 맵에서 현재 엔트리를 삭제하고, erase가 반환 및 유효한 이터레이터로 map_it을 업데이트
            i3 = studentLectureList.erase(i3);
        }
        else {
            // 벡터에 아직 강의가 남아있다면, 다음 맵 엔트리로 이동
            ++i3;
        }
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
        cout << "  1. 수강 중인 강의 보기                     " << endl;
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
            while (getchar() != '\n');
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