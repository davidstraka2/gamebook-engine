# gamebook-engine

## About

This project was among several requirements for passing my C++ class (summer
semester of 2019). My objective was to create a simple gamebook (text RPG)
engine.

### Details

My gamebook engine runs in the command line. It loads a gamebook from a file.
The book contains locations, enemies, and items. Every location can have a
description/lore and offers a selection of actions for the user to choose from.
These actions can be e.g. moving to another location, or some form of
interaction with the surroundings, such as fighting an enemy.

Each enemy and item have its properties. For enemies it's their strength (which
determines the power of creature's attack), dexterity (which determines the
power of creature's defense), and vitality (which determines creature's HP).
Items increase/decrease any of these properties of a player's hero, when they
wear the item.

After loading a new world from a book, the player creates their hero. They
distribute given skill points amongst hero attributes. The game then begins. It
ends when the hero dies or reaches the end (or one of the ends). Progress can be
saved into a save file.

It was required for the source code to be rougly within the range of 1k to 3k
lines. It must be properly formatted and commented. It must be written in
C/C++ (C++ version <= 14), and use g++ and make for compilation. It must show
off OOP principles and some form of polymorphism (excluding function overloading
and a couple other types). Compilation must be error and warning free. There
must be no memory leaks when checking program with valgrind (except for leaks
causes by libraries). Library use was restricted to an approved list; and so was
the use of commands inside Makefile. Documentation must also be included (or
auto generated using Doxygen).

## Usage

First run `make compile` to compile source, `make doc` to generate
documentation, or `make all`/`make` to do both at once.

Place your books into the examples directory (you can already find some premade
ones). Run `make run` to start the program. Choose "New game", enter filename
of the book you want to play and follow game instructions.

If you've already created a game, you can load an existing game by choosing
"Load game" in the main menu.

Run `make clean` to delete all generated files.

Run `make compile` to recompile, `make doc` to remake documentation, or
`make all`/`make` to do both.

## Prerequisites

This repository is set up to work on Windows 10 with WSL. Program was
successfully compiled and run on openSUSE 15 (gcc 7.4), Debian 9.7 (gcc 6.3),
and Windows 10 WSL openSUSE 15 (gcc 7.4); all x64.

**All files must use LF for EOL.**

After compilation, there should be no dependencies.

### Used Linux commands

Here are all the commands used by Makefile: `doxygen`, `g++`, `mkdir`, `rm`. In
addition these you need `make` itself of course.

### Libraries

In addition to the STL, `ncurses` is used.

**Depending on your system, either `#include <ncurses/menu.h>` or
`#include <menu.h>` need to be used in `src/io.cpp` (the former is used by
default on line 11).**

### Used Windows commands

This repository comes with prepared VSCode tasks to be run on Windows. The
following command is used: `wsl`. VSCode extension Code Runner is set up to use
the following: `cd`, and `wsl`.

Inside WSL, you need the Linux commands listed in the previous section.

Other than that, there should be nothing Windows specific in the repo. If you
delete the .vscode folder, or don't use VSCode at all, everything should work
on standard Linux distros.

## License

[MIT](LICENSE)
