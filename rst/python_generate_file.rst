Generating input data using Python
----------------------------------

Previously we wrote a program that uses the quadratic formula. Let's scale this up and have the program calculate the root of 10,000 functions. For this, defining 10,000 functions is required. These 10,000 functions should be defined in a file.

Before we can define 10,000 functions in a file, we need to decide on the file format. A function can be represented by three numbers; a, b and c. Hence the file will need to store 30,000 numbers. One of the simplest ways to achieve this is to store them as *delimiter separated values*, e.g. three numbers per line, delimited by a space character.

How would we go about generating this file in Python? The numbers should be random. Python provides a set of random functions, the simplest one being random.random(), which returns a random floating point number between 0 and 1:

.. literalinclude:: ../material/py/rand.py
   :language: python

Writing files in Python is fairly straightforward. We can use the 'with' clause which ensures the file will be closed after we're done with it:

.. literalinclude:: ../material/py/with.py
   :language: python

This program will create a file called test.txt with five times the line "0.200000 0.500000". (If the file existed before, it will be overwritten.)

*Exercise*: Create the described input file. To make the functions more interesting, don't use the output of random.random() directly, i.e. don't only store numbers between 0.0 and 1.0, but use multiplication and subtraction to generate numbers that are e.g. between -10.0 and 10.0.
