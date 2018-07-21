Finding Sudoku peers
--------------------

Previously we filled out the blanks except for finding peers. Now, let's implement this. In the function, we should consider the three ways we could have a peer:

* Cells in the same horizontal line
* Cells in the same vertical line
* Cells in the same 3x3 grid

Above we used the values in the return container as an index to the "peers" member. This suggests we should return such indices. The input parameter was also an index in this array. This suggests we have the function declaration:

.. code-block:: cpp

    class Puzzle {
        public:
            ...

        private:
            std::vector<int> peers(int index) const;
            ...
    };

That is, our function returns a vector of integers, takes an integer as a parameter, and doesn't modify the data within the Puzzle object. Furthermore our function is *private* as it's not necessary to call this function from outside the class.

Now, here's one way to find the indices to the cells that are in the same vertical line:

.. code-block:: cpp
    :linenos:

    std::vector<int> Puzzle::peers(int index) const
    {
        std::vector<int> ret;

        int column = index % 9;
        for(int i = 0; i < 9; i++) {
            int peer_index = i * 9 + column;
            if(peer_index != index) {
                ret.push_back(peer_index);
            }
        }

        return ret;
    }

Let's go through this in detail.

* Line 3: Our return variable.
* Line 5: We calculate *the column* for the given index by dividing by 9 and taking the remainder. This means that e.g. indices 4, 13, 22 etc. all return the same column (4).
* Line 6: We define a loop that goes through nine elements.
* Line 7: We calculate the *peer index* by multiplying the variable "i" by 9 and adding the column. This means that e.g. with index 4 we get 4, 13, 22 etc.
* Line 9: We add the peer index in the return variable.

Now, let's add the missing logic.

*Exercise*: Add the logic for the horizontal lines and the 3x3 grids in the above function. Try it out.

.. topic:: Digression: static member functions

  You may have noticed that our function above not only does not modify the data in the Puzzle object ("cells" array), it doesn't even read it. This means it could actually be a free standing function and doesn't have to be a member function of the Puzzle class. On the other hand, it may be convenient to group functions that are relevant for certain classes together. There's a mechanism for this: *static member functions*. You can declare one by including the keyword "static" at the beginning of the declaration. You'll then need to name the class when calling it. Here's an example:

  .. literalinclude:: ../material/sudoku/static.cpp
    :language: cpp

  You mustn't use the keyword "const" to annotate the function const as it's a static function and hence won't be able to access object data anyway.

Now, let's put everything together:

* Parse the Sudoku puzzle from the previous section into a Puzzle object
* Call propagate() whenever you set a value of a cell
* Check if it's already solved using the solved() function
* Print out the puzzle after reading it

If everything works correctly, your program should be able to solve the example Sudoku puzzle just by using constraint propagation alone.

*Exercise*: Put everything together and see if you can solve the first puzzle. If not, debug.

Now we have enough to solve one easy Sudoku puzzle, but our program won't solve most puzzles, especially not the difficult ones. To test, here's one more difficult puzzle:

::

    4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......

Searching
=========

The next puzzle to solve is searching. Recall that we wanted to propagate constraints where possible but at some point we won't be able to progress any further and have to guess (search). How searching should work in principle is this:

* We pick one cell where we make a guess (namely the one with the least possible options to minimise the number of searches)
* We make a copy of the current puzzle and make a guess on this copy (by calling the "set" member function of the cell object)
* We propagate this guess
* If propagation shows we've solved the puzzle, we're done
* If propagation shows we haven't solved the puzzle but can't progress any further, we repeat this (search recursively again on this board)
* If propagation shows we've ended up with an invalid puzzle (wrong guess), we throw out this copy and guess a different number on the same cell. If we've tried all numbers then we've made a wrong guess at some point before and return from our (recursive) function call.

What we have here is *depth-first search*. It's called depth first because we follow one "path" until we either find the solution, in which case we return this, or a dead end, in which case we try the next path. It's similar to trying to find the exit from a maze by always following the wall on one side; you may have to traverse the maze quite a lot but you'll find the exit eventually (if it exists). This is in contrast to *breadth-width search* which, instead of following one path down, visits each neighbouring node first before progressing further down the graph.

Generally speaking we can agree on the following structure:

* The Puzzle class has a member function "search", which, when called, will return a Puzzle class which is expected to be the solved puzzle
* The "search" member function does the following:

  * Check if this puzzle is solved; if so then return self
  * Check if any of the cells in this puzzle are invalid, i.e. don't have any valid options; if so then return self
  * Find the cell with the smallest number of valid options (but still more than 1)
  * For all the options in the selected cell, make a copy of the puzzle, set a value in the copy, search in the copy; if solution found then return it, else return self

