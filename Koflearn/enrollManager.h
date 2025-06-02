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
	// Manager �� �������̽��� �����ϵ��� �������̽� ������ ����
	IKoflearnPlatManager* program_interface;

private:
	/*
		std �����̳��� key�� ������ Ÿ���� ���,
		���� �����Ͱ� ����Ű�� ��ü�� ������ �ƴ϶� ������ �ּ� ������ �˻��Ͽ�
		�ǵ����� ���� ��ȯ ���̳� ���� �߻� ���� ����
		-> Ű�� Ŭ���� ��ü ������ �� ���� ��� ��(2��) ���� <unsigned long long, Lecture*> �� ����
			(���� unsigned long long ���� member �� primarykey �� ��)
	*/
	map<unsigned long long, vector<Lecture*>> studentLectureList;
	map<unsigned long long, vector<Lecture*>> instructorLectureList;

public:
	// �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
	EnrollManager(IKoflearnPlatManager* program);
	~EnrollManager();

	void searchAndStudentEnrollLecture();
	void instructorEnrollLecture();
	bool isDuplicationOrSizeCheckStudentEnrollLecture(Member* member, Lecture* lecture);
	// �л��� �����ϴ� Ư�� �� ���� ã��
	Lecture* findStudentLectureFromList(Lecture* lecture);
	// ���簡 �����ϴ� Ư�� �� ���� ã��
	Lecture* findInstructorLectureFromList(Lecture* lecture);
	// �л��� �����ϴ� ��� ���� ã��
	vector<Lecture*>& findStudentLectureAllList(unsigned long long primaryKey);
	// ���簡 �����ϴ� ��� ���� ã��
	vector<Lecture*>& findInstructorLectureAllList(unsigned long long primaryKey);

	string makeWelcomeText();

	vector<string> parseCSV(istream&, char);

	// 1.
	// �����̳� ��ü�� ��� Ư�� ������ ���� �Լ����� ��ȯ�� ���� �Ҵ����� ��,
	// "�ӽ� ��ü" �� �����ǰ� ��ȯ ���� ; �� ���� ������ ������ �ӽ� �����̳� ��ü�� �Ҹ��.
	// => "��۸� ������" �̽� �߻�!!
	// * �ذ� : �����̳� ��ü�� ������ �Ҵ��Ͽ� ��ȯ�� ��, "�������� �ʰ�" "����" ���� ��ȯ�Ѵ�.
	// 2. stl �����̳� ��� ���� : map<unsigned long long, Lecture*> ������ ���
	/*
		�� �л��� �ϳ��� ���� �ۿ� �������� ���� : �� key �� �����ϱ� ������ �ߺ��ؼ�
		���� key �� ������ �� ����. ���� key �� �����Ϸ� �� ���� key, value ���� ��ü�� ���õ�.
	*/
	map<unsigned long long, vector<Lecture*>>& getStudentLectureList();
	map<unsigned long long, vector<Lecture*>>& getInstructorLectureList();
};

#endif // ! _ENROLLMANAGER_H_