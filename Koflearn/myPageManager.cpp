// koflearnPlatManager.h 에 myPageManager.h 가 포함되어 있음
#include "koflearnPlatManager.h"

#include <string>

MyPageManager::MyPageManager() {}
MyPageManager::~MyPageManager() {}

KoflearnPlatManager* MyPageManager::getInstance() const {
	return KoflearnPlatManager::getInstance();
}

void MyPageManager::displayMenu() {


}