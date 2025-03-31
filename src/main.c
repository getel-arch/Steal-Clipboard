#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <string.h>

#pragma comment(lib, "psapi.lib")

int main(void) {
    const char *targetExe = "chrome.exe";
    printf("Waiting for clipboard data set by %s...\n", targetExe);

    while (1) {
        // Get the window that owns the clipboard
        HWND hwndOwner = GetClipboardOwner();
        if (hwndOwner != NULL) {
            DWORD processID = 0;
            // Get process ID associated with the window handle
            GetWindowThreadProcessId(hwndOwner, &processID);
            if (processID != 0) {
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
                if (hProcess != NULL) {
                    char processPath[MAX_PATH] = {0};
                    if (GetModuleFileNameEx(hProcess, NULL, processPath, MAX_PATH)) {
                        size_t lenPath = strlen(processPath);
                        size_t lenTarget = strlen(targetExe);
                        if (lenPath >= lenTarget) {
                            // Compare the end of the processPath with "chrome.exe" (case-insensitive)
                            if (_stricmp(processPath + (lenPath - lenTarget), targetExe) == 0) {
                                printf("Clipboard data set by %s detected.\n", targetExe);
                                CloseHandle(hProcess);

                                // Open the clipboard to retrieve its data
                                if (OpenClipboard(NULL)) {
                                    // Try to get Unicode text from the clipboard
                                    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
                                    if (hData != NULL) {
                                        LPCWSTR pszText = (LPCWSTR)GlobalLock(hData);
                                        if (pszText != NULL) {
                                            wprintf(L"Clipboard text: %s\n", pszText);
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
                                break;  // Exit loop after printing data
                            }
                        }
                    }
                    CloseHandle(hProcess);
                }
            }
        }
        // Sleep for 100ms before checking again
        Sleep(100);
    }
    return 0;
}
