Programming language taxonomy
-----------------------------

There are a few concepts around programming languages that help pick up a new language.

Typing
======

Most programming languages come with a more or less sophisticated type systems. The type system defines how the types of variables and functions are used and interpreted.

A common example is the following expression:

.. code-block:: bash

    4 + "2"

What is the result of this expression? The following provides an overview:

+------------+---------------------------------------------------------------+
| Language   | Output                                                        |
+============+===============================================================+
| Javascript | 42                                                            |
+------------+---------------------------------------------------------------+
| Perl       | 6                                                             |
+------------+---------------------------------------------------------------+
| Lua        | 6                                                             |
+------------+---------------------------------------------------------------+
| Python     | TypeError: unsupported operand type(s) for +: 'int' and 'str' |
+------------+---------------------------------------------------------------+
| Haskell    | Error: No instance for (Num [Char]) arising from a use of ‘+’ |
+------------+---------------------------------------------------------------+
| Ruby       | String can't be coerced into Fixnum (TypeError)               |
+------------+---------------------------------------------------------------+
| C          | 134514105 (undefined behaviour)                               |
+------------+---------------------------------------------------------------+
| C++        | garbage (undefined behaviour)                                 |
+------------+---------------------------------------------------------------+

We see four different kinds of answers:

* Javascript implicitly converts the number 4 into a string, then appends 2, resulting in "42"
* Perl and Lua implicitly convert the string "2" into a number, then add 4, resulting in 6
* Python, Haskell and Ruby throw an error
* C and C++ interpret the addition as pointer arithmetic, accessing memory after the statically allocated string "2", resulting in undefined behaviour

Another example is about defining a simple function (pseudocode):

.. code-block:: bash

    function(x):
        return 2 * x

If we were to define this function in C, it could look for example like this:

.. code-block:: bash

    int function(int x)
    {
        return 2 * x;
    }

This function explicitly defines the input and output types as integers; using other types such as a string when calling this function will issue a compiler warning.

If we were to define this function in Javascript, it would look like this;

.. code-block:: javascript

    function f(x) {
        return 2 * x;
    }

I.e. no variable types explicitly defined. If we were to use Python, it would look like this:

.. code-block:: python

    def function(x):
        return 2 * x

While in Haskell the definition would look like this:

.. code-block:: haskell

    function x = 2 * x

What if we called these functions with a number? A string? The following table summarises this:

+------------+--------------------------------+----------------------------------+------------------------------------------+
| Language   | Result when passing 5 (number) | Result when passing "5" (string) | Result when passing "abc" (string)       |
+============+================================+==================================+==========================================+
| C          | 10                             | garbage (undefined behaviour)    | garbage (undefined behaviour)            |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Javascript | 10                             | 10                               | NaN (a string indicating "not a number") |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Python     | 10                             | "55"                             | "abcabc"                                 |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Haskell    | 10                             | Compile error                    | Compile error                            |
+------------+--------------------------------+----------------------------------+------------------------------------------+

We see the result makes sense when inputting a number for all languages. When inputting "5" as string, C interprets this as a pointer and undefined behaviour ensues. Javascript implicitly converts the string to a number, which works when the string is indeed a number but returns "NaN" otherwise. When passing a string to the function in Python, Python interprets the multiplication as a multiplication of the string, hence duplicating the string. Haskell refuses to work with the string input.

The above can be summarised by categorising the language type systems by how dynamic the typing is (static vs. dynamic), and the strength (strong vs. weak).

Static vs. dynamic typing
~~~~~~~~~~~~~~~~~~~~~~~~~

C and Haskell are statically typed languages: the types of all variables must be defined at compilation time. For C, the types must in most cases be explicitly stated, like we saw in the function definition above. Haskell typically infers the types at compile time.

Javascript and Python are dynamically typed languages: the types of variables may change depending on context. For example, the variable "x" in the function above may be a string or an integer, depending on the caller.

Strong vs. weak typing
~~~~~~~~~~~~~~~~~~~~~~

C and Javascript are weakly typed languages: the types of the variables aren't fixed but may be implicitly converted to other types depending on the context. For example, passing a string as an int will cause implicit conversion of the string to an int.

Python and Haskell are strongly typed languages: the types of variables are fixed and will not implicitly change, such that any type conversions must be explicit by the programmer.

Having this overview of the different type systems helps picking up and understanding new languages.

