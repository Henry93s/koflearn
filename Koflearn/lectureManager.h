#ifndef _LECTURE_MANAGER_H_
#define _LECTURE_MANAGER_H_

#include <vector>
#include <map>
#include "lecture.h"
#include <iostream>
using namespace std;

class LectureManager {
public:
	LectureManager();
	~LectureManager();

	void inputLecture();
	void addLecture(Lecture* lecture);
	void deleteLecture(unsigned long long key);
	void modifyLecture(unsigned long long key);
	Lecture* searchLecture(unsigned long long key);
	void displayAllLecture() const;

	unsigned long long makePrimaryKey();
	vector<string> parseCSV(istream&, char);
	void displayMenu();

private:
	map<unsigned long long, Lecture*> lectureList;
};

#endif