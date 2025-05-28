#ifndef _KOFLEARN_MANAGER_
#define _KOFLEARN_MANAGER_
#include "memberManager.h"
#include "lectureManager.h"

#include <iostream>

class KoflearnPlatManager {
private:
	static KoflearnPlatManager* instance;
	KoflearnPlatManager(){};

public:
	static KoflearnPlatManager* getInstance();

	MemberManager memberManager;
	LectureManager lectureManager;
};

#endif // !_KOFLEARN_MANAGER_
