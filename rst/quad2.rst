Quadratic formula in Python
---------------------------

To improve our programming skills, let's repeat the previous exercises in Python.

This should be easier not only because Python is a higher level language but also because we're already familiar with the problem domain.

From this point on, the book will use Python 2. This is not because Python 3 is a worse language - in fact it's very similar, and probably better - this is mostly due to the author being more used to Python 2. However, because the two languages are so similar you should have no problem picking up Python 3 later on when needed.

It's system specific how to start the Python 2 interpreter. Typically, at the time of writing, there are up to three possible commands you can run:

* "python": If present, this would start the default Python interpreter, which often is Python 2, but could also be Python 3. You can find out which by looking at the first lines of the interpreter output or by running "python --version".
* "python2": If present, this would start the Python 2 interpreter.
* "python3": If present, this would start the Python 3 interpreter.

*Exercise*: Find out which Python interpreter or interpreters you have installed. If you only have Python 3, also install the Python 2 interpreter.

We can now refresh our thinking behind Python. Comparing Python and C for the first exercise - having some code able to solve the quadratic equation at all - there are a few differences:

* In C, all code must reside in functions. The function called "main" is the starting point for the code. In contrast, in Python it is allowed to have code on the top level, i.e. without defining any functions at all. Code is then executed top down. Functions can also be defined at top level but they won't be executed until the function is called.
* In C, we need to denote the floating point values with an "f" to denote the values as single precision values (as opposed to double precision; typically 32 bits vs. 64 bits of storage). In Python, the denotation isn't necessary (precision is interpreter specific and typically 64 bit).
* In C, in order to use square root, we #include <math.h>, link against the math library with the compiler flag -lm and call the sqrt() function. In Python, we *import math* and call the math.sqrt() function.

We should now have what we need. To get started, here's a simple Python program that calculates one root value:

.. code-block:: python

    import math

    a = 3.0
    b = -2.0
    c = -3.0
    det = b * b - 4.0 * a * c
    if det >= 0:
        r1 = (-b + math.sqrt(det)) / (2.0 * a)
        print 'Value of r1 is %.2f' % r1

*Exercise*: Write a Python program that solves the quadratic equation for a=3.0, b=-2.0, c=-3.0. Print out the number of roots as well as all found roots.

Functions and lists
===================

Next up we should encapsulate our quadratic equation code to a function. As C doesn't support returning multiple values from a function we resorted to using pointers. Python handles this differently as it can return *lists* which can contain any number of elements. (To be pedantic, C can return a programmer defined data structure or an array allocated in dynamic memory as well; these topics will be covered later.)

Here are some examples around using lists in Python:

.. code-block:: python
    :linenos:

    l = list()            # l is now an empty list
    x = 3
    l.append(x)           # l now contains [3]
    l.append(x * 2)       # l now contains [3, 6]
    l = [x, x * 2, x * 3] # l now contains [3, 6, 9]
    print len(l)          # prints 3 (the number of elements in l)
    print l[0]            # prints 3 (the first element of l)
    print l[2]            # prints 9 (the third element of l)
    y = l[1]              # y is now 6
    for element in l:     # for loop - creates a new variable called "element"
        print element     # prints 3, 6 and 9 in separate lines