Now, in terms of syntax, there are a couple of useful things to know.

Working with "this"
===================

In C++, "this" is a keyword which means a pointer to the current object. In our case, it allows us to make a copy of our object. Here's an example use:

.. code-block:: cpp
    :linenos:

    Puzzle Puzzle::search()
    {
        if(solved()) {
            return *this;
        } else {
            int cell_index_to_guess = /* ... */
            auto possible_guesses = cells.at(cell_index_to_guess).values();
            for(auto guess : possible_guesses) {
                Puzzle alt = *this;
                alt.set_cell(cell_index_to_guess, guess);
                bool valid = alt.propagate();
                if(valid) {
                    alt = alt.search();
                    if(alt.solved()) {
                        return alt;
                    }
                }
            }
        }
        return *this;
    }

Here, we define a function named "search" which is a member function of the class Puzzle. It has Puzzle as its return value. We check if this Puzzle is solved, and if so, we return the *dereferenced pointer* to "this", meaning a copy of the Puzzle. If this Puzzle is not solved we call "search" recursively. (Because we set a cell value each time we search, we either find a solution or a dead end.)

*Exercise*: Finish the above function. Call it from your program. See if you can solve the more difficult Sudoku puzzle.

Solving more Sudokus
====================

Here are 30 easy Sudoku puzzles, courtesy project Euler:

::

    ..3.2.6..9..3.5..1..18.64....81.29..7.......8..67.82....26.95..8..2.3..9..5.1.3..
    2...8.3...6..7..84.3.5..2.9...1.54.8.........4.27.6...3.1..7.4.72..4..6...4.1...3
    ......9.7...42.18....7.5.261..9.4....5.....4....5.7..992.1.8....34.59...5.7......
    .3..5..4...8.1.5..46.....12.7.5.2.8....6.3....4.1.9.3.25.....98..1.2.6...8..6..2.
    .2.81.74.7....31...9...28.5..9.4..874..2.8..316..3.2..3.27...6...56....8.76.51.9.
    1..92....524.1...........7..5...81.2.........4.27...9..6...........3.945....71..6
    .43.8.25.6.............1.949....4.7....6.8....1.2....382.5.............5.34.9.71.
    48...69.2..2..8..19..37..6.84..1.2....37.41....1.6..49.2..85..77..9..6..6.92...18
    ...9....2.5.1234...3....16.9.8.......7.....9.......2.5.91....5...7439.2.4....7...
    ..19....39..7..16..3...5..7.5......9..43.26..2......7.6..1...3..42..7..65....68..
    ...1254....84.....42.8......3.....95.6.9.2.1.51.....6......3.49.....72....1298...
    .6234.75.1....56..57.....4.....948..4.......6..583.....3.....91..64....7.59.8326.
    3..........5..9...2..5.4....2....7..16.....587.431.6.....89.1......67.8......5437
    63..........5....8..5674.......2......34.1.2.......345.....7..4.8.3..9.29471...8.
    ....2..4...8.35.......7.6.2.31.4697.2...........5.12.3.49...73........1.8....4...
    361.259...8.96..1.4......57..8...471...6.3...259...8..74......5.2..18.6...547.329
    .5.8.7.2.6...1..9.7.254...6.7..2.3.15.4...9.81.3.8..7.9...762.5.6..9...3.8.1.3.4.
    .8...5........3457....7.8.9.6.4..9.3..7.1.5..4.8..7.2.9.1.2....8423........1...8.
    ..35.29......4....1.6...3.59..251..8.7.4.8.3.8..763..13.8...1.4....2......51.48..
    ...........98.51...519.742.29.4.1.65.........14.5.8.93.267.958...51.36...........
    .2..3..9....9.7...9..2.8..5..48.65..6.7...2.8..31.29..8..6.5..7...3.9....3..2..5.
    ..5.....6.7...9.2....5..1.78.415.......8.3.......928.59.7..6....3.4...1.2.....6..
    .4.....5...19436....9...3..6...5...21.3...5.68...2...7..5...2....24367...3.....4.
    ..4..........3...239.7...8.4....9..12.98.13.76..2....8.1...8.539...4..........8..
    36..2..89...361............8.3...6.24..6.3..76.7...1.8............418...97..3..14
    5..4...6...9...8..64..2.........1..82.8...5.17..5.........9..84..3...6...6...3..2
    ..72564..4.......5.1..3..6....5.8.....8.6.2.....1.7....3..7..9.2.......4..63127..
    ..........79.5.18.8.......7..73.68..45.7.8.96..35.27..7.......5.16.3.42..........
    .3.....8...9...5....75.92..7..1.5..8.2..9..3.9..4.2..1..42.71....2...8...7.....9.
    2..17.6.3.5....1.......6.79....4.7.....8.1.....9.5....31.4.......5....6.9.6.37..2

