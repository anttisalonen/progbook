Typing
------

  If someone claims to have the perfect programming language, he is either a fool or a salesman or both.

  -- Bjarne Stroustrup

Most programming languages come with a more or less sophisticated type systems. The type system defines how the types of variables and functions are used and interpreted.

A common example is the following expression:

.. code-block:: bash

    4 + "2"

What is the result of this expression? The following provides an overview:

+------------+---------------------------------------------------------------+
| Language   | Output                                                        |
+============+===============================================================+
| JavaScript | 42                                                            |
+------------+---------------------------------------------------------------+
| Java       | 42                                                            |
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

* JavaScript and Java implicitly convert the number 4 into a string, then appends 2, resulting in "42"
* Perl and Lua implicitly convert the string "2" into a number, then add 4, resulting in 6
* Python, Haskell and Ruby throw an error
* C and C++ interpret the addition as pointer arithmetic, accessing memory after the statically allocated string "2", resulting in undefined behaviour

Another example is about defining a simple function (pseudocode):

.. code-block:: bash

    f(x):
        return 2 * x

If we were to define this function in C, it could look for example like this:

.. code-block:: bash

    int f(int x)
    {
        return 2 * x;
    }

This function explicitly defines the input and output types as integers; using other types such as a string when calling this function will issue a compiler warning.

If we were to define this function in JavaScript, it would look like this:

.. code-block:: javascript

    function f(x) {
        return 2 * x;
    }

I.e. no variable types explicitly defined. If we were to use Python, it would look like this:

.. code-block:: python

    def f(x):
        return 2 * x

While in Haskell the definition would look like this:

.. code-block:: haskell

    f x = 2 * x

Or in Java:

.. code-block:: java

    public static int f(int x) {
        return 2 * x;
    }

What if we called these functions with a number? A string? The following table summarises the results when passing different values to the function:

+------------+------------+---------------+------------------------------------------+
| Language   | 5 (number) | "5" (string)  | "abc" (string)                           |
+============+============+===============+==========================================+
| C          | 10         | garbage       | garbage                                  |
+------------+------------+---------------+------------------------------------------+
| JavaScript | 10         | 10            | NaN (a string indicating "not a number") |
+------------+------------+---------------+------------------------------------------+
| Python     | 10         | "55"          | "abcabc"                                 |
+------------+------------+---------------+------------------------------------------+
| Java       | 10         | Compile error | Compile error                            |
+------------+------------+---------------+------------------------------------------+
| Haskell    | 10         | Compile error | Compile error                            |
+------------+------------+---------------+------------------------------------------+

We see the result makes sense when inputting a number for all languages. When inputting "5" as string, C interprets this as a pointer and undefined behaviour ensues. JavaScript implicitly converts the string to a number, which works when the string is indeed a number but returns "NaN" otherwise. When passing a string to the function in Python, Python interprets the multiplication as a multiplication of the string, hence duplicating the string. Haskell and Java refuse to work with the string input.

The above can be summarised by categorising the language type systems by how dynamic the typing is (static vs. dynamic), and the strength (strong vs. weak).

Static vs. dynamic typing
~~~~~~~~~~~~~~~~~~~~~~~~~

C, Haskell and Java are statically typed languages: the types of all variables must be defined at compilation time. For C and Java, the types must in most cases be explicitly stated, like we saw in the function definitions above. Haskell typically infers the types at compile time.

JavaScript and Python are dynamically typed languages: the types of variables may change depending on context. For example, the variable "x" in the function above may be a string or an integer, depending on the caller.

Strong vs. weak typing
~~~~~~~~~~~~~~~~~~~~~~

C and JavaScript are weakly typed languages: the types of the variables aren't fixed but may be implicitly converted to other types depending on the context. For example, passing a string as an int will cause implicit conversion of the string to an int.

Python and Haskell are strongly typed languages: the types of variables are fixed and will not implicitly change, such that any type conversions must be explicit by the programmer.

