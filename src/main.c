#include <stdio.h>
#include <windows.h>
#include <psapi.h>
#include <string.h>

#pragma comment(lib, "psapi.lib")

void monitorClipboard(const char *targetExe);
int isTargetProcess(HWND hwndOwner, const char *targetExe);
void printClipboardData();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <target_process_name>\n", argv[0]);
        return 1;
    }

    const char *targetExe = argv[1];
    printf("Waiting for clipboard data set by %s...\n", targetExe);
    monitorClipboard(targetExe);

    return 0;
}

void monitorClipboard(const char *targetExe) {
    while (1) {
        HWND hwndOwner = GetClipboardOwner();
        if (hwndOwner != NULL && isTargetProcess(hwndOwner, targetExe)) {
            printClipboardData();
            break;  // Exit loop after printing data
        }
        Sleep(100);  // Sleep for 100ms before checking again
    }
}

int isTargetProcess(HWND hwndOwner, const char *targetExe) {
    DWORD processID = 0;
    GetWindowThreadProcessId(hwndOwner, &processID);
    if (processID == 0) return 0;

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) return 0;

    char processPath[MAX_PATH] = {0};
    int result = 0;
    if (GetModuleFileNameEx(hProcess, NULL, processPath, MAX_PATH)) {
        size_t lenPath = strlen(processPath);
        size_t lenTarget = strlen(targetExe);
        if (lenPath >= lenTarget && _stricmp(processPath + (lenPath - lenTarget), targetExe) == 0) {
            result = 1;
        }
    }
    CloseHandle(hProcess);
    return result;
}

void printClipboardData() {
    if (OpenClipboard(NULL)) {
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
}
