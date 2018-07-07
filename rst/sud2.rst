Containers for Sudoku
---------------------

Recall that our main idea to solve Sudoku (constraint propagation) revolves around the idea of checking how many possible values a cell has, and if it has only one possible value, set it.

We then discussed the concept of a cell - which may either have a value, or at least we should know which values it could and could not have.

After our discussion around classes we now almost have enough to implement a class "Cell", which represents a cell and the value it has or could have. Before we implement this, let's consider for a moment how we could achieve this concept of potential values.

If we know which value a cell has, it's no different from an integer. However, if it can have multiple values, then it can effectively be represented a set of multiple integers.

*Exercise*: What ways or data structures can you think of to store this information?

We've talked about arrays before, but arrays generally have a fixed size. We could, however, e.g. have an array of nine integers, each integer having a value between 1 and 9, or -1 if one or more numbers have been eliminated.

Alternatively we could use a dynamically sized array, and only store the possible values there. If only one value is stored, then we know the value in the cell.

Furthermore, we could use a *set*, i.e. a *binary search tree* which is similar to vector in the sense that it stores values, but with the feature of having a faster time complexity for e.g. lookup functions than a vector (O(log n) vs. O(n)).

The following quickly demonstrates the three approaches in C++, with the example of attempting to find a number 3 in an array as well as checking the size:

.. literalinclude:: ../material/sudoku/arr.cpp
   :language: cpp
   :linenos:

Vector and set also allow initialisation using the {} notation like arrays, but the above demonstrates the functions to add values in them explicitly.

Most of this is possibly rather self explanatory, but it seems two pieces of code are of specific interest:

* Erasing an element in a vector; this is the so-called *erase-remove* idiom which is the (admittedly clunky) way in C++ to remove an element from a vector. It uses std::remove(), a generic function in the C++ standard template library (STL) which takes *iterators* as parameters, which are provided by the standard containers in C++ for iterating over the containers and conceptually are pointers pointing to an element. This is passed to vec.erase() which will then remove the element from the vector. E.g. the first element in a vector or a set can be accessed using "\*vec.begin()".
* In the for loops, we use the construct "const auto &v". This means "v should be a reference to a constant variable of automatically inferred type". What referencing ("&") means in practice is that we don't copy the data, possibly saving some CPU cycles. What "const" means is that we promise not to modify the data within the container. Overall this means we simply want to read the contents. If we wanted to, say, modify the element then we should remove "const". If we wanted to construct a copy of the element (e.g. for modifying it but without modifying the element in the container) then we should remove the "&".

Cell class
==========

Now, we should have everything we need for a Cell class. It should have the following functionality:

* On construction, it should store the numbers from 1 to 9.
* It should be possible to *set* it, i.e. remove all values except one (which is given as a parameter).
* It should be possible to check whether a cell is valid, i.e. whether it has at least one value still available.
* It should be possible to *eliminate* a value, i.e. remove a value if it exists. For convenience, let's have the function "eliminate" return false if the last number in the cell was eliminated and true otherwise.
* It should be possible to ask whether it contains only one number, or multiple.
* It should be possible to retrieve the current values.

More specifically, it should correctly implement an interface as used by this program:

.. code-block:: cpp

    #include <iostream>

    /* Cell class declaration and definition goes here */

    int main()
    {
        Cell c1;
        std::cout << c1.count() << "\n";   // should output "9"
        bool ret = c1.eliminate(7);
        std::cout << c1.count() << "\n";   // should output "8"
        std::cout << ret << "\n";          // should output "1" (true)
        c1.set(3);
        std::cout << c1.count() << "\n";   // should output "1"
        std::cout << c1.valid() << "\n";   // should output "1" (true)
        std::cout << c1.has_one() << "\n"; // should output "1" (true)
        auto values = c1.get_values();     // return type is implementation specific (e.g. either vector<int> or set<int>)
        for(const auto& v : values) {
            std::cout << v << "\n";        // should output "3"
        }
        return 0;
    }

.. topic:: Bounds checking

  Similar to assertion, and relevant in C++, is *bounds checking*. Normally, when you access an element in an array (with e.g. "a[3]"), there's nothing stopping you from trying to access an element beyond the size of the array, leading to undefined behaviour. At a small (typically negligible these days) runtime cost you can, for the classes std::vector and std::array, have the bounds checked at each access by using e.g. "a.at(3)" instead. Now code will be called that will check whether the element is within the bounds and throw an exception if not, hopefully saving lots of debugging time in the future.

*Exercise*: Implement the Cell class. You can use any data structure you like although either vector or set should be the easiest ones. Test it with the test code above.

We may need some more member functions for our Cell class later, but we can implement these as we go along.