It should be pointed out that while, based on the above, Java and Haskell are both strongly typed, some languages are more strongly typed than others. In our 4 + "2" example we saw that Haskell returned an error while Java returned "42". In this case, Java *implicitly converted* the type of the expression 4 to a string in order to avoid a compile error and return a string instead. Hence it can be argued that while Java is also a strongly typed language, Haskell is even more strongly typed.

Having this overview of the different type systems helps picking up and understanding new languages.

Duck typing
~~~~~~~~~~~

The combination of strong, dynamic typing is also often called "duck typing". This is based on the notion that "if it walks like a duck and it quacks like a duck, then it is a duck". Let's take a look at our Python function definition again:

.. code-block:: python

    def f(x):
        return 2 * x

The operation to multiply with an int (2 \*) is defined for both integers and strings. For integers, the traditional multiplication is performed, while for strings the string is duplicated. From the point of view of the function, it makes no difference which type is passed to the function, as long as it can be multiplied by 2, hence duck typing. As we shall see, this becomes more interesting when writing your own data types.

*Exercise*: Write Python code that, when executed, prints the asterisk 50 times, i.e. "**************************************************". (One line of code.)

Interpreted and compiled languages
==================================

Typically (but not always), statically typed languages are compiled languages and dynamically typed languages are interpreted. There are pros and cons to both. The following attempts to summarise this somewhat.

Catching errors
~~~~~~~~~~~~~~~

Compilation can catch errors that wouldn't be caught in interpreted programs until the code with the error was executed. For example, accidentally attempting to assign a string value to an int will be a compiler error. With an interpreted language, the error typically won't be raised until the code is ran.

Possible program space
~~~~~~~~~~~~~~~~~~~~~~

Because compilation and type checking necessarily eliminates some programs which in theory would be sound, the programmer can write programs in dynamically typed languages that aren't possible in statically typed languages. Consider the following example (Python code):

.. code-block:: python
    :linenos:

    def func(which_type, which_value):
        if which_type == 'str':
            return which_value + 'a'
        elif which_type == 'int':
            return which_value + 5
        else:
            return None

    # the following parameters are hard coded here but could e.g. be read from a file instead
    print func('str', 'foo')
    print func('int', 42)

The function 'func' returns either a string, an integer or None depending on the input parameter. The program can be run and it'll work perfectly fine, but if one attempts to write a similar program in a statically typed language then one must circumvent the type checking because the types for both the second input parameter and the return value are dynamic, that is, determined at runtime. While for many languages it's possible to circumvent the type checking, in practice the easiest solution in a statically typed language would be to avoid this kind of a construct altogether and reshape the program such that it won't be necessary.

Note: a more idiomatic way to accomplish this in Python would be to use the information about the type of a variable directly. I.e. running "isinstance(which_value, str)" will return True if the variable "which_value" is of type string.

*Exercise*: Rewrite the above code such that the function "func" only takes one parameter - which_value - and the operation performed on it depends on the output of "isinstance".

In general, there's no clear right or wrong around which kind of typing is the best, and typically arguments around this topic arise the same way normal people would argue about which car make, film, political party or religion is the best. While I attempt to stay objective, the following does include my subjective opinion where necessary - caveat emptor.

Often, for short and simple programs or programs that are easy to test, dynamic typing doesn't hurt, and dynamically typed languages often help implement the program faster. For larger programs that are more difficult to break to smaller pieces that can be executed in isolation, static typing often ends up being a big productivity boost as a large class of errors are trivially caught by the compiler, as opposed to discovering issues at runtime.

In addition to typing, there are other considerations about which language to use when; compiled languages have typically much better performance than dynamically typed languages which, depending on the requirements, may or may not rule out dynamically typed languages; languages with memory safety may be preferred to avoid any security issues or other memory handling related bugs; some platforms, especially embedded systems, may only have a C compiler or an assembler available.
