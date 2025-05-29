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

	// EnrollManager::searchAndStudentEnrollLecture() 에서 lectureList 직접 사용
	friend class EnrollManager;
	// MyPageManager::allDeletedUserData: 회원 삭제 시 연관된 데이터를 지우기 위한 friend
	friend class MyPageManager;

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

	KoflearnPlatManager* getInstance() const;

private:
	map<unsigned long long, Lecture*> lectureList;
};

#endif