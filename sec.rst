Further on programming languages
--------------------------------

Security
========

In the chapter "Writing a toy web server", I warned about potential security holes when writing C. Let's take a look at the following code, which attempts to copy a string represented by a char pointer to a 20-element char array:

.. code-block:: c
    :linenos:

    char *str = /* ... */; /* some string which is somehow dependent on user input */
    char buf[20];
    strcpy(buf, str);

This code is a potential security hole. Can you spot why?

What happens if the string is longer than 20 characters? The function strcpy() will copy past the end of the 20-element array, overwriting something else. Technically, according to the C language standard, the behaviour is undefined, meaning that anything can happen. In practice, some data in stack will be overwritten, like another variable, or the address to return to when the function returns. The attacker needs to construct a suitable input to modify the variables such that some code that the attacker wants to execute will get executed, typically leading to the attacker gaining full access to the system. The attacker can then e.g. read any passwords, credit card numbers or any other sensitive data available on the system.

This is possible in C and some other languages like C++ because of lack of *memory safety*; memory can be accidentally read or written. It's a good reason to consider using another language for the task at hand, if possible.

Statically vs. dynamically typed languages
==========================================

We already touched upon the topic of static and dynamic typing on our programming language taxonomy.

We also briefly touched upon the concept of interpreted vs. compiled languages as we wrote our first Python programs.

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

In general, there's no clear right or wrong around which kind of typing is the best, and typically arguments around this topic arise the same way normal people would argue about which car make, film, political party or religion is the best. While I attempt to stay objective, the following does include my subjective opinion where necessary - caveat emptor.

Often, for short and simple programs or programs that are easy to test, dynamic typing doesn't hurt, and dynamically typed languages often help implement the program faster. For larger programs that are more difficult to break to smaller pieces that can be executed in isolation, static typing often ends up being a big productivity boost as a large class of errors are trivially caught by the compiler, as opposed to discovering issues at runtime.

In addition to typing, there are other considerations about which language to use when; compiled languages have typically much better performance than dynamically typed languages which, depending on the requirements, may or may not rule out dynamically typed languages; languages with memory safety may be preferred to avoid any security issues or other memory handling related bugs; some platforms, especially embedded systems, may only have a C compiler or an assembler available.
