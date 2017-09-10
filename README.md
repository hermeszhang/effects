GNU Guitar Effects
==================

The GNU Guitar Effects project contains all the audio effects that come packaged
with the GNU Guitar project. It contains all of the original effects from the
repository the project started from (originally called 'GNUitar').

### Building

CMake is the primary build system for the project, so you'll have to make sure
it is installed.

```
sudo apt get update
sudo apt install cmake
```

Once CMake is installed, go ahead and build it in the terminal like this:

```
mkdir build && cd build
cmake ..
cmake --build .
```

Then you can install it like this:

```
sudo cmake --build . --target install
```

### Bugs

Please report bugs on [GitHub](https://github.com/gnu-guitar/effects/issues/new").

### About Free Software Development

You should always keep in mind, that development of free software
doesn't work in the same way as commercial development. Every
successful free software project has an active user base behind it.
This means that your comments, ideas and bug reports are extremely
important. If something doesn't work, or some feature is missing,
please mail me about it. Thank you in advance! You can send GNUitar
related mails to me at [10]fonin at gnuitar dot com.

### Legal

GNU Guitar Effects is a free software and is distributed under the
terms of GNU GPL license. You are free to copy and share the program
with other people, you are not limited with the number of computers
where you can use it. You can redistribute the program and the works
based on it under the terms of GPL license. You have complete sources
and detailed compile instructions to build the program yourself, as well
as binaries. You have full freedom with using and sharing the program,
according to the GNU software concept.

