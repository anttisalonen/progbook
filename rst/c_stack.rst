C and the stack
---------------

We touched on stack briefly before. This chapter goes a bit more in depth on this. Arguably, if you understand the stack then you will also understand pointers and C memory management in general.

You may remember that a CPU doesn't really have a concept of variables; it has memory accesses and registers. What happens, then, when we e.g. define an int in our C code, and use it? The value for each variable is stored either in a register or in RAM where it has a specific address, or a specific offset from an address. The C compiler reads our C code, sees that we e.g. define a variable, picks a register or a memory address for this variable and generates CPU instructions that use the variable.

The stack is a chunk of memory (buffer) that the compiler allocates for your program when the program is started. It's used for all your variables and function calls. You can imagine the stack as a stack of papers where, whenever you declare a variable, or call a function, a sheet of paper is added on top. Each variable is a sheet of paper, and the data written on the paper is the value of the variable. Whenever a variable exits the scope or the function call is over, the related sheets of paper are taken out of the stack again.

Let's imagine we have a toy program:

.. literalinclude:: ../material/stack/st1.c
   :language: c

This code effectively generates a stack that looks like this when main is entered:

::

    | Address    | Contents |
    | 0x50000008 | ?        |
    | 0x50000004 | 4        |
    | 0x50000000 | 3        |

What we have here is our stack with three entries: The first one (from the bottom) resides in address 0x50000000 and has been set to 3, i.e. it represents our variable "a". The second one is our variable "b" and the third is our variable "x". (The specific number 0x50000000 is in reality OS and CPU dependent and often arbitrary; we use 0x50000000 here for illustration purposes.)

.. topic:: What's the 0x

  0x denotes *hexadecimal numbers*. They're just like regular (decimal) numbers, but we include letters a-f for numbers 10-15.

  If we, for example, take the number 3. In hex, we write this 0x3, i.e. simply add 0x in the front to denote a hex number. In binary, this is 0b11. Recall that binary can only have 0 or 1, hence, when we add 1 and 1, we "overflow" and have to add another digit, hence 0b10. When we add 1 again to get 3, in binary this is 0b11.

  If we then consider the number 13: in hex, this is 0xd. In binary, this is 0b1101. The reason we call it "13" is because when we added 1 to 9, the decimal system "overflowed" and we had to add another digit. Hex didn't have this issue so it's 0xd.

  Finally, if we consider the number 238: in hex, this is 0xee. Because the number is larger than 16, we had to overflow and hence have another digit in our hex number. However it's still less than 256 (0x100) so only two hex digits are required.

  Why do we do this? Well, you might recall that as computers generally are built using digital logic, they use binary numbers a lot, and they also often use round numbers for the *memory layout* as well as *memory boundaries*. Hence a program *memory space* could e.g. start at the address 0x50000000. In binary this is 0b0101000000000000000000000000 (32 bits). In decimal this is 83886080 but because that's generally a less round number than plain 0x50000000, we use hex instead.

  Tip: You can easily convert between hex, binary and decimal in the Python interpreter. You can start it by running "python2"; e.g. running "hex(30)" will print the number 30 in hex, "bin(30)" will print it in binary, and running e.g. 0xee or 0b11 will convert hex or binary to decimal.

Because we just entered main and the addition hadn't been executed yet, "x" contains undefined data. After the main function has been executed, our stack looks like this:

::

    | Address    | Contents |
    | 0x50000008 | 7        |
    | 0x50000004 | 4        |
    | 0x50000000 | 3        |

Function calls
==============

I wrote the stack is also used for functions. Let's have another example:

.. literalinclude:: ../material/stack/st3.c
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

.. literalinclude:: ../material/stack/st4.c
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

.. literalinclude:: ../material/stack/st5.c
   :language: c

The function "double_input" takes an int pointer and doubles the value of the integer that the pointer points to. Let's look at the stack when we enter that function:

::

    | Address    | Contents   |
    | 0x50000008 | 0x50000000 |
    | 0x50000004 | 0x12345678 |
    | 0x50000000 | 3          |

We've declared two variables ("int a" and "int \*i"), and "i" was assigned to the address of "a" as the function was called.

The function assigns a value to the address pointed to by "i", namely the value of the address itself multiplied by two, such that the value in address 0x50000000 becomes 6.

