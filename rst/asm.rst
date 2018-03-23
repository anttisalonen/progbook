Under the hood
--------------

We've now used a couple of languages and gotten some meaningful output out of them, but we haven't really gone into the details of how exactly do they tell the CPU what to do.

In general, as we know, the CPU executes instructions, one after the other. If we program in a language such as C, our compiler typically takes our code and turns it into a listing of CPU instructions.

What's new is that you can ask the compiler to output the *assembly code* into a file for inspection. Let's do just that. In the chapter "C and the stack", we had an example program:

.. literalinclude:: ../material/stack/st1.c
   :language: c
   :linenos:

Now, typically (i.e. for gcc and clang), you can instruct the compiler to generate assembly by passing the "-S" flag. The assembly code is then generated in the file specified with the "-o" flag. E.g.:

.. code-block:: bash

    $ clang -Wall -S -o st1.s st1.c

*Exercise*: Run the above and inspect the resulting assembly code.

The assembly code is specific to the CPU ISA (instruction set architecture). At time of writing the main CPU ISA families are x86 (which is used in most desktop, laptop and server computers) and ARM (which is used in most mobile devices such as smart phones). Chances are you ran the compiler on an x86 computer.

If you take a look at the assembly code, you see a lot of code that's always necessary in order to set up a program to execute. In general, code that's always required for setup without being heavily dependent on the rest of the program is called *boilerplate*. In our case, we should focus on the assembly code generated from lines 4-8 as those are the most relevant lines in our program.

When I run the above command (using clang 3.9.1 on Linux x86-64), and inspect the output, I see what's listed below (only including the call to "printf" and a few instructions before):

.. code-block:: asm
    :linenos:

	subq	$16, %rsp
	movabsq	$.L.str, %rdi
	movl	$3, -4(%rbp)
	movl	$4, -8(%rbp)
	movl	-4(%rbp), %eax
	addl	-8(%rbp), %eax
	movl	%eax, -12(%rbp)
	movl	-12(%rbp), %esi
	movb	$0, %al
	callq	printf

While we won't go into details of the x86 assembly, for the purposes of attempting to get a basic understanding of what's happening let's go through this instruction by instruction:

* Line 1: we subtract 16 from the *stack pointer* - i.e. move the stack pointer by four integers i.e. allocate four integers in the stack
* Line 2: $.L.str is our static string, "%d\\n". We move this in the rdi register which is used by printf.
* Line 3: We move the fixed value 3 to the address rbp - 4. rbp is the stack base pointer. This means we set a value in the stack to 3.
* Line 4: We set the next value in the stack to 4.
* Line 5: We move the value from address rbp - 4 to the register eax. Register eax can be used for basic arithmetic functions such as addition.
* Line 6: We add the value from address rbp - 8 with the value stored in the register eax, such that the resulting value is stored in eax. In other words, 3 + 4.
* Line 7: We move the value from eax to rbp - 12, i.e. from the register into the stack.
* Line 8: We move the value from rbp - 12 to esi, which is a register also used by printf. printf uses the value from rdi as the first parameter ("%d\\n") and the value from esi as the second parameter (x).
* Line 9: We set the value of the register al to 0. al is used to describe the number of vector registers used, which in this case is 0.
* Line 10: We call the function printf.

Many commands for a simple operation! If you think some of the moves aren't necessary, you're right. In fact, as we compiled the program without any *compiler optimisation flags*, it outputs fairly naive assembly code. We can pass the argument "-O2" to the compiler, which means it should apply most common, yet not too aggressive optimisations. Let's do this:

.. code-block:: bash

    $ clang -Wall -S -O2 -o st1.s st1.c

If we then inspect the assembly, it looks different:

.. code-block:: asm
    :linenos:

	movl	$.L.str, %edi
	movl	$7, %esi
	xorl	%eax, %eax
	callq	printf

This seems shorter:

* Line 1: As before, we move the static string to the register edi.
* Line 2: We move the value 7 to the register esi. The compiler inferred the calculation and calculated the result during compilation.
* Line 3: We reset the register eax. xorl applies the "xor" (exclusive or) operation. Applying "xor" with the same value on both sides results in 0. The compiler does this because it's the fastest way to also clear the register al, which is used to describe the number of vector registers used.
* Line 4: We call the function printf.

Now, in general it's not very common for developers to have to read or write assembly. It's still good to understand what happens when compiling a program, and helps understand the principle of virtual machines.
