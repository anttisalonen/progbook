Lots of quadratic equations
---------------------------

Files
=====

Now that we're able to solve multiple quadratic equations, let's step it up one notch. Let's have the goal of calculating the roots of 10,000 quadratic equations and finding the function with the largest root, whereby the definitions of all functions are stored in a file.

To get started, here's a file with one function definition in it: :download:`download text file <../material/quad/test1.txt>`

This file looks like the following:

.. literalinclude:: ../material/quad/test1.txt
    :language: bash

In other words, it's a file with one line. The line has three floating point numbers. The numbers represent a quadratic function with values a, b and c respectively.

The C standard library provides an interface for opening, reading, writing and closing files. A file can be opened like this:

.. code-block:: c

    FILE *fp = fopen("test.txt", "r");
    if(fp == NULL) {
        fprintf(stderr, "Could not open file.\n");
    }

In other words, there's a standard function called fopen() (for "file open") which takes two parameters. The first parameter is the path and name of the file to open. If no path is given, it is expected that the file is located in the current working directory. The second parameter takes a string which identifies what should be done with the file. The string "r" means the file should be opened for reading only. The string "w" would mean the file is opened for writing; opening a file for writing this way would delete its previous contents. The function fopen() returns a value of type pointer to "FILE". "FILE" is a special type which can be used when reading or writing to a file, or closing it.

If the file could not be opened, a special pointer called *NULL pointer* is returned instead. *NULL pointer* is a pointer which is invalid. We can check whether the pointer is a NULL pointer by using the code like above. The convention is that error messages are written out to *standard error* instead of *standard output* using the fprintf() function like above for reasons that will be expanded upon later in this book.

*Exercise*: Look up the man page for fopen().

Once you have a file open, you can close it using the following code:

.. code-block:: c

    fclose(fp);

*Exercise*: Look up the man page for fclose().

How about reading a file? There are several ways to do this. One way we could use is to use the function fread(), but in our case, the function fscanf() suits better. fscanf() is a bit like the inverse of printf(): it takes a format string and the addresses of variables where the contents of the string should be stored, according to the placeholders in the format string. An example is probably the best way to illustrate this:

.. code-block:: c
    :linenos:

    float a;
    float b;
    float c;
    int num_read = fscanf(fp, "%f %f %f", &a, &b, &c);
    if(num_read != 3) {
        fprintf(stderr, "Could not read three numbers.\n");
    }

Let's go through this line by line:

* Lines 1-3: We define three variables of type float.
* Line 4: We use fscanf() to *read in* the contents of the file. We pass fscanf() five parameters: The first one is the FILE pointer. The second one is the format string. In our format string, we say we're expecting the file to contain three floating point numbers, separated by a space. The last three parameters are the addresses of our floating point variables. We tell fscanf() that we'd like to store the contents of the three floating point numbers from the file to these variables.
* Line 5: We check the *return value* of fscanf(). fscanf() returns the number of variables for which the data could be stored. We passed in three addresses so we expect it to have stored data for three variables. If the return value is not three then something went wrong during reading of the file; for example, only two numbers were available in the file.

*Exercise*: Look up the man page for fread() and fscanf().

We should now have what we need in order to read a quadratic equation definition from the file referenced above, and calculate its roots.

*Exercise*: Download the above text file in your computer. Add code in your program to open that file. Read in the contents of that file to three variables. Use your previously written function solve_and_print() to solve that quadratic equation and print the roots.

Larger files
============

Now that we're able to read in one quadratic equation definition from a file, let's try this for 10,000 equations. See this file: :download:`download text file <../material/quad/test.txt>`

That file contains 10,000 lines, each containing three floating point numbers, each representing a, b and c respectively. Let's see if we can find the equation with the largest root.

*Exercise*: Download the above file. Modify your program to open that file. Add a loop in your program to loop through your code that reads in the values a, b and c and solves the root for that quadratic equation such that your program reads through all the 10,000 equations and calculates the root for all of them. Note: you may want to *not* print out the roots for all equations as that could create a lot of output.

Aside: Arrays
=============

While this problem can be solved without arrays, you may find it useful to use them.

An array simply contains multiple variables of the same type, such that the variables are all stored next to each other. They can be useful if you need multiple variables of the same type. In other words, instead of writing e.g.:

.. code-block:: c

    /* don't do this */
    int a0 = 0;
    int a1 = 10;
    int a2 = 20;
    int a3 = 30;
    int a4 = 40;

...you can write:

.. code-block:: c

    int a[5];
    for(int i = 0; i < 5; i++) {
        a[i] = i * 10;
    }

(The text between /\* and \*/ is a *comment*; you can use these to explain your code.) We don't save very much typing with just five elements, but the more elements you have, the more is saved by using arrays.

Now we can try to find the equation with the largest root, i.e. the equation where we find a root that is arithmetically larger than any other root.

*Exercise*: Find the equation with the largest root. To do this, you need to note the largest root by having a variable to store this *outside* the loop. For all the roots found for an equation, compare those roots with the largest root found so far. If the root for the current equation is larger than the largest found so far, make note of it by modifying the variable which holds the largest root. Similarly, keep track of the variables a, b and c for the equation with the largest root. Print the a, b and c values of the equation with the largest root at the end.

Phew! We've learned a lot in this chapter.
