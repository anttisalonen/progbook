More useful Python constructs
-----------------------------

To demonstrate some of these constructs, let's start by loading some data to work on. As before, let's work on the 10,000 functions we defined before, and load this data from JSON.

.. code-block:: python

    data = json.load(open('/dev/stdin', 'r'))

We read the JSON data from stdin, meaning that we can define the input file on the command line like this:

.. code-block:: bash

    $ python2 useful.py < test.json

In the following, we assume the data is stored in a list of lists, i.e.:

.. code-block:: bash

    [
       [
          6.44887349885013,
          -6.80408794830961,
          5.87205564819696
       ],
       [
          0.839489029433732,
          -8.86306739443021,
          -1.69246328305772
    ...

List comprehensions
===================

List comprehensions are a shorthand notation for doing things with lists. They spare you at times from having to write tedious for loops.

.. code-block:: python

    bs = [l[1] for l in data]

This line fetches the second column of our data, i.e. all the 'b' values for our functions (hence the variable name, 'b's). What it means is:

    "For each data point (l) in our input list (data), take the second element."

.. code-block:: python
 
    print "Number of b's: " + str(len(bs)) # prints 10,000

The variable bs is a list of 10,000 numbers. With "len(bs)" we can obtain the length of the list. (With "str()" we can convert data to a string for output purposes.)

We can now calculate the average of all 'b's:

.. code-block:: python

    b_avg = sum(bs) / float(len(bs))

(We need to convert the denominator to float to ensure the result is also float.)

In other words, while for delimiter separated data we could write a one-liner in awk to calculate the average, for JSON we'll need to write about three lines of Python. On the other hand JSON is in general more powerful than delimiter separated data.

Slices
======

.. code-block:: python

    first_hundred = data[:100] # len(first_hundred) is 100

Here, with "[:100]" we say "*slice* the list such that only the first hundred elements are included".

You can also slice the end of the list by using negative index.

Sorting
=======

Python has sorting built in:

.. code-block:: python

    print sorted(data, reverse=True)[:3]

This line introduces a few concepts:

* By calling the function sorted(data), we can obtain a sorted copy of the data.
* sorted() takes an *optional parameter* "reverse" which we set to True. This means that the returned list will be sorted in descending order instead of ascending.

Useful built in functions
=========================

"sum" sums up all the numbers in a list:

.. code-block:: python

    sum_of_all_numbers = sum([sum(l) for l in data])

"all" returns True if all values are True in a list, and False otherwise. ("any" return True if any element in a list is True.)

.. code-block:: python
 
    def all_numbers_in_list_are_above_zero(l):
        return all([n > 0 for n in l])

List comprehensions also support *filtering*, i.e. applying a function to only include certain elements in the resulting list:

.. code-block:: python

    above_zero_functions = [f for f in data if all_numbers_in_list_above_zero(f)]

*Exercise*: For your data set, find the function where the sum of its values (a, b and c) is the highest.

*Exercise*: For your data set, find the function where the sum of its values (a, b and c) is the highest *and* all values a, b and c are negative.

Tuples
======

Tuples in Python are quite similar to lists, but the size of a tuple is fixed. This means that while you can remove or add elements to a list in Python, this isn't possible with tuples, making tuples a nice way to implicitly document a fixed nature of some of your data.

Tuples can be created using parentheses:

.. code-block:: python

    my_tuple = ('a', 1)

They can be accessed like you access lists:

.. code-block:: python

    my_tuple[0] # returns 'a'
    my_tuple[1] # returns 1

They can be used like any other variable, e.g. added in lists:

.. code-block:: python

    my_list = [('a', 1), ('b', 2)]
    print my_list[1][0] # first element of the second element in the list, i.e. 'b'

A fun function is "zip" - which zips two given lists to one list of tuples:

.. code-block:: python

    list1 = ['a', 'b', 'c']
    list2 = [1, 2, 3]

    zipped_list = zip(list1, list2)
    print zipped_list # prints [('a', 1), ('b', 2), ('c', 3)]
