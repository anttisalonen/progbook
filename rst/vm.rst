Virtual machines
----------------

In the previous chapter we discussed the assembly language. Many languages, including C and C++, are typically compiled to a machine specific assembly code before the program can be executed. Which prompts the question, what do other languages do.

Python
======

Python is an interpreted language. This, however, doesn't mean that the interpreter looks at each character in the code while the program is being executed; instead, the interpreter compiles the code to *bytecode* - a code similar to assembly, which is then interpreted by the interpreter. You may see files with the suffix .pyc after running some Python code - these contain the bytecode.

The standard Python implementation also provides a library for inspecting the resulting bytecode. Take a look at this example:

.. literalinclude:: ../material/vm/vm1.py
   :language: python

What happens here is we define a trivial function, then use the built in module "dis" to display the disassembly of this function. You can also use this module in the Python interpreter.

The program outputs this:

.. code-block:: bash

      4           0 LOAD_FAST                0 (a)
                  3 LOAD_FAST                1 (b)
                  6 BINARY_ADD          
                  7 RETURN_VALUE        

The first two lines load two values, a and b, and the third one adds the two.

If you recall the x86-64 assembly, this is somewhat similar, but arguably simplified. Which is kind of the point: it's relatively fast for the Python interpreter to interpret the bytecode, but it's still independent of the machine, such that Python code can in general be run on any platform that can run the Python interpreter. Hence such interpreters are also called *virtual machines* - they run a program which isn't compiled for a certain platform and is hence platform independent.

Speaking of the Python interpreter, it's also a good idea to take a look at the Python implementation to get a better understanding of how it works.

*Exercise*: Find the source code of the main Python implementation online. (This is also called "cpython", as in, the C implementation of Python.) Make sure you pick the source code of the 2.7 branch, not the Python 3 code. You may either download a package with all the source code, or browse the code as it's stored in version control, though downloading the whole code might be easier for searching (find and grep). See if you can find the function which interprets the command BINARY_ADD.

Other virtual machines
======================

Javascript is implemented by each browser, and several implementations are actually virtual machines which compile Javascript to bytecode and interpret this at run time. However, many of the main implementations actually compile Javascript code directly to machine code before the code is run for better performance; so called *JIT* or just-in-time compilation.

Java is generally not compiled into machine code but to bytecode executed by *JVM*, or Java Virtual Machine. JIT is also used with JVM.

Similar to JVM, .NET from Microsoft contains a virtual machine (called *CLR* or Common Language Runtime) which executes .NET bytecode and uses JIT for improved performance.

As virtual machines generally only take the specific bytecode as the program input, and the bytecode generally is not strictly tied to a higher level language, it's often possible to implement a compiler for another language which outputs bytecode which can then be interpreted by the virtual machine. There are several languages that are not Java but can execute on the JVM, such as Clojure (which is a dialect of Lisp) and Scala. Similarly, there are several languages that can run on the .NET virtual machine such as C# and Visual Basic .NET. There are also implementation of Python that compile Python to JVM and CLR bytecode.
