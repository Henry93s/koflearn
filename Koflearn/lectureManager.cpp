#include "lecture.h"
#include "lectureManager.h"
#include "IKoflearnPlatManager.h"
// program_interface 를 통해서 접근하려는 모든 
// Manager 클래스들이 필요한 헤더 파일이 include 되어 있어야 함. 
// why? 순환참조 방지로 IKoflearnPlatManager 에서 Manager 클래스들을 include 하지 않고
//      전방선언 처리했으므로
#include "sessionManager.h"
#include "enrollManager.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <chrono> // 시간 관련 기능(searching cout 출력에 딜레이 부여 위함)
#include <thread> // 스레드 관련 기능(searching cout 출력에 딜레이 부여 위함)

// 생성자에서 인터페이스 타입의 의존성을 주입받음
LectureManager::LectureManager(IKoflearnPlatManager* program)
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }

    ifstream file;
    file.open("lectureList.csv");
    char* endptr;
    if (!file.fail()) {
        while (!file.eof()) {
            vector<string> row = parseCSV(file, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr, 10);
                int price = atoi(row[3].c_str());
                int enrolledStudentsCount = atoi(row[4].c_str());
                int durationHours = atoi(row[5].c_str());
                Lecture* lecture = new Lecture(primaryKey, row[1], row[2], price, enrolledStudentsCount, durationHours, row[6]);
                lectureList.insert({ primaryKey, lecture });
            }
        }
    }
    file.close();
}

LectureManager::~LectureManager() {
    ofstream file;
    file.open("lectureList.csv");
    if (!file.fail()) {
        for (const auto& v : lectureList) {
            Lecture* lecture = v.second;
            file << lecture->getPrimaryKey() << ", "
                << lecture->getLectureTitle() << ", "
                << lecture->getInstructorName() << ", "
                << lecture->getPrice() << ", "
                << lecture->getEnrolledStudentsCount() << ", "
                << lecture->getDurationHours() << ", "
                << lecture->getDifficultyLevel() << "\n";
        }
    }
    file.close();
}

Lecture* LectureManager::inputLecture() {
    string lectureTitle, instructorName, difficultyLevel;
    int price, enrolledStudentsCount, durationHours;

    unsigned long long primaryKey = makePrimaryKey();
    if (primaryKey == -1) {
        cout << "강의 최대 수용량이 초과하였습니다. (99,999,999,999)" << endl;
        return nullptr;
    }

    // 한 사람당 강의 개설은 최대 9개까지 개설 가능.
    // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
    map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    for (const auto& i : instructorLectureList) {
        Lecture* lecture = nullptr;
        if (i.first == program_interface->getSessionManager().getLoginUser()->getPrimaryKey()) {
            if (i.second.size() > 9) {
                cout << "한 사람당 강의 최대 개설은 9개까지만 가능합니다." << endl;
                return nullptr;
            }
        }
    }

    cout << "강의 명 : ";
    getline(cin, lectureTitle, '\n');
    cout << "강사 명 : ";
    cout << program_interface->getSessionManager().getLoginUser()->getNickName() << endl;
    instructorName = program_interface->getSessionManager().getLoginUser()->getNickName();
    cout << "가격 : ";
    cin >> price;
    enrolledStudentsCount = 0; // 기본 수강자 수는 0 부터 시작
    cout << "수강 시간 : ";
    cin >> durationHours;
    int selectLevel = 2;
    cout << "강의 난이도 (1 : 쉬움, 2 : 보통, 3 : 어려움) : ";
    cin >> selectLevel;
    switch (selectLevel)
    {
    case 1:
        difficultyLevel = "쉬움";
        break;
    case 2:
        difficultyLevel = "보통";
        break;
    case 3:
        difficultyLevel = "어려움";
        break;
    default:
        difficultyLevel = "보통";
        break;
    }


    Lecture* lecture = new Lecture(primaryKey, lectureTitle, instructorName, price,
        enrolledStudentsCount, durationHours, difficultyLevel);

    lectureList.insert({ primaryKey, lecture });
    // 강의를 등록했을 때 "내 강의 보기" 리스트를 출력하기 위한 instructor(강의자) 기준 리스트에 데이터 추가
    Member* member = program_interface->getSessionManager().getLoginUser();

    // 특정 member privateKey 를 key 로 vector 에 여러 개 요소 삽입하기 !
    unsigned long long instructorKey = member->getPrimaryKey();

    // 강사 키로 map 에서 vector 찾기
    auto it = instructorLectureList.find(instructorKey);

    if (it != instructorLectureList.end()) {
        // 강의자가 이미 존재할 때 해당 강의자의 강의 리스트에 새 강의를 추가한다.
        // push_back : 벡터의 맨 끝에 항목 추가
        it->second.push_back(lecture);
    }
    else {
        // 강의자가 존재하지 않으면 새 엔트리 생성 (새로운 벡터를 만들고 강의 추가)
        instructorLectureList.insert({ instructorKey, {lecture} });
    }

    return lecture;
}

