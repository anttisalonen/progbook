Programming language taxonomy
-----------------------------

There are a few concepts around programming languages that help pick up a new language.

Typing
======

Most programming languages come with a more or less sophisticated type systems. The type system defines how the types of variables and functions are used and interpreted.

A common example is the following expression:

.. code-block:: bash

    4 + "2"

What is the result of this expression? The following provides an overview:

+------------+---------------------------------------------------------------+
| Language   | Output                                                        |
+============+===============================================================+
| Javascript | 42                                                            |
+------------+---------------------------------------------------------------+
| Java       | 42                                                            |
+------------+---------------------------------------------------------------+
| Perl       | 6                                                             |
+------------+---------------------------------------------------------------+
| Lua        | 6                                                             |
+------------+---------------------------------------------------------------+
| Python     | TypeError: unsupported operand type(s) for +: 'int' and 'str' |
+------------+---------------------------------------------------------------+
| Haskell    | Error: No instance for (Num [Char]) arising from a use of ‘+’ |
+------------+---------------------------------------------------------------+
| Ruby       | String can't be coerced into Fixnum (TypeError)               |
+------------+---------------------------------------------------------------+
| C          | 134514105 (undefined behaviour)                               |
+------------+---------------------------------------------------------------+
| C++        | garbage (undefined behaviour)                                 |
+------------+---------------------------------------------------------------+

We see four different kinds of answers:

* Javascript and Java implicitly convert the number 4 into a string, then appends 2, resulting in "42"
* Perl and Lua implicitly convert the string "2" into a number, then add 4, resulting in 6
* Python, Haskell and Ruby throw an error
* C and C++ interpret the addition as pointer arithmetic, accessing memory after the statically allocated string "2", resulting in undefined behaviour

Another example is about defining a simple function (pseudocode):

.. code-block:: bash

    function(x):
        return 2 * x

If we were to define this function in C, it could look for example like this:

.. code-block:: bash

    int function(int x)
    {
        return 2 * x;
    }

This function explicitly defines the input and output types as integers; using other types such as a string when calling this function will issue a compiler warning.

If we were to define this function in Javascript, it would look like this;

.. code-block:: javascript

    function f(x) {
        return 2 * x;
    }

I.e. no variable types explicitly defined. If we were to use Python, it would look like this:

.. code-block:: python

    def function(x):
        return 2 * x

While in Haskell the definition would look like this:

.. code-block:: haskell

    function x = 2 * x

Or in Java:

.. code-block:: java

    public static int function(int x) {
        return 2 * x;
    }

What if we called these functions with a number? A string? The following table summarises this:

+------------+--------------------------------+----------------------------------+------------------------------------------+
| Language   | Result when passing 5 (number) | Result when passing "5" (string) | Result when passing "abc" (string)       |
+============+================================+==================================+==========================================+
| C          | 10                             | garbage (undefined behaviour)    | garbage (undefined behaviour)            |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Javascript | 10                             | 10                               | NaN (a string indicating "not a number") |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Python     | 10                             | "55"                             | "abcabc"                                 |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Java       | 10                             | Compile error                    | Compile error                            |
+------------+--------------------------------+----------------------------------+------------------------------------------+
| Haskell    | 10                             | Compile error                    | Compile error                            |
+------------+--------------------------------+----------------------------------+------------------------------------------+

We see the result makes sense when inputting a number for all languages. When inputting "5" as string, C interprets this as a pointer and undefined behaviour ensues. Javascript implicitly converts the string to a number, which works when the string is indeed a number but returns "NaN" otherwise. When passing a string to the function in Python, Python interprets the multiplication as a multiplication of the string, hence duplicating the string. Haskell and Java refuse to work with the string input.

The above can be summarised by categorising the language type systems by how dynamic the typing is (static vs. dynamic), and the strength (strong vs. weak).

Static vs. dynamic typing
~~~~~~~~~~~~~~~~~~~~~~~~~

