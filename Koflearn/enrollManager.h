#ifndef _ENROLLMANAGER_H_
#define _ENROLLMANAGER_H_

#include "member.h"
#include "lecture.h"
#include "IKoflearnPlatManager.h"
#include <iostream>
#include <map>
#include <vector>
using namespace std;

class EnrollManager {
private:
	// Manager 가 인터페이스에 의존하도록 인터페이스 포인터 선언
	IKoflearnPlatManager* program_interface;

private:
	/*
		std 컨테이너의 key가 포인터 타입인 경우, 
		맵은 포인터가 가리키는 객체의 내용이 아니라 포인터 주소 값으로 검색하여
		의도하지 않은 반환 값이나 에러 발생 위험 있음
		-> 키를 클래스 객체 포인터 로 쓰는 모든 맵(2개) 들을 <unsigned long long, Lecture*> 로 변경
			(앞쪽 unsigned long long 에는 member 의 primarykey 가 들어감)
	*/
	map<unsigned long long, vector<Lecture*>> studentLectureList;
	map<unsigned long long, vector<Lecture*>> instructorLectureList;

public:
	// 생성자에서 인터페이스 타입 의존성을 주입받음
	EnrollManager(IKoflearnPlatManager* program);
	~EnrollManager();

	void searchAndStudentEnrollLecture();
	void instructorEnrollLecture();
	bool isDuplicationStudentEnrollLecture(Member* member, Lecture* lecture);
	
	vector<string> parseCSV(istream&, char);

	// 1.
	// 컨테이너 객체의 경우 특정 변수에 값을 함수에서 반환을 통해 할당했을 때,
	// "임시 객체" 가 생성되고 반환 직후 ; 을 만나 문장이 끝나면 임시 컨테이너 객체가 소멸됨.
	// => "댕글링 포인터" 이슈 발생!!
	// * 해결 : 컨테이너 객체를 변수에 할당하여 반환할 때, "복사하지 않고" "참조" 값을 반환한다.
	// 2. stl 컨테이너 사용 에러 : map<unsigned long long, Lecture*> 형태일 경우
	/*
		한 학생은 하나의 강의 밖에 수강하지 못함 : 한 key 는 고유하기 때문에 중복해서 
		같은 key 를 삽입할 수 없음. 이후 key 를 삽입하려 할 때는 key, value 삽입 자체가 무시됨.
	*/
	map<unsigned long long, vector<Lecture*>>& getStudentLectureList();
	map<unsigned long long, vector<Lecture*>>& getInstructorLectureList();
};

#endif // ! _ENROLLMANAGER_H_
