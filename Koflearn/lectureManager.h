#ifndef _LECTURE_MANAGER_H_
#define _LECTURE_MANAGER_H_

#include "lecture.h"
#include "IKoflearnPlatManager.h"
#include <vector>
#include <map>
#include <iostream>
using namespace std;

class LectureManager {
private:
	// Manager 가 인터페이스에 의존하도록 인터페이스 포인터 선언
	IKoflearnPlatManager* program_interface;

public:
	// 생성자에서 인터페이스 타입 의존성을 주입받음
	LectureManager(IKoflearnPlatManager* program);
	~LectureManager();

	Lecture* inputLecture();
	void addLecture(Lecture* lecture);
	void deleteLecture(unsigned long long key);
	void modifyLecture(unsigned long long key);
	Lecture* searchLecture(unsigned long long key);
	void displayAllLecture() const;

	unsigned long long makePrimaryKey();
	vector<string> parseCSV(istream&, char);
	void displayMenu();

	map<unsigned long long, Lecture*> getLectureList();

private:
	map<unsigned long long, Lecture*> lectureList;
};

#endif