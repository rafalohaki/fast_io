.. _print:

print
################################################################################

NAME
********************************************************************************

print, println, perr, perrln,

debug_print, debug_println, debug_perr, debug_perrln,

panic, panicln - output conversion

SYNOPSIS
********************************************************************************
.. code-block:: c++

    #include <fast_io.h>
   
    constexpr void print(args...);
    constexpr void print([output stream device],args...);
    constexpr void println(args...);
    constexpr void println([output stream device],args...);
    constexpr void perr(args...);
    constexpr void perrln(args...);
    constexpr void panic(args...);
    constexpr void panicln(args...);

    constexpr void debug_print(args...);
    constexpr void debug_print([output stream device],args...);
    constexpr void debug_println(args...);
    constexpr void debug_println([output stream device],args...);
    constexpr void debug_perr(args...);
    constexpr void debug_perrln(args...);



DESCRIPTION
********************************************************************************

The functions in the ``print()`` family produce output.

Functions which end up **ln** will print out a newline character after print.

The functions ``print()`` and ``println()`` write output to C's **stdout**, the 
standard output stream by default; if the first argument is an output device,
it will output to that output device.

The functions ``perr()`` and ``perrln()`` write output to **native** stderr, the 
native standard error stream on your platform, which means it is not 
synchronized with C's stderr. They are **not buffered stream** either. When
users call ``perr()`` or ``perrln()`` to print data to **native** stderr, they 
guarantee **atomicity** of syscalls. Ignore CRLF on **all** platforms including 
Windows. Equivalent to ``print(fast_io::err(),...)`` or 
``println(fast_io::err(),...)``

They functions ``panic()`` and ``panicln()`` are similar with ``perr()`` and
``perrln()``, but the program terminates with a builtin trap.

``debug_print()`` family is the same as normal ``print()`` family 
functions, but they are **only available under debugging mode**. This prevents
print leaks for release mode.

Unlike ``print()`` and ``println()``, ``debug_print()`` and ``debug_println``
by default print out to **native** stdout, instead of C's stdout, when
the first argument is not an output device, So even when the program crashes,
the data will still get printed out to the standard output.

RETURN VALUE
********************************************************************************

These functions haven't return value.

PROCESS SAFETY
********************************************************************************

``print()`` family functions guarantee process safety when output device is 
**no buffered output stream**.

THREAD SAFETY
********************************************************************************

Thread safety depends on whether the output device satisfies the mutex
stream concept. For example, ``fast_io::c_stdout()`` is thread-safe. print to
c_stdout is thread-safe if underlining libc provides thread-safety for
``FILE*``. If the output device is process-safety,it is thread-safe too.

EXCEPTHIONS
********************************************************************************

When ``print()``  family functions fail, they throw exceptions. These 
functions are exception safety.

EXAMPLE
********************************************************************************

print out "hello world!\n" to C's stdout

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       print("hello world!\n");
   }


print out "hello world!\n" to an output device. Here we use ``fast_io::out()``
as an example.
.. tip::

   ``fast_io::out()`` is no buffered stream``

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       print(fast_io::out(),"hello world!\n");
   }

print out multiple args.

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       std::string str{"3.14"};
       println("hello",3," ",str,3.14159);
   }

The best practice (with fast_io) to print ``char const*`` ("hello world!")
to C's stdout:

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       char const* str {"hello world!"};
       println("str = ",str);
       // this will print the pointer's value instead of the string.
       // See th correct solutions below:
       println("str = ",std::string_view{str});
       println("str = ",fast_io::manipulators::chvw(str));
   }

print char to C's stdout:

.. code-block:: c++

   #include<fast_io.h>
   int main() {
       char ch{'Y'};
       println(ch);
       // this will print the ASCII code (89) of 'Y' instead of  `Y`
       println(fast_io::manipulators::chvw(ch));
   }


SEE ALSO
********************************************************************************

COLOPHON
********************************************************************************

This page is part of  2996 version of the **fast_io** project. Wiki can be
found out in https://github.com/expnkx/fast_io/wiki or 
https://gitee.com/qabeowjbtkwb/fast_io/wikis/Home?sort_id=2249851
