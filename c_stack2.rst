More C and the stack
--------------------

Arrays
======

Arrays are blocks of memory allocated in stack.

.. literalinclude:: st6.c
   :language: c

Here, at the end of main, our stack looks like this:

::

    | Address    | Contents   |
    | 0x50000010 | 0          |
    | 0x5000000c | 0          |
    | 0x50000008 | 0          |
    | 0x50000004 | 2          |
    | 0x50000000 | 3          |

In other words, each integer in the array is available in the stack.

Arrays and functions
====================

When passing an array to a function, the array is not copied like a pointer or an integer, but instead a *pointer* to the first element is passed. The array is said to *decay* to a pointer:

.. literalinclude:: st7.c
   :language: c
   :linenos:

When entering the function "func", our stack looks like this:

::

    | Address    | Contents   |
    | 0x50000020 | 0          |
    | 0x5000001c | 5          |
    | 0x50000018 | 0x50000000 |
    | 0x50000014 | 0x12345678 |
    | 0x50000010 | 0          |
    | 0x5000000c | 0          |
    | 0x50000008 | 0          |
    | 0x50000004 | 2          |
    | 0x50000000 | 3          |

That is, our array is located from 0x50000000 to 0x50000010. We then have the return address due to our function call, a pointer pointing to the first element (variable "int \*array"), the variable "len" which has value 5, and our index variable "i" with value 0. (Remember, each variable we define must have its location in the stack.)

What happens in the function "func"? The interesting part is the contents of the loop. On line 7, we double the value of the number pointed to by the pointer. We then increment *the pointer itself*, i.e. *the address pointed to* by one. Because the pointer is a pointer to int, incrementing the pointer by one effectively makes the pointer point to the next int. This means that after we've iterated through the loop once, our stack looks like this:

::

    | Address    | Contents   |
    | 0x50000020 | 1          |
    | 0x5000001c | 5          |
    | 0x50000018 | 0x50000004 |
    | 0x50000014 | 0x12345678 |
    | 0x50000010 | 0          |
    | 0x5000000c | 0          |
    | 0x50000008 | 0          |
    | 0x50000004 | 2          |
    | 0x50000000 | 6          |

In other words, we've doubled the value at 0x50000000, added 4 (the size of an int in our example) to the pointer at 0x50000014, and added 1 to "i".

Now, after the function has completed and the execution returns to main, our stack looks like this:

::

    | Address    | Contents   |
    | 0x50000010 | 0          |
    | 0x5000000c | 0          |
    | 0x50000008 | 0          |
    | 0x50000004 | 4          |
    | 0x50000000 | 6          |

That is, the top addresses are no longer "officially" part of the stack and the values in our array have all been doubled.

If you're wondering what happens to the memory above the stack when a function call returns, the answer is probably nothing. What the compiler instructs the CPU to do when returning from a function is simply to decrement the *stack pointer* - a pointer that points to the top of the stack, which is needed when adding new data in the stack. The code to manage the stack pointer is added automatically by the compiler.

Stack overflow
==============

Typically, the compiler allocates a chunk of memory for the stack at the beginning of the program execution. The memory is typically eight megabytes. This means that you could possibly - although the behaviour is undefined, meaning the compiler can generate any code - access memory in stack that's been allocated but not defined. We can try this out:

.. literalinclude:: st8.c
   :language: c
   :linenos:

What happens here is that we first define a variable "a", then a pointer that holds the address of that variable ("b"), and finally a pointer that accesses the memory after "b". At the end of the program our stack could look like this:

::

    | Address    | Contents   |
    | 0x50000010 | ?          |
    | 0x5000000c | ?          |
    | 0x50000008 | 0x5000000c |
    | 0x50000004 | 0x50000000 |
    | 0x50000000 | 4          |

That is, "a" has value 4 and "b" has the address of "a" as value, but "c" points to an address in the stack where the contents are unknown. Now, because the behaviour of this program is undefined, the program could output anything, and the output could depend e.g. on the compiler optimisation flags.

.. topic:: Undefined behaviour

    Undefined behaviour is a concept that mainly only exists in C and C++ and refers to behaviour which according to the language standard isn't defined. The language specifications in these cases explicitly allow the compiler to do anything. What compilers could do is insert code that would crash your program, or possibly send data over a network or reboot the system. What the compilers typically insert is the "expected" code - e.g. if you read from a variable or a buffer where the contents are undefined, the compiler will insert code to read from it anyway, such that you might get *stale* data - for example, the value of a variable that doesn't exist anymore. In general, bugs involving undefined behaviour are unpleasant to work with, such that it's recommended to avoid undefined behaviour where possible.

If you do use more than eight megabytes of stack, for example by allocating a very large array, then this is typically caught by the operating system, causing it to send the SIGTERM signal to your program, effectively killing it. This is called stack overflow.

