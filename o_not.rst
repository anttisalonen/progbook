Big O notation
--------------

An algorithm is, succinctly put, a piece of code (or logic) which solves a class of problems.

When discussing algorithms, it's handy to understand what big O notation is.

To put it short (possibly oversimplified), the big O notation is a notation to express the run time of an algorithm or a function in terms of the input size.

Maybe it's easiest to go through some examples.

Constant time
=============

Let's assume we have a C array with five integers in it.

.. code-block:: c

    int my_array[5];
    /* define values in the array */

If you wanted to access, say, the third element of the array, conceptually this can be done in *constant time* - meaning that the time it takes to read the value of the variable is independent of the index:

.. code-block:: c

    int val = my_array[2]; /* whether we access the third or the fourth element, the run time is the same */

Linear time
===========

Now, let's assume we want to check whether the number 42 is included in the array:

.. literalinclude:: linear_search.c
   :language: c

Here, we have a for loop that goes through the whole array (in the worst case). If our array had 10 elements, it would need twice the time (conceptually speaking). Hence, the run time is *linear* to the size of the array.

.. topic:: Decay to pointer

  You had an array, then you passed it to a function, and the function parameter is a pointer! What's going on?

  In C, while array in general is not the same thing as a pointer, the two are sometimes interexchangable. In C, arrays are always passed by reference to a function, which means that the array is not automatically copied for the function (like a single int would be), but instead *a pointer* is passed to the function, whereby the pointer points to the first element in the array. Hence the array will *decay* into a pointer.

.. topic:: What do you mean "conceptually?"

  You may remember that the CPU has a few registers, and the computer (or mobile phone, or a car, or whatever) also has RAM for storing more data. When you declare an array of, say, five elements, the array is allocated in RAM, and whenever the CPU wants to do anything with any of the values in the array (e.g. compare the value of an element with another value like in our search), it has to fetch the value from the memory. What happens in practice for such a small array is that all of it will be fetched in a *cache* - an extra small memory which is faster for the CPU to access than RAM (but not as fast as registers). The cache is part of a *cache hierarchy* - register, level 1 (L1) cache, L2 cache, L3 cache and finally RAM.

  Using data in a register or fetching data from the L1 cache typically takes one CPU cycle. Fetching data from the L2 cache can take perhaps 10-15 cycles. Fetching data from RAM could take perhaps 200 cycles, or 200 times as long as fetching data from the L1 cache. If fetching data from the L1 cache is like getting a glass of water from the kitchen (one minute), fetching data from RAM is like going shopping for groceries and then cooking and eating dinner (three hours and 20 minutes).

  Because it's silly to go shopping and cooking every time you need to eat or drink something, if the computer needs to go to RAM, it typically fetches a bunch of stuff from there that it thinks it needs later on, like the next few bytes beyond the one address it needs. Hence, if you define an array of five elements, and later look up the first one but the data isn't cached but in RAM, the computer will bring the whole array to the L2 (and L1) cache. However, if you have an array with a million elements (if we assume 32 bits or 4 bytes per integer, that would be 4 megabytes) and randomly access elements in the array then you might need to go to RAM several times, or not, depending on your access pattern. However, *conceptually* the accesses are still in linear time.

In terms of big O notation, constant time is annotated O(1). Linear time, i.e. run time depends directly on the size of the array (n) is annotated O(n).

Quadratic run time
==================

Furthermore, let's assume we want to sort the array. We can invent a simple sorting algorithm for this:

.. literalinclude:: sort.c
    :language: c
    :linenos:

This got a bit interesting so let's go through this line by line.

* Line 5: We define the *outer* for loop that goes through all elements, indexed with j.
* Line 6: We define a variable to store the index of the smallest number so far, initialised to j.
* Line 7: We define the *inner* for loop that goes through all elements from j + 1 until the end.
* Line 8-10: If the value at position i (which is bound to be "to the right" of j, if we imagine the array to be a row of boxes extending to the right) is smaller than the value at position smallest_index (which was initialised to j) then we note this index as the index with the smallest number in the rest of the array.
* Line 12-14: We finally *swap* the value at j with the smallest value we found. We swap by copying the value at j to a temporary variable, then putting the smallest value to j'th element, and finally putting the temporary value to where the smallest value was.

