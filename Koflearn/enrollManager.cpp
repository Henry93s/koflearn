#include "koflearnPlatManager.h"
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

EnrollManager::EnrollManager() {
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
                Lecture* lecture = new Lecture(primaryKey, row[7], row[8], price, enrolledStudentsCount, durationHours, row[12]);

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
                Lecture* lecture = new Lecture(primaryKey, row[7], row[8], price, enrolledStudentsCount, durationHours, row[12]);

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

KoflearnPlatManager* EnrollManager::getInstance() const {
	return KoflearnPlatManager::getInstance();
}

void EnrollManager::searchAndStudentEnrollLecture() {
    unsigned long long privateKey = 0;
    KoflearnPlatManager* program = getInstance();
    Lecture* lecture = nullptr;

    cout << "수강할 강의 privateKey 를 입력하세요 : ";
    cout << "('-1' : 취소)" << endl;
    cin >> privateKey;
    if (privateKey == -1) { return; }
    while (getchar() != '\n');

    lecture = program->getLectureManager().searchLecture(privateKey);
    if (lecture == nullptr) {
        cout << "조회된 강의가 없습니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        cin.ignore();
        return;
    }
    bool is_duplication = this->isDuplicationStudentEnrollLecture(program->getLoginUser(), lecture);
    if (is_duplication == false && lecture != nullptr) {
        cout << "수강 신청이 완료되었습니다." << endl;
        this->studentLectureList.insert({ program->getLoginUser(), lecture });
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
    }
    else if (is_duplication == true && lecture != nullptr) {
        cout << "이미 수강 신청한 강의입니다." << endl;
        cout << "[Enter] 를 눌러 뒤로가기" << endl;
        while (getchar() != '\n');
    }

    return;
}

bool EnrollManager::isDuplicationStudentEnrollLecture(Member* member, Lecture* lecture) {
    KoflearnPlatManager* program = getInstance();
    for (const auto& i : program->getEnrollManager().studentLectureList) {
        if (i.first->getPrimaryKey() == member->getPrimaryKey()
            && i.second->getPrimaryKey() == lecture->getPrimaryKey()) {
            return true;
        }
    }
    return false;
}

void EnrollManager::instructorEnrollLecture() {
    KoflearnPlatManager* program = getInstance();
    Lecture* lecture = nullptr;

    lecture = program->getLectureManager().inputLecture();
    if (lecture != nullptr) {
        cout << "강의 등록이 완료되었습니다." << endl;
        this->instructorLectureList.insert({ program->getLoginUser(), lecture });
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
