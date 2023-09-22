# 8bithack

Simple roguelike, with the goal of targetting multiple (mostly 8-bit)
systems, such as: commodore 64, NES, and PC.

# Building

You'll need the curses library to build this for PC. Then, you should be
able to build with:

`make rl`

To build the commodore 64 version, build with:

`make rl.prg`

Currently, if you want to build another target you'll have to do a `make
clean` in between compiles (due to the different object file binaries).

The PC version uses the gcc compiler. You should be able to compile on
windows with something like pdcurses and MinGW (or perhaps something
like Windows subsystem for linux) but this has not been tested.
