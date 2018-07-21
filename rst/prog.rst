The basics of programming
-------------------------

  And programming computers was so fascinating. You create your own little universe, and then it does what you tell it to do.

  -- Vint Cerf

It's debatable what the basics of programming are. This section presents the author's view and will cover:

* Variables
* Loops
* Branches
* Functions

We already have a Python programming environment so we'll continue using that. This section will include some code examples. The reader is expected to type these into a file, and then run the Python interpreter with that file as input. For example:

.. code-block:: python

    print('Hello world')

This is the program we wrote a couple of sections ago. Make sure you still know how to create and write such a program.

*Exercise*: Run your 'Hello world' program.

*Exercise*: Modify the text "Hello world" (e.g. change it to 'Hallo Welt') in your program and run it again.

It's best not to copy-paste the code from this book but instead type it yourself as this reinforces the learning effect.

Variables
=========

Variables hold some value. The following example defines and uses a variable named 'x':

.. code-block:: python

    x = 5
    print(x)
    x = 7
    print(x)

The above will write '5' followed by '7' in the terminal. 'x' is first defined with the value 5. It is then written out before its value is changed to 7.

You can have several variables, and the assignment can refer to other variables:

.. code-block:: python

    x = 5
    y = 6
    print(x)
    print(y)
    y = x
    print(y)

*Exercise*: What does the above code write out? Try it out.

*Exercise*: Create a program that has a variable with a value 42 and writes it out.

Loops
=====

A loop can repeat a section of code multiple times.

The following is an example of using a loop in Python:

.. code-block:: python

    for i in range(10):
        print(i)

The above will print (write to the terminal) the numbers from 0 to 9. The words "for" and "in" are Python keywords; range() is a Python function that can be used to describe the iterations of a loop; "i" is a variable that is defined using the "for" loop and will hold the current value that is provided by the range() function.

.. topic:: Indentation matters (in Python)

  Indentation refers to the whitespace (spaces or tabs) between the start of a row and the start of a text within that row. In the above example, the line containing "print(i)" has four spaces before the word "print". Whitespace must exist to indicate the body of the loop; without it, Python with raise a syntax error. The convention is to use four spaces for each indentation level in Python. Tab characters and spaces must not be mixed in Python.

Here's another loop:

.. code-block:: python

    x = 5
    for i in range(10):
        print(x)

*Exercise*: What does the above code write out? Try it out.

*Exercise*: Write a program that prints the number 42 five times.

The "print" function can also include both text and a variable. This can be achieved using e.g. the following:

.. code-block:: python

    for i in range(10):
        print('i has the value %d' % i)

This looks like it needs some explaining. What we have here is the text "i has the value %d", whereby the fragment "%d" is a *placeholder* for a number. The number for the placeholder is provided by having the percent character ("%") after the text string, followed by the variable the value of which we want to insert to the placeholder. Hence, the above program will print e.g. "i has the value 0", "i has the value 1" etc.

*Exercise*: Write a program that prints the value of a variable as part of other text five times.

The range() function can also be used to start from another number than 0. To start from 1 you can use:

.. code-block:: python

    for i in range(1, 10):
        print('i has the value %d' % i)

This will print the numbers from 1 to 9, inclusive.

Branches
========

A branch refers to the if-then structure. This is a branch:

.. code-block:: python

    x = 5
    if x < 10:
        print('x is smaller than 10')

Here, we declare a variable named 'x' with the value 5. We then compare its value against 10; if it's less, we print out some text.

The else-part is optional (in Python). If we want to have it, it could look like this:

.. code-block:: python

    x = 5
    if x < 10:
        print('x is smaller than 10')
    else:
        print('x is larger than or equal to 10')

The if-then-else-statements can be *chained*. In Python, the keyword "elif" (portmanteau of "else if") is used in this case:

.. code-block:: python

    x = 5
    if x < 10:
        print('x is smaller than 10')
    elif x < 20:
        print('x is smaller than 20')
    else:
        print('x is larger than or equal to 20')

Furthermore, the keywords "or" and "and" can be used to combine conditions:

.. code-block:: python

    x = 5
    if x <= 10 and x >= 0:
        print('x is between 0 and 10')
    if x < 0 or x > 10:
        print('x is either negative or larger than 10')

All the constructs can be combined. Here's a branch within a loop:

.. code-block:: python

    for i in range(10):
        if i <= 5:
            print(i)
        else:
            print('i is larger than 5')

*Exercise*: What does the above print? Try it out.

Apart from comparing for larger and smaller values, we can also check for equality by using two equals-to characters:

.. code-block:: python

    for i in range(10):
        if i == 5:
            print('i is 5')
        else:
            print('i is not 5')

Here's a loop within a branch:

.. code-block:: python

    x = 5
    if x < 10:
        for i in range(10):
            print(x)
    else:
        print('x is too large')

*Exercise*: What does the above print? Try it out.

*Exercise*: Write a program that prints "Hello world" five times, followed by printing "Hallo Welt" five times. Note that this can be written either with or without using a branch. Write both versions.

Functions
=========

Functions can be used to capture certain code in one block. Here's an example of a function definition and usage:

