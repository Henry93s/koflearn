#ifndef IKOFLEARN_PLAT_MANAGER_H
#define IKOFLEARN_PLAT_MANAGER_H

#include <iostream>
using namespace std;

// ��ȯ ���� ������ �� �Ŵ��� Ŭ�������� [���� ����]
/* ���� ���� ��� CASE
 ��� ���Ͽ��� �ٸ� Ŭ������ ��ü ���ǰ� �ʿ� ����
 �ܼ��� �����ͳ� ���� Ÿ�����θ� ����� ���, #include ��� ���� ������ ���
*/
class MemberManager;
class LectureManager;
class LoginManager;
class EnrollManager;
class MyPageManager;
class SessionManager;

// KoflearnPlatManager �� �������̽� ����
class IKoflearnPlatManager {
public:
	virtual ~IKoflearnPlatManager() = 0;

	// ��� �Ŵ���(��Ʈ�ѷ�) �� ���� ���� getter ���� �Լ��� ����
	virtual MemberManager& getMemberManager() = 0;
	virtual LectureManager& getLectureManager() = 0;
	virtual LoginManager& getLoginManager() = 0;
	virtual EnrollManager& getEnrollManager() = 0;
	virtual MyPageManager& getMyPageManager() = 0;
	// SessionManager ������� & Ŭ���� �߰�
	virtual SessionManager& getSessionManager() = 0;
};

#endif