Lecture* LectureManager::searchLecture(unsigned long long primaryKey) {
    // lecture = lectureList[primaryKey] 로 lecture 객체 포인터를 찾을 때
    // 객체 포인터가 없을 때 nullptr 이 해당 primary key 값 인덱스에 "실제 맵" 에 저장될 수 있음 !
    // => 따라서 lecture를 찾는 방법을 전체 lectureList 를 순회하면서 요소의 first 와 primarykey가 동일한 lecture 를 반환하도록 함
    for (const auto& i : lectureList) {
        if (i.first == primaryKey) {
            return i.second;
        }
    }
    return nullptr;
}

bool LectureManager::searchLectureList(string text) {
    bool is_size = false;
    char* endptr;
    unsigned long long searchKey = strtoull(text.c_str(), &endptr, 10);

    cout << endl;
    // 1. text 가 숫자엿을 경우 -> Lecture 의 primaryKey 로만 조회
    if (*endptr == '\0' && !text.empty()) {
        cout << "searching PrimaryKey .";
        for (auto i = 0; i < 5;i++) {
            cout << " .";
            this_thread::sleep_for(chrono::milliseconds(350));
            cout.flush(); // 버퍼에 있는 내용을 바로 화면에 뿌림으로써
                          // 점들이 thread chrono timer 에 맞춰서 하나씩 나타나는 프로세싱 효과 부여
        }
        cout << endl;

        // 먼저 데이터를 찾아보고 없으면 이터레이터가 end() 를 반환하는 걸 체크한다.
        auto it = this->lectureList.find(searchKey);
        if (it != lectureList.end()) {
            // 출력되는 Lecture 가 있을 때 is_size = true
            is_size = true;
            cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
            it->second->displayInfo();
        }
    } 

    cout << endl << endl;
    
    // 2. text 가 숫자이거나 문자가 하나라도 포함된 문자열이였을 경우 
    //    -> Lecture 의 title 또는 instructorName 으로 조회
    cout << "searching title or instructorName .";
    for (auto i = 0; i < 5;i++) {
        cout << " .";
        this_thread::sleep_for(chrono::milliseconds(350));
        cout.flush(); // 버퍼에 있는 내용을 바로 화면에 뿌림으로써
                        // 점들이 thread chrono timer 에 맞춰서 하나씩 나타나는 프로세싱 효과 부여
    }
    cout << endl;

    /* 부분 문자열 찾기 메소드 적용 : find()
        부분 문자열을 찾으면 해당 부분 문자열이 시작하는 인덱스(위치)를 반환
        찾지 못하면 string::npos 특수 값을 반환한다.
    */
    for (const auto& i : lectureList) {
        if (i.second->getLectureTitle().find(text) != string::npos
            || i.second->getInstructorName().find(text) != string::npos) {
            if (is_size == false) {
                cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
                // 출력되는 Lecture 가 하나라도 있을 때 is_size = true
                is_size = true;
            }
            i.second->displayInfo();
        }
    }
    
    return is_size;
}

