#ifndef ARG_H
#define ARG_H

// C
#include <argp.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

// The default 80 cols of argp is a bit limiting. If user hasn't setup their own
// ARGP_HELP_FMT variable, change it to be wider. Will not replace if already set.
#define ARGP_HELP_FMT_EVAR "ARGP_HELP_FMT"
#define ARGP_HELP_FMT_FMT  "rmargin=180, opt-doc-col=40, no-dup-args-note"

const char prog_doc[] = "Simple cache simulator that demonstrates cache effectiveness under various configurations.";
static char args_doc[] = "INPUT_FILE";

enum class replace_policy {
    random,
    lru
};

struct arg_struct {
    FILE* cache_sim_file = nullptr;
    uint64_t cache_block_size = 1;
    uint64_t cache_block_count = 1;
    uint64_t cache_associativity = 1;
    uint64_t cache_hit_time = 10;
    uint64_t cache_miss_time = 20;
    replace_policy cache_replace_policy = replace_policy::lru;
    bool cache_settings_valid = 0;
};

const struct argp_option arg_options[] = {
    {"block-size",      's', "SIZE",        0,  "Specifies the size of a cache block in words",                             0},
    {"block-count",     'c', "COUNT",       0,  "Specifies the number of cache blocks",                                     0},
    {"associativity",   'a', "COUNT",       0,  "Set the associativity of the cache. Must be a factor of block-count",      0},
    {"hit-time",        'h', "CYCLES",      0,  "Set the time penalty for a cache hit in cycles",                           0},
    {"miss-time",       'm', "CYCLES",      0,  "Set the time penalty for a cache miss in cycles",                          0},
    {"replace-policy",  'r', "TYPE",        0,  "Set the replacement policy for the cache. TYPE can be 'random' or 'lru'",  0},
    { 0 }
};

error_t parse_opt(int key, char* arg, struct argp_state* state);

const struct argp arg_params = {arg_options, &parse_opt, args_doc, prog_doc};

void pquit(int ret, const char* format, ...);

#endif // ARG_H
