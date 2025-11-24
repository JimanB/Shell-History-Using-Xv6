# xv6 Shell History Implementation

## Overview
This project extends the xv6 operating system's shell (`sh.c`) to maintain a command history feature. All commands entered by users are logged to a file called `sh_history`, and a separate `history` program is provided to display the command history.

## Features
- **Command Logging**: All shell commands are automatically logged to `sh_history` before execution
- **History Viewing**: A dedicated `history` program displays the complete command history
- **File Persistence**: Command history persists across shell sessions (until system reboot)
- **Backward Compatibility**: Existing shell functionality remains unchanged

## Requirements
- Extend xv6 shell implementation in `sh.c` without modifying existing functionality
- Create `sh_history` file automatically if it doesn't exist
- Log commands before execution
- Implement `history.c` program to read and display command history
- Only add new statements to shell; don't modify existing functionality

## Implementation Details

### Shell Modifications (`sh.c`)
The shell has been enhanced with:
- **History File Management**: Opens `sh_history` once at shell startup using `O_WRONLY | O_CREATE` flags
- **Command Logging**: Writes each command to the history file before execution
- **Error Handling**: Proper error checking for file operations
- **Clean Resource Management**: History file is properly closed when shell exits

### History Program (`history.c`)
A standalone program that:
- Opens `sh_history` in read-only mode
- Reads and displays the entire command history to the console
- Handles file I/O errors gracefully
- Uses proper system calls for file operations

## Installation & Usage

### 1. Add to xv6
Copy both `sh.c` and `history.c` to your xv6 source directory and modify the `Makefile` to include the history program:

```makefile
# In UPROGS add:
_history\
```

### 2. Compile and Run
```bash
make
make qemu
```

### 3. Usage
The shell automatically logs all commands. To view history:

```bash
history
```

### 4. Sample Output
```
$ echo "Hello World"
Hello World
$ ls
.              1 1 1024
README         2 2 2286
$ history
echo "Hello World"
ls
history
$ cat sh_history
echo "Hello World"
ls
history
cat sh_history
```

## Code Structure

### Shell Modifications (sh.c)
Key additions to the main function:
```c
// History file setup at shell startup
history_fd = open("sh_history", O_WRONLY | O_CREATE);

// Command logging before execution
if(write(history_fd, buf, strlen(buf)) != strlen(buf)) {
    printf("sh: error writing to history file\n");
}

// Proper cleanup on shell exit
close(history_fd);
```

### History Program (history.c)
Complete implementation:
```c
#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main(int argc, char *argv[]) {
    char buf[512];
    int fd, n;
    
    fd = open("sh_history", O_RDONLY);
    if(fd < 0) {
        fprintf(2, "history: cannot open sh_history\n");
        exit(1);
    }
    
    while((n = read(fd, buf, sizeof(buf))) > 0) {
        if (write(1, buf, n) != n) {
            fprintf(2, "history: write error\n");
            close(fd);
            exit(1);
        }
    }
    
    close(fd);
    exit(0);
}
```

## Technical Details

### File Operations
- **sh_history Creation**: Uses `O_CREATE` flag to create file if non-existent
- **Write Mode**: Shell opens file with `O_WRONLY` for appending commands
- **Read Mode**: History program uses `O_RDONLY` for reading
- **Buffer Management**: 512-byte buffer for efficient file reading

### Error Handling
- File open failures are properly reported
- Write errors during command logging are handled gracefully
- Read/write errors in history program are caught and reported

### Persistence Behavior
- Command history persists while xv6 is running
- History is stored in `sh_history` file in the root directory
- File contents remain available until system reboot

## Limitations
- History file grows indefinitely (no size limits)
- No history filtering or search capabilities
- Commands are logged exactly as entered (including errors)
- No protection against concurrent shell instances

## Files Included
- `sh.c` - Modified xv6 shell with history logging
- `history.c` - History viewing program
- `Makefile` - Modified xv6 Makefile (ensure `_history\` is added to `UPROGS`)

## Testing
Test the implementation by:
1. Running various commands in the shell
2. Using the `history` command to verify logging
3. Checking `sh_history` file contents with `cat`
4. Verifying error handling with invalid operations

## Notes
- The implementation maintains full backward compatibility with existing shell features
- Only new code was added to the shell; no existing functionality was modified
- History file management follows xv6 file system conventions
- Proper resource cleanup ensures no file descriptor leaks


