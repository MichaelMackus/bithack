enum MobType {
    MOB_GOBLIN = 'g',
    MOB_KOBOLD = 'k',
    MOB_ORC = 'o',
    MOB_OGRE = 'O',
    MOB_DRAGON = 'D',
};
typedef enum MobType MobType;

struct Mob {
    MobType type;
    unsigned char hp, maxHP;
};
typedef struct Mob Mob;
