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

    print sorted(data, reverse=True, key=lambda l: l[2])[:3]

This line introduces a few concepts:

* By calling the function sorted(data), we can obtain a sorted copy of the data.
* sorted() takes an *optional parameter* "reverse" which we set to True. This means that the returned list will be sorted in descending order instead of ascending.
* We can specify a *key* for sorting: here we specify a *lambda function*, i.e. an anonymous function.
* Our anonymous function says "use the third element in the list as the key", i.e. the value "c" in the functions.

We could also have defined a named function for this:

.. code-block:: python

    def get_third_element(l):
        return l[2]

    print sorted(data, reverse=True, key=get_third_element)[:3]

*Exercise*: What would you expect this statement to print in the end? Which functions would be included in the output?

*Exercise*: Execute this statement and see if the output matches your expectations.

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

