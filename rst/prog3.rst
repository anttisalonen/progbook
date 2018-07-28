The basics of programming in C
------------------------------

  It has nothing to do with dinosaurs. Good taste doesn't go out of style.

  -- Linus Torvalds

Variables in C
==============

Now that we're able to compile and run C programs, let's see how to use variables.

A variable is defined in C by first defining the *type* of the variable, followed by the name of the variable. Optionally, the value of the variable can be set when defining the variable. Associating a type with a variable is a way of the programmer telling the compiler how the data is intended to be used. There are several possible types and programs can define new types. For example, *int* and *float* are types (integers and floating point numbers respectively, whereby the number of bits used for storing the data, i.e. the minimum and maximum integer values and the floating point number precision are platform specific).

Once a variable has been defined, it can be used by *assigning* a value to it, using standard operations and functions such as arithmetic and passed to functions such as printf(). Here's an example of defining and using variables:

.. literalinclude:: ../material/intro/var.c
   :language: c
   :linenos:

Let's go through this line by line again:

* Line 5: We define a variable named "x". It has type "int". We assign the value 5 to it.
* Line 6: We define a variable named "y". It has type "int". We assign the value 42 to it.
* Line 7: We define a variable named "z". It has type "int". We don't assign any value to it. As a result, its contents are *undefined*; it can contain anything.
* Line 8: We assign a value to the variable "z", namely the result of arithmetic operations.
* Line 9: We print the value of "z". We pass the *placeholder* "%d" to printf(). printf() is a function which takes a *variable number of parameters*. Each parameter is separated by a comma (","). Here, we pass printf() two parameters; the first one is (and must be) the format string. The number of placeholders in the format string defines the number of extra parameters that must be passed. Because we have one placeholder, we must pass one extra parameter. Here, our second parameter is "z". This results in printf() printing the value of "z".

