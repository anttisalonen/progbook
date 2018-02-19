Overview of some strongly, statically typed languages
-----------------------------------------------------

We've so far mostly used three languages:

* Javascript: a weakly, dynamically typed language
* C: a weakly, statically typed language
* Python: a strongly, dynamically typed language

Let's see what alternatives we have for a strongly, statically typed language.

Disclaimer: different programming languages are another topic that can get fairly religious among programmers. While I attempt to remain objective, 90% of programmers will probably disagree with most of the contents of this chapter. That is their right.

This chapter attempts to summarise different programming languages by identifying their most important traits. The languages have vast differences but the scope of this chapter is not to capture all of them, but instead only serve as an introduction. We will make a deeper dive to some of the languages later.

All the languages here are strongly, statically typed as well as imperative, such that it shouldn't be very difficult for the reader to implement FizzBuzz for any of the languages after seeing the "Hello world" program. To make things more interesting we'll also, for each language, quote a program for doing a common operation: splitting a string by a space, with the goal to be able to access and print out each word separately. This may be helpful in illustrating the different concepts between languages.

To prepare, here's how to split a string in C, Python and Javascript:

C:

.. literalinclude:: str.c
   :language: c
   :linenos:

(Compile and run e.g. using "gcc -Wall -O2 -o str str.c && ./str".)

Some explanation to the above: we create a buffer where we store a copy of the original string, with each space replaced with a string terminator ('\0'). We index into the buffer in order to access the individual words. In order to do this, we create an array with MAX_WORDS (20) elements, each one including the index (position) of the word.

Python:

.. code-block:: python

    string = "this is a string.\n";
    for word in string.split(' '):
        print word

Javascript:

.. code-block:: js

    var str = "this is a string.\n";
    var res = str.split(" ");
    for(var i = 0; i < res.length; i++) {
        console.log(res[i]);
    }

Established languages
=====================

C++
~~~

C++ started in the 80s as an improvement over C. Since then it has accumulated an impressive set of features. It's still mostly backwards compatible to C - such that most C can be compiled with a C++ compiler with little changes - but has a lot of features that C doesn't have.

Some things that C++ "fixes" comparing to C are:

* Automatic code execution when a variable leaves the scope: We saw in our BSD socket handling code that we had to call close() and sometimes shutdown() whenever we saw something go wrong. This is easily forgotten or neglected, causing resource leaks (in this case file descriptors but can also be e.g. memory). C++ uses the concept of classes to enable automatic cleanup when necessary. This also enables *smart pointers* - pointers with *reference counting* such that no manual memory management is necessary, and instead the pointers should know themselves when the memory pointed to can be freed.
* Arrays: In C, the programmer has to keep track of the length of an array (or pointer to an array) manually - it's not automatically associated with the array. C++ has array datatypes which include the length, greatly simplifying the code.
* Generic programming: In C, if one were to e.g. implement a function to return the larger of two numbers, it's necessary to implement the function for all numeric data types, e.g. float, int, etc. even when the body of the function is the same. C++ solves this using *templates*.
* Extended standard library: E.g. the max() function but also several string handling functions are included in the C++ standard library. Furthermore, data structures such as linked lists, sets and dictionaries (maps) are included.

Things that C++ hasn't (completely) fixed are:

* Memory safety: While it's possible to write safe C++, it still allows things like pointer arithmetic and manual memory management for backwards compatibility reasons.
* Module system: It's still necessary to have a header file (.h) for declaring the functions and data types and the source file (.cpp) for function definitions.

Hello world program:

.. code-block:: cpp

    #include <iostream>

    int main()
    {
        std::cout << "Hello world" << std::endl;
    }

In C++, instead of calling printf() with its inherent lack of type safety (as the format string can be dynamic, it's in general not possible for the compiler to check that the format string matches the types that are provided as extra parameters, or indeed that the number of extra parameters is correct), the C++ standard library provides an object called std::cout which has *overloaded* the "<<" operator (left shift) to mean writing to it.

Split string:

.. literalinclude:: str.cpp
   :language: cpp
   :linenos:

* Line 1-6: Necessary includes.
* Line 10: Our input string.
* Line 11: Our *vector* (dynamic array) to hold the result (individual words).
* Line 12: Input stream, a class that holds the string, but as a stream.
* Line 13-15: We copy the data from the stream to our vector. When reading the stream, it returns words one at a time, which are inserted into our vector using the class std::back_inserter.
* Line 16-18: Writing out each word.

This can be compiled and run e.g. with the following command: g++ --std=c++11 -Wall -O2 -o str str.cpp && ./str

Java
~~~~

C#
~~

Newer established languages
===========================

Go
~~

Swift
~~~~~

Rust
~~~~


