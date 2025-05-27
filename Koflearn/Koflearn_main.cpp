#include <iostream>
#include "koflearnPlatManager.h"
using namespace std;

int main()
{
    KoflearnPlatManager* program = KoflearnPlatManager::getInstance();
    
    int ch, key;
    bool isContinue = true;

    while (isContinue == true) {
        cout << "\033[2J\033[1;1H";
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "          Koflearn Main page                 " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << "  1. 로그인                                " << endl;
        cout << "  2. 모든 강의 보기                        " << endl;
        cout << "  3. 신청한 강의 보기                         " << endl;
        cout << "  4. 강의 등록하기                     " << endl;
        cout << "  5. Koflearn 멤버(회원) 관리 프로그램                           " << endl;
        cout << "  6. Koflearn 강의(제품) 관리 프로그램                           " << endl;
        cout << "  7. 종료                              " << endl;
        cout << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
        cout << " 기능을 선택하세요 : ";
        cin >> ch;
        switch (ch) {
        case 1: default:
            // login();
            break;
        case 2:
            // all lecture(); -> Order(강의 구매) 클래스 의 all_displaymenu 로 진입 -> 강의 수강 신청 가능 메소드 추가 예정
            break;
        case 3:
            // my lecture(); (로그인 시 접근 가능 메뉴) -> order 클래스의 my_displaymenu 로 진입
            break;
        case 4:
            // upload lecture(); (로그인 시 접근 가능 메뉴) -> order 클래스의 have_displaymenu 로 진입
            break;
        case 5:
            program->memberManager.displayMenu();
            break;
        case 6:
            // lecturemanager
            break;
        case 7:
            isContinue = false;
            break;
        }
    }

    // ! 싱글톤 인스턴스를 동적으로 할당했다면, 프로그램 종료 전에 반드시 delete !
    // getInstance() 내부에서 new KoflearnPlatManager()를 사용했으므로, 여기서 delete 해야 합니다.
    delete program;

    return 0;
}
