#include <iostream>
#include <windows.h>
#include <cstdio>
#include "koflearnPlatManager.h"
using namespace std;

// 화면을 지우는(Clear Screen) ANSI 이스케이프 코드 를 
// 배포 파일(release EXE 파일) 에도 동작시키기 위한 함수 생성
void enableAnsiColors() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) {
        return;
    }

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode)) {
        return;
    }
}

int main()
{
    // 콘솔이 ANSI 이스케이프 시퀀스를 해석하도록 함
    enableAnsiColors();

    // koflearnPlatManager 구현체
    KoflearnPlatManager mainProgram;

    // displayMenu는 주입받은 program_context를 통해 
    // 이미 mainProgram 내부에 존재하는 매니저 객체들에 접근 가능함
    mainProgram.displayMenu(&mainProgram);
    return 0;
}
