Sudoku Puzzle class
-------------------

Now, we can start implementing our Puzzle class. From data point of view, this is nothing else but an array of cells. Let's declare this:

.. code-block:: cpp

    class Puzzle {
        public:
            Puzzle();
            bool solved() const;

        private:
            std::array<Cell, 81> cells;
    };

*Exercise*: Implement the constructor and the "solved" function. The constructor doesn't need any contents as the array elements will be constructed using the default constructor for Cell, which as per the previous exercise initialises the cell such that all values are possible. The "solved" function will need to check whether all cells have only one number, and only return true in this case. (Initially, it should hence return false.)
Now, we can look into actually constructing an object of class Puzzle by reading a Sudoku puzzle from a file.

As per Norvig, let's define the file format for a Sudoku puzzle such that a number in a file defines the value for a cell, while either a dot (.) or a zero (0) denote an unknown cell, and all characters can be ignored. This means that e.g. the following line is a valid puzzle (again courtesy Norvig, who sourced the puzzle from Project Euler):

::

    ..3.2.6..9..3.5..1..18.64....81.29..7.......8..67.82....26.95..8..2.3..9..5.1.3..

The same puzzle could be represented like this:

::

    ..3 .2. 6..
    9.. 3.5 ..1
    ..1 8.6 4..
    
    ..8 1.2 9..
    7.. ... ..8
    ..6 7.8 2..
     
    ..2 6.9 5..
    8.. 2.3 ..9
    ..5 .1. 3..

Now, let's write the code to read this. In C++, one can read a file to a string with the following snippet (this uses the first parameter given to the program):

.. code-block:: cpp

    #include <iostream>
    #include <fstream>
    #include <string>

    int main(int argc, char** argv)
    {
        std::ifstream ifs(argv[1]);
        std::string contents((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));
        return 0;
    }

Now that we can read file into a string, let's turn this into a puzzle. First, let's define the interface:

.. code-block:: cpp

    int main()
    {
         std::string s = /* ... */;
         std::vector<int> values = parse_string(s);
         Puzzle b(values);
         return 0;
    }

In other words, we need the Puzzle class constructor to receive an std::vector<int> as input. Each element in the vector should either be a number between 1 or 9, denoting a pre-filled cell, or something else (like 0) to denote a clear cell. We should also have a function "parse_string" to turn the string to an int vector.

As a skeleton, let's put something together that also introduces character-to-integer conversions in C++, exceptions and reference parameters.

.. code-block:: cpp
    :linenos:

    std::vector<int> parse_string(const std::string& s) {
        int buf_location = 0;
        std::vector<int> my_buf;
        for(auto c : contents) {
            if(c >= '1' && c <= '9') {
                my_buf.push_back(c - '0');
            } else if(c == '.' || c == '0') {
                /* TODO */
            }
            if(buf_location == 81) {
                /* TODO */
            }
        }
        throw std::runtime_error("Unable to parse");
    }

Let's go through this line by line:

* Line 1: We define the function such that it takes a parameter "const std::string& s". A simpler version to write effectively the same thing would be "string s", however the latter would cause the input string to be copied for the function. With the former version we receive a reference to a constant string instead, saving an unnecessary data copy.
* Line 2: We define a variable to keep track of the current cell we might fill.
* Line 3: We define our return variable.
* Line 4: We iterate over all characters in the string.
* Line 5: We check if the character value is between '1' and '9'. As the character values are based on the ASCII table, meaning they can be treated as numeric values, we can use arithmetic operators (>= and <=) to compare them.
* Line 6: We subtract the ASCII value '0' from "c". This results in the integer value 0 for the character '0', 1 for character '1', 2 for character '2' etc.
* Line 7: We check whether the character is a dot or a zero.
* Line 10: We check whether we've found a symbol for each cell in the puzzle.
* Line 14: If we didn't find enough symbols of interest, then we cannot fill our array of cells any further and *throw an exception*: this effectively ends the current execution of the code and goes up the function call stack until a caller is found who *catches* the exception thrown. As we have no code to catch the exception, this effectively prints the error message on the screen and terminates the program.

