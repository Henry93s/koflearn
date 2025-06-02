#include "lecture.h"

#include <iostream>
#include <iomanip>
using namespace std;

// lecutre 자식 클래스 생성자에서 부모 클래스 생성자 포함
Lecture::Lecture(unsigned long long primaryKey, string lectureTitle, string instructorName
	, int price, int enrolledStudentsCount, int durationHours, string difficultyLevel)
	: KoflearnPlatform(primaryKey), lectureTitle(lectureTitle), instructorName(instructorName)
	, price(price), enrolledStudentsCount(enrolledStudentsCount), durationHours(durationHours)
	, difficultyLevel(difficultyLevel)
{
}

// getter 함수 구현
string Lecture::getLectureTitle() {
	return this->lectureTitle;
}
string Lecture::getInstructorName() {
	return this->instructorName;
}
int Lecture::getPrice() {
	return this->price;
}
int Lecture::getEnrolledStudentsCount() {
	return this->enrolledStudentsCount;
}
int Lecture::getDurationHours() {
	return this->durationHours;
}
string Lecture::getDifficultyLevel() {
	return this->difficultyLevel;
}
// override 
void Lecture::displayInfo() {
	cout << setw(11) << setfill('0') << right << getPrimaryKey() << "  | " << left;
	cout << setw(29) << setfill(' ') << this->lectureTitle << "    | ";
	cout << setw(12) << setfill(' ') << this->instructorName << "| ";
	cout << setw(10) << setfill(' ') << this->price << "  | ";
	cout << setw(10) << setfill(' ') << this->enrolledStudentsCount << "   | ";
	cout << setw(10) << setfill(' ') << this->durationHours << "| ";
	cout << setw(6) << setfill(' ') << this->difficultyLevel << "   | ";
	cout << endl;
}


// setter 함수 구현
void Lecture::setLectureTitle(string lectureTitle) {
	this->lectureTitle = lectureTitle;
}
void Lecture::setInstructorName(string instructorName) {
	this->instructorName = instructorName;
}
void Lecture::setPrice(int price) {
	this->price = price;
}
void Lecture::setEnrolledStudentsCount(int enrolledStudentCount) {
	// 두 값 중 큰 값으로 처리됨. 즉 외부에서 음수로 처리된 값이 와도 최소 0은 보장함.
	this->enrolledStudentsCount = max(0, enrolledStudentCount);  // 음수 방지
}
void Lecture::setDurationHours(int durationHours) {
	this->durationHours = durationHours;
}
void Lecture::setDifficultyLevel(string difficultyLevel) {
	this->difficultyLevel = difficultyLevel;
}