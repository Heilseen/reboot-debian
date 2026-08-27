# Project Reboot Injector for Debian 🚀

A lightweight, command-line C++ injector designed to launch Fortnite in dedicated server mode and inject `ProjectReboot.dll` seamlessly. Perfect for Debian/Ubuntu VPS environments running Wine and Xvfb.

---

## ✨ Features

* **Suspended Launch:** Opens the game process in a suspended state to prevent crashes before injection.
* **Anti-Cheat Bypass:** Automatically appends `-NoEAC -NoBattlEye -NoMCP` parameters to the game launch.
* **Absolute Path Resolution:** Fixes common Wine/Linux injection bugs by converting relative DLL paths to absolute Windows paths.
* **Smart Detection:** Monitors the thread exit code to verify if the DLL was actually accepted by the game engine or rejected due to missing dependencies.
* **Lightweight:** No unnecessary dependencies, compiled purely in C++.

---

## ⚠️ Where is `ProjectReboot.dll`? (IMPORTANT)

Due to DMCA and Epic Games' copyright policies, **the compiled `.dll` is NOT provided in this repository**. You must compile it yourself from the original source.

### How to compile it for Linux/Wine (The `/MT` trick):

1. Download the source code from the [Official Project Reboot 3.0 Repository](https://github.com/Milxnor/Project-Reboot-3.0).
2. Open `ProjectReboot.sln` in Visual Studio 2022.
3. Change the build configuration at the top to **Release** and **x64**.
4. Right-click the `ProjectReboot` project in the Solution Explorer -> **Properties**.
5. Go to **C/C++** > **Code Generation**.
6. Change **Runtime Library** to **`Multi-threaded (/MT)`** (Do NOT use the one with DLL). *This statically links C++ libraries so Wine accepts the file without crashing.*
7. Click Apply, then Build the solution. Your `ProjectReboot.dll` is now ready for Linux.

---

## 🛠️ Requirements (Linux VPS)

Before running the injector, your Debian/Ubuntu server needs the capability to emulate Windows environments and run C++ redistributables. Run these commands as root:

```bash
# 1. Install base packages (including Wine and Screen)
sudo apt update
sudo apt install wine xvfb winbind cabextract wget screen -y

# 2. Download and setup Winetricks
sudo wget https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks -O /usr/local/bin/winetricks
sudo chmod +x /usr/local/bin/winetricks

# 3. Install Visual Studio C++ libraries in Wine
xvfb-run -a winetricks -q vcrun2015
```

---

## 🚀 How to Use (For Server Owners)

Before running the server, make sure your basic network and configuration are ready.

### 1. Open your ports (Firewall):

Players won't be able to join if your VPS blocks the connection. Open port **7777** (the default Unreal Engine port) for both TCP and UDP. If you are using `ufw` on Debian/Ubuntu, run:

```bash
sudo ufw allow 7777/tcp
sudo ufw allow 7777/udp
```

### 2. Prepare the files:

Download `Reboot Inyector for Debian.exe` from the **Releases** tab. Ensure you have your statically compiled `ProjectReboot.dll` ready.

### 3. Place the files:

Move both files inside your game directory, right next to the game executable. The path usually looks like this:

```
FortniteGame/Binaries/Win64/
```

### 4. Keep the server alive 24/7 (Screen):

If you launch the server normally, it will instantly shut down the moment you close your SSH client (PuTTY, Termius, etc.). To prevent this, use a virtual terminal like `screen`:

```bash
screen -S fortniteserver
```

### 5. Launch the server:

Navigate to the folder and run the injector using Wine. *(Note the quotes around the file name!)*:

```bash
cd /path/to/your/FortniteGame/Binaries/Win64/
xvfb-run -a wine "Reboot Inyector for Debian.exe"
```

> 💡 **Tip:** Once the server is running, press `Ctrl + A` and then `D` to detach the screen. You can now safely close your SSH terminal, and the server will stay online!

---

## 💻 How to Compile the Injector (For Developers)

If you want to modify the source code and compile this injector yourself:

1. Clone this repository.
2. Open the `.sln` file in **Visual Studio 2022** (Make sure you have the *"Desktop development with C++"* workload installed).
3. Set the build configuration at the top menu to **Release** and **x64**.
4. Build the solution (`Ctrl + Shift + B`).
5. The freshly compiled `.exe` will be located in the `x64/Release` folder.

---

## 📄 License

This project is licensed under the **MIT License** - see the `LICENSE` file for details.

---

<div align="center">

⭐ **If this project helped you, consider giving it a star!** ⭐

</div>