*Exercise*: Implement the above function. Fill out the correct code for the TODOs such that the return variable is updated correctly and returned. See if you can run it without an exception being thrown with the example input from above.

Now that we're able to parse a string to an int vector, let's turn this int vector to a Puzzle.

*Exercise*: Rewrite the constructor of the Puzzle class to take an int vector as a parameter, and loop through it to set the contents of the "cells" member variable. Use the "set" member function of the Cell class to set the values.

Displaying puzzles
==================

We're now able to read in a Sudoku puzzle but have no visibility over the contents of the Puzzle class. To remedy this, let's write a function to display the puzzle. Here's the declaration:

.. code-block:: cpp

    class Puzzle {
        public:
            ...
            void print() const;
            ...
    };

That is, a public member function which doesn't modify the object.

Now, we can implement this function by looping through the array of cells, and for each cell, print out something. What we print should have the same number of characters for each cell for proper formatting. The simplest way to do this is to either print out a number if a cell has one, or a placeholder (e.g. a space or a dot) otherwise.

*Exercise*: Implement this function and test it.

We can now start thinking about the meat of our program: actually implementing the first strategy of constraint propagation. Recall that what we want to do is:

1) For each cell that has only a single value set, eliminate that value from all its peers
2) Since eliminating a value from a cell can cause it to only have a single value set, if this is the case, we should eliminate that value from all its peers

In order to implement this, what we need is:

1) A function to eliminate a value from a cell
2) Identifying what the peers are for a cell
3) A function that calls the above functions, i.e. checks, for all cells, which values can be eliminated

We already implemented 1) when we implemented the Cell class.

Recall that the peers of a cell are the cells that are on either the same horizontal line, on the same vertical line, or in the same 3x3 sub-grid, i.e. in the same *unit* as the cell.

When implementing a function for 2), the question arises on what data types should the function use as input and output. It's often easier to answer this if we consider the user of this function, i.e point 3).

Let's put this function together in C++:

.. code-block:: cpp
    :linenos:

    bool Puzzle::propagate(int i)
    {
        bool has_only_one = cells[i].has_one();
        if(has_only_one) {
             auto value = cells[i].get_one();
             auto peers = peers(i);
             for(const auto& peer : peers) {
                 if(cells[peer].has(value)) {
                     bool still_valid = cells[peer].eliminate(value);
                     if(!still_valid)
                         return false;
                     still_valid = propagate(peer);
                     if(!still_valid)
                         return false;
                 }
             }
        }
        return true;
    }

The summary of this function is that we go through each cell, and if it only has one value, we eliminate the value from the peers if they had it. If a peer ends up only having one value, we repeat for that cell. If we invalidate the puzzle with this (which shouldn't happen), we stop.

Now, a couple of notes:

* Line 3: we previously introduced the Cell class member function "has_one". This is used here.
* Line 5: We need an additional function to get the single value (although "get_values" could be used as well).
* Line 6: We call the function "peers" which should be a member function of the Puzzle class. This isn't yet defined. It takes an integer as input (index of a cell in the array "cells") and returns an array or a vector of indices which are the indices of the peer cells.
* Line 7: We iterate through all the peers.
* Line 8: We can access the peers in the "cells" array by index "peer", and call the Cell class member function "has" which should return true if the value is possible for the cell. The function is yet to be defined.
* Line 10: We call the function we're defining for the peer; recursion. What happens is another function call is pushed to the stack, such that we enter the function "propagate" again but with the variable "i" being set to "peer" for this second call. Once that function call returns then we end up at line 10 again, with "i" at the original value, continuing the original for loop to process the rest of the peers. With recursion, it's important to have a *base case*, i.e. a case where the recursive call will not be made, to avoid infinite loop. Our base cases are either no cells having only one value, or all peers already having a single value.

What we have here, after filling out the blanks, is the propagation function, which eliminates numbers from peer cells, and also propagates this if the peer cell ends up with only one value.

*Exercise*: Implement the "get_one" and "has" member functions for the Cell class.

*Exercise*: What happens when the user of the Cell class calls "get_one" but the cell has more than one value? What is the cell has no values at all (all numbers eliminated)? What would you expect to happen? Is it possible to improve the has_one/get_one interface to avoid invalid use?

