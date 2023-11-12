#include <windows.h>
#include <cstdio>
#include <tchar.h>
#include <psapi.h>
#include <string>
#include <thread>
#include <iostream>
#include <map>
#include <winhttp.h>
#include <fstream>
#include <Tlhelp32.h>
#include <winbase.h>
#include "argparse.hpp"

#pragma comment(lib, "winhttp.lib")

using namespace std;

string VERSION = "0.0.2.1";
int scanIterator = 0;
string toKill[32] = {};
bool scan = true;
string configFile = "blacklist.proc";

string RemoveWhiteSpaces(string input) {
    input.erase(std::remove_if(input.begin(), input.end(), [](char c) {return std::isspace(static_cast<unsigned char>(c));}), input.end());
    return input;
}

void KillProcessByName(const string& filename) {
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
    PROCESSENTRY32 pEntry; pEntry.dwSize = sizeof (pEntry); BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes) {
        if (pEntry.szExeFile == filename) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD) pEntry.th32ProcessID);
            if (hProcess != nullptr) {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}

void PrintProcessNameAndID(DWORD processID, bool extensiveLogging) {
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess != nullptr) {
        HMODULE hMod;
        DWORD cbNeeded;
        if (EnumProcessModulesEx(hProcess, &hMod, sizeof(hMod),&cbNeeded, LIST_MODULES_ALL))
            GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR));
    }
    string processName = (string)szProcessName;
    if (processName != "<unknown>") {
        if (extensiveLogging) cout << processName << endl;
        scanIterator++;
        for (string processToKill : toKill) {
            if (processToKill.empty()) continue;
            processToKill = RemoveWhiteSpaces(processToKill);
            if (processToKill == processName) {
                cout << "[!] Found " << processToKill << "!" << endl;
                KillProcessByName(processToKill);
            }
        }
    }
    CloseHandle( hProcess );
}

void ScanForProcesses(bool extensiveLogging) {
    DWORD aProcesses[2048], cbNeeded, cProcesses;
    unsigned int i;

    if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded)) return;
    cProcesses = cbNeeded / sizeof(DWORD);
    for ( i = 0; i < cProcesses; i++ )
        if( aProcesses[i] != 0 )
            PrintProcessNameAndID(aProcesses[i], extensiveLogging);
}

void CheckVersion() {
    DWORD dwSize = 0; DWORD dwDownloaded = 0; LPSTR pszOutBuffer; BOOL bResults = FALSE; HINTERNET hSession = nullptr, hConnect = nullptr, hRequest = nullptr;
    std::string extractedVersion;
    hSession = WinHttpOpen(L"WinHTTP SESSIONKILLER-RECODE", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
    if (hSession) hConnect = WinHttpConnect(hSession, L"api.j3rzy.dev", INTERNET_DEFAULT_HTTP_PORT, 0);
    if (hConnect) hRequest = WinHttpOpenRequest(hConnect, L"GET", L"/versions/processkiller-recode", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);
    if (hRequest) bResults = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
    if (bResults) bResults = WinHttpReceiveResponse(hRequest, nullptr);
    std::string version;
    if (bResults) {
        do {
            dwSize = 0;
            if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) printf("Error %lu in WinHttpQueryDataAvailable.\n", GetLastError());
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
    // Logo
    cout << R"(
   _____          __ ________
  /  _  \________/  |\_____  \ __ __  ______________   ____   ____     _____   ___________   ____
 /  /_\  \_  __ \   __\_(__  <|  |  \/  ___/\___   /  /    \ /  _ \   /     \ /  _ \_  __ \_/ __ \
/    |    \  | \/|  | /       \  |  /\___ \  /    /  |   |  (  <_> ) |  Y Y  (  <_> )  | \/\  ___/
\____|__  /__|   |__|/______  /____//____  >/_____ \ |___|  /\____/  |__|_|  /\____/|__|    \___  >
        \/                  \/           \/       \/      \/               \/                   \/
)" << "ProcessKiller-Recode v" << VERSION << " by Jerzy W (https://github.com/JamJestJerzy)" << endl;
    CheckVersion();
    argparse::ArgumentParser program("ProcessKiller-Recode", VERSION);

    program.add_argument("delay").help("Delay between scans.").scan<'i', int>();
    program.add_argument("--debug").help("Enabled debug output").flag();
    program.add_argument("--log").help("Logs every process. Spams terminal a lot. Trust me.").flag();

    try { program.parse_args(argc, argv); }
    catch (const std::exception& err) { std::cerr << err.what() << std::endl; std::cerr << program; return 1; }

    auto delay = program.get<int>("delay");
    bool doExtensiveLogging = (program["--log"] == true);
    bool debugInfo = (program["--debug"] == true);

    /* Getting processes from config file */
    ifstream config(configFile, std::ios::in | std::ios::binary);
    if (!config.is_open()) {
        cout << "[!] Created config file." << endl;
        ofstream create(configFile);
        if (!create.is_open()) {
            cerr << "[X] Error creating config file!" << endl;
            return 1;
        }
        create << "# Never.exe" << endl << "# Gonna.exe" << endl << "# Give.exe" << endl << "# You.exe" << endl << "# Up.exe" << endl << "# Names ARE CaSe SeNsItIvE!" << endl;
        create.close();
        return 0;
    } else {
        string line; int i = 0; int len = 0;
        while (getline(config, line)) {
            if (line.empty()) continue;
            if (line[0] == '#') continue;
            toKill[i] = line;
            i++; len++;
        }
        config.close();
        if (len <= 0) {
            cout << "Write processes you want to kill to config file." << endl;
            return 0;
        }
    }

    cout << "Blacklisted processes:" << endl;
    for (string process : toKill) {
        process = RemoveWhiteSpaces(process);
        if (process.empty()) continue;
        cout << process << endl;
    }
    cout << "Scanning processes with " << delay << "ms delay." << endl;

    while (scan) {
        ScanForProcesses(doExtensiveLogging);
        if (debugInfo) cout << "[?] Scan occured" << endl;
        this_thread::sleep_for(chrono::milliseconds(delay));
    }

    return 0;
}





