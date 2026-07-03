#include <zephyr/kernel.h>
#include "temp_voter.h"

static struct temp_reading sources[NUM_TEMP_SOURCES];
static const float VOTE_TOLERANCE = 0.5;

void temp_voter_init(void)
{
    for (int i = 0; i < NUM_TEMP_SOURCES; i++) {
        sources[i].temperature = 0.0f;
        sources[i].valid = false;
        sources[i].timestamp = 0;
    }
}

void temp_voter_feed(int source, float temp)
{
    if (source >= 0 && source < NUM_TEMP_SOURCES) {
        sources[source].temperature = temp;
        sources[source].valid = true;
        sources[source].timestamp = k_uptime_get();
    }
}

struct temp_vote_result temp_voter_get_vote(void)
{
    struct temp_vote_result result = {0};
    int valid_count = 0;
    int best_count = 0;
    float best_temp = 0;

    for (int i = 0; i < NUM_TEMP_SOURCES; i++) {
        if (sources[i].valid) {
            valid_count++;
            int matches = 0;
            for (int j = 0; j < NUM_TEMP_SOURCES; j++) {
                if (i != j && sources[j].valid) {
                    float diff = sources[i].temperature - sources[j].temperature;
                    if (diff < 0) diff = -diff;
                    if (diff <= VOTE_TOLERANCE) {
                        matches++;
                    }
                }
            }
            if (matches > best_count) {
                best_count = matches;
                best_temp = sources[i].temperature;
            }
        }
    }

    result.sources_agreeing = best_count + 1;
    result.temperature = best_temp;
    result.safe = (valid_count >= 2 && result.sources_agreeing >= 2);

    return result;
}

bool temp_voter_has_quorum(void)
{
    return temp_voter_get_vote().safe;
}
