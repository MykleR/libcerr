<div align="center">

# libcerr

</div>

> [!NOTE]
> `libcerr` is a lightweight C library that extends standard error handling capabilities by providing a simple try-catch-like exception mechanism and enhanced logging utilities.

## ✨ Features

> - 🎯 **Exception Handling**: A `try-catch`-like system for C using `TRY`, `CATCH`, and `THROW` macros.
> - 📣 **Rich Logging**: Colorful, leveled logging macros (`LOG_INFO`, `LOG_WARN`, `LOG_ERR`, etc.).
> - 🚧 **Assertions**: An `ASSERT` macro with enhanced logging for better debugging.
> - 🔩 **Thread-Local**: Designed with thread safety in mind using thread-local storage for error contexts.
> - 🛠️ **Customizable**: Easily configurable via macros.

## 📖 Basic Usage

### Catching Exceptions
```c
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
> Use LOG_LEVEL to filter output: 0 is nothing, 1 is OK/ERR, 2 adds WARN, 3 adds INFO and 4 adds DEBUG.

```c
#define LOG_FDOUT stdout
#define LOG_LEVEL 3
#include <libcerr.h>

int main() {
	int x = 5;
	LOG_INFO("Starting application with x = %d...", x);
	LOG_DEBUG("This is a debug message."); // Log level 3, will not print
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

## 🚀 Getting Started

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
> [!IMPORTANT]
> The project is still in its early stages and, for now, everything fits in the header, so no linking is necessary.
> (GCC or Clang remains mandatory for compilation)

## ✍️ Authors

- **Michael ROUVES**
