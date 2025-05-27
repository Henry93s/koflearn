#ifndef _KOFLEARN_PLATFORM_
#define _KOFLEARN_PLATFORM_
#include <vector>
#include <iostream>
using namespace std;

class KoflearnPlatform {
// 글로벌 id 변수인 primary key
protected:
	unsigned long long primaryKey; // 글로벌 id 변수인 primary key
	// 해당 primaryKey 를 통해서 특정 멤버나 강의를 식별할 수 있음 !

public:
	KoflearnPlatform(unsigned long long primaryKey)
		: primaryKey(primaryKey) {}
	virtual ~KoflearnPlatform() {}

	virtual unsigned long long getPrimaryKey() = 0;
	virtual void setPrimaryKey(unsigned long long primaryKey) = 0;
};

#endif // !_KOFLEARN_PLATFORM_
