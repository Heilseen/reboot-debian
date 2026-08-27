# Project Reboot Injector for Debian 🚀

A lightweight, command-line C++ injector designed to launch Fortnite in dedicated server mode and inject `ProjectReboot.dll` seamlessly. Perfect for Debian/Linux VPS environments running Wine and Xvfb.

## Features
* **Suspended Launch:** Opens the game process in a suspended state to prevent crashes before injection.
* **Automated Injection:** Allocates memory and forces the game to load the server DLL.
* **Lightweight:** No unnecessary dependencies, compiled purely in C++.

## How to Use (For Server Owners)
1. Download `Reboot Inyector for Debian.exe` from the **Releases** tab.
2. Place the injector in the same folder as your game executable (`FortniteGame/Binaries/Win64/`).
3. Ensure `ProjectReboot.dll` is also in the exact same folder.
4. Run the injector using Wine. **Note the quotes around the file name:**
   ```bash
   xvfb-run -a wine "Reboot Inyector for Debian.exe"
