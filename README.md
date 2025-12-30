# Arguś

A simple, single-header argument parser for C that eliminates boilerplate and code duplication.

## Features

- **Single header file:** Just include `argus.h` and you're good to go
- **Zero duplication:** Define your arguments once to get struct fields, parsing, and help
  generation automatically
- **Type-safe:** Supports all common C types with built-in parsing
- **Flexible:** Handle required arguments, optional arguments with defaults, and boolean flags
- **Automatic help generation:** Professional-looking usage and help text generated from your
  definitions
- **Error-safe**: Supports error handling and printing on error messages. This follows the
  philosophy of not validating but parsing.
- **Multiple flag support**: Both short and long flags are supported. You can choose to have both or
  only one.

## Quick Start

Check out ./examples to see some use cases!

### 1. Define your arguments

```c
// Define required positional arguments
#define REQUIRED_ARGS \
    REQUIRED_STRING_ARG(input_file, "input", "Input file path") \
    REQUIRED_STRING_ARG(output_file, "output", "Output file path")

// Define optional arguments with defaults
#define OPTIONAL_ARGS \
    OPTIONAL_UINT_ARG(threads, t, threads, "threads", "Number of threads to use")

// Define boolean flags
#define BOOLEAN_ARGS \   // <- long flag is ommited
    BOOLEAN_ARG(help, h, NONE, "Show help")
```

### 2. Include Argus

```c
#include "argus.h"
```

### 3. Use it in your main function

```c
int main(int argc, char* argv[]) {
    // Initialize with the default values specified above
    args_t args = make_default_args();

    // Parse arguments
    if (!parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return 1;
    }

    // Use your arguments
    printf("Processing %s -> %s\n", args.input_file, args.output_file);
    printf("Using %u threads\n", args.threads);

    return 0;
}
```

### 4. Compile and Run

```bash
gcc file_processor.c -o file_processor
./file_processor input.txt output.txt -t 4
```

## Argument Types

### Required Arguments

Required arguments are positional and must be provided in order:

```c
#define REQUIRED_ARGS \
    REQUIRED_STRING_ARG(name, "label", "description") \
    REQUIRED_SIZE_ARG(count, "count", "Number of items") \
    REQUIRED_DOUBLE_ARG(threshold, "threshold", "Processing threshold")
```

**Supported types:**

- `REQUIRED_STRING_ARG` - `char*`
- `REQUIRED_CHAR_ARG` - `char`
- `REQUIRED_INT_ARG` - `int`
- `REQUIRED_UINT_ARG` - `unsigned int`
- `REQUIRED_LONG_ARG` - `long`
- `REQUIRED_ULONG_ARG` - `unsigned long`
- `REQUIRED_LONG_LONG_ARG` - `long long`
- `REQUIRED_ULONG_LONG_ARG` - `unsigned long long`
- `REQUIRED_SIZE_ARG` - `size_t`
- `REQUIRED_FLOAT_ARG` - `float`
- `REQUIRED_DOUBLE_ARG` - `double`
- `REQUIRED_LONG_DOUBLE_ARG` - `long double`

### Optional Arguments

Optional arguments have flags and default values:

```c
#define OPTIONAL_ARGS \
    OPTIONAL_INT_ARG(verbose, v, verbose, "level", 0, "Verbosity level") \
    OPTIONAL_STRING_ARG(config, c, config, "file", "config.ini", "Configuration file") \
    OPTIONAL_DOUBLE_ARG(factor, f, factor, "factor", 1.5, "Scaling factor", 2)
```

**Note:** `OPTIONAL_FLOAT_ARG`, `OPTIONAL_DOUBLE_ARG` and `OPTIONAL_LONG_DOUBLE_ARG` take an
additional precision parameter for displaying defaults in help text.

**Supported types:** Same as required arguments, but with `OPTIONAL_` prefix.

### Boolean Arguments

Boolean flags toggle between true and false if present or missing, respectively:

```c
#define BOOLEAN_ARGS \
    BOOLEAN_ARG(verbose, v, verbose "Enable verbose output") \
    BOOLEAN_ARG(quiet, q, NONE, "Suppress all output") \
    BOOLEAN_ARG(force, NONE, force, "Force overwrite existing files")
```

## Advanced Usage

### Custom Parsing

For basic types, parsing is handled automatically. For required arguments that need custom parsing,
you can specify a parser function:

```c
#define REQUIRED_ARG(type, name, label, description, parser)
```

The `parser` word should be a function name of the following signature:

```cpp
/**
 * @param[in] text The string to parse
 * @param[out] out The parsed value
 * @param[out] advance A pointer to the next value to be parsed (used in short combined flags).
 * Setting it to NULL or '\0' means that the whole string was succesfuly read. This is ignored for
 * long flags.
 * @retval The return value is an exit code. Any non-zero exit code will be treated as an error
 * This function should also print any errors that may occur in format "Error: %s".
 */
int parser(const char* text, type* out, char** advance) {
```

### Error Handling

`parse_args()` returns 1 if parsing fails (e.g., not enough required arguments). Always check the
return value:

```c
if (parse_args(argc, argv, &args)) {
    fprintf(stderr, "Error parsing arguments\n");
    print_help(argv[0]);
    return 1;
}
```

## Installation

1. Download `argus.h`
2. Place it in your project directory or include path
3. Include it in your source files: `#include "argus.h"`

No compilation or linking required &mdash; it's header-only!

### System-wide installation

Place the `argus.h` header file in one of these location on the following OS:

- Linux: `/usr/local/include/argus`
- macOS: `/usr/local/include/argus`
- Windows has no global include directory

# Credits

This library is heavily based on [easy-args](https://github.com/gouwsxander/easy-args) by Xander
Gouws, published under the MIT license. The copy of this license can be found in the file
`THIRD_PARTY_LICENSES`. Most important changes are:

- Different macro argument order
- Optional long and short options
- Better error handling
- Combined short flag parsing
- Inverted `parse_args` return value