.. code-block:: python

    def my_function(variable):
        print('Hello')
        print(variable)

    x = 5
    my_function(x)
    x = 7
    my_function(x)

Here, we define the function using the keyword "def", followed by the name of the function and the *parameters* to the function. Here, the parameter is called "variable" and is available within the function. The function receives this parameter as input. Like with loops and branches, the function body must be indented. We then call the function twice with different values.

*Exercise*: What do you think the above program prints? Try it out.

There are also pre-defined functions as part of the Python programming language. We've already used some of them, e.g. range() and the numeric comparator functions (<, <= etc.). print() is also a function (although technically only in Python 3, not in Python 2).

There are a lot more pre-defined functions in Python. For example, the arithmetic operations (+, -, \*, /) are all predefined functions. Here's an example of using them:

.. code-block:: python

    x = 5
    y = x * 3 + 2
    print(y)

The above will print the number 17.

Functions can also *return* values. These can be seen as the output of the function. This way, a function can be seen as something that takes input and produces output, similarly to our logic gates. Here's such an example function:

.. code-block:: python

    def square(x):
        return x * x

    number1 = 5
    number1_squared = square(number1)
    print(number1_squared)

The above will define a variable that holds the value 5, squares it (25), stores the squared value to another variable and prints it out.

*Exercise*: Define and use a function that takes a number as input and returns that number plus one.

Functions can be combined with branches and loops:

.. code-block:: python

    def square(x):
        return x * x

    for i in range(10):
        print(i)
        print(square(i))

The above will print numbers from 0 to 9 as well as the squares of those numbers. The formatting is a bit ugly because we print "i" on one line and the square of "i" on the next line. This can be fixed by using the following construct:

.. code-block:: python

    def square(x):
        return x * x

    for i in range(10):
        i_squared = square(i)
        print("%d %d" % (i, i_squared))

Here, we use the placeholder syntax from earlier, but we write two numbers in each line. It's similar to what we did before but when more than one placeholder is used then the values to be inserted in the placeholders need to be enclosed in parentheses (here, "i" and "i_squared").

We don't need to define a variable before printing it out, so we could save some typing by doing the following:

.. code-block:: python

    def square(x):
        return x * x

    for i in range(10):
        print("%d %d" % (i, square(i)))

The next program combines functions, branches and loops:

.. code-block:: python

    def square(x):
        return x * x

    for i in range(10):
        i_squared = square(i)
        if i_squared > 10:
            print(i_squared)

*Exercise*: What does the above code print? Try it out.

Loops can also contain loops:

.. code-block:: python

    for i in range(10):
        for j in range(10):
            added = i + j
            print("i=%d; j=%d; i+j=%d" % (i, j, added))

*Exercise*: Print the multiplication table for numbers from 1 to 10. I.e. the numbers 1 * 1, 1 * 2 etc. up to 10 * 10. 

Python also supports *floating point numbers*, i.e. numbers with a decimal point (with the number of supported digits before and after the comma varying depending on the magnitude of the number). Here's an example:

.. code-block:: python

     x = 5.2
     y = 3.4
     print('x and y summed is %f' % (x + y))

Here, because x and y are floating point numbers, we need to use "%f" as the placeholder instead of "%d". (If we used %d, we'd only see the number rounded down to the first integer.)

*Exercise*: Define and use a function that calculates the area of a circle. The function should receive the radius as the input and return the area as the output. Use the formula "area = 3.14 * radius * radius". (You can also use the Python built-in power function by writing e.g. "radius ** 2" for radius squared.)

*Exercise*: Use the above function to print out the areas of circles with radius 1, 2, 3... up to 10.

*Exercise*: Write a program that, for numbers from 1 to 10, will print the area of the circle with that radius if the area is between 10 and 100.

*Exercise*: For numbers from 1 to 10, calculate both the area and the square of the number. Print out the difference between the area and the square.

Apart from the standard arithmetic operators and the power function, there's another potentially useful operator, namely *modulo*. It returns the remainder after a division, e.g.:

.. code-block:: python

    a = 23
    b = 3
    print('a = %d; b = %d; a / b = %d' % (a, b, a / b))
    print('a = %d; b = %d; a % b = %d' % (a, b, a % b))

*Exercise*: What do you think the above will print? Try it out.

*Exercise*: Write a program that prints out the numbers between 1 to 10 which, after being divided by 3, have a remainder of 1.

We now have enough in our toolbox to write *FizzBuzz*: FizzBuzz is originally a children's game where each player is expected to tell the next number, starting from 1, except if the number is divisible by 3 then the player should instead say "Fizz", and if the number is divisible by 5 then the player should say "Buzz", and if the number is divisible by both 3 and 5 then the player should say "FizzBuzz". We should write a program that plays this game; from 1 to 100, it should print the correct answer. The correct output should start with:

.. code-block:: bash

    1
    2
    Fizz
    4
    Buzz
    Fizz
    7
    8
    Fizz
    Buzz
    11
    Fizz
    13
    14
    FizzBuzz
    16
    17
    Fizz
    ...

*Exercise*: Write a program that produces the correct output, for numbers from 1 to 100.

If you succeeded in all the exercises of this section, congratulations! Not all software engineering applicants are able to write the code to solve FizzBuzz.

