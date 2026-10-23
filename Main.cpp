#include <Windows.h>
#include "Controller.h"
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    Controller controller = Controller(hInstance, nCmdShow);
    controller.StartGame();
}