bool LectureManager::displayAllLecture() const {
    if (lectureList.empty()) {
        cout << "등록된 강의가 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cout << endl;
    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    int cnt = 0;
    for (const auto& lecture : lectureList) {
        lecture.second->displayInfo(); // 단순 강의 객체(Lecture()) read 책임은 Lecture 클래스에서 처리
        cnt++;
        // 처음 출력은 최대 50개 까지만 출력 이후 조회를 통해서 처리 가능
        if (cnt == 50) {
            break;
        }
    }
    cout << endl << endl << endl;
    return true;
}

void LectureManager::deleteLecture(unsigned long long primaryKey) {
    // erase : primaryKey 인덱스에 value 가 있으면 pair 를 지우고, value 가 없으면 그냥 넘어감
    lectureList.erase(primaryKey);
}

void LectureManager::modifyLecture(unsigned long long primaryKey) {
    Lecture* lecture = searchLecture(primaryKey);
    if (lecture != nullptr) {
        cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
        lecture->displayInfo();

        int op = -1;
        string lectureTitle;
        int price;
        int durationHours;
        int integerLevel = 2;
        string difficultyLevel = "보통";

        while (1) {
            cout << "수정할 항목을 선택하세요. \n(강의 명 : 1, 가격 : 2, 강의 시간 : 3, 난이도 : 4, [수정 종료] : 5) : ";
            cin >> op;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (op)
            {
            case 1:
                cout << "강의 명 수정 : ";
                getline(cin, lectureTitle, '\n');
                lecture->setLectureTitle(lectureTitle);
                break;
            case 2:
                cout << "가격 수정 : ";
                cin >> price;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                lecture->setPrice(price);
                break;
            case 3:
                cout << "강의 시간 수정 : ";
                cin >> durationHours;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                lecture->setDurationHours(durationHours);
                break;
            case 4:
                cout << "난이도 수정(1 : 쉬움, 2: 보통, 3 : 어려움) : ";
                cin >> integerLevel;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (integerLevel == 1) {
                    difficultyLevel = "쉬움";
                }
                else if (integerLevel == 3) {
                    difficultyLevel = "어려움";
                }
                else {
                    difficultyLevel = "보통";
                }
                lecture->setDifficultyLevel(difficultyLevel);
                break;
            default:
                break;
            }
            if (op == 5) { break; }
        }
        lectureList[primaryKey] = lecture;
    }
    else {
        cout << "primaryKey 로 조회된 강의가 없습니다." << endl;
    }
}

void LectureManager::addLecture(Lecture* lecture) {
    lectureList.insert({ lecture->getPrimaryKey(), lecture });
}

unsigned long long LectureManager::makePrimaryKey() {
    if (lectureList.size() == 0) {
        // member 1 ~ 10,000,000,000
        // lecture 10,000,000,001 ~ 99,999,999,999
        return 10000000001;
    }
    else {
        auto elem = lectureList.end();
        unsigned long long primaryKey = (--elem)->first;
        if (primaryKey == 99999999999) {
            return -1;
        }
        return ++primaryKey;
    }
}

vector<string> LectureManager::parseCSV(istream& file, char delimiter)
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

// 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
// "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
// => "댕글링 포인터" 이슈 발생!!
// * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.
map<unsigned long long, Lecture*>& LectureManager::getLectureList() {
    return this->lectureList;
}

bool LectureManager::deleteLectureProcess(unsigned long long key) {
    bool is_deleted = false;
    string is_delete = "";

    cout << "삭제 시 선택 강의 및 모든 수강 리스트에서 해당 강의도 모두 제거됩니다." << endl;
    cout << "정말 제거하시겠습니까?" << endl;
    cout << "제거하시려면 [Delete lecture] 의 [ ] 안 문구를 정확히 입력해주세요." << endl;
    cout << "입력 : ";
    getline(cin, is_delete, '\n');

    if (is_delete.compare("Delete lecture") == 0) {
        Lecture* lecture = program_interface->getLectureManager().searchLecture(key);
        // 삭제 대상 강의 관련 데이터 우선 제거(instructorLectureList, studentLectureList)
        allDeletedLectureData(key);

        // 삭제 대상 강의 본 데이터 제거(lectureList)
        lectureList.erase(key);

        cout << "강의 삭제가 정상적으로 완료되었습니다." << endl;
        cout << "[Enter] 를 누르면 메인 페이지로 이동합니다." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return true;
    }
    else {
        cout << "문구를 정상적으로 입력하지 않아 강의 삭제를 진행할 수 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    return is_deleted;
}

void LectureManager::allDeletedLectureData(unsigned long long key) {
    unsigned long long lecturePrimaryKey = key;
    Lecture* lecture = program_interface->getLectureManager().searchLecture(lecturePrimaryKey);
    if (lecture == nullptr) {
        return;
    }

    auto& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
    auto& studentLectureList = program_interface->getEnrollManager().getStudentLectureList();

    // 1. instructorLectureList 에서 lecture 제거
    for (auto it = instructorLectureList.begin(); it != instructorLectureList.end(); ++it) {
        vector<Lecture*>& lectures = it->second;

        for (auto vecIt = lectures.begin(); vecIt != lectures.end(); ) {
            if ((*vecIt)->getPrimaryKey() == lecturePrimaryKey) {
                // 현재 요소를 삭제하고 list 의 다음 이터레이터를 받음
                vecIt = lectures.erase(vecIt);
            }
            else {
                // next
                ++vecIt;
            }
        }

        // 벡터에서 lecture 를 제거함으로써 list 가 비어있다면 해당 컨테이너에서 엔트리 자체를 삭제해야한다.
        if (lectures.empty()) {
            it = instructorLectureList.erase(it);
        }
    }

    // 2. studentLectureList 에서 lecture 제거
    for (auto it = studentLectureList.begin(); it != studentLectureList.end(); ++it) {
        vector<Lecture*>& lectures = it->second;

        for (auto vecIt = lectures.begin(); vecIt != lectures.end();) {
            if ((*vecIt)->getPrimaryKey() == lecturePrimaryKey) {
                // 현재 요소를 삭제하고 studentLectureList의 다음 이터레이터 받기
                vecIt = lectures.erase(vecIt);
            }
            else {
                // next
                ++vecIt;
            }
        }

        // 벡터에서 lecture 를 제거함으로써 list 가 비어있다면 해당 컨테이너에서 엔트리 자체를 삭제해야한다.
        if (lectures.empty()) {
            it = studentLectureList.erase(it);
        }
    }

    return;
}

void LectureManager::exitLecture(unsigned long long primaryKey) {
    Lecture* lecture = this->searchLecture(primaryKey);
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

void LectureManager::displayMenu()
{
    int ch, ch2;
    bool isContinue = true;
    Lecture* lecture = nullptr;
    string text = "";
    bool all_is_size = false;
    bool is_size = false;
    unsigned long long primaryKey = 0;
    bool is_deleted = false;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Lecture Manager                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Koflearn 강의(상품) 리스트 조회(삭제, 수정)         " << endl;
        cout << "  2. 강의 추가                            " << endl;
        cout << "  3. 메인 메뉴로 돌아가기                           " << endl;
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
            cout << " 강의 조회 메인 화면에서는 최대 50개의 강의만 조회됩니다. " << endl;
            cout << "  [ 특정 강의 확인은 검색 기능을 활용하세요. ] " << endl;
            all_is_size = displayAllLecture();
            if(all_is_size == true){
                // 수정 또는 삭제할 강의 조회 text 를 입력받고 출력 처리
                cout << endl;
                cout << "   'primaryKey' 또는 '강의명' 또는 '강사 이름' 을 입력하세요" << endl;
                cout << "       primaryKey 조회는 정확히 일치해야하며, '강의명' 또는 '강사 이름' 은 부분 조회가 가능합니다." << endl;
                cout << endl;
                cout << "-1 : 취소" << endl;
                cout << "검색 : ";
                getline(cin, text, '\n');
                if (text == "-1") {
                    continue;
                }
                // 조회된 강의가 있을 땐 true, 없을 땐 false 반환처리
                is_size = program_interface->getLectureManager().searchLectureList(text);
                if (is_size == true) {
                    cout << "조회된 강의 중 수정 또는 삭제할 강의 의 privateKey 를 입력하세요." << endl;
                    cout << "-1 : 취소" << endl;
                    cout << "입력 : ";
                    cin >> primaryKey;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (primaryKey == -1) {
                        continue;
                    }
                    lecture = program_interface->getLectureManager().searchLecture(primaryKey);
                    if (lecture != nullptr) {
                        cout << "     1. 수정 하기        " << endl;
                        cout << "     2. 삭제 하기        " << endl;
                        cout << "     3. 메인 메뉴로 돌아가기        " << endl;
                        cout << "   기능을 선택하세요 : ";
                        cin >> ch2;

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

                        if (ch2 == 1) {
                            program_interface->getLectureManager().modifyLecture(primaryKey);
                        }
                        else if (ch2 == 2) {
                            is_deleted = deleteLectureProcess(primaryKey);
                            isContinue = false;
                        }
                        else {
                            continue;
                        }
                    }
                }
                cout << "[Enter] 를 눌러 뒤로가기" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            break;
        case 2:
            lecture = inputLecture();
            if (lecture != nullptr) {
                cout << "강의 등록이 완료되었습니다." << endl;
            }
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
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