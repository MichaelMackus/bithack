# 8bithack

![test status](https://github.com/MichaelMackus/bithack/actions/workflows/test.yml/badge.svg)

![bithack](/res/screenshot.png?raw=true)

Simple roguelike, with the goal of targetting multiple (mostly 8-bit)
systems, such as: commodore 64, NES, and PC.

# Building

You'll need the curses library to build this for PC. Then, you should be
able to build with:

`make rl`

The 8-bit versions use the "cc65" compiler and assembler.

To build the commodore 64 version, build with:

`make rl.prg`

To build the NES version, build with:

`make rl.nes`

Currently, if you want to build another target you'll have to do a `make
clean` in between compiles (due to the different object file binaries).

The PC version uses the gcc compiler. You should be able to compile on
windows with something like pdcurses and MinGW (or perhaps something
like Windows subsystem for linux) but this has not been tested.

# License

Everything is FOSS. All game code is licensed under MIT, graphics &
tileset(s) are licensed under Creative Commons (BY).

TLDR: you are free to use this work as you wish, but must attribute me
via the license file(s) if you use any of the code or graphics.
