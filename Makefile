objs = dungeon_generator.o dungeon.o main.o random.o init.o math.o render.o player.o
CCFLAGS=-Osir -Cl

rl.prg: $(objs)
	ld65 -o rl.prg -t c64 $(objs) c64.lib

main.s: main.c
	cc65 $(CCFLAGS) -o main.s -t c64 main.c
dungeon_generator.s: dungeon_generator.c
	cc65 $(CCFLAGS) -o dungeon_generator.s -t c64 dungeon_generator.c
dungeon.s: dungeon.c
	cc65 $(CCFLAGS) -o dungeon.s -t c64 dungeon.c
player.s: player.c
	cc65 $(CCFLAGS) -o player.s -t c64 player.c

%.o: %.s
	ca65 -t c64 $< -o $@

clean:
	rm *.prg *.o