*Exercise*: Type (don't copy-paste) the above program into a file. Compile it and run the generated executable file.

*Exercise*: Create a new variable of type "int". Assign a value to it which is dependent on "x", "y" and "z". Print out its value.

*Exercise*: Create a new variable of type "int". Assign a very large number to it (e.g. 9999999999) and print out its value. Compile and run. What is the output? (If the output is unexpected, this is because the value *overflowed*; the number required more bits than your system allocated for "int".)

Loops
=====

There are several ways to define a loop in C. They're all equivalent but have somewhat different syntax.

A *for loop* has three clauses; the initialisation clause, the controlling expression and the expression to be performed after each iteration. An example may be the best way to illustrate this:

.. literalinclude:: ../material/intro/for.c
    :language: c
    :linenos:

* Line 5: We define our loop. The keyword *for* is followed by parentheses, and the parentheses contain the three clauses. The first one defines a variable called "i". Defining a variable here only makes it available within the loop; using it after the loop would cause a *compiler error*. (It's said that its *scope* is limited to the loop.) The second clause is an expression that, if evaluated to false, will terminate the loop. The third clause is an expression that is performed after each loop iteration. The syntax "i++" is shorthand for "i = i + 1"; it means "i" is incremented by one.
* Line 6: This is the body of the loop. We print out the value of "i" for each iteration.

The curly brackets ("{" and "}") can optionally be omitted if the loop body is a single statement (like in this case). However, it's often beneficial to always include the curly brackets for clarity.

*Exercise*: Type (don't copy-paste) the code to a file. Compile it and run it. What's the output?

Here's a for loop which also prints out the value of "i", but in reverse order:

.. literalinclude:: ../material/intro/for2.c
    :language: c

As you can see, it's similar, but the variable "i" is instead initialised at 9, the condition is changed such that the loop is repeated as long as the value of "i" is 0 or higher, and the value of "i" is decremented at each iteration.

A *while loop* is similar to the for loop but in some ways simplified. The following program is functionally equivalent to the first for loop example:

.. literalinclude:: ../material/intro/while.c
    :language: c
    :linenos:

* Line 5: We define a variable "i" of type "int" and assign the value 0 to it.
* Line 6: We define our *while loop* by using the keyword "while", followed by parentheses which include the controlling expression similar to the for loop, followed by the loop body.
* Line 7: We print out the value of the variable "i".
* Line 8: We increment the value of "i".

*Exercise*: Type the above code to a file. Compile it and run it.

*Exercise*: Rewrite the second for loop example (printing out the value of "i" in reverse order) by replacing the for loop with a while loop.

The third loop construct in C is the *do loop*. It's very similar to the *while* loop but ensures the loop body is executed at least once. Here's an example:

.. literalinclude:: ../material/intro/do.c
    :language: c
    :linenos:

This is functionally equivalent to the while loop example from above. Here, the loop body is executed once before checking whether the control expression evaluates to true. The control expression is evaluated after each following iteration.

.. topic:: Whitespace in C

  C is not whitespace sensitive. This means that the number of spaces and newlines between tokens is not relevant. Hence e.g. the first for loop example could equivalently be written like this:

  .. literalinclude:: ../material/intro/for3.c
      :language: c

  This will behave exactly the same as the original example. Generally it makes sense to pick one form of indentation and stick with it.

Loops can also contain loops:

.. literalinclude:: ../material/intro/for4.c
    :language: c

*Exercise*: Print the multiplication table for numbers from 1 to 10. I.e. the numbers 1 * 1, 1 * 2 etc. up to 10 * 10. 

Branches
========

Branches in C are defined using the "if" and "else" keywords. For example:

.. literalinclude:: ../material/intro/if.c
    :language: c

*Exercise*: Type, compile and run the above program.

As is expected, branches and loops can be freely combined. Here's a branch within a loop:

.. literalinclude:: ../material/intro/if2.c
    :language: c

*Exercise*: What does the above print? Try it out.

Like in Python, equality between two values can be checked for using double equality sign ("==").

Furthermore, the tokens "||" and "&&" can be used to combine conditions. "||" means "or" and "&&" means "and":

.. literalinclude:: ../material/intro/if3.c
    :language: c

*Exercise*: Write a program that has a loop within a branch.

*Exercise*: Write a program that prints "Hello world" five times, followed by printing "Hallo Welt" five times. Note that this can be written either with or without using a branch. Write both versions.

Functions
=========

We've already seen a function definition with the definition of "main". Here's a definition of a function that takes an int as a parameter and doesn't return anything:

.. literalinclude:: ../material/intro/func.c
    :language: c
    :linenos:

* Lines 3-6: We define our function called "my_func". "void" means it doesn't return anything. "int x" in parentheses means it receives one input variable of type int, which will be named "x" within the function.
* Line 5: We print out the value of the parameter passed to the function.
* Line 10: We call our function.
* Lines 11-12: We call our function again.

*Exercise*: Type, compile and run the above program.

Here's an example of a function that returns a value:

.. literalinclude:: ../material/intro/func2.c
    :language: c
    :linenos:

Here, on line 5, we can see the use of the "return" keyword. Using this will cause the execution of the function to stop and the value after the return keyword to be assigned to the caller of the function, in our case to the variable "x_squared" on line 11.

*Exercise*: Type, compile and run the above program.

Functions can be combined with branches and loops:

.. literalinclude:: ../material/intro/func3.c
    :language: c

*Exercise*: What does the above program print? Try it out.

As an aside, here's an example use of a floating point number:

.. literalinclude:: ../material/intro/float.c
    :language: c

Here we define some floating point variables using the keyword "float". Note that we need to use the letter "f" after the numeric literal to denote a (single precision) floating point number. We can pass a floating point number to printf() using "%f" as the placeholder.

With floating point numbers, it's possible that printf() by default includes several digits after the comma, making numbers more difficult to read. The above code illustrates modifying the placeholder to "%.2f" which tells printf() to only display two digits after the decimal point.

*Exercise*: Type, compile and run the above program.

*Exercise*: Define and use a function that calculates the area of a circle. The function should receive the radius as the input and return the area as the output. The types should be float for both input and output. Use the formula "area = 3.14 * radius * radius".

*Exercise*: Use the above function to print out the areas of circles with radius 1, 2, 3... up to 10. Note that you can pass an integer to a function that expects a float as an input parameter. The compiler will *implicitly* convert an int to a float.

*Exercise*: Write a program that, for numbers from 1 to 10, will print the area of the circle with that radius if the area is between 10 and 100.

Similarly to Python, C supports operators such as / (division) and % (modulo, or remainder).

*Exercise*: Write a program that prints out the numbers between 1 to 10 which, after being divided by 3, have a remainder of 1.

As before, we should now have everything we need to write FizzBuzz.

*Exercise*: Write the code to solve FizzBuzz in C.
