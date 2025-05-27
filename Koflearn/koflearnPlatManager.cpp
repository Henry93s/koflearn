#include "koflearnPlatManager.h"

// 소스 코드 파일(cpp)에서 singleton 인스턴스 초기화
KoflearnPlatManager* KoflearnPlatManager::instance = nullptr;

KoflearnPlatManager* KoflearnPlatManager::getInstance() {
    if (!instance)
        instance = new KoflearnPlatManager();
    return instance;
}