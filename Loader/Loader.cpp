#include <Windows.h>
#include <Entry.h>
#include <Tests.h>
#pragma comment(lib, "HfEngine.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmd, int nCmdShow) {
    HFEngineInitialize();
    //Tests::TestSimpleRender();
    //return 0;
    return HFEngineRubyEntry();
}