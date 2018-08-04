Introduction to Sudoku
----------------------

In this part of the book we'll learn some new programming concepts using Sudoku, a Japanese game.

We'll learn about the following concepts:

* Using a statically, strongly typed language to solve a problem
* Some bits around algorithms, namely search and constraint propagation, i.e. some classical AI
* Recursion

What is Sudoku? Sudoku is a game with a grid of 9x9 numbers, whereby each cell in the grid can have a number between 1 and 9. Some cells are pre-filled and the goal of the game is to fill all the cells, such that each cell contains a number that is not already used by any of its peers. The peers of a cell are the cells either on the same horizontal line, on the same vertical line, or within the same 3x3 grid. The cells that are peers of each other, i.e. on the same horizontal or vertical line or within the same 3x3 grid are called a *unit*.

Here's an example Sudoku puzzle:

::

    1  |42 |   
       |  6|   
       |   |   
    ---+---+---
     51| 4 | 87
      3| 5 | 61
       |3  |  5
    ---+---+---
    28 |  7|  4
       |   |5  
    6 5| 18| 2 

Now, what we're going to write is a program that, given any Sudoku puzzle, will solve it.

In order to be able to write such a program, we must first be capable of understanding the problem, i.e. understanding what our program should do. To gain this understanding it's helpful to try to solve the Sudoku puzzle manually first. Then, "all" we need to do is replicate this logic in code.

*Exercise*: Try to solve the above Sudoku puzzle. (No need to actually solve it.)

Now, when trying to solve the puzzle, you probably came up with one common strategy: try and see if a cell only has one possible value because its peers rule all other values out, and in this case, fill the cell with this value.

Another common strategy is to see if a number has been ruled out for all cells in a unit except one. In this case it must be the correct number for that cell.

If you fill a cell with a value, then this rules out that value of its peers, meaning you can again check those peers if they only have one possible value.

At some point you may have seen that it's not possible to progress using these strategies: all open cells may all have more than one possible value. What you could try then is guess: pick one of the possible values, assume it's the correct one and see if you end up with the solution for the Sudoku.

While there are several more strategies to solve a Sudoku puzzle, these two, combined with the guessing, are enough for our purposes.

Our program will have the following parts:

* Reading in a Sudoku puzzle from a file
* Checking if a cell has only one possible value, and if so, set it
* If a value was set in a cell, see if this causes some other cell to only have one possible value (also called *constraint propagation* - whereby constraints propagate to create further constraints)
* Guess, or search: assume a value is correct for a cell, then propagate the constraint and search further until a solution is found, or guess another value if the previous guess turned out to be wrong (also called *backtracking*)
* Checking we solved the puzzle, and printing out the solution

You can use any programming language but the snippets in the book necessary for writing the program will be in C++. Should you choose to use another language you'll need to find out the relevant snippets yourself. It's been demonstrated this problem can be solved in about 20 lines of Python, though I'd recommend using a statically typed language for the sole reason of gaining more experience using one. The solution for the problem in this book is about 300 lines of C++.

I'd also like to credit Peter Norvig for writing an excellent article about this problem which is the foundation for this part of the book. You may want to look it up online after completing the exercises.

Sudoku and classes
==================

As mentioned we'll be using C++ to solve this. We'll also use some object oriented concepts to see how these can be used for such a problem.

Generally speaking, it's often useful to first identify the main data structures and how they relate to each other. In this case, we can see there are following structures:

* The puzzle itself - either in solved or unsolved state
* Cells - whereby each cell can be represented by which value it has, or better, could have

On relations, we can see the puzzle contains multiple cells, namely 9x9=81.

In the spirit of object oriented programming, we can construct objects for these two structures.

In C++, the syntax for declaring a class is the following:

.. code-block:: cpp
    :linenos:

    class Counter {
        public:
            Counter(int my_value); // constructor
            int get_value() const; // getter function
            void increment();

        private:
            int value;
    };

Let's go through this line by line:

* Line 1: We use the keyword *class* to define a class. This is similar to "struct" - indeed, almost equivalent.
* Line 2: We use the keyword *public* to denote functions (and possibly variables) that can be called from code that is not part of the class. This specifies the *interface* of the class.
* Line 3: We declare a function with the same name as our class, and without a return value. This function is called the *constructor* and is called whenever an object of this class is *instantiated*, i.e. whenever a variable of this type is defined. In this case, the constructor takes one int as a parameter.
* Line 4: We define a *getter function* - a function which has the simple function of returning some variable for which direct access is not allowed from the outside. We denote this function with the keyword *const* at the end - this means that the function does not modify the object (i.e. the variable of this type).
* Line 5: We define a function that modifies the object somehow. In this case, we'd expect the function to increment the value of the internal int by one.
* Line 7: We use the keyword *private* to denote functions and variables that are part of the class but not accessible from code outside the class.
* Line 8: We declare an int as the internal data for this class.

Now, the above defines the interface for the class as well as its (hidden) data. We still need to define the member functions. This can be done as follows:

.. code-block:: cpp
    :linenos:

    Counter::Counter(int my_value)
         : value(my_value)
    {
    }

    int Counter::get_value() const
    {
        return value;
    }

    void Counter::increment()
    {
        value++;
    }

In other words:

* Line 1-4: We define the constructor. We first copy-paste the declaration and add "Counter::" in front of the function name. Second line demonstrates the syntax for initialising internal variables. The function body itself is empty.
* Line 6-9: We define the function "get_value". We again copy-paste the declaration and add "Counter::" in front of the function name. Instead of a semicolon at the end we have the function body. It's very simple in this case, returning an integer containing the value of the variable "value".
* Line 11-15: We define the function "increment" which simply increments "value" by one.

If we wanted to use this class, we could e.g. do the following:

.. code-block:: cpp
    :linenos:

    int main()
    {
        Counter a(3);
        a.increment();
        std::cout << a.get_value() << "\n";

        Counter b(12);
        std::cout << b.get_value() << "\n";
        return 0;
    }

This would print "4" and "12".

This class is fairly trivial but demonstrates most of the concepts we'll be needing. In terms of usability of this class, it effectively *encapsulates* an integer such that most integer options are not possible, for example subtraction or multiplication. It could be used e.g. as a counter for something.

*Exercise*: Add a "decrement" function to our class. Test it by using it in test code.

