.. _to:

to
################################################################################

NAME
********************************************************************************

to - do the opposite of ``fast_io::concate()``. Convert string to integer
/ floating point / etc.

SYNOPSIS
********************************************************************************

.. code-block:: c++

   #include<fast_io.h>

  constexpr T to<T>(args ...)

DESCRIPTION
********************************************************************************

The function ``to()`` take one or more strings and returns the conversion 
result truncated from the first unexpected character (depending on the 
specified template parameter T).

RETURN VALUE
********************************************************************************

The function returns the conversion result with specified type T. Return
value type can be specified by specifying the first template parameter T
while calling ``to()``.

THREAD SAFETY
********************************************************************************

EXCEPTIONS
********************************************************************************

When call to function ``to()`` fails, ``to()`` throws a exception. The function
is exception safe. The truncation does not cause exception. But when the return
type is too small to hold the conversion result, an 
``fast_io::input_overflow_error`` will be thrown

EXAMPLE
********************************************************************************

convert string to integer:

.. code-block:: c++
   
   #include<fast_io.h>

   int main() {
       std::string answer{"42"};
       auto result{fast_io::to<uint32_t>(answer)};
       // tell to() what to do by specifying T
       println("the answer is ",result);
   }

convert multiple strings to one integer:

.. code-block:: c++

   #include<fast_io.h>
   int main() {
       std::string part1{"123"};
       std::string part2{"456"};
       std::string part3{"789"};

       auto result = fast_io::to<int32_t>(part1,part2,part3);
       println("the result is: ",result);
   }

anything after the first unexpected character will be ignored, even if they are
expected ones:

.. code-block:: c++

   #include<fast_io.h>

   int main() {
       std::string with_point{"123456789"};
       std::string with_char{"123456A789"};
       std::string with_star{"123456*789"};
       // if the first character is illegal, 0 will be returned.
       std::string with_etc{"~!@#$%^&*()_+{}|:\"<>?\`-=[]\;',./"};
   }


SEE ALSO
********************************************************************************
:ref:`print  <print>`

COLOPHON
********************************************************************************

This page is part of  2996 version of the **fast_io** project. Wiki can be
found out in https://github.com/expnkx/fast_io/wiki or 
https://gitee.com/qabeowjbtkwb/fast_io/wikis/Home?sort_id=2249851
