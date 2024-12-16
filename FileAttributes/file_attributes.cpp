
#include <windows.h>
#include <iostream>
#include <Aclapi.h>
#include <tchar.h>

using namespace std;

void getFileAttributesAndInfo(const char* filePath) {
    HANDLE fileHandle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        cout << "Error opening file: " << GetLastError() << endl;
        return;
    }

    LARGE_INTEGER fileSize;
    if (GetFileSizeEx(fileHandle, &fileSize)) {
        cout << "File size: " << fileSize.QuadPart << " bytes" << endl;
    }

    FILETIME creationTime, lastAccessTime, lastWriteTime;
    SYSTEMTIME sysTime;

    if (GetFileTime(fileHandle, &creationTime, &lastAccessTime, &lastWriteTime)) {
        FileTimeToSystemTime(&creationTime, &sysTime);
        cout << "Creation time: " << sysTime.wDay << "/" << sysTime.wMonth << "/" << sysTime.wYear << endl;

        FileTimeToSystemTime(&lastAccessTime, &sysTime);
        cout << "Last access time: " << sysTime.wDay << "/" << sysTime.wMonth << "/" << sysTime.wYear << endl;

        FileTimeToSystemTime(&lastWriteTime, &sysTime);
        cout << "Last write time: " << sysTime.wDay << "/" << sysTime.wMonth << "/" << sysTime.wYear << endl;
    }

    DWORD attributes = GetFileAttributes(filePath);
    if (attributes == INVALID_FILE_ATTRIBUTES) {
        cout << "Error getting file attributes: " << GetLastError() << endl;
    } else {
        cout << "File attributes: " << endl;
        if (attributes & FILE_ATTRIBUTE_HIDDEN) cout << " - Hidden" << endl;
        if (attributes & FILE_ATTRIBUTE_READONLY) cout << " - Read-only" << endl;
        if (attributes & FILE_ATTRIBUTE_SYSTEM) cout << " - System file" << endl;
    }

    PSECURITY_DESCRIPTOR pSD = NULL;
    PSID pOwner = NULL;
    BOOL ownerDefaulted;

    if (GetSecurityInfo(fileHandle, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &pOwner, NULL, NULL, NULL, &pSD) == ERROR_SUCCESS) {
        TCHAR ownerName[256];
        DWORD nameSize = sizeof(ownerName);
        TCHAR domainName[256];
        DWORD domainSize = sizeof(domainName);
        SID_NAME_USE sidType;

        if (LookupAccountSid(NULL, pOwner, ownerName, &nameSize, domainName, &domainSize, &sidType)) {
            cout << "Owner: " << ownerName << endl;
        } else {
            cout << "Error getting owner name: " << GetLastError() << endl;
        }
        LocalFree(pSD);
    }

    CloseHandle(fileHandle);
}

int main() {
    const char* filePath = "example.txt";
    getFileAttributesAndInfo(filePath);
    return 0;
}
