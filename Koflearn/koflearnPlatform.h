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

	// getPrimaryKey 와 setPrimarykey 는 기능 자체는 동일한 기능이기 때문에 다형성과 거리가 있으므로
	// 순수 가상 함수로 구현하지 않으나, 추후 유연성을 위해 일반 가상 함수로는 선언하였음.
	virtual unsigned long long getPrimaryKey() const;
	virtual void setPrimaryKey(unsigned long long primaryKey);
};

#endif // !_KOFLEARN_PLATFORM_
