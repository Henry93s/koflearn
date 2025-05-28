#include <iostream>
#include "koflearnPlatManager.h"
using namespace std;

int main()
{
    KoflearnPlatManager* program = KoflearnPlatManager::getInstance();
    program->displayMenu();

    return 0;
}
