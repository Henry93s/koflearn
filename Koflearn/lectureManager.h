#ifndef _LECTURE_MANAGER_H_
#define _LECTURE_MANAGER_H_

#include <vector>
#include <map>
#include "lecture.h"
#include <iostream>
using namespace std;

// KoflearnPlatManager 전방 선언
class KoflearnPlatManager;

class LectureManager {
public:
	LectureManager();
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

	KoflearnPlatManager* getInstance() const;

private:
	map<unsigned long long, Lecture*> lectureList;
};

#endif