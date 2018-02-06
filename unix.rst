Basic Unix shell commands
-------------------------

Basic Unix shell commands are quite useful in day to day software development.

Let's assume you have the file with 10,000 lines defining the functions as discussed in "Generating input data using Python". In order to inspect this file, you could open it in the text editor, but there are also other, typically easier, ways to achieve this.

In the following, the character '$' means the shell prompt, i.e. you typically type in the shell whatever comes after '$'.

If you wanted to take a quick look browsing a file, you could use the utility 'less':

.. code-block:: bash

    $ less test.txt

This will open the file test.txt in 'less', which allows you to read and browse the file. Less has a few useful commands:

* 'q' exists less
* '/' allows you to search in the file - type '/' followed by the string you want to search for, followed by 'return'
* 'n' repeats the previous search
* 'N' repeats the previous search but searches backwards

There are a few more, and you can look up the reference for 'less' to see all of them (by running "man less"). These same commands are to some degree shared across several other Unix tools. For example vim has the same commands for searching.

Let's say you only wanted to see the beginning of the file. You can achieve this using 'head':

.. code-block:: bash

    $ head test.txt
    7.269553 3.427526 6.633603
    1.980206 -3.655827 -2.629755
    -8.687820 -6.930905 -8.731439
    -0.608791 -8.126272 -8.652504
    [...]

This will print the first ten lines of the file. You can adjust the number of lines output by using the "-n" switch. For example, the command "head -n5 test.txt" would only output the first five lines.

The command 'tail' can be used to output the last lines of a file, by default ten:

.. code-block:: bash

    $ tail test.txt
    [...]
    -7.058864 -5.461025 -8.337095
    -0.197971 -1.485949 -0.748672
    -3.051099 -9.215679 3.597125
    0.868340 -2.444818 -3.173135

If you want to search for a string in the file, you can use 'grep':

.. code-block:: bash

    $ grep "1234" test.txt
    6.133025 -4.612349 4.969612
    -0.904910 -5.976920 8.821234
    -6.123416 7.494195 4.350390
    -1.123468 0.327963 1.291879

This command would print all lines which have the string "1234" in them. The command 'grep' also has a lot of useful switches, for example including lines before or after the line that includes the search string, or inverse search (only print lines that don't have a match).

If you only wanted to print one column of the file, you could achieve this using 'awk':

.. code-block:: bash

    $ awk '{print $1}' test.txt
    7.269553
    1.980206
    -8.687820
    -0.608791
    [...]

This example only prints the first column in the file. ('awk' by default assumed the space is the delimiter character.)

'awk' is actually a programming language capable of much more, but much of the daily use is simple one-liners. Let's say we want to calculate the average of all values in the first column. This can be achieved using 'awk':

.. code-block:: bash

    $ awk '{sum += $1; n++} END {print sum / n}' test.txt
    0.0478027

What this line means is:

* For each line, take the value of the number in the first column, and add it in the variable called "sum" (automatically initialised to 0). Increment the variable "n" by one (also automatically initialised to 0).
* After the file has been processed, print "sum" divided by "n", i.e. the sum divided by the number of lines.

Another useful command is 'wc', or "word count", which can be used to count the words in a file:

.. code-block:: bash

    $ wc test.txt
    10000  30000 284890 test.txt

The output includes the number of lines, words, characters and the file name. By adding the switch '-l', only the number of lines and the file name are output:

.. code-block:: bash

   $ wc -l test.txt
   10000 test.txt

The final command introduced here is 'sed', or "stream editor", which modified the input using a given expression. For example we can use 'sed' to change all space characters in the file to commas:

.. code-block:: bash

   $ sed 's/ /,/g' test.txt
   7.269553,3.427526,6.633603
   1.980206,-3.655827,-2.629755
   -8.687820,-6.930905,-8.731439
   -0.608791,-8.126272,-8.652504
   [...]

Let's break down this expression 's/ /,/g':

* The character '/' is the delimiter; the expression consists of four parts: 's', ' ', ',' and 'g':
* 's': initial command: search (and replace)
* ' ': search for the space character
* ',': replace with the comma
* 'g': do this globally, i.e. not only once per line (the default), but for all occurrences in the file.

Another example is removing all '-' characters from the file:

.. code-block:: bash

   $ sed 's/-//g' test.txt
   7.269553 3.427526 6.633603
   1.980206 3.655827 2.629755
   8.687820 6.930905 8.731439
   0.608791 8.126272 8.652504
   [...]

We can also pass multiple expressions to 'sed' by using the '-e' switch, for example to replace spaces with commas and remove the dashes:

.. code-block:: bash

    $ sed -e 's/ /,/g' -e 's/-//g' test.txt
    7.269553,3.427526,6.633603
    1.980206,3.655827,2.629755
    8.687820,6.930905,8.731439
    0.608791,8.126272,8.652504
    [...]

Unix pipelines
==============

The 'awk' command above lists the first column for all 10,000 lines which might not be what you want. Let's say you only wanted to see the first ten lines, i.e. apply the 'head' command to the output of the 'awk' command. You can achieve this using pipelines:

.. code-block:: bash

    $ awk '{print $1}' test.txt | head
    7.269553
    1.980206
    -8.687820
    -0.608791
    [...]

In this case, head doesn't take a file name as input, but instead reads from standard input, i.e. the output from 'awk'. This is the typical behaviour for Unix commands.

In general, the commands can be combined in any way, giving a lot of power to the user.

*Exercise*: Find out how many lines in the file have no '-' character.

Redirecting
===========

You can always write the output of a command to a file by redirecting, i.e. using the '>' character:

.. code-block:: bash

    $ awk '{print $1}' test.txt > output.txt

This will create a file called output.txt, overwriting any previous contents, with the output of the 'awk' command.

This will allow us to simplify writing our own software. For example, it might not be necessary to open a file for writing in Python, so instead of this:

.. literalinclude:: with.py
   :language: python

...we can simply do this:

.. code-block:: python

    for i in xrange(5):
        print "%f\n" % 0.5

...and then run it like this:

.. code-block:: bash

    $ python with.py > test.txt

This has the added flexibility that we haven't hard coded the output file name.

*Exercise*: rewrite your program that generates the 10,000 functions file to write to standard output.