The above also demonstrates the commenting syntax in Python; hash (#) until the end of the line are comments.

Similarly, one can return a list from a function in Python:

.. code-block:: python

    def return_a_list(x):
        return [x, x * 2]

    return_list = return_a_list(3)
    for elem in return_list:
        print 'Element in list: %d' % elem

The above function returns a list with the input parameter as well as the double of the input parameter. The following code calls that function and prints out the output.

*Exercise*: Write a function to solve the quadratic equation. It should have input parameters a, b and c and return a list of 0, 1 or 2 elements, depending on how many roots the equation has. In your code outside the function, call the function such that you can solve the quadratic equation for a=3.0, b=-2.0, c=-3.0. Print out the number of roots found (using len()), as well as the roots themselves. Only call "print" outside the function. You can iterate over the list of roots using a for loop.

Now, following our playbook, let's consider the need for solving, and printing, the roots for three different equations. Again, our toplevel code should only contain the following:

.. code-block:: python

    solve_and_print(1.0, -2.0, -3.0)
    solve_and_print(-1.0, -1.0, -1.0)
    solve_and_print(2.0, 2.0, 0.5)

Here, we should write the function solve_and_print() ourselves, and it should first call our solver function, followed by the necessary print statements to print the results.

*Exercise*: Write a Python program that solves the quadratic equation for the three functions listed above, and prints the results. Your top level code should look like the one above. The function solve_and_print() should take three parameters, a, b and c, call the solver function and print the results of the solver function.

Files
=====

We can now consider reading files in Python. While files can be opened in Python by using open() followed by a close() to close the file in the end, we can do better by using the *with* statement which will automatically close the file when the "with" statement is exited. The following code snippet illustrates this by opening a file called "test1.txt" for reading and reads all its contents to a string variable called "text":

.. code-block:: python

    with open('test1.txt', 'r') as f:
        text = f.read()

There are a few points to make:

* The built-in function open(), like in C, takes two parameters: the file to open and the mode. The mode is here set to 'r' indicating read mode. If the mode was set to 'w' for write mode, the file contents would be deleted at open.
* With the "with" statement, the name of the variable representing the file object is defined after the "as" keyword - in the above it is "f".
* We then use the file object, which was the output of the open() function, to call its member function read(). The open() function returns a file object, like in C, and the member function read() reads all the contents of the file and returns a string. This string is stored in the variable "text" in the code above.

*Exercise*: Try opening a file in Python and storing its contents to a variable like above. Print out the contents by printing out the value of the variable.

We now have a variable with text inside, and like in C, we need to somehow *parse* the three floating point variables, a, b and c, from it. Instaed of using a function from the scanf() family, Python provides different means:

.. code-block:: python
    :linenos:

    words = text.split()
    numbers = list()
    for word in words:
        number = float(word)
        numbers.append(number)

Let's see what this code does:

* Line 1: We call the member function split() on "text". This function, turns a string to *a list of strings*, whereby a whitespace (space or tab) defines the number of elements. E.g. evaluating a string "1.23 -2.34 3.45".split() will return ["1.23", "-2.34", "3.45"].
* Line 2: We create an empty list.
* Line 3: We loop through each element in the list of strings.
* Line 4: We *convert* a string to a floating point number by using the built-in float() function. This takes a value of e.g. a string or an int as input and returns a float.
* Line 5: We append the number to our list. In the end we have a list of numbers.

A thing to note at this point is that you can use the Python interpreter to try out different code. After running e.g. "python2" you end up with a prompt (">>>") where you can type any Python expression and see the result.

*Exercise*: Start the Python interpreter. Set a variable resembling our input string, e.g. "s = '1.23 -2.34 3.45'" (without double quotes but with single quotes). Split it by running "s.split()".

*Exercise*: Look up the Python documentation for split() by running "help(str.split)" in the Python interpreter.

After the above we should ensure we parsed the correct number of numbers from the file:

.. code-block:: python

    if len(numbers) != 3:
        print >> sys.stderr, 'Could not parse file correctly'

The above demonstrates the Python syntax for writing to standard error, like we did in C. (You will need to "import sys" first.)

*Exercise*: Add code in your Python program to open the file with a single line from the previous chapter. Read in the contents of that file to a list, converting the data to floating point numbers. Use your previously written function solve_and_print() to solve that quadratic equation and print the roots.

We can now approach the file with 10,000 equations and find the equation with the largest root. How would we handle a file with 10,000 lines in Python, each holding three numbers? We can first open() the file and read() all its contents to a string like before. After that, we should split that string so we have a list with 10,000 strings, one for each line. This can be done using the following:

.. code-block:: python

    with open('test.txt', 'r') as f:
        text = f.read()
        lines = text.split('\n')

Here, we use the split() member function again, but we pass it the *optional parameter* set to '\n' which denotes newline. This causes the split() function to split the string to a list of strings where each element in the list is a line. Once we have that, we can have a loop that parses the three numbers from each line. 

However, there's actually a shortcut in Python for this common use case. We can simply iterate over the file object, which by default provides us with a line for each iteration:

.. code-block:: python

    with open('test.txt', 'r') as f:
        for line in f:
            # process each line here

*Exercise*: Modify your program to open the larger file from the previous chapter. Add a loop in your program to loop through your code that reads in the values a, b and c and solves the root for that quadratic equation such that your program reads through all the 10,000 equations and calculates the root for all of them. Note: you may want to *not* print out the roots for all equations as that could create a lot of output.

*Exercise*: Find the equation with the largest root. As before, to do this, you need to note the largest root by having a variable to store this outside the loop. For all the roots found for an equation, compare those roots with the largest root found so far. If the root for the current equation is larger than the largest found so far, make note of it by modifying the variable which holds the largest root. Similarly, keep track of the variables a, b and c for the equation with the largest root. Print the a, b and c values of the equation with the largest root at the end.

