#include "arg.h"

// C++
#include <regex>
#include <string>

error_t parse_opt(int key, char* arg, argp_state* state) {
    struct arg_struct* as = (struct arg_struct*)state->input;

    switch (key) {
        case 's': {
            char* endptr;
            uint64_t bsize = strtoul(arg, &endptr, 10);

            if ((bsize == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read integer for block size.\n");
            }

            as->cache_block_size = bsize;

            break;
        }
        case 'c': {
            char* endptr;
            uint64_t bcount = strtoul(arg, &endptr, 10);

            if ((bcount == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read integer for block count.\n");
            }

            as->cache_block_count = bcount;

            break;
        }
        case 'a': {
            char* endptr;
            uint64_t assoc = strtoul(arg, &endptr, 10);

            if ((assoc == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read integer for associativity.\n");
            }

            as->cache_associativity = assoc;

            break;
        }
        case 'h': {
            char* endptr;
            uint64_t hit_time = strtoul(arg, &endptr, 10);

            if ((hit_time == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read integer for hit time.\n");
            }

            as->cache_hit_time = hit_time;

            break;
        }
        case 'm': {
            char* endptr;
            uint64_t miss_time = strtoul(arg, &endptr, 10);

            if ((miss_time == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read integer for miss time.\n");
            }

            as->cache_miss_time = miss_time;

            break;
        }
        case 'r': {
            std::string type = std::string(arg);

            if (type == "random") {
                as->cache_replace_policy = replace_policy::random;
            }
            else if (type == "lru") {
                as->cache_replace_policy = replace_policy::lru;
            }
            else {
                pquit(128, "Unknown replacement policy '%s' specified. Use either 'random' or 'lru'.\n", arg);
            }

            break;
        }

        case ARGP_KEY_ARG: {
            // Attempt to open file specified.
            FILE* fptr = fopen(arg, "r");

            if (fptr == NULL) {
                pquit(256, "Cannot open file '%s': %s", arg, strerror(errno));
            }

            as->cache_sim_file = fptr;

            break;
        }
        case ARGP_KEY_END: {
            if (as->cache_block_count % as->cache_associativity) {
                pquit(128, "Associativity must be a factor of block count. Please check this.\n");
            }

            if (state->arg_num < 1) argp_usage(state);
            else as->cache_settings_valid = true;
            break;
        }

        default: {
            return ARGP_ERR_UNKNOWN;
        }
    }

    return 0;
}

void pquit(int ret, const char* format, ...) {
    va_list vp;
    va_start(vp, format);

    vfprintf(stderr, format, vp);
    va_end(vp);

    exit(ret);
}

