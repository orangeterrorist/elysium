#include "auth.h"
#include "main.hpp"
#include "XorStr.h"
#include <iostream>
#include <random>
#include <string>
#include <fstream>

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

/* AES stuff later
#include <crypto++>
#include <crypto++/modes.h>
#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/osrng.h>
*/

// SHUT THE FUCK UP YOU STUPID FUCKING PIECE OF SHIT IDE AND BUILD MY PROJECT YOU SUFFER FROM BRAINROT
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)
#pragma warning(disable:4244)
#pragma warning(disable:4267)

#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <vector>
#include <algorithm>

size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

void download_file(const std::string& url, const std::string& path) {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(path.c_str(), "wb");
        if (fp) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            /* always cleanup */
            curl_easy_cleanup(curl);
            fclose(fp);
        }
        else {
            std::cout << "Error: could not create file " << path << std::endl;
        }
    }
}

std::string get_DeviceId();

void Auth::authThread() {

    const std::string VerNum{ "BETA b0.7.3" };
    AllocConsole();

    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);

    std::cout << XorStr("Elysium Client Authentication") << std::endl;
    const char* ely_authtitle = XorStr("Elysium Client Authentication | by neptune#1995 & VT#1065");
    std::string title = ely_authtitle;
    SetConsoleTitleA(title.c_str());
    CURL* curl;
    CURLcode res;

    // Get hardware profile info
    HW_PROFILE_INFO hwProfileInfoStruct;
    if (GetCurrentHwProfileA(&hwProfileInfoStruct)) {
        std::string hwProfileInfo = std::string(hwProfileInfoStruct.szHwProfileGuid);
    }
    curl = curl_easy_init();
    if (curl) {
        const char* auth_url = XorStr("https://google.com/");
        curl_easy_setopt(curl, CURLOPT_URL, auth_url);

        HW_PROFILE_INFO hwProfileInfo;
        GetCurrentHwProfile(&hwProfileInfo);

        char username[UNLEN + 1];
        DWORD username_len = UNLEN + 1;
        GetUserName(username, &username_len);

        char computer_name[MAX_COMPUTERNAME_LENGTH + 1];
        DWORD size = sizeof(computer_name);
        GetComputerNameA(computer_name, &size);

        std::string device_id = get_DeviceId();
        std::string key;

        std::cout << XorStr("Enter your license key: ") << std::endl;
        std::cin >> key;
        const char* auth_pc1 = XorStr("&pc=");
        const char* auth_user1 = XorStr("&user=");
        const char* auth_hwid1 = XorStr("&hwid=");
        const char* auth_hwidl1 = XorStr("&hwidl=");
        const char* auth_key1 = XorStr("&key=");
        const char* auth_v1 = XorStr("&v=");
        const char* auth_vNum1 = XorStr("b0.7.3");

        std::string pc1 = auth_pc1;
        std::string user1 = auth_user1;
        std::string hwid1 = auth_hwid1;
        std::string hwidl1 = auth_hwidl1;
        std::string key1 = auth_key1;
        std::string v1 = auth_v1;
        std::string vNum1 = auth_vNum1;


        std::string data = pc1 + std::string(computer_name) + user1 + std::string(username) + hwid1 + device_id + hwidl1 + std::string(hwProfileInfo.szHwProfileGuid) + key1 + key + v1 + vNum1;

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

        const char* auth1_data = XorStr("&data=");
        std::string enreq = auth1_data + encoded_data;

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, enreq.c_str());

        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, XorStr("Elysium"));
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        res = curl_easy_perform(curl);


        const char* auth_keykey = XorStr("key");
        const char* auth_message = XorStr("Message");
        const char* auth_b = XorStr("b");
        const char* auth_q = XorStr("q");

        if (res != CURLE_OK) {
            std::cout << XorStr("Authentication Error: ") << curl_easy_strerror(res) << std::endl;
            std::exit(0);
        }
        else {
            nlohmann::json jsonResponse = nlohmann::json::parse(response);

            if (jsonResponse.find(auth_message) != jsonResponse.end())
            {
                if (std::string(jsonResponse[auth_message].get<std::string>().c_str()) == auth_b)
                {
                    BOOLEAN bl;
                    ULONG Response;
                    RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
                    NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    std::exit(0);
                }
                else if (std::string(jsonResponse[auth_message].get<std::string>().c_str()) == auth_q)
                {
                    std::exit(0);
                }
                else
                {
                    MessageBox(NULL, jsonResponse[auth_message].get<std::string>().c_str(), "Elysium Notification", MB_OK);
                }
            }

            if (jsonResponse[auth_keykey] == key + device_id)
            {

                system("cls");
                std::cout << XorStr("Session Authenticated.") << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                system(XorStr("cls"));

                const char* ely_e1 = XorStr("Elysium Client ");
                const char* ely_e2 = XorStr(" >:3 | by neptune#1995 & VT#1065");
                const char* ely_e3 = XorStr("Loading Elysium Client ");
                const char* ely_e4 = XorStr(" . . . \nMade by neptune#1995 & VT#1065");

                const char* font_url = XorStr("https://github.com/mikage-961pro/Apple-Fonts/raw/master/SF%20Font/windows/SF-Pro-Display-Regular.otf");
                const char* font_path = XorStr("/Documents");

                std::string url = font_url;
                // Get the home directory
                const char* homeDir = getenv(XorStr("USERPROFILE"));

                // Check if we were able to find the home directory
                if (homeDir == nullptr) {
                    std::cout << XorStr("Error Code: 5 (contact neptune#1995)") << std::endl;
                }

                // Construct the path to the downloads directory
                std::string path = std::string(homeDir) + "/Documents/SF-Pro-Display-Regular.otf";

                if (!file_exists(path)) {
                    std::cout << XorStr("[Elysium] Font File does not exist, downloading now . . .") << std::endl;
                    download_file("https://github.com/mikage-961pro/Apple-Fonts/raw/master/SF%20Font/windows/SF-Pro-Display-Regular.otf", path);
                    std::cout << XorStr("[Elysium] Font Download complete.") << std::endl;
                }
                else {
                    std::cout << XorStr("[Elysium] Font File Found.") << std::endl;
                }


                std::string title = ely_e1 + std::string(VerNum) + ely_e2;
                SetConsoleTitleA(title.c_str());
                std::cout << ely_e3 + std::string(VerNum) + ely_e4 << std::endl;
                std::thread(&main).detach();
                return;
            }
            else if (jsonResponse[auth_keykey] == "b")
            {
                std::cout << XorStr("Invalid Key!") << std::endl;
                std::cin;
                BOOLEAN bl;
                ULONG Response;
                RtlAdjustPrivilege(19, TRUE, FALSE, &bl); // Enable SeShutdownPrivilege
                NtRaiseHardError(STATUS_ASSERTION_FAILURE, 0, 0, NULL, 6, &Response); // Shutdown
            }
            else
            {
                std::cout << jsonResponse[auth_keykey] << std::endl;
                std::cout << XorStr("Authentication Failed!") << std::endl;
                std::exit(0);
            }
        }
    }
}

