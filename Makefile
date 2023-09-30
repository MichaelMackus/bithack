OBJS = dungeon_generator.o dungeon.o player.o mob.o
TEST_OBJS = test/dungeon_generation.o
MAKEFLAGS+= --no-builtin-rules

rl:
	make -f Makefile-gcc OBJS="main.o $(OBJS)" all
rl.prg:
	python img_to_6502.py # generate our tilesets
	make -f Makefile-c64 OBJS="main.o $(OBJS)" all
rl.nes:
	python img_to_6502.py # generate our tilesets
	make -f Makefile-nes OBJS="main.o $(OBJS)" all

clean:
	rm rl *.o rl.prg rl.nes
	rm test/*.o
	make -f Makefile-test clean

test: FORCE
	make -f Makefile-test OBJS="$(OBJS)" remake && test/test

FORCE: ;

$(TESTS): $(TEST_OBJS) $(OBJS)
	ld65 -o $@ -t c64 $(TEST_OBJS) $(OBJS) c64.lib
	./$@
