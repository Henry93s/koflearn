#ifndef _ENROLLMANAGER_H_
#define _ENROLLMANAGER_H_

#include "member.h"
#include "lecture.h"
#include "IKoflearnPlatManager.h"
#include <iostream>
#include <map>
using namespace std;

class EnrollManager {
private:
	// Manager 가 인터페이스에 의존하도록 인터페이스 포인터 선언
	IKoflearnPlatManager* program_interface;

private:
	map<Member*, Lecture*> studentLectureList;
	map<Member*, Lecture*> instructorLectureList;

public:
	// 생성자에서 인터페이스 타입 의존성을 주입받음
	EnrollManager(IKoflearnPlatManager* program);
	~EnrollManager();

	void searchAndStudentEnrollLecture();
	void instructorEnrollLecture();
	bool isDuplicationStudentEnrollLecture(Member* member, Lecture* lecture);
	
	vector<string> parseCSV(istream&, char);

	map<Member*, Lecture*> getStudentLectureList();
	map<Member*, Lecture*> getInstructorLectureList();
};

#endif // ! _ENROLLMANAGER_H_
