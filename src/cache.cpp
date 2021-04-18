#include "cache.h"

cache::cache(arg_struct* iarg) {
    // Set internal function pointers
    if (iarg->cache_associativity == assoc::dmap) {
        m_assoc_ptr = &cache::assoc_dmap;
    }
    else {
        m_assoc_ptr = &cache::assoc_two_way;
    }

    if (iarg->cache_replace_policy == replace_policy::random) {
        m_replace_ptr = &cache::replace_random;
    }
    else {
        m_replace_ptr = &cache::replace_lru;
    }

    // Set sizes and allocate some memory.
    m_block_size = iarg->cache_block_size;
    m_block_count = iarg->cache_block_count;

    m_cache_mem = (uint64_t*)malloc((m_block_size * m_block_count) * sizeof(uint64_t));

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

}

std::string cache::status() {
    return std::string();
}

uint64_t cache::next_addr() {
    // Skip the '0x'.
    fseek(this->m_file_ptr, 2, SEEK_CUR);

    // Read the number
    char* hex_str = nullptr;
    fscanf(this->m_file_ptr, "%ms", &hex_str);

    if (feof(this->m_file_ptr)) return UINT64_MAX;

    //printf("hex_str: %s\n", hex_str);

    fseek(this->m_file_ptr, 1, SEEK_CUR);

    // Convert
    char* endptr;
    uint64_t addr = strtoul(hex_str, &endptr, 16);

    if ((addr == 0 && errno != 0) || (*endptr != '\0')) {
        pquit(128, "Unable to convert input from file.\n");
    }

    free(hex_str);
    return addr;
}

void cache::assoc_dmap() {

}

void cache::assoc_two_way() {

}

void cache::replace_random() {

}

void cache::replace_lru() {

}
