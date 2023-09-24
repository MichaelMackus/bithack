OBJS = dungeon_generator.o dungeon.o player.o mob.o
ASM_OBJS = init.o render.o input.o
TEST_OBJS = test/dungeon_generation.o
TESTS = test/dungeon_generation
MAKEFLAGS+= --no-builtin-rules

rl:
	make -f Makefile-gcc OBJS="main.o $(OBJS)" all

rl.prg:
	make -f Makefile-c64 OBJS="main.o $(ASM_OBJS) $(OBJS)" all

clean:
	rm rl *.o rl.prg
	rm test/*.o
	rm $(TESTS)

test: $(TESTS)
	make $(TESTS)

$(TESTS): $(TEST_OBJS) $(OBJS)
	ld65 -o $@ -t c64 $(TEST_OBJS) $(OBJS) c64.lib
	./$@
