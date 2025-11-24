#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <string.h>

#pragma comment(lib, "psapi.lib")

// Prints the clipboard data to the console and optionally logs it to a file
void printClipboardData(const char *logFilePath) {
    if (OpenClipboard(NULL)) {
        // Get the clipboard data in Unicode text format
        HANDLE hData = GetClipboardData(CF_UNICODETEXT);
        if (hData != NULL) {
            LPCWSTR pszText = (LPCWSTR)GlobalLock(hData);
            if (pszText != NULL) {
                // Print the clipboard text to the console
                wprintf(L"Clipboard text: %s\n", pszText);
                // Log the clipboard text to a file if a log file path is provided
                if (logFilePath != NULL) {
                    logClipboardData(pszText, logFilePath);
                }
                GlobalUnlock(hData);
            } else {
                printf("Failed to lock global memory for clipboard data.\n");
            }
        } else {
            printf("No text data available in the clipboard (CF_UNICODETEXT not found).\n");
        }
        CloseClipboard();
    } else {
        printf("Failed to open clipboard.\n");
    }
}

// Checks if the clipboard owner belongs to the target process
int isTargetProcess(HWND hwndOwner, const char *targetExe) {
    DWORD processID = 0;
    // Get the process ID of the clipboard owner
    GetWindowThreadProcessId(hwndOwner, &processID);
    if (processID == 0) return 0;

    // Open the process to query information
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) return 0;

    char processPath[MAX_PATH] = {0};
    int result = 0;
    // Get the executable path of the process
    if (GetModuleFileNameEx(hProcess, NULL, processPath, MAX_PATH)) {
        size_t lenPath = strlen(processPath);
        size_t lenTarget = strlen(targetExe);
        // Compare the executable name with the target process name
        if (lenPath >= lenTarget && _stricmp(processPath + (lenPath - lenTarget), targetExe) == 0) {
            result = 1;
        }
    }
    CloseHandle(hProcess);
    return result;
}

// Monitors the clipboard for data set by the target process
void monitorClipboard(const char *targetExe, const char *logFilePath) {
    while (1) {
        // Get the current clipboard owner window handle
        HWND hwndOwner = GetClipboardOwner();

        // Check if the clipboard owner belongs to the target process
        if (isTargetProcess(hwndOwner, targetExe)) {
            // Print and optionally log the clipboard data
            printClipboardData(logFilePath);
            break;  // Exit loop after printing data
        }

        // Sleep for 100ms before checking again
        Sleep(100);
    }
}

// Logs the clipboard data to the specified file
void logClipboardData(const wchar_t *data, const char *logFilePath) {
    // Open the log file in append mode
    FILE *logFile = fopen(logFilePath, "a");
    if (logFile == NULL) {
        printf("Failed to open log file: %s\n", logFilePath);
        return;
    }

    // Write the clipboard data to the log file
    fwprintf(logFile, L"%s\n", data);
    fclose(logFile);
}

int main(int argc, char *argv[]) {
    // Ensure the correct number of arguments are provided
    if (argc < 2 || argc > 3) {
        printf("Usage: %s <target_process_name> [log_file_path]\n", argv[0]);
        return 1;
    }

    // Get the target process name and optional log file path from arguments
    const char *targetExe = argv[1];
    const char *logFilePath = (argc == 3) ? argv[2] : NULL;

    // Notify the user that monitoring has started
    printf("Waiting for clipboard data set by %s...\n", targetExe);
    monitorClipboard(targetExe, logFilePath);

    return 0;
}
