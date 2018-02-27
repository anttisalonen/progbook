Overview of some statically, strongly typed languages
-----------------------------------------------------

We've so far mostly used three languages:

* Javascript: a dynamically, weakly typed language
* C: a statically, weakly typed language
* Python: a dynamically, strongly typed language

Let's see what alternatives we have for a statically, strongly typed language.

Disclaimer: different programming languages are another topic that can get fairly religious among programmers. While I attempt to remain objective, 90% of programmers will probably disagree with most of the contents of this chapter. That is their right.

This chapter attempts to summarise different programming languages by identifying their most important traits. The languages have vast differences but the scope of this chapter is not to capture all of them, but instead only serve as an introduction. We will make a deeper dive to some of the languages later.

All the languages here are statically, strongly typed as well as imperative, such that it shouldn't be very difficult for the reader to implement FizzBuzz for any of the languages after seeing the "Hello world" program and possibly a search or two online on syntax. To make things more interesting we'll also, for each language, quote two programs for relatively common operations:

* Splitting a string by a space, with the goal to be able to access and print out each word separately.
* Filtering numbers less than zero from a list (or an array)

For the second operation, the implementation should be implemented generically, i.e. for any different numeric types, if possible in each language.

This may be helpful in illustrating the different concepts between languages.

*Exercise*: Implement both of the above operations in one of the languages (C, Python or Javascript). Note that example implementations will be listed below. If you pick C, you may want to use the function "realloc". If you pick Javascript, you may want to look up the reference for the Javascript array class first.

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

Here's the program to filter numbers less than zero in C, Python and Javascript:

.. literalinclude:: str.c
   :language: c

(This also introduces "realloc" for re-allocating existing memory.)

Python:

.. code-block:: python

    inp = [3, -1, 4, -2, 5, -3, 6, -4]
    out = [n for n in inp if n < 0]
    print out

Javascript:

.. code-block:: javascript

    var inp = [3, -1, 4, -2, 5, -3, 6, -4];
    var out = inp.filter(function(n) {
        return n < 0;
    });
    console.log(out);

(We use the built-in array class member function "filter" and pass it an anonymous function as the callback that defines what to filter on.)

What's new
==========

What could statically, strongly typed languages improve on compared to the above?

Python and Javascript are fairly concise but, when compared to C, have some characteristics that can be perceived as disadvantages:

* The performance of Python and Javascript is generally not as good as C and much of this is due to dynamic typing, although there are optimisations that can be done to alleviate this (for example *JIT*, or just-in-time compilation, where parts of a program can be compiled during execution after the runtime has collected data on which type is typically associated with each variable)
* There's arguably more room for error with dynamic typing; errors that could be caught before running the program

On the other hand, C has some characteristics that can be perceived as disadvantages compared to Python and Javascript:

* C is fairly verbose; based on the number of lines already there's arguably more effort required in C to achieve the same behaviour
* C is not memory safe, opening the possibility of nasty memory related bugs and security holes
* C is not as strongly typed as Python, such that some bugs in the code could go undetected which would raise a (run time) error in Python
* While we haven't discussed C projects with multiple files in detail, in general the system in C with header files and source files, with the header files being included in source files using the #include preprocessor directive (which practically automatically copy-pastes the header file in the source file) introduces some drawbacks that are ideally to be improved upon

Generic functions
~~~~~~~~~~~~~~~~~

Another point to note is that because types have to be explicitly defined in C, it is generally not possible to implement some *generic* functions in C without eliminating type safety. A classic example is a function to return the maximum value of two values:

.. code-block:: c

    int max_int(int a, int b)
    {
        if(a > b)
            return a;
        return b;
    }

    int max_float(float a, float b)
    {
        if(a > b)
            return a;
        return b;
    }

    /* repeat for all types for which you want to find the maximum */

Typically, in C, this particular case would be handled with a *macro*:

.. code-block:: c

    #define max(a, b) ((a) > (b) ? (a) : (b))

    int x = 5;
    int y = 4;
    int res = max(x, y);

(The *ternary operator* ?: returns the term before the : if the term before the ? is true and the term after the : otherwise.)

However, macros provide no type safety, such that it's e.g. possible to mix the types of "a" and "b" in the above without warning. (Macros also have other issues; if, for example, an expression with side effects such as "x++" was passed to the above "max" macro, then x could be incremented twice.)

Statically, strongly typed languages attempt to improve on the above points. It should be noted that there is no silver bullet, i.e. even with static, strong typing it can be argued that no language is categorically better than all other languages.

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

Filtering numbers less than zero:

.. literalinclude:: filt.cpp
   :language: cpp
   :linenos:

Due to how C++ works, we have to define the function that does the actual work before we define main. Let's go through main first.

* Line 21: We define our input, a vector of integers.
* Line 22: We call our processing function. We don't need to type the type of the variable but use "auto" instead, meaning the compiler can deduce it for us. In this case, the type is std::vector<int>.
* Line 8-17: Our processing function implementation.
* Line 8: We define a *template* for our function. This means that the type named "T" will be replaced by whatever the called used to call the function. In this case, int. This way our function only needs to be implemented once for all numeric types including floating point numbers.
* Line 9: We define the signature of our function such that the input is a *reference to* a std::vector which holds elements of type "T" (in our case, int). Reference is created using the "&" symbol. It's very similar to a pointer in C, but it is more restricted. For example it must always be defined to refer to a variable, and it cannot be set to NULL. The output of our function is "auto", i.e. automatically derived, but in our case std::vector<int>.
* Line 11: We define our variable which we'll return in the end.
* Line 12-14: We use C++ standard template library functions to describe copying values from one vector (array) to another.
* Line 15: Here, we define a *lambda* function that takes "T i" (in our case, "int i") as a parameter, and returns true if i is less than 0. This is used as the predicate for our copying function.

C++ is a fairly large language, both from syntax and the standard template library. I'd expect most C++ programmers to spend a significant portion of their programming time perusing the various language and library references.

Java
~~~~

Java is a language introduced in mid-90s and has a strong notion of *object oriented programming*. Compared to C++, Java:

* Generally has memory safety - no manually managed memory, or pointers
* Uses *garbage collection* - instead of relying on the programmer to free memory that's not used, Java periodically checks whether memory is still in use or not and if not, will mark it as free
* Has an improved module system - with no need to define header files

Java has also been evolving and the newer versions of the language include some features that haven't been part of Java before, such as lambda functions and generic functions.

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


