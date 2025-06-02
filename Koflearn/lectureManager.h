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

	// 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
	// "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
	// => "댕글링 포인터" 이슈 발생!!
	// * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.
	map<unsigned long long, Lecture*>& getLectureList();

private:
	map<unsigned long long, Lecture*> lectureList;
};

#endif