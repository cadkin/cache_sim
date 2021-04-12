// C
#include <stdio.h>
#include <stdlib.h>

// Local
#include "arg.h"

int main(int argc, char** argv) {
    setenv(ARGP_HELP_FMT_EVAR, ARGP_HELP_FMT_FMT, 0);

    struct arg_struct cache_settings;
    argp_parse(&arg_params, argc, argv, 0, nullptr, &cache_settings);
    if (!cache_settings.cache_settings_valid) return 128;

    printf("skel main\n");
}
