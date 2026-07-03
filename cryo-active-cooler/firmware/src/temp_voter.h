#ifndef TEMP_VOTER_H
#define TEMP_VOTER_H

#include <stdbool.h>

#define TEMP_SOURCE_PATCH_BLE    0
#define TEMP_SOURCE_PHONE_RELAY  1
#define TEMP_SOURCE_LOCAL        2
#define NUM_TEMP_SOURCES         3

struct temp_reading {
    float temperature;
    bool valid;
    uint32_t timestamp;
};

struct temp_vote_result {
    float temperature;
    bool safe;
    int sources_agreeing;
};

void temp_voter_init(void);
void temp_voter_feed(int source, float temp);
struct temp_vote_result temp_voter_get_vote(void);
bool temp_voter_has_quorum(void);

#endif