Here's an "animation" of how this algorithm would work, for an array of size 5:

.. code-block:: none

    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 1 (points to 7) - smallest index is 0
    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 2 (points to 5) - smallest index is 0
    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 3 (points to 1) => smallest index is set to 3 (1 < 3)
    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 4 - inner loop finished. We swap 0th element (3) with the 3rd element (1).
    | 1 | 7 | 5 | 3 | 8 | # j = 1; i = 2 (points to 5) => smallest index is set to 2 (5 < 7)
    | 1 | 7 | 5 | 3 | 8 | # j = 1; i = 3 (points to 3) => smallest index is set to 3 (3 < 5)
    | 1 | 7 | 5 | 3 | 8 | # j = 1; i = 4 - inner loop finished. We swap the 1st element (7) with the 3rd element (3).
    | 1 | 3 | 5 | 7 | 8 | # j = 2; i = 3 (points to 7) - smallest index is 2
    | 1 | 3 | 5 | 7 | 8 | # j = 2; i = 4 - inner loop finished. We swap the 2nd element with itself (no change).
    | 1 | 3 | 5 | 7 | 8 | # j = 3; i = 4 - inner loop finished. No change.
    | 1 | 3 | 5 | 7 | 8 | # j = 4 - inner loop not started because i would start at 5 - sort is done.

This sorting algorithm is called *selection sort* and it's one of the simpler ones.

What's the run time of this algorithm? We saw it has two for loops, and the number of iterations of both is only dependent (and linear to) the array size. Hence, the run time is *quadratic* to n - in terms of big O notation, O(n\ :sup:`2`\ ). For every new element in the array we'll have to run the inner loop *and* the outer loop once more. If n = 10, we have 9 * 8 = 72 iterations. If n = 100, we have 99 * 98 = 9702 iterations.

Logarithmic run time
====================

With a quadratic run time, the run time grows exponentially as the number of elements grows, which is pretty bad. The opposite of exponential is *logarithmic*, where the run time grows relatively slowly: in order to double the run time, you need to grow the number of elements exponentially. This is annotated O(log n). An example is searching for a person in a phone book: you could pick a page in the middle, see if the person is alphabetically before or after the page you picked, and pick a page in the middle of either the section before or after your page - with each lookup, you halve the number of pages you need to look at.

O(n log n)
==========

The final common run time class is O(n log n) - logarithmic run time multiplied with a linear run time. This means the run time is generally dominated by the linear part as the logarithmic run time grows relatively slowly. O(n log n) is also been proven to be the fastest possible run time class for a generic sorting algorithm.

.. topic:: Fast sorting algorithms

  I won't go through the implementation of a fast sorting algorithm here. Interested readers can look them up online. Suffice to say, there are two popular fast sorting algorithms: *quicksort* and *mergesort*.
  
  Quicksort works by picking one element at random, moving smaller elements than the selected one to its left and larger ones to its right and recursively sorting the elements on the left and on the right: for all the elements on the left, you pick one element at random, then move smaller elements than the selected one to its left and larger ones to its right and recursively sort the elements on the left and on the right. At some point you only have one or zero elements to consider and have thus sorted that part of the array. While quicksort has O(n\ :sup:`2`\ ) run time in the worst case, it's O(n log n) in the typical case.
  
  Merge sort divides the original array into sub-arrays of one element each (hence trivially sorted), and then works backwards by merging each sub-array, sorting the resulting array in the process. Merge sort has O(n log n) run time.

  Typically it's rare for a programmer to have to implement a sort algorithm, especially a fast one. All main programming languages (even C) provide sorting as part of the standard library. For example, Python:

  .. code-block:: python

    >>> my_array = [3, 7, 5, 1, 8]
    >>> my_array.sort()
    >>> my_array
    [1, 3, 5, 7, 8]

  (Python uses "Timsort" as its sorting algorithm, which is merge sort when n is large and insertion sort when n is small.) In C, one would do:

  .. literalinclude:: qsort.c
     :language: c

  For qsort() in C, one has to define a *callback function* - a function that qsort() calls to get information on what the ordering between the elements should be, as well as the *size of* each element in the array. We'll touch more on these constructs later.

