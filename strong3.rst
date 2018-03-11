Newer languages
---------------

The previous section covered languages that are relatively established, with C++ originating from the 80s and Java and C# from mid-90s and early 2000s respectively. This section covers some of the languages which are a bit newer and hence somewhat less common, though still interesting. I should also point out that there are in fact hundreds if not thousands of new programming languages created in the past couple of decades so this section only covers a couple of hand picked ones, however there are several others that are widely used, to some extent more widely used than the selection here, and also arguably more different from other languages covered in this book.

Go
==

Go is a language created by Google and originally launched in 2009. Its standard implementation compiles to machine code and it aims to be an improvement over C, with memory safety and garbage collection included, among other features.

Splitting a string in Go:

.. literalinclude:: split.go
    :language: go
    :linenos:

This looks a bit different to e.g. C# and Java:

* Line 3-6: We define the list of modules to import, namely "strings" and "fmt"
* Line 9: We define our string, assigning to it with the := operator
* Line 10: We split our string by calling the function "Split" from the strings module
* Line 11: We iterate over our string array using a for loop and the *range* keyword. What range does is provide an iterator for the loop, with two variables in each iteration, namely the index and the element itself. We capture the element in the "word" variable and ignore the index by denoting it with an underscore ("_").
* Line 12: We use the "fmt" module to print out each word to stdout

If the Go compiler is installed, this can be compiled and run using "go run split.go".

Swift
=====

Rust
====


