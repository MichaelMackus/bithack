#ifndef DUNGEON_GENERATOR_H
#define DUNGEON_GENERATOR_H

#define MIN_MOBS 3

void generate_dlevel();

// return actual number generated (will be less than num_mobs if generation failed)
unsigned char generate_mobs(unsigned char num_mobs);

#endif
