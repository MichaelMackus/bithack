OBJS = dungeon_generator.o dungeon.o random.o init.o math.o render.o player.o input.o mob.o
TEST_OBJS = test/dungeon_generation.o
TESTS = test/dungeon_generation
CCFLAGS=-Osir -Cl
MAKEFLAGS+= --no-builtin-rules

rl.prg: main.o $(OBJS)
	ld65 -o rl.prg -t c64 main.o $(OBJS) c64.lib

%.s: %.c
	cc65 $(CCFLAGS) -o $@ -t c64 $<

%.o: %.s
	ca65 -t c64 $< -o $@

clean:
	rm *.prg *.o main.s mob.s dungeon.s player.s dungeon_generator.s

