#ifndef _ENROLLMANAGER_H_
#define _ENROLLMANAGER_H_

#include "member.h"
#include "lecture.h"
#include <iostream>
#include <map>
using namespace std;

// KoflearnPlatManager 전방 선언
class KoflearnPlatManager;

class EnrollManager {
private:
	map<Member*, Lecture*> studentLectureList;
	map<Member*, Lecture*> instructorLectureList;

	// 250529 commit 로그에 작성하지 못한 내용(friend - daily 과제에 추가할 것.)
	// myPageManager 에서는 EnrollManager 의 private 인 "강의듣는 리스트", "강의하는 리스트"
	// 를 받아와야 하기 때문에 myPageManager 와의 [friend] 선언
	friend class MyPageManager;

public:
	EnrollManager();
	~EnrollManager();

	void searchAndStudentEnrollLecture();
	void instructorEnrollLecture();
	bool isDuplicationStudentEnrollLecture(Member* member, Lecture* lecture);
	
	vector<string> parseCSV(istream&, char);

	KoflearnPlatManager* getInstance() const;
};

#endif // ! _ENROLLMANAGER_H_
