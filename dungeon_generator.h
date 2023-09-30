#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#define MIN_MOBS 3
#define NUM_MOBS_DUNGEON_CRAVES 20

void generate_dlevel();

// return actual number generated (will be less than num_mobs if generation failed)
unsigned char generate_mobs(unsigned char num_mobs);

// generate some mobs that the player cannot see
// return actual number generated (will be less than num_mobs if generation failed)
unsigned char generate_offscreen_mobs(unsigned char mobs_to_generate);

#endif