C, Haskell and Java are statically typed languages: the types of all variables must be defined at compilation time. For C and Java, the types must in most cases be explicitly stated, like we saw in the function definitions above. Haskell typically infers the types at compile time.

Javascript and Python are dynamically typed languages: the types of variables may change depending on context. For example, the variable "x" in the function above may be a string or an integer, depending on the caller.

Strong vs. weak typing
~~~~~~~~~~~~~~~~~~~~~~

C and Javascript are weakly typed languages: the types of the variables aren't fixed but may be implicitly converted to other types depending on the context. For example, passing a string as an int will cause implicit conversion of the string to an int.

Python and Haskell are strongly typed languages: the types of variables are fixed and will not implicitly change, such that any type conversions must be explicit by the programmer.

It should be pointed out that while, based on the above, Java and Haskell are both strongly typed, some languages are more strongly typed than others. In our 4 + "2" example we saw that Haskell returned an error while Java returned "42". In this case, Java *implicitly converted* the type of the expression 4 to a string in order to avoid a compile error and return a string instead. Hence it can be argued that while Java is also a strongly typed language, Haskell is even more strongly typed.

Having this overview of the different type systems helps picking up and understanding new languages.

Duck typing
~~~~~~~~~~~

The combination of strong, dynamic typing is also often called "duck typing". This is based on the notion that "if it walks like a duck and it quacks like a duck, then it is a duck". Let's take a look at our Python function definition again:

.. code-block:: python

    def function(x):
        return 2 * x

The operation to multiply with an int (2 \*) is defined for both integers and strings. For integers, the traditional multiplication is performed, while for strings the string is duplicated. From the point of view of the function, it makes no difference which type is passed to the function, as long as it can be multiplied by 2, hence duck typing.

Defining your own data types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Most programming languages will make it possible for the programmer to define own data types. While a lot can be written about this topic, in this section we'll describe two approaches: C structs and Python classes.

In C, data types are defined by defining a struct, for example:

.. code-block:: c

    struct my_datatype {
        int a;
        int b;
        int c;
    };

This struct consists of three integers. As C is relatively close to the actual hardware, we can reason about what this struct looks like in practice. In general, the amount of memory used by an integer in C is implementation defined but for the purpose of this section we can assume it's 4 bytes (32 bits). Defining a structure like this typically means the data will be packed well, such that 12 bytes will be required for one allocation of struct my_datatype and the layout will only contain the memory required for a, b and c, nothing more. (Mixing different data types of different sizes may cause padding memory to be added by the C compiler, depending on the hardware constraints.)

In this sense, defining structures in C mostly serves to combine various data into one unit, simplifying code and aiding in having the necessary memory available and allocated.

In Python, on the other hand, data types can only be defined using the "class" keyword:

.. literalinclude:: cl.py
   :language: python

Let's go through this line by line:

* Line 1: We define a *class* named A. At a high level, a class is a data type, describing data types and functions common to the class.
* Line 2: We define a *member function* - a function that's part of a class.
* Line 3: Calling this member function will produce output
* Line 5: We define another class named B.
* Line 6: We define a *constructor* for class B: a function that will be called when an object of this type is created.
* Line 7: The constructor will create a *member variable* named foo - a variable that's contained within the object.
* Line 9: We define a member function for class B - with the same name as we did for class A.
* Line 10: This member function produces some output and also reads the member variable value.
* Line 12: We define a list with two elements, and create (or *instantiate*) one object of each class; one of class A, one of class B. Class B has a constructor that requires one parameter: we pass in 42.
* Line 14: We iterate through our list.
* Line 15: We call the member function "call_me" for each element in the list, i.e. once for our object of class A, once for our object of class B.

Executing this will output:

.. code-block:: bash

    $ python2 cl.py
    A called
    B called - my foo is: 42

We see an example of duck typing here: while classes A and B technically have nothing to do with each other, they do share the same *interface*, namely calling the function "call_me". We also see that the classes in Python are more powerful than the structs in C: in Python we can combine multiple variables in one object, but in addition we can have member functions and use these to define an interface on interacting with the data.
