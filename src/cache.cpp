#include "cache.h"

// C
#include <stdlib.h>

// C++
#include <string>

cache::cache(arg_struct* iarg) {
    // Set internal function pointers
    if (iarg->cache_replace_policy == replace_policy::random) {
        m_replace_ptr = &cache::replace_random;
    }
    else {
        m_replace_ptr = &cache::replace_lru;
    }

    // Set sizes and allocate some memory.
    m_block_size = iarg->cache_block_size;
    m_block_count = iarg->cache_block_count;
    m_associativity = iarg->cache_associativity;
    m_set_count = m_block_count / m_associativity;

    m_cache_mem = (cache_block*)malloc(m_block_count * sizeof(cache_block));

    // Set timing.
    m_hit_time = iarg->cache_hit_time;
    m_miss_time = iarg->cache_miss_time;

    // Read file.
    m_file_ptr = iarg->cache_sim_file;
}

cache::~cache() {
    free(m_cache_mem);
}

void cache::simulate() {
    printf("Running sim...\n");

    for(;;) {
        //printf("access no %lu\n", m_access_count);

        uint64_t addr = this->next_addr();
        if (addr == UINT64_MAX) break;

        //printf("addr: %lu\n", addr);

        // Find block set containing this address.
        uint64_t set = (((uint64_t)(addr / m_block_size)) % m_set_count);
        //printf("set no: %lu\n", set);

        // Loop through set to find if any contain this block.
        cache_block* blk = nullptr;
        for (uint64_t i = (set * m_associativity) ; i < (set * m_associativity) + m_associativity; i++) {
            if (m_cache_mem[i].valid) {
                uint64_t max_addr = m_cache_mem[i].addr + (m_block_size - 1);
                if (addr >= m_cache_mem[i].addr && addr <= max_addr) {
                    blk = &m_cache_mem[i];
                }
            }
        }

        // Hit or miss.
        if (blk) {
            //printf("cache hit\n");
            blk->access = m_access_count;

            m_hit_count++;
        }
        else {
            //printf("replacing\n");
            (this->*m_replace_ptr)(addr, set);

            m_miss_count++;
        }


        m_access_count++;
        //printf("----\n");
    }

}

void cache::status() {
    printf("Post run results:");
    printf("\n\nCache contents:\n\n");

    printf("%-10s | %-10s | %-10s | %-10s | %-10s\n", "Block ID", " Set ID", "Valid", "Addr", "Access");

    printf("%s\n", std::string(60, '=').c_str());
    for (uint64_t i = 0; i < m_block_count; i++) {
        printf("%-10lu | %-10lu | %-10.10s | 0x%-8lx | %-10lu\n",
                i, i / m_associativity, (m_cache_mem[i].valid ? "true" : "false"),
                m_cache_mem[i].addr, m_cache_mem[i].access);
    }

    printf("\n\nStatistics:\n\n");

    printf("Total Access Count: %lu\n", m_access_count);
    printf("Cache Hits: %lu\n", m_hit_count);
    printf("Cache Misses: %lu\n", m_miss_count);
    printf("Hit Rate: %.2f %%\n", (m_hit_count * 100.0) / m_access_count );
    printf("Miss Rate: %.2f %%\n", (m_miss_count * 100.0) / m_access_count);
    printf("AMAT: %.2f\n", m_hit_time + (((m_miss_count * 1.0) / m_access_count) * m_miss_time));
}

uint64_t cache::next_addr() {
    // Read the number
    uint64_t addr = 0;
    if (fscanf(this->m_file_ptr, "%li", &addr) != 1) {
        if (feof(this->m_file_ptr)) return UINT64_MAX;
        else pquit(128, "Unable to convert input from file.\n");
    }

    return addr;
}

void cache::replace_random(uint64_t addr, uint64_t set_id) {
    // Pick a random number to replace in the set.
    uint64_t set_min = (set_id * m_associativity);
    uint64_t repl = rand() % (m_associativity);

    m_cache_mem[set_min + repl].addr = addr;
    m_cache_mem[set_min + repl].access = m_access_count;
    m_cache_mem[set_min + repl].valid = true;

    //printf("replaced set %lu, block %lu\n", set_id, repl);
}

void cache::replace_lru(uint64_t addr, uint64_t set_id) {
    uint64_t set_min = (set_id * m_associativity);
    uint64_t repl = 0;

    uint64_t min_access = UINT64_MAX;

    // Loop through blocks to find earliest access time.
    for (uint64_t i = set_min ; i < set_min + m_associativity; i++) {
        if (m_cache_mem[i].valid) {
            if (m_cache_mem[i].access < min_access) {
                repl = (i - set_min);
                min_access = m_cache_mem[i].access;
            }
        }
        else {
            repl = (i - set_min);
            break;
        }
    }

    m_cache_mem[set_min + repl].addr = addr;
    m_cache_mem[set_min + repl].access = m_access_count;
    m_cache_mem[set_min + repl].valid = true;

    //printf("replaced set %lu, block %lu\n", set_id, repl);
}
