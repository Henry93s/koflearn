#include "enrollManager.h"
// program_interface 를 통해서 접근하려는 모든 
// Manager 클래스들이 필요한 헤더 파일이 include 되어 있어야 함. 
// why? 순환참조 방지로 IKoflearnPlatManager 에서 Manager 클래스들을 include 하지 않고
//      전방선언 처리했으므로
#include "lectureManager.h"
#include "sessionManager.h"

#include <string>
#include <vector>
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
    file1.open("studentLectureList.txt");
    char* endptr;
    char* endptr2;
    if (!file1.fail()) {
        while (!file1.eof()) {
            vector<string> row = parseCSV(file1, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr, 10);
                Member* member = new Member(primaryKey, row[1], row[2], row[3], row[4], row[5]);

                // 6 index 부터
                unsigned long long primaryKey2 = strtoull(row[6].c_str(), &endptr2, 10);
                int price = atoi(row[9].c_str());
                int enrolledStudentsCount = atoi(row[10].c_str());
                int durationHours = atoi(row[11].c_str());
                Lecture* lecture = new Lecture(primaryKey2, row[7], row[8], price, enrolledStudentsCount, durationHours, row[12]);

                studentLectureList.insert({ member, lecture });
            }
        }
    }
    file1.close();

    file2.open("instructorLectureList.txt");
    char* endptr3;
    char* endptr4;
    if (!file2.fail()) {
        while (!file2.eof()) {
            vector<string> row = parseCSV(file2, ',');
            if (row.size()) {
                // strtoull : str to Unsigned Long Long
                unsigned long long primaryKey = strtoull(row[0].c_str(), &endptr3, 10);
                Member* member = new Member(primaryKey, row[1], row[2], row[3], row[4], row[5]);

                // 6 index 부터
                unsigned long long primaryKey2 = strtoull(row[6].c_str(), &endptr4, 10);
                int price = atoi(row[9].c_str());
                int enrolledStudentsCount = atoi(row[10].c_str());
                int durationHours = atoi(row[11].c_str());
                Lecture* lecture = new Lecture(primaryKey2, row[7], row[8], price, enrolledStudentsCount, durationHours, row[12]);

                instructorLectureList.insert({ member, lecture });
            }
        }
    }
    file2.close();
}
EnrollManager::~EnrollManager() {
    ofstream file1, file2;
    file1.open("studentLectureList.txt");
    file2.open("instructorLectureList.txt");

    if (!file1.fail()) {
        for (const auto& v : studentLectureList) {
            Member* member = v.first;
            Lecture* lecture = v.second;

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
    if(!file2.fail()){
        for (const auto& v : instructorLectureList) {
            Member* member = v.first;
            Lecture* lecture = v.second;

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
    unsigned long long privateKey = 0;
    Lecture* lecture = nullptr;
    
    if (!program_interface->getLectureManager().getLectureList().empty()) {
        cout << "수강할 강의 privateKey 를 입력하세요 : ";
        cout << "('-1' : 취소)" << endl;
        cin >> privateKey;
        if (privateKey == -1) { return; }
        while (getchar() != '\n');

    }

    lecture = program_interface->getLectureManager().searchLecture(privateKey);
    if (lecture == nullptr) {
        cout << "조회된 강의가 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');

        return;
    }
    else {
        bool is_duplication = this->isDuplicationStudentEnrollLecture(program_interface->getSessionManager().getLoginUser(), lecture);
        if (is_duplication == false) {
            cout << "수강 신청이 완료되었습니다." << endl;
            Member* member = program_interface->getSessionManager().getLoginUser();
            this->studentLectureList.insert({ member, lecture });
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

        }
        else if (is_duplication == true) {
            cout << "이미 수강 신청한 강의입니다." << endl;
            cout << "[Enter] 를 눌러 뒤로가기" << endl;
            while (getchar() != '\n');

        }
    }

    return;
}

bool EnrollManager::isDuplicationStudentEnrollLecture(Member* member, Lecture* lecture) {
    
    for (const auto& i : program_interface->getEnrollManager().studentLectureList) {
        if (i.first->getPrimaryKey() == member->getPrimaryKey()
            && i.second->getPrimaryKey() == lecture->getPrimaryKey()) {
            return true;
        }
    }
    return false;
}

void EnrollManager::instructorEnrollLecture() {
    Lecture* lecture = nullptr;

    lecture = program_interface->getLectureManager().inputLecture();
    if (lecture != nullptr) {
        cout << "강의 등록이 완료되었습니다." << endl;
        Member* member = program_interface->getSessionManager().getLoginUser();
        this->instructorLectureList.insert({ member, lecture });
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');

    }
    else {
        cout << "강의가 정상적으로 생성되지 않았습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');

    }
    return;
}

map<Member*, Lecture*> EnrollManager::getStudentLectureList() {
    return this->studentLectureList;
}

map<Member*, Lecture*> EnrollManager::getInstructorLectureList() {
    return this->instructorLectureList;
}