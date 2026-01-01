// Usage: ./printer <input> <output> [-c<pattern>] [-t<threads>] [-h<lines>] [-t<lines>] [-s] [-r]
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static inline int parse_positive_int(const char* text, int* out, const char** advance) {
    *out = (int)strtol(text, (char**)advance, 10);
    if (errno != 0) {
        fprintf(stderr, "Error: failed to parse '%s' as int\n", text);
        return 1;
    }
    if (*out < 0) {
        fprintf(stderr, "Error: %d is not a positive integer\n", *out);
        return 1;
    }
    return 0;
}

#define REQUIRED_ARGS                                           \
    REQUIRED_STRING_ARG(input_file, "input", "Input file path") \
    REQUIRED_STRING_ARG(output_file, "output", "Output file path")

#define OPTIONAL_ARGS                                                                                              \
    OPTIONAL_STRING_ARG(pattern, c, contains, "pattern", "", "Print only lines containing the pattern")            \
    OPTIONAL_UINT_ARG(threads, t, threads, "threads", 1, "Number of threads to use")                               \
    OPTIONAL_ARG(int, head, h, head, "lines", -1, "Number of lines to print from start", "%d", parse_positive_int) \
    OPTIONAL_ARG(int, tail, t, tail, "lines", -1, "Number of lines to print from end", "%d", parse_positive_int)

#define BOOLEAN_ARGS                                     \
    BOOLEAN_ARG(sort, s, sort, "Sort lines")             \
    BOOLEAN_ARG(reverse, r, reverse, "Print in reverse") \
    BOOLEAN_ARG(help, NONE, help, "Print help")

#include "../includes/argus.h"

int main(int argc, const char* argv[]) {
    args_t args = make_default_args();

    if (parse_args(argc, argv, &args) || args.help) {
        print_help(argv[0]);
        return 1;
    }

    printf("Processing %s -> %s\n", args.input_file, args.output_file);
    printf("Threads: %u\n", args.threads);
    printf("Sorted: %s\n", args.sort ? "true" : "false");
    printf("Reverse: %s\n", args.reverse ? "true" : "false");
    printf("Head: %u\n", args.head);
    printf("Tail: %u\n", args.tail);

    return 0;
}
