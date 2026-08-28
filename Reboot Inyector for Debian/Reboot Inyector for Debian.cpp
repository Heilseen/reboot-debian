#include <windows.h>
#include <iostream>

void printErr(const char* msg) {
    DWORD e = GetLastError();
    std::cerr << msg << " (GetLastError=" << e << ")" << std::endl;
}

int main() {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = {0};
    
    char cmd[] = "FortniteClient-Win64-Shipping.exe -server -log -NoEAC -NoBattlEye -NoMCP";
    
    char dllPath[MAX_PATH];
    if (!GetFullPathNameA("ProjectReboot.dll", MAX_PATH, dllPath, NULL)) {
        printErr("[ERROR] GetFullPathNameA failed");
        return 1;
    }
    std::cout << "[INFO] Target DLL Path: " << dllPath << std::endl;
    
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        printErr("[ERROR] CreateProcessA failed");
        return 1;
    }

    // Optional: wait briefly to let the process initialize / map modules
    Sleep(5000);
    
    SIZE_T size = strlen(dllPath) + 1;
    LPVOID pRemoteBuf = VirtualAllocEx(pi.hProcess, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!pRemoteBuf) {
        printErr("[ERROR] VirtualAllocEx failed");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    if (!WriteProcessMemory(pi.hProcess, pRemoteBuf, (LPVOID)dllPath, size, NULL)) {
        printErr("[ERROR] WriteProcessMemory failed");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }
    
    LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    if (!pLoadLibrary) {
        printErr("[ERROR] GetProcAddress(LoadLibraryA) failed");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteBuf, 0, NULL);
    if (!hThread) {
        printErr("[ERROR] CreateRemoteThread failed");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    WaitForSingleObject(hThread, INFINITE);

    DWORD exitCode = 0;
    if (!GetExitCodeThread(hThread, &exitCode)) {
        printErr("[ERROR] GetExitCodeThread failed");
    } else if (exitCode == 0) {
        std::cout << "[ERROR] The game REJECTED the .dll file." << std::endl;
    } else {
        std::cout << "[SUCCESS] ProjectReboot.dll successfully injected!" << std::endl;
    }

    // Resume main thread and wait on process; check returns
    if (ResumeThread(pi.hThread) == (DWORD)-1) {
        printErr("[ERROR] ResumeThread failed");
    } else {
        std::cout << "[INFO] Server running... keeping process alive." << std::endl;
        WaitForSingleObject(pi.hProcess, INFINITE);
    }

    CloseHandle(hThread);
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}
