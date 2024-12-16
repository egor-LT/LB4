
#include <windows.h>
#include <iostream>
#include <ctime>
using namespace std;

void unbufferedIO(const char* filePath) {
    HANDLE fileHandle = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        cout << "Error opening file: " << GetLastError() << endl;
        return;
    }

    HANDLE outFileHandle = CreateFile("output_unbuffered.txt", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (outFileHandle == INVALID_HANDLE_VALUE) {
        CloseHandle(fileHandle);
        cout << "Error creating output file: " << GetLastError() << endl;
        return;
    }

    char buffer[8192];
    DWORD bytesRead, bytesWritten;

    clock_t start = clock();
    while (ReadFile(fileHandle, buffer, sizeof(buffer), &bytesRead, NULL) && bytesRead > 0) {
        WriteFile(outFileHandle, buffer, bytesRead, &bytesWritten, NULL);
    }
    clock_t end = clock();

    cout << "Unbuffered I/O time: " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;

    CloseHandle(fileHandle);
    CloseHandle(outFileHandle);
}

int main() {
    const char* filePath = "large_file.txt";
    unbufferedIO(filePath);
    return 0;
}
