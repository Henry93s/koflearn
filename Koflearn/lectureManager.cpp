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
using namespace std;

// 생성자에서 인터페이스 타입의 의존성을 주입받음
LectureManager::LectureManager(IKoflearnPlatManager* program) 
    : program_interface(program)
{
    if (!program_interface) {
        cerr << "오류: MyPageManager에 유효한 IKoflearnPlatManager가 주입되지 않았습니다!\n";
    }

	ifstream file;
	file.open("lectureList.txt");
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
    file.open("lectureList.txt");
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

    // 컨테이너 객체 반환받을 때 임시 객체 이슈로 댕글링 포인터될 수 있으므로 참조 값 받기
    map<unsigned long long, vector<Lecture*>>& instructorLectureList = program_interface->getEnrollManager().getInstructorLectureList();
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

void LectureManager::displayAllLecture() const {
    if (lectureList.empty()) {
        cout << "등록된 강의가 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');

        return;
    }

    cout << "    key      |            Title          |   teacher   |    price    |   students   |   hours   |   level  |" << endl;
    for (const auto& lecture : lectureList) {
        lecture.second->displayInfo(); // 단순 강의 객체(Lecture()) read 책임은 Lecture 클래스에서 처리
    }
    cout << endl << endl << endl;
    return;
}

void LectureManager::deleteLecture(unsigned long long primaryKey){
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
            while (getchar() != '\n');


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
                while (getchar() != '\n');

                lecture->setPrice(price);
                break;
            case 3:
                cout << "강의 시간 수정 : ";
                cin >> durationHours;
                while (getchar() != '\n');

                lecture->setDurationHours(durationHours);
                break;
            case 4:
                cout << "난이도 수정(1 : 쉬움, 2: 보통, 3 : 어려움) : ";
                cin >> integerLevel;
                while (getchar() != '\n');


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

void LectureManager::displayMenu()
{
    int ch;
    unsigned long long key;
    bool isContinue = true;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Lecture Manager                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. Koflearn 강의(상품) 리스트 출력                     " << endl;
        cout << "  2. 강의 추가                            " << endl;
        cout << "  3. 강의 삭제                           " << endl;
        cout << "  4. 강의 수정                           " << endl;
        cout << "  5. 메인 메뉴로 돌아가기                       " << endl;
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
        case 1: default:
            displayAllLecture();
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 2:
            inputLecture();
            cout << "강의 등록이 완료되었습니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 3:
            displayAllLecture();
            cout << "   멤버 primaryKey 입력 : ";
            cin >> key;
            while (getchar() != '\n');

            deleteLecture(key);
            cout << "강의 삭제 작업이 종료되었습니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 4:
            displayAllLecture();
            cout << "   멤버 primaryKey 입력 : ";
            cin >> key;
            while (getchar() != '\n');

            modifyLecture(key);
            cout << "강의 수정 작업이 종료되었습니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

            break;
        case 5:
            isContinue = false;
            break;
        }
    }
    return;
}