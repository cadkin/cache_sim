#ifndef CACHE_H
#define CACHE_H

// Local
#include "arg.h"

// C
#include <stdint.h>

struct cache_block {
    uint64_t addr = 0x0;
    uint64_t access = 0x0;
    bool valid = false;
};

class cache {
    public:
        // Constructor
        cache(arg_struct* iarg);
        // Destructor
        ~cache();
        // Perform simulation.
        void simulate();
        // Get status and statistics.
        void status();
    private:
        // Get next address from file. UINT64_MAX represents EOF.
        uint64_t next_addr();

        void replace_random(uint64_t blk, uint64_t set_id);
        void replace_lru(uint64_t blk, uint64_t set_id);

        // File to read from.
        FILE* m_file_ptr;

        // Memory
        uint64_t m_block_size;
        uint64_t m_block_count;
        uint64_t m_associativity;
        uint64_t m_set_count;
        cache_block* m_cache_mem;

        // Time variables
        uint64_t m_hit_time;
        uint64_t m_miss_time;

        // Counts
        uint64_t m_access_count = 0;
        uint64_t m_hit_count = 0;
        uint64_t m_miss_count = 0;

        // Function pointers
        void (cache::*m_replace_ptr)(uint64_t blk, uint64_t set_id);
};

#endif // CACHE_H