*Exercise*: Save the above to a file. Rewrite your program to read each of these, and solve them all one after another. See how long it takes. (On Unix, you can time your program execution by prefixing the command with "time", e.g. "time ls".)

Here are 30 difficult Sudoku puzzles, courtesy http://magictour.free.fr/top95:

::

    4.....8.5.3..........7......2.....6.....8.4......1.......6.3.7.5..2.....1.4......
    52...6.........7.13...........4..8..6......5...........418.........3..2...87.....
    6.....8.3.4.7.................5.4.7.3..2.....1.6.......2.....5.....8.6......1....
    48.3............71.2.......7.5....6....2..8.............1.76...3.....4......5....
    ....14....3....2...7..........9...3.6.1.............8.2.....1.4....5.6.....7.8...
    ......52..8.4......3...9...5.1...6..2..7........3.....6...1..........7.4.......3.
    6.2.5.........3.4..........43...8....1....2........7..5..27...........81...6.....
    .524.........7.1..............8.2...3.....6...9.5.....1.6.3...........897........
    6.2.5.........4.3..........43...8....1....2........7..5..27...........81...6.....
    .923.........8.1...........1.7.4...........658.........6.5.2...4.....7.....9.....
    6..3.2....5.....1..........7.26............543.........8.15........4.2........7..
    .6.5.1.9.1...9..539....7....4.8...7.......5.8.817.5.3.....5.2............76..8...
    ..5...987.4..5...1..7......2...48....9.1.....6..2.....3..6..2.......9.7.......5..
    3.6.7...........518.........1.4.5...7.....6.....2......2.....4.....8.3.....5.....
    1.....3.8.7.4..............2.3.1...........958.........5.6...7.....8.2...4.......
    6..3.2....4.....1..........7.26............543.........8.15........4.2........7..
    ....3..9....2....1.5.9..............1.2.8.4.6.8.5...2..75......4.1..6..3.....4.6.
    45.....3....8.1....9...........5..9.2..7.....8.........1..4..........7.2...6..8..
    .237....68...6.59.9.....7......4.97.3.7.96..2.........5..47.........2....8.......
    ..84...3....3.....9....157479...8........7..514.....2...9.6...2.5....4......9..56
    .98.1....2......6.............3.2.5..84.........6.........4.8.93..5...........1..
    ..247..58..............1.4.....2...9528.9.4....9...1.........3.3....75..685..2...
    4.....8.5.3..........7......2.....6.....5.4......1.......6.3.7.5..2.....1.9......
    .2.3......63.....58.......15....9.3....7........1....8.879..26......6.7...6..7..4
    1.....7.9.4...72..8.........7..1..6.3.......5.6..4..2.........8..53...7.7.2....46
    4.....3.....8.2......7........1...8734.......6........5...6........1.4...82......
    .......71.2.8........4.3...7...6..5....2..3..9........6...7.....8....4......5....
    6..3.2....4.....8..........7.26............543.........8.15........8.2........7..
    .47.8...1............6..7..6....357......5....1..6....28..4.....9.1...4.....2.69.
    ......8.17..2........5.6......7...5..1....3...8.......5......2..4..8....6...3....

*Exercise*: Solve these puzzles as well. If you get tired of waiting, remember to compile your program with optimisations, that is, with the compiler command line flag "-O2" which can speed up C++ programs significantly.

Constraint propagation with units
=================================

We've missed one part of our plan: in the introduction we said that "another common strategy is to see if a number has been ruled out for all cells in a unit except one. In this case it must be the correct number for that cell."

This means that if e.g. we have a row where a cell could have values 2, 3 and 7, but the number 7 was eliminated in all peers, then we can assign the number 7 for this cell.

*Exercise*: Implement this strategy as part of the propagation function. Note that you may find it necessary to use recursion. See if this strategy speeds up your program execution. You may want to reuse parts of your function to identify peers of a cell, and rewrite it to suit you better.

Now we have a program that can solve all Sudoku puzzles fairly quickly.

*Exercise*: Look up Peter Norvig's essay on solving Sudoku puzzles online, which served as inspiration for this part of the book.
