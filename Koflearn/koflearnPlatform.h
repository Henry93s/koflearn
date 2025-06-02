#ifndef _KOFLEARN_PLATFORM_
#define _KOFLEARN_PLATFORM_
#include <vector>
#include <iostream>
using namespace std;

class KoflearnPlatform {
	// �۷ι� id ������ primary key
protected:
	unsigned long long primaryKey; // �۷ι� id ������ primary key
	// �ش� primaryKey �� ���ؼ� Ư�� ����� ���Ǹ� �ĺ��� �� ���� !

public:
	KoflearnPlatform(unsigned long long primaryKey)
		: primaryKey(primaryKey) {
	}
	virtual ~KoflearnPlatform() {}

	// getPrimaryKey �� setPrimarykey �� ��� ��ü�� ������ ����̱� ������ �������� �Ÿ��� �����Ƿ�
	// ���� ���� �Լ��� �������� ������, ���� �������� ���� �Ϲ� ���� �Լ��δ� �����Ͽ���.
	virtual unsigned long long getPrimaryKey() const;
	virtual void setPrimaryKey(unsigned long long primaryKey);

	// (250527 fix)
	// ���� ���� �Լ� : void displayInfo()
	// CRUD �� ��� �� �Ļ� Ŭ���� Manager �� ���������, 
	// �� �Ļ� Ŭ���� "��ü ��ü" ���� ���� ����� �Ļ� Ŭ���� ���� ���ǵǴ°� ����.
	// => displayInfo() �Լ��� �⺻ Ŭ������ ���� ���� �Լ��� �����ϰ�, �Ļ� Ŭ�������� �̸� overriding �Ѵ�
	virtual void displayInfo() = 0;
};

#endif // !_KOFLEARN_PLATFORM_