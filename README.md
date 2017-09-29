# DevSlashLirc &mdash; Object oriented access to `/dev/lirc`-hardware.

This library makes objects from `/dev/lirc`-hardware. These objects have properties like `canSend` etc.,
indicating the capabilities of the currently connected hardwarec, and methods like `send` and `receive`
for sending and receiving. See the examples in `src/test/c++`.

As opposed to standard C/Lirc, where a device is opened, and it "turns out" that it is "mode2" or "lirccode",
here the user has to decide if he wants a mode2 or lirccode device, and open it with the appropriate class.
This will fail if the connected hardware does not have the expected properties.  For this,
there are the concrete classes `Mode2LircDevice` (modeling a "mode2" device, i.e. with the possibility of
handling IR signals with "arbitrary" timings), and `LircCodeLircDevice` (modeling a "LircCode" device, i.e.
one that can only use IR signals from its own set, and codes them on an integer, representing the IR
signal.) The latter class is presently not completely implemented.

The classes may be instantiated multiple times,
but of course only one can open a particular device file at a particular time.

The core library is written in C++, using classes and the stdc++ library.
There are no other dependencies. In particular, there is no dependency of the
Lirc sources, includes, or libraries. Only `media/lirc.h`, see `man 4 lirc`, which is considered
belonging to the kernel, is included. The C++ code is documented using doxygen.

The C++-code uses the ("non-portable") `#pragma once` instead of the traditional
[include guards](https://en.wikipedia.org/wiki/Include_guard).

The C++ code is compiled into a shared library. This can be linked into application programs (see the test
programs in `src/test/c++`) or used as a JNI library for accessing it from Java.
Also a static library is built.

As `man 4 lirc` shows, there is a large number of hardly ever implemented properties supported. I have followed
the "agile" commandment _maximize the amount of work not done_, and simply ignored the ones not useful or not
commonly implemented.

As mentioned, there are also Java bindings using JNI. With some extra effort, bindings
from other languages can be added, for example using [SWIG](https://en.wikipedia.org/wiki/SWIG).
Contributions are welcome, (see [issue #1](https://github.com/bengtmartensson/DevSlashLirc/issues/1)).

The code should compile on any platform supporting `/dev/lirc`, possibly after adapting the Makefile
(see [issue #2](https://github.com/bengtmartensson/DevSlashLirc/issues/2)).

A natural extension would be to extend the library to support Lirc plugin drivers. This is discussed in [issue #3](https://github.com/bengtmartensson/DevSlashLirc/issues/3)).
There is also a branch containing some work in this directon, `lircdriver`.

Maven is used to compile the Java parts. A Makefile compiles the C++ code, and also invokes the Maven process.
Redundantly, to build, just issue the command

     make lib

The command

    make doc

creates the Doxygen and Javadoc documentation.

The code is entirely written from scratch (not counting the branch `lircdriver`).

## Usage
There is no "install". Just copy the `so` file and the `jar` file to anywhere you like.
(Or use maven instead of the latter.)

For C++ (or a similar language), just link with the shared library `libdevslashlirc.so`
(alternatively the static library `libdevslashlirc.a`) in the usual manner.

For Java, copy `libdevslashlirc.so` to an "arbitrary" location. In the Java code,
that library must be loaded with `org.harctoolbox.devslashlirc.LircHardware.loadLibrary()`.
There are two possibilites:

1. Either call `loadLibrary()` without arguments, (see
`src/test/java/org/harctoolbox/devslashlirc/Mode2LircDeviceNGTest.java` for an example),
in which case the actual directory has to be given to the JVM using the `-Djava.library.path`
option (like `java -Djava.library.path=/usr/local/lib` ...), or,

2. Call `loadLibrary(File)` with argument, either the path name of the library,
or the path name of the containing directory.
For example `LircHardware.loadLibrary(new File("/home/trump/covfefe/libdevslashlirc.so"))`.
