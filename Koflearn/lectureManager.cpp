#include "koflearnPlatform.h"
#include "lecture.h"
#include "lectureManager.h"

#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

LectureManager::LectureManager() {
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
    while (getchar() != '\n');
    string lectureTitle, instructorName, difficultyLevel;
    int price, enrolledStudentsCount, durationHours;

    unsigned long long primaryKey = makePrimaryKey();

    cout << "강의 명 : ";
    getline(cin, lectureTitle, '\n');
    cout << "강사 명 : ";
    // 싱글톤 객체받아서 getmember 넘겨야함.
    instructorName = "testName";
    cout << "testName\n"; // temp instructorName
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
    return lecture;
}

Lecture* LectureManager::searchLecture(unsigned long long primaryKey) {
    Lecture* lecture = nullptr;
    lecture = lectureList[primaryKey];
    if (lecture != nullptr) {
        return lecture;
    }
    else {
        return nullptr;
    }
    // none : nullptr 반환
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
}

void LectureManager::deleteLecture(unsigned long long primaryKey){
    lectureList.erase(primaryKey);
}

void LectureManager::modifyLecture(unsigned long long primaryKey) {
    Lecture* lecture = searchLecture(primaryKey);
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
        switch (op)
        {
        case 1:
            cout << "강의 명 수정 : ";
            cin >> lectureTitle;
            lecture->setLectureTitle(lectureTitle);
            break;
        case 2:
            cout << "가격 수정 : ";
            cin >> price;
            lecture->setPrice(price);
            break;
        case 3:
            cout << "강의 시간 수정 : ";
            cin >> durationHours;
            lecture->setDurationHours(durationHours);
            break;
        case 4:
            cout << "난이도 수정(1 : 쉬움, 2: 보통, 3 : 어려움) : ";
            cin >> integerLevel;
            
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
        switch (ch) {
        case 1: default:
            displayAllLecture();
            cin.ignore();
            while (getchar() != '\n');
            break;
        case 2:
            inputLecture();
            break;
        case 3:
            displayAllLecture();
            cout << "   멤버 primaryKey 입력 : ";
            cin >> key;
            deleteLecture(key);
            break;
        case 4:
            displayAllLecture();
            cout << "   멤버 primaryKey 입력 : ";
            cin >> key;
            modifyLecture(key);
            break;
        case 5:
            isContinue = false;
            break;
        }
    }
    return;
}