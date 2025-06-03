#include "enrollManager.h"
// program_interface 를 통해서 접근하려는 모든 
// Manager 클래스들이 필요한 헤더 파일이 include 되어 있어야 함. 
// why? 순환참조 방지로 IKoflearnPlatManager 에서 Manager 클래스들을 include 하지 않고
//      전방선언 처리했으므로
#include "lectureManager.h"
#include "sessionManager.h"
#include "memberManager.h"

#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// 생성자에서 인터페이스 타입의 의존성을 주입받음
EnrollManager::EnrollManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }

    ifstream file1, file2;
    file1.open("studentLectureList.csv");
    char* endptr;
    char* endptr2;
    if (!file1.fail()) {
        while (!file1.eof()) {
            vector<string> row = parseCSV(file1, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr, 10);

                // memberList 와 포인터 동일 유지하기 위한 new 가 아닌 memberList 에서 주소를 가져와야 한다.
                Member* member = program_interface->getMemberManager().searchMember(primaryKey);

                // 6 index 부터
                // lectureList 와 포인터 동일 유지하기 위한 new 가 아닌 lectureList 에서 주소를 가져와야 한다.
                unsigned long long primaryKey2 = strtoull(row[6].c_str(), &endptr2, 10);
                Lecture* lecture = program_interface->getLectureManager().searchLecture(primaryKey2);

                // map<unsigned long long, vector<Lecture*>> 컨테이너 사용
                // 
                /* map<unsigned long long, Lecture*> 형태일 경우
                한 학생은 하나의 강의 밖에 수강하지 못함 : 한 key 는 고유하기 때문에 중복해서
                같은 key 를 삽입할 수 없음.
                */

                // 해당 학생의 primaryKey가 맵에 존재하는지 확인
                auto it = studentLectureList.find(member->getPrimaryKey());

                if (it != studentLectureList.end()) {
                    // 이미 존재하는 경우, 해당 vector<Lecture*>에 새로운 강의를 push_back
                    it->second.push_back(lecture);
                }
                else {
                    // 존재하지 않는 경우, 새로운 엔트리를 추가하고 vector에 강의를 넣음
                    studentLectureList.insert({ member->getPrimaryKey(), { lecture } });
                }
            }
        } 
    }
    file1.close();

    file2.open("instructorLectureList.csv");
    char* endptr3;
    char* endptr4;
    if (!file2.fail()) {
        while (!file2.eof()) {
            vector<string> row = parseCSV(file2, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr3, 10);

                // memberList 와 포인터 동일 유지하기 위한 new 가 아닌 memberList 에서 주소를 가져와야 한다.
                Member* member = program_interface->getMemberManager().searchMember(primaryKey);

                // 6 index 부터
                // lectureList 와 포인터 동일 유지하기 위한 new 가 아닌 lectureList 에서 주소를 가져와야 한다.
                unsigned long long primaryKey2 = strtoull(row[6].c_str(), &endptr4, 10);
                Lecture* lecture = program_interface->getLectureManager().searchLecture(primaryKey2);

                // 해당 강사의 primaryKey가 맵에 존재하는지 확인
                auto it = instructorLectureList.find(member->getPrimaryKey());

                if (it != instructorLectureList.end()) {
                    // 이미 존재하는 경우, 해당 vector<Lecture*>에 새로운 강의를 push_back
                    it->second.push_back(lecture);
                }
                else {
                    // 존재하지 않는 경우, 새로운 엔트리를 추가하고 vector에 강의를 넣음
                    instructorLectureList.insert({ member->getPrimaryKey(), { lecture } });
                }
            }
        }
    }
    file2.close();
}
EnrollManager::~EnrollManager() {
    ofstream file1, file2;
    file1.open("studentLectureList.csv");
    file2.open("instructorLectureList.csv");

    if (!file1.fail()) {
        for (const auto& v : studentLectureList) {
            Member* member = program_interface->getMemberManager().searchMember(v.first);
            // map<unsigned long long, vector<Lecture*>> 에서 vector<Lecture*> 를 먼저 얻고
            // for 문 순환 필요함
            for (Lecture* lecture : v.second) {
                file1 << member->getPrimaryKey() << ", "
                    << member->getNickName() << ", "
                    << member->getEmail() << ", "
                    << member->getPassword() << ", "
                    << member->getPhoneNumber() << ", "
                    << member->getIsManager() << ", "

                    << lecture->getPrimaryKey() << ", "
                    << lecture->getLectureTitle() << ", "
                    << lecture->getInstructorName() << ", "
                    << lecture->getPrice() << ", "
                    << lecture->getEnrolledStudentsCount() << ", "
                    << lecture->getDurationHours() << ", "
                    << lecture->getDifficultyLevel() << "\n";
            }
        }
    }
    if (!file2.fail()) {
        for (const auto& v : instructorLectureList) {
            Member* member = program_interface->getMemberManager().searchMember(v.first);
            // map<unsigned long long, vector<Lecture*>> 에서 vector<Lecture*> 를 먼저 얻고
            // for 문 순환 필요함
            for (Lecture* lecture : v.second) {
                file2 << member->getPrimaryKey() << ", "
                    << member->getNickName() << ", "
                    << member->getEmail() << ", "
                    << member->getPassword() << ", "
                    << member->getPhoneNumber() << ", "
                    << member->getIsManager() << ", "

                    << lecture->getPrimaryKey() << ", "
                    << lecture->getLectureTitle() << ", "
                    << lecture->getInstructorName() << ", "
                    << lecture->getPrice() << ", "
                    << lecture->getEnrolledStudentsCount() << ", "
                    << lecture->getDurationHours() << ", "
                    << lecture->getDifficultyLevel() << "\n";
            }
        }
    }
    file1.close();
    file2.close();
}

