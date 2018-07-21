Callbacks and anonymous functions
---------------------------------

This section explains some concepts that will be needed later.

Functions as first order values
===============================

The concept of functions as first order values means that variables can have functions as values like any other type. Code speaks more than a thousand words so let's take a look at an example:

.. code-block:: python

    my_list = [3, 4, 5]
    my_functions = [min, max, sum]

    for function in my_functions:
        print function(my_list)

This snippet defines two lists. The first one looks typical but the second has three functions in it: "min", "max" and "sum". All of these functions can take a list of numbers as input; they'll return the smallest, largest and sum of the list respectively.

*Exercise*: What do you think this code will print? Try it out.

Callback
========

A callback, put shortly, is a function that will be called back. It follows the Hollywood principle - "don't call us, we will call you". Let's take another look at our at our example of sorting a list in Python.

Sorting in Python
~~~~~~~~~~~~~~~~~

Recall that we can read in our data of 10,000 functions to a list of lists, such that we have a list with 10,000 entries, each a list of three elements, e.g. [[6.44, -6.80, 5.87], ...]. We can sort this list to get the first three elements like this:

.. code-block:: python

    print sorted(data)[:3]

However, what if we wanted to sort by the third element in the inner lists, i.e. by the value "c" in the functions? The function "sorted" supports this by allowing us to supply a *callback* function which will be called for each element, such that the return value of the function defines the ordering of the resulting list. Let's show the code:

.. code-block:: python

    def my_callback(values):
        # values is a function, i.e. list with three elements
        return values[2]

    print sorted(data, key=my_callback)[:3]

What happens here is that we define a function called "my_callback" which returns the third value in a list. That's a fairly boring function, but we provide this function as a *callback* to the sorted() function, namely as the "key" parameter. As sorted() calls the key function for each element and uses that to define the ordering, we end up with the resulting list being sorted by the third value in the functions, i.e. the value "c".

Sorting in C
~~~~~~~~~~~~

In the section "Big O notation" we touched upon sorting in C using the built in "qsort" function. Here's the code again:

  .. literalinclude:: ../material/o_not/qsort.c
     :language: c
     :linenos:

Now, what happens here is the following:

* Line 4: We define a function which we use as our callback. It takes two parameters (const void pointers, i.e. pointers to any unknown data) and returns an int, whereby the two input parameters are the values to compare and the return value should describe how to sort them, such that negative value means the first parameter should be before the second.
* Line 6: We cast the void pointers to int pointers (as we know our data is ints) and subtract a from b such that if a is less than b, it will be before b in the result.
* Line 12: We call the qsort() standard library function which takes a *pointer to a function* as the fourth parameter. Pointer to a function is how a callback function is defined and used in C.

Callbacks in Javascript
~~~~~~~~~~~~~~~~~~~~~~~

We already had an example when we defined a function to call at the page load phase:

.. code-block:: js

    function init() {
        /* my code goes here */
    }
    window.onload = init;

Here, our function "init" is defined as the callback function for the object "window".

Anonymous functions
===================

In some languages including Python and Javascript, we can define functions *anonymously*, meaning we don't have to really define a function with a name, but merely a function without one. Going back to our Python example about providing a callback function as the "key" parameter, we wrote a function to return the third element of a list but this function is actually very simple. We can shorten our code by doing the following:

.. code-block:: python

    print sorted(data, key=lambda l: l[2])[:3]

In Python, *lambda* is a key word for defining an anonymous function: instead of using "key=my_callback", we set "key" to a lambda function, which is otherwise defined like a regular function but we skip the "return" keyword. This will have the same behaviour as our earlier example where we defined a named function as the callback.

