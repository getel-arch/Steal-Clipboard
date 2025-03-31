# Steal-Clipboard

[![Build & Release](https://github.com/getel-arch/Steal-Clipboard/actions/workflows/build_and_release.yaml/badge.svg)](https://github.com/getel-arch/Steal-Clipboard/actions/workflows/build_and_release.yaml)

## Overview

**Steal-Clipboard** is a tool designed to monitor the clipboard and retrieve text data set by a specific process. The target process name is passed as a parameter when running the program. The program continuously checks the clipboard owner and retrieves text data when the specified process sets it.

> **Note:** This tool is for educational purposes only. Unauthorized use of this tool to access or monitor clipboard data without consent is unethical and may violate laws or regulations.

## Features

- Monitors clipboard activity in real-time.
- Retrieves text data set by a specific process (specified as a parameter).
- Outputs the retrieved clipboard text to the console.
- Optionally logs the clipboard text to a specified file.

## Disclaimer

This tool is intended for educational and ethical use only. Misuse of this tool to monitor or access clipboard data without proper authorization may violate privacy laws and ethical guidelines. The authors are not responsible for any misuse of this tool.

## License

This project is licensed under the [GNU General Public License v3.0](LICENSE).

## How to Build

### Prerequisites

- **Windows OS**: The tool is designed to run on Windows.
- **GCC Compiler**: Ensure `gcc` is installed and available in your PATH.
- **Dependencies**: The program uses the `psapi` library, which is included in Windows SDK.

### Build Instructions

1. Open a terminal or command prompt.
2. Navigate to the `src` directory:
   ```bash
   cd /workspaces/Steal-Clipboard/src
   ```
3. Compile the program using gcc:
    ```bash
    gcc main.c -o steal_clipboard_x64.exe -s -m64 -lpsapi
    ```

## How to Run

1. After building the program, navigate to the directory containing the compiled binary:
   ```bash
   cd /workspaces/Steal-Clipboard/src
   ```
2. Run the program with the target process name as a parameter:
   ```bash
   ./steal_clipboard_x64.exe <target_process_name>
   ```
   For example:
   ```bash
   ./steal_clipboard_x64.exe chrome.exe
   ```
3. Optionally, specify a log file path to save the clipboard text:
   ```bash
   ./steal_clipboard_x64.exe <target_process_name> <log_file_path>
   ```
   For example:
   ```bash
   ./steal_clipboard_x64.exe chrome.exe clipboard_log.txt
   ```
4. The program will monitor the clipboard and output text data set by the specified process (e.g., `chrome.exe`) to the console and optionally log it to the specified file.
