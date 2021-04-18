#ifndef CACHE_H
#define CACHE_H

// Local
#include "arg.h"

// C
#include <stdint.h>

// C++
#include <string>

class cache {
    public:
        // Constructor
        cache(arg_struct* iarg);
        // Destructor
        ~cache();
        // Perform simulation.
        void simulate();
        // Get status and statistics.
        std::string status();
    private:
        // Get next address from file. UINT64_MAX represents EOF.
        uint64_t next_addr();

        void assoc_dmap();
        void assoc_two_way();
        void replace_random();
        void replace_lru();

        arg_struct* arg;

        // File to read from.
        FILE* m_file_ptr;

        // Memory
        uint64_t m_block_size;
        uint64_t m_block_count;
        uint64_t* m_cache_mem;

        // Time variables
        uint64_t m_hit_time;
        uint64_t m_miss_time;

        // Function pointers
        void (cache::*m_assoc_ptr)();
        void (cache::*m_replace_ptr)();
};

#endif // CACHE_H
