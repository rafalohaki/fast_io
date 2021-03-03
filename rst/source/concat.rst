.. _concat:

concat
################################################################################

NAME
********************************************************************************

concat, concatln - string generators

SYNOPSIS
********************************************************************************

.. code-block:: c++

   #include<fast_io.h>

   constexpr T concat(Args&& ...args)
   constexpr std::string concat(Args&& ...args)

   constexpr T concatln(Args&& ...args)
   constexpr std::string concatln(Args&& ...args)
   




DESCRIPTION
********************************************************************************

The functions in the ``concat`` family generate a ``std::string`` by default 
using the given parameters.

Function ``concatln()`` will add a newline character to the concatenating 
result.

Neither ``concat()`` nor ``concatln()`` will dereference a pointer argument, 
since such action is **un-safe** and may lead to SegmentFault. Function 
``chvw()``  may help if you really want to do so. See EXAMPLES to get more 
information.

When dealing with pointers, ``concat()`` and ``concatln()`` return the value of
the pointers(themselves) as a hexadecimal literal.

The functions ``concat()`` and ``concatln()`` take one or more printable objects
as a hexadecimal literal.

The functions ``concat()`` and ``concatln()`` take one or more printable objects
as parameter and return a ``std::string`` by default, with content of the 
concatenation  of the parameters.

RETURN VALUE
********************************************************************************

These functions return std::string as default. Return type can be specified
by specifying the first template parameter T when calling ``concat()``
and ``concatln()``.

THREAD SAFETY
********************************************************************************

Function ``concat()`` and ``concatln()`` guarantee thread safety during  each
call.

EXCEPTIONS
********************************************************************************

generate a ``std::string`` with content ``It's a sunny day!``:

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       std::string weather{"sunny"};
       println(fast_io::concat("It's ",weather," day!"));
       // type of str is std::string as default
   }

generate a ``std::string`` with multiple different type args:

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       std::size_t answer{42u};
       std::string color{"red"};
       println(fast_io::concat("The answer is ",answer, 
                               "and the color is ",color,"."));
   }

generate a ``std::string`` ending with newline character:

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       print(fast_io::concatln("This example show how to use fast_io::concatln()",
                               "to generate a std::string ending with newline",
                               "character"));
       print(fast_io::concatln("This can also take more than 1 parameters.",
                               "And even floating number: ",3.14159));
   }

.. warning::

   pointers passed to concat() and concatln() are treated only as a value that
   describes the address and is converted to a hexadecimal literal:

.. code-block:: c++

   #include<fast_io.h>
   int main() {
       char const* ptr{"hello,pointer?"};
       // this will not dereference ptr to string literal "hello,pointer?"
       // but the literal value of ptr itself.
       println(fast_io::concat("The address that ptr describes is "),ptr);
       // this prints the address that ptr descibes is 0X...
       // where the dots(...) represents the value of  ptr
   }

perform with single character and ``char const*``:

.. code-block:: c++

   #include<fast_io.h>
   int main() {
       char const* str{"correct usage: "};
       println(fast_io::concat(fast_io::manipulators::chvw(str)),
               fast_io::manipulators::chvw('Y'));
   }

generate ``std::wstring``:

.. warning::

   print() family print out to c_stdout() by default, which is only support 
   char, So we must tell print() it should print out to wc_stdout()

.. code-block:: c++

   #include"../include/fast_io.h"
   int main() {
       std::wstring wstr1{L"hello, "};
       auto wstr2{fast_io::concat<std::wstring>(wstr1,L"world!")};
       println(fast_io::wc_stdout(),wstr2);
   }

perform the same action of ``concat()`` with functions: ``print()`` 

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       std::string str;
       fast_io::ostring_ref ref{str};
       print(ref,"hello world",14," fa ",16846);

       println("result of do concat with print(): ",str);

       println("result of do concat with concat(): ",
               fast_io::concat("hello world",14," fa ",16846));
   }

SEE ALSO
********************************************************************************

:ref:`print <print>`

COLOPHON
********************************************************************************

This page is part of  2996 version of the **fast_io** project. Wiki can be
found out in https://github.com/expnkx/fast_io/wiki or 
https://gitee.com/qabeowjbtkwb/fast_io/wikis/Home?sort_id=2249851



