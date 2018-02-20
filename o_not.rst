Big O notation
--------------

When discussing data structures, it's handy to understand what big O notation is.

To put it short (possibly oversimplified), the big O notation is a notation to express the run time of an algorithm or a function in terms of the input size.

Maybe it's easiest to go through some examples.

Some operations on arrays
=========================

Let's assume we have a C array with five integers in it.

.. code-block:: c

    int my_array[5];
    /* define values in the array */

If you wanted to access, say, the third element of the array, conceptually this can be done in *constant time* - meaning that the time it takes to read the value of the variable is independent of the index:

.. code-block:: c

    int val = my_array[2]; /* whether we access the third or the fourth element, the run time is the same */

Now, let's assume we want to check whether the number 42 is included in the array:

.. code-block:: c

    int find_in_array(const int *arr, int num_elems, int num_to_find)
    {
        for(int i = 0; i < num_elems; i++) {
            if(arr[i] == num_to_find) {
                return 1;
            }
        }
        return 0;
    }

    int main(void)
    {
        int my_array[5] = { 3, 7, 5, 1, 8 };
        int found = find_in_array(my_array, 5, 42);
        if(found) {
            printf("Found it!\n");
        } else {
            printf("Not found\n");
        }
    }

Here, we have a for loop that goes through the whole array (in the worst case). If our array had 10 elements, it would need twice the time (conceptually speaking). Hence, the run time is *linear* to the size of the array.

In terms of big O notation, constant time is annotated O(1). Linear time, i.e. run time depends directly on the size of the array (n) is annotated O(n).

Furthermore, let's assume we want to sort the array. We can invent a simple sorting algorithm for this:

.. code-block:: c
    :linenos:

    void sort_array(int *arr, int num_elems)
    {
        for(int j = 0; j < num_elems; j++) {
            int smallest_index = j;
            for(int i = j + 1; i < num_elems; i++) {
                if(arr[i] < arr[smallest_index]) {
                    smallest_index = i;
                }
            }
            int tmp = arr[j];
            arr[j] = arr[smallest_index];
            arr[smallest_index] = tmp;
        }
    }

    int main(void)
    {
        int my_array[5] = { 3, 7, 5, 1, 8 };
        sort_array(my_array, 5);
    }

This got a bit interesting so let's go through this line by line.

* Line 1: We define the *outer* for loop that goes through all elements, indexed with j.
* Line 2: We define a variable to store the index of the smallest number so far, initialised to j.
* Line 3: We define the *inner* for loop that goes through all elements from j + 1 until the end.
* Line 4-6: If the value at position i (which is bound to be "to the right" of j, if we imagine the array to be a row of boxes extending to the right) is smaller than the value at position smallest_index (which was initialised to j) then we note this index as the index with the smallest number in the rest of the array.
* Line 8-10: We finally *swap* the value at j with the smallest value we found. We swap by copying the value at j to a temporary variable, then putting the smallest value to j'th element, and finally putting the temporary value to where the smallest value was.

Here's an "animation" of how this algorithm would work, for an array of size 5:

.. code-block:: none

    | 3 | 7 | 5 | 1 | 8 | # initially unsorted - j = 0 (points to 3); i = 1 (points to 7) - smallest index is 0
    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 2 (points to 5) - smallest index is 0
    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 3 (points to 1) => smallest index is set to 3
    | 3 | 7 | 5 | 1 | 8 | # j = 0; i = 4 - inner loop finished. We swap 0th element (3) with the 3rd element (1).
    | 1 | 7 | 5 | 3 | 8 | # j = 1; i = 2 (points to 5) => smallest index is set to 2
    | 1 | 7 | 5 | 3 | 8 | # j = 1; i = 3 (points to 3) => smallest index is set to 3
    | 1 | 7 | 5 | 3 | 8 | # j = 1; i = 4 - inner loop finished. We swap the 1st element (7) with the 3rd element (3).
    | 1 | 3 | 5 | 7 | 8 | # j = 2; i = 3 (points to 7) - smallest index is 2
    | 1 | 3 | 5 | 7 | 8 | # j = 2; i = 4 - inner loop finished. We swap the 2nd element with itself (no change).
    | 1 | 3 | 5 | 7 | 8 | # j = 3; i = 4 - inner loop finished. No change.
    | 1 | 3 | 5 | 7 | 8 | # j = 4 - inner loop not started because i would start at 5 - sort is done.

This sorting algorithm is called *selection sort* and it's one of the simpler ones.

What's the run time of this algorithm? We saw it has two for loops, and the number of iterations of both is only dependent (and linear to) the array size. Hence, the run time is *quadratic* to n - in terms of big O notation, O(n\ :sup:`2`\ ).
