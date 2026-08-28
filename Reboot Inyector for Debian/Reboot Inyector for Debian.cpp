#include <windows.h>
#include <iostream>

int main() {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    char cmd[] = "FortniteClient-Win64-Shipping.exe -server -log -NoEAC -NoBattlEye -NoMCP";
    
    char dllPath[MAX_PATH];
    GetFullPathNameA("ProjectReboot.dll", MAX_PATH, dllPath, NULL);
    std::cout << "[INFO] Target DLL Path: " << dllPath << std::endl;
    
    // 1. Iniciamos el juego NORMAL (el 0 sustituye a CREATE_SUSPENDED)
    if (CreateProcessA("FortniteClient-Win64-Shipping.exe", cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        
        // 2. Le damos a Wine 2.5 segundos para que cargue la memoria del juego
        std::cout << "[INFO] Game started. Waiting 2.5 seconds for memory to map..." << std::endl;
        Sleep(2500); 
        
        // 3. Inyectamos
        LPVOID pRemoteBuf = VirtualAllocEx(pi.hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
        WriteProcessMemory(pi.hProcess, pRemoteBuf, (LPVOID)dllPath, strlen(dllPath) + 1, NULL);
        
        LPVOID pLoadLibrary = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
        HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pRemoteBuf, 0, NULL);
        WaitForSingleObject(hThread, INFINITE);
        
        DWORD exitCode;
        GetExitCodeThread(hThread, &exitCode);
        if (exitCode == 0) {
            std::cout << "[ERROR] The game REJECTED the .dll file." << std::endl;
        } else {
            std::cout << "[SUCCESS] ProjectReboot.dll successfully injected!" << std::endl;
        }
        
        std::cout << "[INFO] Server running... keeping process alive." << std::endl;
        WaitForSingleObject(pi.hProcess, INFINITE);
        
    } else {
        std::cout << "[ERROR] Game executable not found." << std::endl;
    }
    return 0;
}
