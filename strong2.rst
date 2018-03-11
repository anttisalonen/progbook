Established languages
---------------------

C++
===

C++ started in the 80s as an addition of features to C. Since then it has accumulated an impressive set of features. It's still mostly backwards compatible to C - such that most C can be compiled with a C++ compiler with little changes - but has a lot of features that C doesn't have.

Some things that C++ "fixes" comparing to C are:

* Automatic code execution when a variable leaves the scope: We saw in our BSD socket handling code that we had to call close() and sometimes shutdown() whenever we saw something go wrong. This is easily forgotten or neglected, causing resource leaks (in this case file descriptors but can also be e.g. memory). C++ uses the concept of classes to enable automatic cleanup when necessary. This also enables *smart pointers* - pointers with *reference counting* such that no manual memory management is necessary, and instead the pointers should know themselves when the memory pointed to can be freed.
* Arrays: In C, the programmer has to keep track of the length of an array (or pointer to an array) manually - it's not automatically associated with the array. C++ has array datatypes which include the length, greatly simplifying the code.
* Generic programming: In C, if one were to e.g. implement a function to return the larger of two numbers, it's necessary to implement the function for all numeric data types, e.g. float, int, etc. even when the body of the function is the same. C++ solves this using *templates*.
* Extended standard library: E.g. the max() function but also several string handling functions are included in the C++ standard library. Furthermore, data structures such as linked lists, sets and dictionaries (maps) are included.

Things that C++ hasn't (completely) fixed include:

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
* Line 17-19: Writing out each word.

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
====

Java is a language introduced in mid-90s and has a strong notion of *object oriented programming*. Compared to C++, Java:

* Generally has memory safety - no manually managed memory, or pointers
* Uses *garbage collection* - instead of relying on the programmer to free memory that's not used, Java periodically checks whether memory is still in use or not and if not, will mark it as free
* Has an improved module system - with no need to define header files

Java has also been evolving and the newer versions of the language include some features that haven't been part of Java before, such as lambda functions and generic functions.

Splitting a string in Java:

.. literalinclude:: split.java
   :language: java
   :linenos:

You can compile and run this e.g. by running "javac split.java && java Split". (You'll need to have the Java framework installed.)

Let's see what we have:

* Line 1: We put all our code in *class* Split. All Java code must reside in a class. The class name will specify what the output file will be (here Split.class) which is needed when executing the program ("java Split").
* Line 4: We call the member function (or "method" in Java terminology) "split" of the class String which returns an array of Strings.
* Line 5: We define a for loop where we iterate over the array, printing out each word in stdout.

As you can see, the Java code is for this example not very different from Python or Javascript.

Filtering numbers in an array:

.. literalinclude:: filt.java
    :language: java
    :linenos:

* Line 5: We define our array.
* Line 6: We call our method which does the filtering.
* Line 13: We convert the array into a *stream* using the "stream" method, then use the "filter" method which takes a lambda function as a callback to specify which values we want to keep. Finally we convert the stream back into an array using the "toArray" method and return the result.
* Lines 7-9: We iterate over our resulting array, printing out the values.

Note that this function only works with integers, not e.g. with floating point numbers unlike C++. The reason is that the templating system in C++ simply replaces the type T with the used type, i.e. int in our case, such that were we to e.g. use a float, the comparison with literal zero (0) would still work. However, the generics system in Java doesn't allow this.

C#
==

C# is a language by Microsoft which is similar to Java and runs on a virtual machine called CLR (Common Language Runtime) which is part of a framework called .NET.

Although there are many differences between C# and Java, on a grand scale they're fairly similar, such that e.g. differences between C++ and Java or C++ and C# are relatively larger. Like Java, C# has memory safety, garbage collection and a proper module system.

The main implementation of C# (compiler and the CLR runtime) is available from Microsoft for Windows, Mac and Linux. Alternative implementations exist such as Mono.

Here's a program to split strings in C#:

.. literalinclude:: split.cs
    :language: csharp
    :linenos:

There are a few subtle differences between C# and Java:

* In C#, we need to use the "using" directive in order to access the console (stdout)
* In C#, Main is capitalised but string isn't
* In C#, the method "split" takes a character as a parameter while in Java it takes a regular expression; also, in C# the method name is capitalised (technically speaking, the C# Split method takes a character array as a parameter with the single character implicitly converted to an array)
* In C#, the for loop is somewhat different. The main difference is that we don't need to define the type of the variable we use for iterating, but can use simple "var" instead, which automatically infers the correct type.
* Instead of System.out.println in Java, C# has Console.WriteLine().

Overall many of the differences here are cosmetic.

Filtering elements in a list in C#:

.. literalinclude:: filt.cs
    :language: csharp
    :linenos:

This is again fairly similar to Java, with the main difference being that the syntax for the actual filter operation is somewhat different.


