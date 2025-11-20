#  Terminus Outcome Library

Modern and predictable C++ error handling primitives built on top of
`std::error_code` semantics.  The library provides light-weight `Result`
and `Optional` types, ergonomic macros, and helpers for registering custom
error domains so failures can be modeled and propagated explicitly.

## Highlights

- Header-only delivery with Conan packaging.
- Zero-overhead helpers (`TERMINUS_OUTCOME_TRY`, `TERMINUS_OUTCOME_MARK_ENUM`, etc.).
- Seamless interop with standard error utilities and existing Terminus services.
- Tested with CMake/CTest and ready for CI consumption.

## Repository Structure

| Path                | Purpose                                  |
|---------------------|------------------------------------------|
| `include/terminus`  | Public headers for the Outcome library.  |
| `docs/`             | Additional developer documentation.      |
| `test/`             | Unit tests and Conan package example.    |

Refer to `docs/developers.md` for deeper background on the integrated build
environment.

## Requirements

- Conan 2.x
- CMake 3.24+
- A C++20-capable compiler (Clang 15+, GCC 12+, MSVC 2022)
- Terminus CMake scripts (`terminus-cmake` repository) added to `PATH`

## Build Instructions

All commands are intended to run from the repository root unless noted.

```bash
# Configure & build (clean configuration)
conan-build.bash -c

# Re-run an existing build (e.g., release profile)
conan-build.bash -r
```

### Build Options

The Conan recipe exposes toggles you can pass via `-o`:

| Option             | Default | Description                              |
|--------------------|---------|------------------------------------------|
| `with_tests`       | `True`  | Enable unit tests.                       |
| `with_docs`        | `True`  | Generate documentation assets.           |
| `with_coverage`    | `False` | Collect coverage data during the build.  |

Example:

```bash
conan-build.bash -c -o terminus_outcome/*:with_docs=False
```

## Running Tests

```bash
conan-build.bash -r
cd build
ctest --output-on-failure
```

## Conan Package Verification

```bash
conan test test/package/conanfile.py terminus_outcome/<version>@
```

Replace `<version>` with the package version you are validating.

## Basic Usage

```cpp
#include <terminus/outcome.hpp>

enum class IoErrorCode { Success = 0, NotFound = 1 };
TERMINUS_OUTCOME_MARK_ENUM( IoErrorCode )

class IoCategory : public tmns::outcome::Error_Category<IoCategory>
{
    protected:
        const char* name_() const noexcept override { return "Io"; }
        std::string message_( int code ) const override
        {
            switch( static_cast<IoErrorCode>( code ) )
            {
                case IoErrorCode::Success:   return "success";
                case IoErrorCode::NotFound:  return "not found";
                default:                     return "unknown";
            }
        }
};
TERMINUS_OUTCOME_ASSOCIATE_ERROR_CATEGORY( IoCategory, IoErrorCode )

tmns::outcome::Result<std::string, IoErrorCode> read_config()
{
    TERMINUS_OUTCOME_TRY( data, load_file() );
    return tmns::outcome::ok<std::string>( std::move( data ) );
}

int main()
{
    auto result = read_config();
    if( result.has_error() )
    {
        std::cerr << result.assume_error().message() << std::endl;
        return 1;
    }
    std::cout << result.assume_value() << std::endl;
}
```

## Additional Resources

- `docs/developers.md` – deeper dive into developer workflow.
- `test/package/example.cpp` – end-to-end sample showing macro usage.

