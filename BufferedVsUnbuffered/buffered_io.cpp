
#include <cstdio>
#include <ctime>
#include <iostream>
using namespace std;

void bufferedIO(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (!file) {
        cout << "Error opening file!" << endl;
        return;
    }

    FILE* outFile = fopen("output_buffered.txt", "wb");
    if (!outFile) {
        fclose(file);
        cout << "Error creating output file!" << endl;
        return;
    }

    char buffer[8192];
    size_t bytesRead;

    clock_t start = clock();
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        fwrite(buffer, 1, bytesRead, outFile);
    }
    clock_t end = clock();

    cout << "Buffered I/O time: " << double(end - start) / CLOCKS_PER_SEC << " seconds" << endl;

    fclose(file);
    fclose(outFile);
}

int main() {
    const char* filePath = "large_file.txt";
    bufferedIO(filePath);
    return 0;
}
