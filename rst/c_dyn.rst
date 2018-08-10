Dynamic memory allocation
-------------------------

You might ask yourself any of the following questions:

* The stack can hold up to eight megabytes. What if I need more memory?
* What if I want to allocate memory in a function and have the caller use it?
* What if I want to allocate memory where I don't know how long I'll be needing it?
* What if I want to allocate memory where the exact amount of memory I need isn't known until runtime?

The answer to all of these is *dynamic memory allocation*.

The C standard library provides two functions for this: "malloc" and "free". The function "malloc" (memory allocation) allocates memory, which can then be freed using the function "free". Here's a simple example:

.. literalinclude:: ../material/stack/st9.c
   :language: c
   :linenos:

Let's first go through this line by line:

* Line 8: We call "malloc" to allocate memory for 20 integers (80 bytes if our int is four bytes). We *cast* the type of the "malloc" return value from void pointer to int pointer. This is because "malloc" in general returns a void pointer, which means any pointer, because "malloc" isn't specific to any type. We'll go a bit deeper around casting later.
* Line 9: We *assert* the return value of "malloc" is non-zero. If "malloc" fails, it will return a NULL pointer, and in this case our assertion would fail, meaning our program would crash itself. This is to ensure our memory allocation succeeded. (In practice there should be no problem allocating 80 bytes.)
* Line 10: We set all of the allocated memory to 0.
* Line 11: We set the first element in our array to 3.
* Line 12: We set the second element in our array to 4. The syntax is different but the semantics here is equivalent to what was used in line 11.
* Line 13: We define a variable "b" which has the same value as "a".
* Line 14: We increment the value of "b" by two (that is, eight bytes, assuming our int is four bytes). It now points to the third element in our array.
* Line 15: We set the value pointed to by "b" to 5.
* Line 17: We free the memory we allocated.

At the end of the main function, our stack will look something like this:

.. verb::

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
