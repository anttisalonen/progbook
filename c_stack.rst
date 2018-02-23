C and the stack
---------------

We touched on stack briefly before. This chapter goes a bit more in depth on this. Arguably, if you understand the stack then you will also understand pointers and C memory management in general.

The stack is a chunk of memory (buffer) that the compiler allocates for your program when the program is started. It's used for all your variables and function calls. You can imagine stack as a, well, stack where, whenever you declare a variable, or call a function, a sheet of paper is added on top. Whenever a variable exits the scope or the function call is over, the related sheets of paper are taken out of the stack again.

Let's imagine we have a toy program:

.. literalinclude:: st1.c
   :language: c

This code effectively generates a stack that looks like this when main is entered:

::

    | Address    | Contents |
    | 0x50000008 | ?        |
    | 0x50000004 | 4        |
    | 0x50000000 | 3        |

.. topic:: What's the 0x

  0x denotes *hexadecimal numbers*. They're just like regular (decimal) numbers, but we include letters a-f for numbers 10-15.

  If we, for example, take the number 3. In hex, we write this 0x3, i.e. simply add 0x in the front to denote a hex number. In binary, this is 0b11. Recall that binary can only have 0 or 1, hence, when we add 1 and 1, we "overflow" and have to add another digit, hence 0b10. When we add 1 again to get 3, in binary this is 0b11.

  If we then consider the number 13: in hex, this is 0xd. In binary, this is 0b1101. The reason we call it "13" is because when we added 1 to 9, the decimal system "overflowed" and we had to add another digit. Hex didn't have this issue so it's 0xd.

  Finally, if we consider the number 238: in hex, this is 0xee. Because the number is larger than 16, we had to overflow and hence have another digit in our hex number. However it's still less than 256 (0x100) so only two hex digits are required.

  Why do we do this? Well, you might recall that as computers generally are built using digital logic, they use binary numbers a lot, and they also often use round numbers for the *memory layout* as well as *memory boundaries*. Hence a program *memory space* could e.g. start at the address 0x50000000. In binary this is 0b0101000000000000000000000000 (32 bits). In decimal this is 83886080 but because that's generally a less round number than plain 0x50000000, we use hex instead.

  Tip: You can easily convert between hex, binary and decimal in the Python interpreter. You can start it by running "python2"; e.g. running "hex(30)" will print the number 30 in hex, "bin(30)" will print it in binary, and running e.g. 0xee or 0b11 will convert hex or binary to decimal.

What we have here is our stack with three entries: The first one (from the bottom) resides in address 0x50000000 and has been set to 3, i.e. it represents our variable "a". The second one is our variable "b" and the third is our variable "x".

Because we just entered main and the addition hadn't been executed yet, "x" contains undefined data. After the main function has been executed, our stack looks like this:

::

    | Address    | Contents |
    | 0x50000008 | 7        |
    | 0x50000004 | 4        |
    | 0x50000000 | 3        |

Function calls
==============

I said the stack is also used for functions. Let's have another example:

.. literalinclude:: st3.c
   :language: c

Here, we define a function and call it. At the beginning of main, the stack looks like this:

::

    | Address    | Contents |
    | 0x50000004 | ?        |
    | 0x50000000 | 3        |

Fairly unspectacular. However, when main calls the function "double_input" and we enter that function, the stack looks like this:

::

    | Address    | Contents   |
    | 0x5000000c | 3          |
    | 0x50000008 | 0x12345678 |
    | 0x50000004 | ?          |
    | 0x50000000 | 3          |

We now have two new entries in our stack. The top one (0x5000000c) is our variable "i" which has been assigned the value 3 - a copy of our variable a. The third one (0x50000008) contains the *return address of our function call* - it's an *address in the code of main* where the code execution should return to once the function call returns. This is something that's added by the compiler to ensure the program runs correctly.

Pointers
========

Now, let's have some pointers.

.. literalinclude:: st4.c
   :language: c
   :linenos:

Let's go through this line by line.

* Line 5: We declare a normal int and assign 3 to it.
* Line 6: We declare a pointer to an int.
* Line 7: We assign the pointer the *address of* "a", i.e. make the pointer point to "a".
* Line 8: We assign the *pointed value* of "b", i.e. "a" to 4.
* Line 9: We print out the value of "b", i.e. the address that "b" points to (just for fun).
* Line 10: We print out the value that "b" points to, i.e. "a".

How does the stack look like for this at the end of main? Can you guess?

::

    | Address    | Contents   |
    | 0x50000004 | 0x50000000 |
    | 0x50000000 | 4          |

We declare two variables so we have two entries in our stack. The first one (from the bottom) is "a". The second one is our pointer, whereby its value is the address of "a", i.e. 0x50000000.

