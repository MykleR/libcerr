<div align="center">

# libcerr

</div>

> [!NOTE]
> `libcerr` is a lightweight C library that extends standard error handling capabilities by providing a simple try-catch-like exception mechanism and enhanced logging utilities.

## ✨ Features

> - 🎯 **Exception Handling**: A `try-catch`-like system for C using `TRY`, `CATCH`, and `THROW` macros.
> - 📣 **Rich Logging**: Colorful, leveled logging macros (`LOG_INFO`, `LOG_WARN`, `LOG_ERR`, etc.).
> - 🚧 **Assertions**: An `ASSERT` macro that integrates seamlessly with the exception system.
> - 🔩 **Thread-Local**: Designed with thread safety in mind using thread-local storage for error contexts.
> - 🛠️ **Customizable**: Easily configurable via macros.

## 📖 Basic Usage

#### Example Setup
```c
#include <libcerr.h>
// Define your own exception
#define MY_EXCEPTION        10
#define MY_OTHER_EXCEPTION  11

void might_fail(int should_fail)
{
    THROW_IF_MSG(MY_EXCEPTION, should_fail, "Function failed !");
    LOG_OK("Function did not fail.");
}
```
#### Catching Exceptions
```c
TRY {
    might_fail(1); // This will throw
    LOG_INFO("This will not be printed.");
}
CATCH(MY_EXCEPTION, MY_OTHER_EXCEPTION) {
    LOG_ERR("Exception: %s", CERR_WHY());
}
```

#### Logging

> The logging macros provide colorful, formatted output to `stderr` by default. There are four different log levels,
> 0 is nothing, 1 is OK/ERR, 2 is WARN/OK/ERR, 3 is INFO/WARN/OK/ERR and 4 is everything before with DEBUG

```c
#define LOG_LEVEL 2
#include <libcerr.h>

LOG_INFO("Starting application..."); // Log level 2, will not print
int x = 5;
LOG_DEBUG("This is a debug message. x = %d", x); // same here
LOG_WARN("This is a warning message.");
LOG_ERR("This is a fatal error message.);
LOG_OK("Application finished.");
```
#### Assertions
 > Assertions will throw an exception (if in try) or exit the program when the condition is not met.
```c
int x = 5;
TRY {
    ASSERT(x == 10, "Fortunately, it will be catched !")
} CATCH(CERR_E_ASSERT) {
    LOG_ERR("Exception: %s", CERR_WHY());
}
ASSERT(x == 10, "This will not... bye");
```

## 🚀 Getting Started

### Prerequisites

> - C Compiler (like `gcc` or `Clang`)
> - `GNU Make`

### Installation & Usage

```bash
# Clone the repository
git clone https://github.com/MykleR/libcerr.git
cd libcerr

# Build it as a static/shared library
# Link as you wish and include libcerr.h
make
```
> [!IMPORTANT]
> The project is still in its early stages and, for now, everything fits in the header, so no linking is necessary.
> (GCC or Clang remains mandatory for compilation)

## ✍️ Authors

- **Michael ROUVES**
