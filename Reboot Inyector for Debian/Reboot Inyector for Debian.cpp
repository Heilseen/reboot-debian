#include <windows.h>
#include <iostream>

int main() {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // 1. Open the game in server mode, but suspended
    char cmd[] = "FortniteClient-Win64-Shipping.exe -server -log";
    if (CreateProcessA("FortniteClient-Win64-Shipping.exe", cmd, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {

        const char* dllPath = "ProjectReboot.dll";

        // 2. Allocate memory in the game process and write the .dll path
        LPVOID pRemoteBuf = VirtualAllocEx(pi.hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(pi.hProcess, pRemoteBuf, (LPVOID)dllPath, strlen(dllPath) + 1, NULL);

        // 3. Force the game to load our .dll
        LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteBuf, 0, NULL);
        WaitForSingleObject(hThread, INFINITE);

        // 4. Resume the injected game thread
        ResumeThread(pi.hThread);

        std::cout << "Server and map successfully injected!" << std::endl;
    }
    else {
        std::cout << "Error: Game executable not found." << std::endl;
    }
    return 0;
}