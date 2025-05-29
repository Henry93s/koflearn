#ifndef _MYPAGE_MANAGER_H_
#define _MYPAGE_MANAGER_H_

#include <iostream>
using namespace std;

// KoflearnPlatManager 전방 선언
class KoflearnPlatManager;

class MyPageManager {
public:
	MyPageManager();
	~MyPageManager();
	        
	KoflearnPlatManager* getInstance() const;
	void displayMenu();
	void myStudentLecturePrint();
	void myInstructorLecturePrint();
};

#endif // !_MYPAGE_MANAGER_H_
