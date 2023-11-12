#include <windows.h>
#include <cstdio>
#include <tchar.h>
#include <psapi.h>
#include <string>
#include <thread>
#include <iostream>
#include <map>
#include "argparse.hpp"
#include <winhttp.h>
#pragma comment(lib, "winhttp.lib")

using namespace std;

string VERSION = "0.0.0.29";

void PrintProcessNameAndID( DWORD processID ) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != NULL) {
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod),&cbNeeded, LIST_MODULES_ALL))
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
    }
    _tprintf( TEXT("%s  (PID: %lu)\n"), szProcessName, processID );
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

void checkVersion() {
    DWORD dwSize = 0; DWORD dwDownloaded = 0; LPSTR pszOutBuffer; BOOL bResults = FALSE; HINTERNET hSession = nullptr, hConnect = nullptr, hRequest = nullptr;
    std::string extractedVersion;
    hSession = WinHttpOpen(L"WinHTTP SESSIONKILLER", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession) hConnect = WinHttpConnect(hSession, L"api.j3rzy.dev", INTERNET_DEFAULT_HTTP_PORT, 0);
    if (hConnect) hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/versions/processkiller-recode", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hRequest) bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (bResults) bResults = WinHttpReceiveResponse(hRequest, nullptr);
    std::string version;
    if (bResults) {
        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) printf("Error %u in WinHttpQueryDataAvailable.\n", GetLastError());
            pszOutBuffer = new char[dwSize + 1];
            if (!pszOutBuffer) {
                printf("Out of memory\n");
                dwSize = 0;
            }
            else {
                ZeroMemory(pszOutBuffer, dwSize + 1);
                if (!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) printf("Error %lu in WinHttpReadData.\n", GetLastError());
                else version += pszOutBuffer;
                delete[] pszOutBuffer;
            }
        } while (dwSize > 0);
        size_t startPos = version.find(R"("version":")");
        if (startPos != std::string::npos) {
            startPos += 11; // length of "\"version\":\""
            size_t endPos = version.find('\"', startPos);
            if (endPos != std::string::npos) {
                extractedVersion = version.substr(startPos, endPos - startPos);
            }
        }
    }
    if (!bResults) printf("Error %lu has occurred.\n", GetLastError());
    if (hRequest) WinHttpCloseHandle(hRequest); if (hConnect) WinHttpCloseHandle(hConnect); if (hSession) WinHttpCloseHandle(hSession);
    string latestReleaseStr = extractedVersion; std::string latestReleaseTag = latestReleaseStr;
    if (latestReleaseStr.empty()) return;
    latestReleaseStr.erase(std::remove(latestReleaseStr.begin(), latestReleaseStr.end(), '.'), latestReleaseStr.end());
    string currentVersionStr = VERSION; currentVersionStr.erase(std::remove(currentVersionStr.begin(), currentVersionStr.end(), '.'), currentVersionStr.end());
    int latestRelease = std::stoi(latestReleaseStr, nullptr, 10); int currentVersion = std::stoi(VERSION, nullptr, 10);
    if (currentVersion == latestRelease) cout << "You are running latest release!" << endl;
    else if (currentVersion < latestRelease) cout << "You are running outdated version of program." << endl
    << "Download latest release at https://github.com/JamJestJerzy/ProcessKiller-Recode/releases/tag/" << latestReleaseTag << endl;
    else cout << "You are running never version than is released." << endl
    << "Any bugs report to issues@j3rzy.dev" << endl;
}

int main(int argc, char* argv[]) {
    argparse::ArgumentParser program("ProcessKiller-Recode", VERSION);

    program.add_argument("delay").help("Delay between scans.").scan<'i', int>();

    try { program.parse_args(argc, argv); }
    catch (const std::exception& err) { std::cerr << err.what() << std::endl; std::cerr << program; return 1; }

    auto delay = program.get<int>("delay");
    cout << delay << endl;

    checkVersion()

    while (true) {
        EnumerateThru();
        this_thread::sleep_for(chrono::milliseconds(50));
    }

    return 0;
}










