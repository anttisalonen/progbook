Security
--------

In the chapter "Writing a toy web server", I warned about potential security holes when writing C. Let's take a look at the following code, which attempts to copy a string represented by a char pointer to a 20-element char array:

.. code-block:: c
    :linenos:

    char *str = /* ... */; /* some string which is somehow dependent on user input */
    char buf[20];
    strcpy(buf, str);

This code is a potential security hole. Can you spot why?

What happens if the string is longer than 20 characters? The function strcpy() will copy past the end of the 20-element array, overwriting something else. Technically, according to the C language standard, the behaviour is undefined, meaning that anything can happen. In practice, some data in the stack will be overwritten, like another variable, or the address to return to when the function returns. The attacker needs to construct a suitable input to modify the variables such that some code that the attacker wants to execute will get executed, typically leading to the attacker gaining full access to the system. The attacker can then e.g. read any passwords, credit card numbers or any other sensitive data available on the system.

This is possible in C and some other languages like C++ because of lack of *memory safety*; memory can be accidentally read or written. It's a good reason to consider using another language for the task at hand, if possible.

*Exercise*: Take a look at your HTTP server code and see if you can spot any security issues.


