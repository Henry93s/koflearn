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
	// Manager �� �������̽��� �����ϵ��� �������̽� ������ ����
	IKoflearnPlatManager* program_interface;

public:
	// �����ڿ��� �������̽� Ÿ�� �������� ���Թ���
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

	// �����̳� ��ü�� ��� Ư�� ������ ���� �Լ����� ��ȯ�� ���� �Ҵ����� ��,
	// "�ӽ� ��ü" �� �����ǰ� ��ȯ ���� ; �� ���� ������ ������ �ӽ� �����̳� ��ü�� �Ҹ��.
	// => "��۸� ������" �̽� �߻�!!
	// * �ذ� : �����̳� ��ü�� ������ �Ҵ��Ͽ� ��ȯ�� ��, "�������� �ʰ�" "����" ���� ��ȯ�Ѵ�.
	map<unsigned long long, Lecture*>& getLectureList();

private:
	map<unsigned long long, Lecture*> lectureList;
};

#endif