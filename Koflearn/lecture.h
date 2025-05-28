#ifndef _LECTURE_H
#define _LECTURE_H
#include "koflearnPlatform.h"
#include <iostream>
using namespace std;

class Lecture : public KoflearnPlatform {
public:
	// 생성자 선언
	Lecture(unsigned long long primaryKey, string lectureTitle, string instructorName, int price,
		int enrolledStudentsCount, int durationHours, string difficultyLevel);

	// getter Method
	// unsigned long long getPrimaryKey(); KoflearnPlatform
	string getLectureTitle();
	string getInstructorName(); // login user
	int getPrice();
	int getEnrolledStudentsCount();
	int getDurationHours();
	string getDifficultyLevel();
	void displayInfo() override;

	// setter Method
	// void setPrimaryKey(unsigned long long primaryKey); KoflearnPlatform
	void setLectureTitle(string lectureTitle);
	void setInstructorName(string instructorName); // login user
	void setPrice(int price);
	void setEnrolledStudentsCount(int enrolledStudentCount);
	void setDurationHours(int durationHours);
	void setDifficultyLevel(string difficultyLevel);

private:
	string lectureTitle;
	string instructorName; // login user
	int price;
	int enrolledStudentsCount;
	int durationHours;
	string difficultyLevel;
};

#endif // !_LECTURE_H
