Generating input data using Python
----------------------------------

Previously we wrote a program that calculates the roots of 10,000 quadratic equations. You may ask, where did the input data, i.e. the definitions of 10,000 quadratic equations, come from?

The short answer is that is was generated using a Python script. How would we go about generating this file in Python? The file contains 10,000 lines, each having three floating point numbers. The numbers should be random. Python provides a set of random functions, the simplest one being random.random(), which returns a random floating point number between 0 and 1:

.. literalinclude:: ../material/py/rand.py
   :language: python

Writing files in Python is fairly straightforward. We can use the 'with' clause which ensures the file will be closed after we're done with it:

.. literalinclude:: ../material/py/with.py
   :language: python

This program will create a file called test.txt with five times the line "0.200000 0.500000". (If the file existed before, it will be overwritten.)

*Exercise*: Create the described input file. To make the functions more interesting, don't use the output of random.random() directly, i.e. don't only store numbers between 0.0 and 1.0, but use multiplication and subtraction to generate numbers that are e.g. between -10.0 and 10.0.
