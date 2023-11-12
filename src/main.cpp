#include <windows.h>
#include <cstdio>
#include <tchar.h>
#include <psapi.h>
#include <string>
#include <thread>
#include <algorithm>

std::string VERSION = "0.0.0.1";

void PrintProcessNameAndID( DWORD processID ) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod),&cbNeeded, LIST_MODULES_ALL))
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
    }
    _tprintf( TEXT("%s  (PID: %u)\n"), szProcessName, processID );
    CloseHandle( hProcess );
}

void EnumerateThru() {
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return;
    cProcesses = cbNeeded / sizeof(DWORD);
    for ( i = 0; i < cProcesses; i++ )
        if( aProcesses[i] != 0 )
            PrintProcessNameAndID( aProcesses[i] );
}

int main(int argc, char* argv[]) {


    /*
    while (true) {
        EnumerateThru();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
     */
}