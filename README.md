<div align="center">

# libcerr

</div>

> [!NOTE]
> `libcerr` is a lightweight C library that extends standard error handling capabilities by providing a simple try-catch-like exception mechanism and enhanced logging utilities. 

## ✨ Features

> - 🎯 **Exception Handling**: A `try-catch`-like system for C using `TRY`, `CATCH`, and `THROW` macros.
> - 📣 **Rich Logging**: Colorful, leveled logging macros (`LOG_INFO`, `LOG_WARN`, `LOG_ERR`, etc.).
> - 🚧 **Assertions**: An `ASSERT` macro with enhanced logging for better debugging.
> - 🔩 **Thread-Local**:  Designed with thread safety in mind using thread-local storage for error contexts. 
> - 🗂️ **Memory Cache**: Automatic memory allocation tracking with `MALLOC()`, `CALLOC()`, `REALLOC()`, `FREE()` macros.
> - 🛠️ **Customizable**: Easily configurable via macros.

## 📖 Basic Usage

### Catching Exceptions
```c
#define CERR_IMPLEMENTATION  // Only define this ONCE in your entire project
#include <libcerr.h>
// Define your own exceptions
#define MY_EXCEPTION 10

void might_fail() {
    THROW_MSG(MY_EXCEPTION, "Function failed !");
}

int main() {
    TRY {
        might_fail();
    } CATCH(MY_EXCEPTION) {
        LOG_ERR("Caught: %s", CERR_WHY());
    }
    might_fail(); // this will abort
}
```
<img src="https://github.com/MykleR/libcerr/blob/main/screenshots/Screenshot_20251009_170959.png" height="200"/>

### Logging

> The logging macros provide colorful, formatted output to `stderr` by default.
> Use CERR_LOG_LEVEL to filter output: 0 is nothing, 1 is OK/ERR, 2 adds WARN, 3 adds INFO and 4 adds DEBUG.

```c
#define CERR_LOG_FDOUT stdout
#define CERR_LOG_LEVEL 3
#include <libcerr.h>

int main() {
	int x = 5;
	LOG_INFO("Starting application with x = %d...", x);
	LOG_DEBUG("This is a debug message. "); // Log level 3, will not print
	LOG_WARN("This is a warning message.");
	LOG_ERR("This is a fatal error message.");
	LOG_OK("Application finished.");
}
```
<img src="https://github.com/MykleR/libcerr/blob/main/screenshots/screenshot_20251003_122852.png" height="200"/>

### Assertions
 > Assertions will exit the program when the condition is not met even if protected by a TRY block. 
```c
int x = 5;
TRY {
    ASSERT(x == 10, "This will fail and not be caught !")
} CATCH_ALL_LOG() {}
```

### Memory Cache

> The library provides memory allocation macros that automatically track all allocations. When the program exits, any unfreed memory is automatically cleaned up and a warning is logged about potential memory leaks.
> The cache system uses a fixed-size hash table (default `CERR_CACHE_SIZE` = 65536 entries) for O(1) average insertion and removal. You can customize this by defining `CERR_CACHE_SIZE` before including the header (must be a power of 2).

```c
#define CERR_CACHE
#define CERR_IMPLEMENTATION
#include <libcerr.h>

int main() {
    char *str = MALLOC(100);      // Tracked allocation
    char *arr = CALLOC(10, sizeof(int));
    str = REALLOC(str, 200);      // Tracked reallocation
    FREE(str);                    // Safe free with double-free prevention
    FREE(arr);
    // Any unfreed memory is automatically cleaned up on exit
}
```

## ⚙️ Configuration

> [!IMPORTANT]
> These macros must be defined **before** including `<libcerr.h>`.

| Macro | Description | Where to Define |
|-------|-------------|-----------------|
| `CERR_IMPLEMENTATION` | Instantiates global variables and cleanup functions required by the library. | Define in **exactly one** source file, preferably your entry point (e.g., `main.c`). |
| `CERR_CACHE` | Enables the automatic memory caching system. Or else `MALLOC()`, `CALLOC()`, `REALLOC()`, and `FREE()` become direct wrappers to standard library functions. | Define first in **all** source files that include `<libcerr.h>` if you want to enable caching.  |
| `CERR_CACHE_SIZE` | Sets the maximum number of tracked allocations (default: `0x10000` = 65536). Must be a power of 2. | Define before including the header if you need a different limit. |
| `CERR_LOG_LEVEL` | Sets the logging verbosity (0-4). | Define before including the header. |
| `CERR_LOG_FDOUT` | Sets the output file descriptor for logging (default: `stderr`). | Define before including the header. |
| `CERR_ASSERT_FDOUT` | Sets the output file descriptor for asserting (default: `stderr`). | Define before including the header. |

> [!NOTE]
> you can also define `CERR_NCACHE` at compile time:
> - `gcc -DCERR_CACHE -I libcerr/headers main.c other.c -o myprogram`

### Example: Multi-file Project Setup

> **main.c** (entry point):
```c
#define CERR_CACHE
#define CERR_IMPLEMENTATION  // Only define this ONCE in your entire project
#include <libcerr.h>

int main() {
	void *ptr = some_function();
    FREE(ptr);
}
```

> **other_file.c** (other source files):
```c
#define CERR_CACHE
#include <libcerr.h>  // No CERR_IMPLEMENTATION here

void *some_function() {
    return MALLOC(256);
}
```

## 🚀 Getting Started

> [!IMPORTANT]
> The project is still in its early stages and, for now, everything fits in the header, so no linking is necessary, just include libcerr.h.
> (GCC or Clang remains mandatory for compilation)

### Prerequisites

> - C Compiler `GCC` or `Clang`
> - `GNU Make`

### Installation & Usage

```bash
# Clone the repository
git clone https://github.com/MykleR/libcerr.git
cd libcerr

# Build it as a static/shared library (Link as you wish and include libcerr.h)
make
```