vector<string> EnrollManager::parseCSV(istream& file, char delimiter)
{
    stringstream ss;
    vector<string> row;
    string t = " \n\r\t";

    while (!file.eof()) {
        char c = file.get();
        if (c == delimiter || c == '\r' || c == '\n') {
            if (file.peek() == '\n') file.get();
            string s = ss.str();
            s.erase(0, s.find_first_not_of(t));
            s.erase(s.find_last_not_of(t) + 1);
            row.push_back(s);
            ss.str("");
            if (c != delimiter) break;
        }
        else {
            ss << c;
        }
    }
    return row;
}

void EnrollManager::searchAndStudentEnrollLecture() {
    string text = "";
    unsigned long long primaryKey = 0;
    Lecture* lecture = nullptr;
    bool is_size = false;
    map<unsigned long long, Lecture*>& searchLectureList = program_interface->getLectureManager().getLectureList();

    bool isContinue = true;
    bool isTwice = false;
    int ch;
    while (isContinue == true) {
        // isTwice : 해당 메소드 실행 전 displayAllLecture 에서 조회되는 항목들이 처음에 출력
        //           먼저되고 나서부터 콘솔 text 클리어 처리해야함.
        if (isTwice == true) {
            cout << "\033[2J\033[1;1H";
        }
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Search and Enroll                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 강의 조회 메인 화면에서는 최대 50개의 강의만 조회됩니다. " << endl;
        cout << "  [ 특정 강의 확인은 검색 기능을 활용하세요. ] " << endl;
        cout << "  1. 강의 검색하기" << endl;
        cout << "  2. 메인 메뉴로 돌아가기" << endl;
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
        isTwice = true;

        switch (ch)
        {
        case 1: 
            // 수강할 강의 조회 text 를 입력받고 출력 처리
            cout << endl;
            cout << "   'primaryKey' 또는 '강의명' 또는 '강사 이름' 을 입력하세요" << endl;
            cout << "       primaryKey 조회는 정확히 일치해야하며, '강의명' 또는 '강사 이름' 은 부분 조회가 가능합니다." << endl;
            cout << endl;
            cout << "검색 : ";
            getline(cin, text, '\n');
            // 조회된 강의가 있을 땐 true, 없을 땐 false 반환처리
            is_size = program_interface->getLectureManager().searchLectureList(text);

            if (is_size == false) {
                cout << "조회된 강의가 없습니다." << endl;
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            else {
                cout << "조회된 강의 중 수강할 강의 의 privateKey 를 입력하세요" << endl;
                cout << "-1 : 취소" << endl;
                cout << "입력 : ";
                cin >> primaryKey;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (primaryKey == -1) {
                    continue;
                }
                // 수강 신청 처리
                lecture = program_interface->getLectureManager().searchLecture(primaryKey);
                if (lecture != nullptr) {
                    bool is_duplication = false;
                    Member* member = program_interface->getSessionManager().getLoginUser();
                    is_duplication = this->isDuplicationOrSizeCheckStudentEnrollLecture(member, lecture);

                    if (is_duplication == false) {
                        cout << "수강 신청이 완료되었습니다." << endl;
                        unsigned long long studentKey = member->getPrimaryKey();

                        // 추가하려는 강의의 "수강자 수" 를 증가 시킴
                        int temp = lecture->getEnrolledStudentsCount();
                        temp++;
                        // 강의 data에 수강자 수 적용
                        lecture->setEnrolledStudentsCount(temp);

                        // 학생 키로 map 에서 vector 찾기
                        auto it = this->studentLectureList.find(studentKey);
                        if (it != this->studentLectureList.end()) {
                            // 학생이 이미 존재할 때 해당 학생 강의 리스트에 새 강의 추가
                            it->second.push_back(lecture);
                        }
                        else {
                            // 학생이 존재하지 않으면, 새로운 엔트리 생성 (새로운 벡터를 만들고 강의 추가)
                            this->studentLectureList.insert({ studentKey, {lecture} });
                        }
                    }
                }
                else {
                    cout << "조회된 강의가 없습니다." << endl;
                }
                cout << endl << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 2:
            isContinue = false;
            break;
        default:
            break;
        }
    }
    return;
}

bool EnrollManager::isDuplicationOrSizeCheckStudentEnrollLecture(Member* member, Lecture* lecture) {
    // 컨테이너 객체 '댕글링 포인터' 이슈 방지로 참조 값 변수 할당
    map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 1. 먼저 학생 key 로 Map 에서 검색
    auto it = studentLectureList.find(member->getPrimaryKey());

    if (it != studentLectureList.end()) {
        // 2. 학생을 찾았다면 해당 학생의 강의 리스트(std::vector<Lecture*>)를 가져옴
        const vector<Lecture*>& lecturesOfStudent = it->second;

        // 3. (학생이 수강하는 강의 갯수가 9개 초과인지 체크)
        if (lecturesOfStudent.size() >= 9) {
            cout << "최대 수강 신청한 강의는 9개 입니다." << endl;
            return true; // 초과 시 더 이상 수강할 수 없음.
        }

        // 4. 이 리스트 내에서 현재 강의가 이미 있는지 순회로 체크
        for (const auto& i : lecturesOfStudent) {
            if (i && i->getPrimaryKey() == lecture->getPrimaryKey()) {
                cout << "이미 수강 신청한 강의입니다." << endl;
                return true; // 중복 발견
            }
        }
    }
    return false; // 중복 아님
}

void EnrollManager::instructorEnrollLecture() {
    Lecture* lecture = nullptr;

    lecture = program_interface->getLectureManager().inputLecture();
    if (lecture != nullptr) {
        cout << "강의 등록이 완료되었습니다." << endl;
        Member* member = program_interface->getSessionManager().getLoginUser();
        this->instructorLectureList.insert({ member->getPrimaryKey() , {lecture} });
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    else {
        cout << "강의가 정상적으로 생성되지 않았습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return;
}

string EnrollManager::makeWelcomeText() {
    string welcomeText = "님 안녕하세요. 지금 새 강의를 수강신청해보세요 !";
    unsigned long long myPrimaryKey = program_interface->getSessionManager().getLoginUser()->getPrimaryKey();
    map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
    bool is_own = false;

    for (const auto& i : studentLectureList) {
        if (i.first == myPrimaryKey) {
            is_own = true;
        }
    }
    if (is_own) {
        welcomeText = "님 안녕하세요. 현재 수강중인 강의가 있어요 !";
    }
    return welcomeText;
}

// 학생이 수강하는 특정 한 강의 찾기
Lecture* EnrollManager::findStudentLectureFromList(Lecture* lecture) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
        map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
        for (const auto& i : studentLectureList) {
            if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
                for (const auto& i2 : i.second) {
                    if (i2->getPrimaryKey() == lecture->getPrimaryKey()) {
                        return lecture;
                    }
                }
            }
        }
    }
    return nullptr;
}
// 강사가 진행하는 특정 한 강의 찾기
Lecture* EnrollManager::findInstructorLectureFromList(Lecture* lecture) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
        map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
        for (const auto& i : instructorLectureList) {
            if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
                for (const auto& i2 : i.second) {
                    if (i2->getPrimaryKey() == lecture->getPrimaryKey()) {
                        return lecture;
                    }
                }
            }
        }
    }
    return nullptr;
}
// 특정 학생이 수강하는 모든 강의 찾기
vector<Lecture*>& EnrollManager::findStudentLectureAllList(unsigned long long primaryKey) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
        map<unsigned long long, vector<Lecture*>>& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();
        auto it = studentLectureList.find(primaryKey);
        if (it != studentLectureList.end()) {
            return it->second;
        }
    }
    static vector<Lecture*> emptyLectureList;
    return emptyLectureList;
}
// 특정 강사가 진행하는 모든 강의 찾기
vector<Lecture*>& EnrollManager::findInstructorLectureAllList(unsigned long long primaryKey) {
    if (program_interface->getSessionManager().getIs_login() == true) {
        // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
        map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
        auto it = instructorLectureList.find(primaryKey);
        
        if (it != instructorLectureList.end()) {
            return it->second;
        }
    }
    static vector<Lecture*> emptyLectureList;
    return emptyLectureList;
}


// 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
// "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
// => "댕글링 포인터" 이슈 발생!!
// * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.
map<unsigned long long, vector<Lecture*>>& EnrollManager::getStudentLectureList() {
    return this->studentLectureList;
}
map<unsigned long long, vector<Lecture*>>& EnrollManager::getInstructorLectureList() {
    return this->instructorLectureList;
}