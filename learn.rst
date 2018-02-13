Learning to learn
-----------------

There are a few concepts to point out about learning programming.

When learning a programming language, there are a few different areas that need to be learned. One is the syntax. For example, the syntax for branches in Python looks like this:

.. code-block:: python

    if a > 5:
        print 'a is greater than 5'

Another is the standard library. For example, Python has a library for math functions, called "math":

.. code-block:: python

    import math

    print math.sqrt(5)
    print math.pi

The "math" library has lots of functions, and the *reference* for them can be accessed by running "help(math)" in Python interpreter (after "import math"). The concept of reference is important: it's a factual listing of a library or a programming language. It typically doesn't include examples or tutorials but is useful for getting a deeper understanding of a subject.

For example, we will introduce the Unix command "ls", a tool for listing files. In order to gain initial understanding of this tool, this book will give a couple of examples:

.. code-block:: bash

    $ ls
    genindex.html  index.html  objects.inv                regex.html   searchindex.js  _static
    _images        js.html     python_generate_file.html  search.html  _sources        unix.html
    $ ls -l
    total 108
    drwxr-xr-x 2 antti users  4096 Feb  5 23:51 _images
    drwxr-xr-x 2 antti users  4096 Feb  7 00:48 _sources
    drwxr-xr-x 2 antti users  4096 Feb  5 23:51 _static
    -rw-r--r-- 1 antti users  2603 Feb  7 23:33 genindex.html
    [...]

This example shows that merely running "ls" will list the files, while adding the switch "-l" will give more information, such as the last modify time of the files. However, "ls" has a lot more possible switches which are not all covered in this book, although they will become useful and will to some extent be necessary to use, learn and understand as a software engineer. Hence it'll be important to refer to the reference of the command to get an overview. This can typically be done either by passing the switch "--help" to the relevant command, or reading the manual for the command (by running "man", e.g. "man ls").

The same principle applies not only to Unix commands, but also to programming languages as well as standard and non-standard libraries. For example, for most programming languages there is either a language specification or reference material available which should be consulted whenever questions arise about a specific behaviour in a programming language. Similarly, libraries typically come with references. The standard library for C is included in the Unix man pages, such that for example running "man scanf" will present a detailed documentation of the scanf() function family. For other libraries, the website of the library or other similar material should be consulted. Many libraries also install their own man pages as part of the library installation.

Terminology wise, this book will use the terms "API" and "library" interexchangably. The acronym "API" stands for "Application Programming Interface" and generally means the interface, i.e. the available functions, variables and other constructs, that a library provides to make use of the library. For example, if one were to use the libcurl library, a library for downloading files via URLs, one would want to find the reference of the library on the webpage of the software.

*Exercise*: Find the reference of the libcurl library online. See if you can find both the entry point to the reference as well as the reference of a specific libcurl function, for example curl_easy_init().
