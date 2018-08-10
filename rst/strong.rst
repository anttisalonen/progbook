The case for statically, strongly typed languages
-------------------------------------------------

  There are only two kinds of languages: the ones people complain about and the ones nobody uses.

  -- Bjarne Stroustrup

We've so far mostly used three languages:

* JavaScript: a dynamically, weakly typed language
* C: a statically, weakly typed language
* Python: a dynamically, strongly typed language

Let's see what alternatives we have for a statically, strongly typed language.

Disclaimer: different programming languages are another topic that can get fairly religious among programmers. While I attempt to remain objective, 90% of programmers will probably disagree with at least some of the contents of this section. That is their right.

This section attempts to summarise different programming languages by identifying their most important traits. The languages have vast differences but the scope of this section is not to capture all of them, but instead only serve as an introduction. We will make a deeper dive to some of the languages later.

All the languages here are statically, strongly typed as well as imperative, such that it shouldn't be very difficult for the reader to implement FizzBuzz for any of the languages after seeing the "Hello world" program and possibly a search or two online on syntax. To make things more interesting we'll also, for each language, quote two programs for relatively common operations:

* Splitting a string by a space, with the goal to be able to access and print out each word separately.
* A separate function which filters numbers less than zero from a list (or an array)

For the second operation, the implementation should be implemented generically, i.e. for any different numeric types, if possible, in each language.

This may be helpful in illustrating the different concepts between languages.

*Exercise*: Implement both of the above operations in one of the languages (C, Python or JavaScript). Note that example implementations will be listed below. If you pick C, you may want to use the function "realloc". If you pick JavaScript, you may want to look up the reference for the JavaScript array class first.

To prepare, here's how to split a string in C, Python and JavaScript:

C:

.. literalinclude:: ../material/strong/str.c
   :language: c
   :linenos:

(Compile and run e.g. using "gcc -Wall -O2 -o str str.c && ./str".)

Some explanation to the above: we create a buffer where we store a copy of the original string, with each space replaced with a string terminator ('\\0'). We index into the buffer in order to access the individual words. In order to do this, we create an array with MAX_WORDS (20) elements, each one including the index (position) of the word.

Python:

.. code-block:: python

    string = "this is a string.\n";
    for word in string.split(' '):
        print word

JavaScript:

.. code-block:: js

    var str = "this is a string.\n";
    var res = str.split(" ");
    for(var i = 0; i < res.length; i++) {
        console.log(res[i]);
    }

Here's the program to filter numbers less than zero in C, Python and JavaScript:

.. literalinclude:: ../material/strong/filt.c
   :language: c

(This uses "realloc" for re-allocating existing memory.)

Python:

.. code-block:: python

    def filt(arr):
            return [n for n in arr if n < 0]
    inp = [3, -1, 4, -2, 5, -3, 6, -4]
    print filt(inp)

JavaScript:

.. code-block:: javascript

    var inp = [3, -1, 4, -2, 5, -3, 6, -4];
    function filt(arr) {
        var out = arr.filter(function(n) {
            return n < 0;
        });
        return out;
    }
    console.log(filt(inp));

(We use the built-in array class member function "filter" and pass it an anonymous function as the callback that defines what to filter on.)

What's new
==========

What could statically, strongly typed languages improve on compared to the above?

Python and JavaScript are fairly concise but, when compared to C, have some characteristics that can be perceived as disadvantages:

* The performance of Python and JavaScript is generally not as good as C and much of this is due to dynamic typing, although there are optimisations that can be done to alleviate this (for example *JIT*, or just-in-time compilation, where parts of a program can be compiled during execution after the runtime has collected data on which type is typically associated with each variable)
* There's arguably more room for error with dynamic typing; errors that could be caught before running the program may cause a run time error during execution

On the other hand, C has some characteristics that can be perceived as disadvantages compared to Python and JavaScript:

* C is fairly verbose; based on the number of lines already there's arguably more effort required in C to achieve the same behaviour
* C is not memory safe, opening the possibility of nasty memory related bugs and security holes
* C is not as strongly typed as Python, such that some bugs in the code could go undetected which would raise a (run time) error in Python
* While we haven't discussed C projects with multiple files in detail, in general the system in C with header files and source files, with the header files being included in source files using the #include preprocessor directive (which practically automatically copy-pastes the header file in the source file) introduces some drawbacks that ideally can be improved upon

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

Other languages may attempt to improve on the above points. It should be noted that there is no silver bullet, i.e. even with static, strong typing it can be argued that no language is categorically better than all other languages.

