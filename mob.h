#ifndef MOB_H
#define MOB_H

#define MAX_MOBS 100

#define MOB_GOBLIN 1
#define MOB_KOBOLD 2
#define MOB_ORC 3
#define MOB_DRAGON 4

struct Mob {
    unsigned char type;
    unsigned char hp, max_hp;
    unsigned char x, y;
};
typedef struct Mob Mob;

extern Mob mobs[MAX_MOBS];
extern unsigned short num_mobs;

// initialize mobs at start of game
void init_mobs();

// clear mobs in dungeon level
void clear_mobs();

// cleanup dead mobs, decrementing num_mobs & adjusting the memory
void cleanup_mobs();

// generate new mob & insert it into mob list
void generate_mob(unsigned char mob_type, unsigned char x, unsigned char y);

// get tile for mob
unsigned char mob_tile(unsigned char mob_type);

// get mob at xy
Mob *mob_at(unsigned char x, unsigned char y);

// get mob AC
signed char mob_ac(unsigned char mob_type);

// damage a mob
void hurt(Mob *target, unsigned char damage);

// move mob closer to point
void move_or_attack_towards(Mob *mob, unsigned char target_x, unsigned char target_y);

// returns amount of mobs that are alive
unsigned char alive_mobs();

#endif
