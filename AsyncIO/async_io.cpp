
#include <windows.h>
#include <iostream>
using namespace std;

void asyncIO(const char* filePath) {
    HANDLE fileHandle = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        cout << "Error opening file: " << GetLastError() << endl;
        return;
    }

    OVERLAPPED overlap = {0};
    char buffer[8192];
    DWORD bytesRead;

    overlap.Offset = 0;

    if (ReadFile(fileHandle, buffer, sizeof(buffer), &bytesRead, &overlap)) {
        cout << "Asynchronous read completed immediately with " << bytesRead << " bytes read." << endl;
    } else if (GetLastError() == ERROR_IO_PENDING) {
        cout << "Read is pending..." << endl;
        WaitForSingleObject(fileHandle, INFINITE);
        GetOverlappedResult(fileHandle, &overlap, &bytesRead, TRUE);
        cout << "Asynchronous read completed with " << bytesRead << " bytes read." << endl;
    }

    CloseHandle(fileHandle);
}

int main() {
    const char* filePath = "example_async.txt";
    asyncIO(filePath);
    return 0;
}
