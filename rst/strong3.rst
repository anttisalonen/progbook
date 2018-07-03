Newer languages
---------------

The previous section covered languages that are relatively established, with C++ originating from the 80s and Java and C# from mid-90s and early 2000s respectively. This section covers some of the languages which are a bit newer and hence somewhat less common, though still interesting. I should also point out that there are in fact hundreds if not thousands of new programming languages created in the past couple of decades so this section only covers a couple of hand picked ones, however there are several others that are widely used, to some extent more widely used than the selection here, and also arguably more different from other languages covered in this book.

Go
==

Go is a language created by Google and originally launched in 2009. Its standard implementation compiles to machine code and it aims to be an improvement over C, with memory safety and garbage collection included, among other features.

Splitting a string in Go:

.. literalinclude:: ../material/strong/split.go
    :language: go
    :linenos:

This looks a bit different to e.g. C# and Java:

* Line 3-6: We define the list of modules to import, namely "strings" and "fmt"
* Line 9: We define our string, assigning to it with the := operator
* Line 10: We split our string by calling the function "Split" from the strings module
* Line 11: We iterate over our string array using a for loop and the *range* keyword. What range does is provide an iterator for the loop, with two variables in each iteration, namely the index and the element itself. We capture the element in the "word" variable and ignore the index by denoting it with an underscore ("_").
* Line 12: We use the "fmt" module to print out each word to stdout

If the Go compiler is installed, this can be compiled and run using "go run split.go".

Filtering elements from an array in Go:

.. literalinclude:: ../material/strong/filt.go
    :language: go
    :linenos:

* Line 9: We call our function filt which does the work.
* Line 15: The function implementation begins. It takes one parameter called "arr" which is an array of integers. Its return value is called "ret" and is also an array of integers.
* Line 16-20: We define a simple for loop for adding the relevant numbers in the return value. The function "append" appends an element to an array, and returns the array with the element appended.

Go doesn't support defining generic functions so this function only works for integers.

Swift
=====

Swift is a language announced by Apple in 2014. It's also compiled to machine code and is designed as the main language for writing applications for Mac and other Apple devices.

Splitting a string in Swift:

.. literalinclude:: ../material/strong/split.swift
    :language: swift

Swift infers the types of the variables in this example.

Filtering elements from an array in Swift:

.. literalinclude:: ../material/strong/filt.swift
    :language: swift
    :linenos:

This seems interesting:

* Line 3: We define our filtering function. It takes an array of T as input, and returns an array of T as output. Swift allows use of generics here such that we can define T to be of generic type *BinaryInteger*, which means any kind of integer, but not e.g. floating point. This means our function can be used for different kinds of integer numbers but not for any numbers.
* Line 4: Function body. We use the filter function with a lambda function as the predicate. The special variable $0 is the element in the array passed to the lambda function.

Swift infers the types of our arrays (d and d2) but the types needed to be explicitly defined for the function definition.

Rust
====

Rust is another fairly recent language, being announced by Mozilla in 2010. It's intended to be an improvement over C++ with good performance and improved memory safety.

Splitting a string in Rust:

.. literalinclude:: ../material/strong/split.rs
    :language: rust

Rust infers the types of our variables in this program.

Filtering elements from an array in Swift:

.. literalinclude:: ../material/strong/filt.rs
    :language: rust
    :linenos:

Again, this seems interesting:

* Line 5: We define our input data. This is an array of integers (specifically 32-bit integers), whereby the type is inferred by Rust.
* Line 6: We call our function. We pass a reference of "d" to the function.
* Line 12: We define our function. The parameter called "arr" is a reference to an array of T's. It returns a vector (dynamically allocated array) of T's. T is a generic type parameter but the function mandates it must implement the Num, PartialOrd and Copy *traits*, meaning it must be numeric, sortable and copyable.
* Line 13: We first convert our input variable, an array, to iterable using "into_iter", which is required by the next step. We then filter on it using the built in "filter" function. We pass an anonymous function as the predicate, whereby we call the variable passed to the predicate "i". We need to use "&&i" instead of plain "i" to copy "i" for comparison with T::zero(), which is a generic function representing a generic zero. Finally we clone the resulting array which is required for turning it into a vector, which is done by collect().

The type system and generics in Rust allows us to write the function such that it works for all numeric types.

To summarise, Rust allows fairly direct control over allocation and its type system is extensive, however some work is required from the developer to take advantage of all Rust features.

*Exercise*: Out of the six new languages described in this and the previous chapter, pick one that seems to appeal to you the most, and one that seems to appeal to you the least.

*Exercise*: For both of the two languages you picked, implement Fizz Buzz and the quadratic equation exercise of finding the largest root in a file of 10,000 equations. Note that for most of the languages you don't necessarily have to install the compiler on your computer; there are several web pages that provide an interactive code editor and compiler for testing out a language. Try searching for e.g. "rust online" or "rust playground".
