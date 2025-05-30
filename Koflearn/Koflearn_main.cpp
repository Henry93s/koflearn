#include <iostream>
#include "koflearnPlatManager.h"
using namespace std;

int main()
{
    // koflearnPlatManager 구현체
    KoflearnPlatManager mainProgram;

    // displayMenu는 주입받은 program_context를 통해 
    // 이미 mainProgram 내부에 존재하는 매니저 객체들에 접근 가능함
    mainProgram.displayMenu(&mainProgram);
    return 0;
}