std::string get_DeviceId() {
    std::string result;

    const char* hwid1 = XorStr("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0");
    const char* hwid2 = XorStr("ProcessorNameString");

    HKEY hKey;
    DWORD dwType;
    DWORD dwSize = 16;
    std::vector<char> szProcessorId(dwSize + 1);
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, hwid1, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueExA(hKey, hwid2, NULL, &dwType, reinterpret_cast<LPBYTE>(szProcessorId.data()), &dwSize) == ERROR_SUCCESS) {
            szProcessorId[dwSize] = '\0';
            result += szProcessorId.data();
        }
        RegCloseKey(hKey);
    }

    const char* hwid3 = XorStr("HARDWARE\\DESCRIPTION\\System\\BIOS");
    const char* hwid4 = XorStr("BaseBoardProduct");
    const char* hwid5 = XorStr("BaseBoardSerialNumber");

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, hwid3, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = 40;
        std::vector<char> szBaseBoardId(dwSize + 1);
        if (RegQueryValueExA(hKey, hwid4, NULL, &dwType, reinterpret_cast<LPBYTE>(szBaseBoardId.data()), &dwSize) == ERROR_SUCCESS) {
            szBaseBoardId[dwSize] = '\0';
            result += szBaseBoardId.data();
        }


        std::vector<char> szBaseBoardSerial(dwSize + 1);
        if (RegQueryValueExA(hKey, hwid5, NULL, &dwType, reinterpret_cast<LPBYTE>(szBaseBoardSerial.data()), &dwSize) == ERROR_SUCCESS) {
            szBaseBoardSerial[dwSize] = '\0';
            result += szBaseBoardSerial.data();
        }
        RegCloseKey(hKey);
    }

    const char* hwid6 = XorStr("SYSTEM\\MountedDevices");
    const char* hwid7 = XorStr("\\DosDevices\\C:");

    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, hwid6, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        dwSize = 1024;
        std::vector<BYTE> pData(dwSize);
        if (RegQueryValueExA(hKey, hwid7, NULL, &dwType, pData.data(), &dwSize) == ERROR_SUCCESS) {
            std::stringstream ssBootDriveId;
            ssBootDriveId << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(pData[8])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[9])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[10])
                << std::setw(2) << std::setfill('0') << static_cast<int>(pData[11]);
            result += ssBootDriveId.str();
        }
        RegCloseKey(hKey);
    }

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    result += std::to_string(sysInfo.dwNumberOfProcessors);

    ULONGLONG memSizeKB;
    if (GetPhysicallyInstalledSystemMemory(&memSizeKB)) {
        result += std::to_string(memSizeKB);
    }

    uint64_t hash_high = 0x9A15D9AEBB1BDA7CULL;
    uint64_t hash_low = 0x5E7DFF6AA04C6D2FULL;
    uint64_t prime1 = 0x100000001B3ULL;
    uint64_t prime2 = 0xC2B2AE3D27D4EB4FULL;

    for (char c : result) {
        hash_high = (hash_high ^ static_cast<uint64_t>(c)) * prime1;
        hash_low = (hash_low ^ static_cast<uint64_t>(c)) * prime2;
    }

    // Convert the hash to a hexadecimal string
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hash_high
        << std::setw(16) << std::setfill('0') << hash_low;

    return ss.str();
}


