#ifndef _KOFLEARN_MANAGER_
#define _KOFLEARN_MANAGER_
#include "memberManager.h"
#include "lectureManager.h"
#include "loginManager.h"
#include "myPageManager.h"
#include "enrollManager.h"
#include "sessionManager.h"
#include "IKoflearnPlatManager.h"
#include <iostream>

// �������̽��� ��ӹ��� ����ü
class KoflearnPlatManager : public IKoflearnPlatManager {
private:
	// ���� manager ��ü�� ����
	// KoflearnPlatManager ���� ��Ʈ�ѷ� Manager Ŭ������ ��ü�� ����� �� ��
	// �̹� ���� manager ��ü���� KoflearnPlatManager() �������� �ʱ�ȭ ����Ʈ���� 
	// 'this' �����͸� ����Ͽ� �� �Ŵ����� �����ڿ� ���� !!
	// �ش� ��ü���� KoflearnPlatManager ���� ��Ʈ�ѷ��� ���� �ֱ⸦ �Բ��Ѵ�.
	MemberManager memberManager;
	LectureManager lectureManager;
	LoginManager loginManager;
	EnrollManager enrollManager;
	MyPageManager myPageManager;
	SessionManager sessionManager;

public:
	KoflearnPlatManager();

	// ���� ��Ʈ�ѷ� Manager Ŭ������ �ִ� Ư�� ��� ������ ���� reference value ��ȯ�� ����
	MemberManager& getMemberManager() override;
	LectureManager& getLectureManager() override;
	LoginManager& getLoginManager() override;
	MyPageManager& getMyPageManager() override;
	EnrollManager& getEnrollManager() override;
	SessionManager& getSessionManager() override;

	// main ���� KoflearnPlatManager �� �� �� �����ϹǷ� displayMenu() ��� �Լ��� 
	// ���� ��Ʈ�ѷ� Manager(�������̽� ����ü) ���� ���� !
	void displayMenu(IKoflearnPlatManager* program);
};

#endif // !_KOFLEARN_MANAGER_