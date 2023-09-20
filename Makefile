objs = main.o random.o init.o math.o generate.o render.o

rl.prg: $(objs)
	ld65 -o rl.prg -t c64 $(objs) c64.lib

%.o: %.s
	ca65 -t c64 $< -o $@

clean:
	rm *.prg *.o
