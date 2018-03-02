Under the hood
--------------

We've now used a couple of languages and gotten some meaningful output out of them, but we haven't really gone into the details of how exactly do they tell the CPU what to do.

In general, as we know, the CPU executes instructions, one after the other. If we program in a language such as C, our compiler typically takes our code and turns it into a listing of CPU instructions.

What's new is that you can ask the compiler to output the *assembly code* into a file for inspection. Let's do just that. In the chapter "C and the stack", we had an example program:

.. literalinclude:: st1.c
   :language: c

Now, typically (i.e. for gcc and clang), you can instruct the compiler to generate assembly by passing the "-s" flag. The assembly code is then generated in the file specified with the "-o" flag. E.g.:

.. code-block:: bash

    $ clang -Wall -S -O2 -o st1.s st1.c

*Exercise*: Run the above and inspect the resulting assembly code.


