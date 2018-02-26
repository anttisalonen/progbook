Functional programming
----------------------

The programming languages we've used so far are all *imperative*; they're based on the mental model of a computer executing instructions in a series, from a list, from the top, one by one. This is furthermore based on the mathematical model of a *Turing machine*, described by Alan Turing; a machine that points to an element on a memory tape and has instructions to move on the tape and modify elements on it.

There are several programming paradigms other than imperative, but one of interest for us is *functional programming*. We won't go to details and actually program in a functional programming language in this book, but there are some aspects you should know about. (Again, as this chapter is about different programming languages, several programmers will disagree with the contents.)

Functional programming is based on *lambda calculus*, a formal system described by Alonzo Church in the 1930s. It was later hypothesised that the work by Turing and Church is actually equivalent; that is, a Turing machine can be formally translated to lambda calculus and vice versa.

The first functional programming language was Lisp, described in 1958 by John McCarthy. At the time, one of the most common imperative languages was Fortran. To get a high level understanding on how the languages look like, here's an example snippet from both:

Lisp:

.. code-block:: lisp

    (if (> b 3) (println "b is bigger than 3"))

Fortran:

.. code-block:: fortran

    IF (NB.GT.4) THEN
      PRINT *, 'B is larger than 3.'
    END IF

Fortran was originally mainly used when entering code to computers using punch cards, while Lisp was originally described in a research paper but it was later discovered that an interpreter for this originally theoretical language could be implemented. So they have very different backgrounds and approaches but Lisp had some features that were at the time not available for other languages, including:

* Dynamic typing
* Recursion
* if-then-else
* Anonymous functions
* First class functions
* Dynamic memory allocation
* Macros i.e. running code at compile time
* Garbage collection, i.e. memory is freed automatically when it's not used anymore

There's been a trend since the 1960's and continuing that mainstream (imperative) languages are slowly adopting features that Lisp and functional programming in general have pioneered. This is mostly because there are less limitations around computer hardware, and the focus is shifting to increasing programmer productivity and ergonomics. Indeed Python and Javascript as well as several other languages such as Java have incorporated one or more of the above list either as best practices, or as integral parts of the language.

This chapter is mostly concerned with providing examples around best practices in Python which can often be applied for other languages as well, as a means to reduce the number of bugs, reduce typing, and make code easier to understand.

Map and reduce
==============

Map and reduce are the most primitive operations to apply to lists (or arrays). As much of the power of computers and software development lies in the fact that once an operation has been described in code, it can easily be applied thousands or millions of times, it's important to understand how to extend basic logic to be applied multiple times.

"Map" relates to the general concept of applying a function that processes data over a list. For example, let's have a simple function named "process" that tells how many digits are there in an integer, which would, for example, return 1 when given 5 as input, or 2 for 15, or 3 for 115.

*Exercise*: Implement the described function. (You can convert from string to integer using the int() function.)

This function takes a value of type "int" and returns a value of type "int". Now, we can pass a value to this function and receive the answer. If we had a list of numbers, and wanted to have a list of results of this function, we could do:

.. code-block:: python

    inp = [5, 15, 115]
    out = list()
    for i in inp:
        out.append(process(i))

However, with a list comprehension, we can reduce the amount of code and simply use:

.. code-block:: python

    inp = [5, 15, 115]
    out = [process(i) for i in inp]

Either way, this process of applying a function over a list is called *map*.

A related operation is *reduce*, where we also apply a function to multiple elements of a list but the result is a single value that's accumulated over each element, not a list. For example, our function could be a simple addition:

.. code-block:: python

    def add(x, y):
        return x + y

Now, our input could e.g. be [1, 2, 3, 4]. In order to *reduce* this we want to apply the function "add" continuously, and also define the initialisation value, in this case 0:

.. code-block:: python

    inp = [1, 2, 3, 4]
    out = 0
    for i in inp:
        out = add(out, inp)

*Note*: "add" was here implemented for demonstration purposes. In actual code you'd rather just use the '+' operator directly. However, if you do want to pass addition as a function you could either use a lambda function, or import operator and pass operator.add.

There's no list comprehension for this pattern. However, there's a special case of reduction, namely filtering, or creating a new list with only values that fulfil a given condition. For example, if we wanted to only keep numbers less than 3:

.. code-block:: python

    inp = [1, 2, 3, 4]
    out = [i for i in inp if i < 3]

Finally, map and filter can be combined, e.g.:

.. code-block:: python

    inp = [5, 15, 115]
    out = [process(i) for i if i < 100]

This snippet returns a list with two elements, with the function "process" called for both of them.

*Exercise*: Write a list comprehension that returns a list of elements that have less than three digits in them. For the example input of [5, 15, 115] it should return [5, 15].

Now, while the examples here are in Python, the concepts of map, reduce and filter apply to several languages, both static and dynamic.