*Exercise*: Run the program yourself and check what the value of "b" is on your computer. (Probably something more difficult to read than 0x50000004. The number of hex digits in your output is a clue as to whether you're running on a 32-bit or a 64-bit CPU.)

Let's then mix pointers and functions:

.. literalinclude:: st5.c
   :language: c

The function "double_input" takes an int pointer and doubles the value of the integer that the pointer points to. Let's look at the stack when we enter that function:

::

    | Address    | Contents   |
    | 0x50000008 | 0x50000000 |
    | 0x50000004 | 0x12345678 |
    | 0x50000000 | 3          |

We've declared two variables ("int a" and "int \*i"), and "i" was assigned to the address of "a" as the function was called.

The function assigns a value to the address pointed to by "i", namely the value of the address itself multiplied by two, such that the value in address 0x50000000 becomes 6.

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

If you do use more than eight megabytes of stack, for example by allocating a very large array, then this is typically caught by the operating system, causing it to send the SIGTERM signal to your program, effectively killing it. This is called stack overflow.

Dynamic memory allocation
=========================

You might ask yourself any of the following questions:

* The stack can hold up to eight megabytes. What if I need more memory?
* What if I want to allocate memory in a function and have the caller use it?
* What if I want to allocate memory where I don't know how long I'll be needing it?
* What if I want to allocate memory where the exact amount of memory I need isn't known until runtime?

The answer to all of these is *dynamic memory allocation*.

The C standard library provides two functions for this: "malloc" and "free". The function "malloc" (memory allocation) allocated memory, which can then be freed using the function "free". Here's a simple example:

.. literalinclude:: st9.c
   :language: c
   :linenos:

Let's first go through this line by line:

* Line 8: We call "malloc" to allocate memory for 20 integers (80 bytes if our int is four bytes). We *cast* the type of the "malloc" return value from void pointer to int pointer. This is because "malloc" in general returns a void pointer, which means any pointer, because "malloc" isn't specific to any type.
* Line 9: We *assert* the return value of "malloc" is non-zero. If "malloc" fails, it will return a NULL pointer, and in this case our assertion would fail, meaning our program would crash itself. This is to ensure our memory allocation succeeded. (In practice there should be no problem allocating 80 bytes.)
* Line 10: We set all of the allocated memory to 0.
* Line 11: We set the first element in our array to 3.
* Line 12: We set the second element in our array to 4. The syntax here is equivalent to what was used in line 11.
* Line 13: We declare a variable "b" which has the same value as "a".
* Line 14: We increment the value of "b" by two (that is, eight bytes, assuming our int is four bytes). It now points to the third element in our array.
* Line 15: We set the value pointed to by "b" to 5.
* Line 17: We free the memory we allocated.

At the end of the main function, our stack will look something like this:

::

    | Address    | Contents   |
    | 0x50000004 | 0x23456788 |
    | 0x50000000 | 0x23456780 |

That is, we have two variables, "a" and "b", whereby "a" points to the address of the first byte of the memory we allocated (and freed) and "b" points to an address eight bytes after. The function "malloc", when allocating memory, will, with the help of the operating system, retrieve and provide the address to a block of memory somewhere in RAM, such that the address may look random but will *not* be in the stack. It is said that the memory is in this case allocated in the *heap*.

*Exercise*: Allocate some memory using "malloc". Print out the address of the memory returned using printf("%p\\n", pointer). Compare against the address of a variable you have in stack. Run your program multiple times. Do the addresses change?

Dynamic memory allocation gotchas
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

There are a few things that could go wrong with dynamic memory allocations, providing job security for programmers for years to come.

Use after free: As we saw in our program above, the variable "a", which contained the address of memory allocated in heap stayed unchanged after freeing the memory. In principle there's nothing preventing the programmer from accidentally reading the memory after freeing it, or writing to it. Reading it and then using the value, for example, in an "if" statement, will cause the code to randomly either branch into the "if" statement or not. Writing to it will possibly mess up some other memory in your program, e.g. accidentally changing some unrelated variable. This is all nasty.

One way to prevent this is to always reset your pointers after freeing the memory that was pointed to. E.g.:

.. code-block:: c

    int *a = malloc(...);
    free(a);
    a = NULL;

By setting "a" to NULL, which is a special pointer value roughly meaning "invalid", if we now try to either read the value pointed to by it, or write to it, the program will immediately crash. (Actually reading or writing to NULL is undefined behaviour, but in practice compilers seem to do us a favour and crash.) This makes it more obvious what has gone wrong.

Double free: You may accidentally free memory pointed to by a pointer after it's already been freed. This will possibly free some other memory that you're still using, or otherwise generally corrupt your memory. Again, resetting pointers to NULL after "free" helps as calling "free" for a NULL pointer is defined to do nothing.

Memory leaks: You may have a pointer to memory, then point the pointer somewhere else but not free the memory. The memory is now inaccessible but still allocated to your program. If you do this often enough, or if your program runs long enough, it will soak up a significant amount of memory and will need to be restarted.

In general, it's sometimes not clear from using a function what the semantics for memory ownership are. Let's say you call a function from a library which returns a pointer. Should you call "free" on that pointer when you're done with it, or not? This is typically (hopefully) described in the documentation for the function.
