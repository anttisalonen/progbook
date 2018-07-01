Quadratic formula in C
----------------------

Now that we're able to create some C programs, let's try a bit more complicated one. Let's write a program to solve a quadratic equation.

As you may know, a quadratic equation is an equation ax\ :sup:`2`\ +bx+c=0, whereby a, b and c are constants. A quadratic function, when plotted, may look e.g. like this:

.. image:: ../material/quad/quad.png

Solving the quadratic equation provides the *roots* of the equation, i.e. the x values at which the x-axis is crossed. For the above function, the roots are at points x=-0.72 and x=1.39. There may be 0, 1 or 2 roots, and the formula to solve the quadratic equation is:

.. image:: ../material/quad/formula.png

This mathematical formula can be implemented in C. For example like this:

.. code-block:: c

    x = (-b + sqrt(b * b - 4.0f * a * c)) / (2.0f * a);

The +/- part of the formula causes up to two possible solutions for x to be available; the above C formulation only identifies one.

There may be 0, 1 or 2 roots; the case of 0 roots occurs when the part for which the square root is calculated (also called the *determinant*) is negative, as a negative number cannot have a real square root. The case of 1 root occurs when the determinant is exactly 0.

The C function sqrt() is part of the standard C library but there's a twist: in order to use the function, the header file <math.h> should be included. Furthermore, you may need to link your program against the *math library* by passing "-lm" to your C compiler when compiling.

*Exercise*: Write a program that solves the quadratic equation for a=3.0, b=-2.0, c=-3.0. Print out the number of roots as well as all found roots.

Ok, so we now have a program that solves the quadratic equation for some values. What if I asked you to solve the quadratic equation for three, or ten, different functions (a, b and c values), in the same program? There are several ways to approach this but one thing we should avoid is copy-pasting the code to solve the equation multiple times. Instead we should capture this logic in a *function*.

What we require is a function which takes a, b and c as input parameters and returns the roots, as well as an indication as to how many roots were found. Once we have this function we can then call it with different parameters and write out the information about the roots after each call.

Now, there are a few ways this could be approached. An interesting limitation we arrive at is that C can only return one value in a function. However, we can still write our function by using *pointers*.

Pointers
========

Every variable in C has an *address* in memory. Speaking in metaphors, if a variable was a house, then the value of the variable would be the contents of the house, and the memory address would be the address of the house. A pointer in C is a variable for which the value can be the address of another variable.

Here's an example:

.. literalinclude:: ../material/quad/pointer.c
    :language: c
    :linenos:

* Line 5: We define an integer variable called "a".
* Line 6: We define a variable called "p" which is a pointer to an integer. The asterisk ("\*") means the type is a pointer.
* Line 7: We assign a value to "p", namely *the address of* a. The ampersand ("&") means "address of".
* Line 8: We assign a value to *what "p" points to*. The asterisk operator ("\*") means *dereferencing* the pointer.
* Lines 9-11: We print out the values of "a", "p" and "\*p".

*Exercise*: What would you expect the output to be? Try it out.

In this example, using the pointer wasn't necessary because if we wanted to change the value of "a", we could have done it directly. However, if you want to change the value of a variable of another function then pointers become useful. What you can do is pass the address of a variable to the other function. The other function can then modify the value of that variable. In this way we can "return" multiple values from a single function.

The following example illustrates the principle:

.. literalinclude:: ../material/quad/pointer2.c
    :language: c
    :linenos:

* Lines 3-7: We define a function called "increment_by_one" which doesn't have a return value and takes two input parameters. Both input parameters are pointers to int.
* Line 5: We assign the value of what "p1" points to plus one to what "p1" points to.
* Line 6: The same for "p2".
* Lines 11-13: In our main function, we define two int variables, and pass the addresses of these to the "increment_by_one" function.

Here, we have a function that takes two pointers and increments the values of the variables that are pointed to by one.

We should now have what we need to create a function that solves the quadratic equation.

*Exercise*: Write such a function. It should have parameters a, b and c (all floats) as well as r1 and r2 (pointers to floats). It should return an int which is the number of roots found for the given function. In other words, its function *signature* should be e.g. "int quad(float a, float b, float c, float \*r1, float \*r2)". In your main function, define the variables a, b, c, r1, r2 and number_of_roots. Set the variables such that you can solve the quadratic equation for a=3.0, b=-2.0, c=-3.0. Print out the roots found. Only call printf() within main.

*Exercise*: Modify your code to solve the quadratic equation for a=3.0, b=-2.0, c=3.0 (positive c instead of negative). This equation has no roots. What happens if you print out the values of r1 and r2?

Now that we have a function that can solve the quadratic equation, we should try solving several equations and outputting the results. For the next exercise, let's assume our goal is to provide the roots for three different quadratic equations, namely.

* x\ :sup:`2`\ -2x-3
* -x\ :sup:`2`\ -x-1
* 2x\ :sup:`2`\ +2x+0.5

How should we structure the code for this? We should keep our solver function clean of side effects by not using printf() in it. Indeed, the ideal main function for this task could look like this:

.. code-block:: c

    int main(void)
    {
        solve_and_print(1.0f, -2.0f, -3.0f);
        solve_and_print(-1.0f, -1.0f, -1.0f);
        solve_and_print(2.0f, 2.0f, 0.5f);
    }

In other words, for each of the functions to find the roots for, we call a function which does the solving and the printing. We already have a function that can solve the equation so what's left to implement is capturing the behaviour of using the solver function and printing the results.

*Exercise*: Write a program that solves the quadratic equation for the three functions listed above, and prints the results. Your main function should look like the one above. The function solve_and_print() should take three parameters, a, b and c, call the solver function and print the results of the solver function.

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
