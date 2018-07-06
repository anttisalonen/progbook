Basic Unix shell usage
----------------------

  UNIX is simple and coherent, but it takes a genius (or at any rate, a programmer) to understand and appreciate its simplicity.

  -- Dennis Ritchie

Basic Unix shell commands are quite useful in day to day software development.

Let's assume you have the file with 10,000 lines defining the functions as discussed in "Quadratic formula". In order to inspect this file, you could open it in the text editor, but there are also other, typically easier, ways to achieve this.

In the following, the character '$' means the shell prompt, i.e. you typically type in the shell whatever comes after '$'.

If you wanted to take a quick look browsing a file, you could use the utility 'less':

.. code-block:: bash

    $ less test.txt

This will open the file test.txt in 'less', which allows you to read and browse the file. Less has a few useful commands:

* 'q' exits less
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

*Exercise*: Find out which grep switch makes grep only select non-matching lines.

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

*Exercise*: Find out how many lines in the file have no '0' character in them.

Further Unix shell tips and tricks
==================================

man
~~~

The command "man" (short for "manual") allows you to browse the documentation of different tools. For example, running "man grep" will display the documentation for grep. The documentation is opened using "less", such that you can browse the text freely and exit with 'q'.

There can be multiple man pages for a single command. For example, "man signal" can mean either looking up the signal() C standard library functions or the general overview of signals. The man pages are categorised by type, such that for example category 1 means commands that can be run, 3 means C standard library functions and 7 means miscellaneous documentation. You can specify which category you mean by including it in your command, for example:

.. code-block:: bash

    $ man 7 signal

...will look up the man page on signal in category 7, providing the reader with an overview of signals in Unix.

*Exercise*: Look up the man page for the command "man".

*Exercise*: Look up the man page for the C function call "printf".

sort
~~~~

The tool "sort" sorts its input line by line. By default it sorts alphabetically, but by passing the switch "-n" it will interpret its input data numerically. It by default sorts based on the beginning of each line but this can be changed:

.. code-block:: bash

    $ sort -n test.txt | head
    -9.995596 8.887278 2.325502
    -9.995454 -0.339710 4.518171
    -9.993047 -9.059912 -0.660508
    -9.990530 -5.503126 -8.374026
    [...]

(If your sort command output seems nonsensical, it might be due to the locale set on your shell such that the decimal point is defined as ',' instead of '.', confusing sort. You should be able to fix this by running "export LC_ALL=C".)

The above sorted the data based on the first column. If we wanted to sort by the second column instead, we can use:

.. code-block:: bash

    $ sort -n -k 2 test.txt | head
    0.649875 -9.998834 2.834749
    -3.819303 -9.998413 -7.295722
    0.985071 -9.997176 1.182082
    -6.991833 -9.995815 -7.523136

"sort" also allows redefining the delimiter character using the "-t" switch. For more information, run "man sort".

Variables
~~~~~~~~~

Variable support is typically something that Unix shells have built in. That is, defining variables isn't executing a program per se, but rather using a feature of the shell itself.

Terminology wise, there are different Unix shells (called for example bash, tcsh, zsh), each with different characteristics and quirks, but each one typically implements the same core functionality, namely being compatible with the original Unix shell (sh) and conforming to the POSIX shell specification.

This example defines a variable in bash:

.. code-block:: bash

    $ MY_FILE=test.txt
    $ head -n1 $MY_FILE
    7.269553 3.427526 6.633603

In other words, defining a variable is trivial, and you can use the variable by prefixing it with a dollar sign.

Sometimes you might want to combine the variable with other bits. In those cases it's typically safe to enclose the variable with curly brackets ({ and }). This will make it clear when the variable name starts and ends. For example, if we wanted to combine two variables in one file name:

.. code-block:: bash

    $ MY_FILE_START=test
    $ MY_FILE_SUFFIX=txt
    $ echo ${MY_FILE_START}.${MY_FILE_SUFFIX}
    test.txt

echo and cat
~~~~~~~~~~~~

The command "echo" simply prints its input. For example:

.. code-block:: bash

    $ echo "hello"
    hello
    $ echo $MY_FILE
    test.txt
    $ echo "abc,def,ghi" | sed -e 's/,/ /g'
    abc def ghi

The command "cat" concatenates files. It can also be used to display the contents of a file:

.. code-block:: bash

    $ cat test.txt
    7.269553 3.427526 6.633603
    1.980206 -3.655827 -2.629755
    -8.687820 -6.930905 -8.731439
    -0.608791 -8.126272 -8.652504
    [...]

Exit codes
~~~~~~~~~~

Whenever you've finished running a program in Unix, it will return an exit code. The convention is that the exit code 0 means success while non-0 means failure. You can typically see the conditions under which a program returns success or failure by looking at the documentation. For example, grep returns exit code 1 if the search string was not found at all. You can use the special built-in variable $? to access the exit code:

.. code-block:: bash

    $ grep 2345 test.txt
    5.145898 3.219212 3.234599
    3.323714 3.883829 -4.722345
    6.142345 -4.611688 0.817618
    -7.761082 9.886385 -5.742345
    $ echo $?
    0
    $ grep 23456 test.txt
    $ echo $?
    1

Multiple commands
~~~~~~~~~~~~~~~~~

You can run multiple commands in series in one line. The following runs "head", followed by "tail":

.. code-block:: bash

    $ head -n 1 test.txt ; tail -n 1 test.txt
    7.269553 3.427526 6.633603
    0.868340 -2.444818 -3.173135

You can also run multiple commands depending on the exit code of the previous execution. The shorthand "&&" means "run the following command only if the previous command succeeded, i.e. returned an exit code 0". The shorthand "||" means "run the following command only if the previous command failed". You can also group commands using parentheses. For example:

.. code-block:: bash

    $ (grep 2345 test.txt && echo "found") || echo "not found"
    5.145898 3.219212 3.234599
    3.323714 3.883829 -4.722345
    6.142345 -4.611688 0.817618
    -7.761082 9.886385 -5.742345
    found
    $ (grep 23456 test.txt && echo "found") || echo "not found"
    not found

Globbing
~~~~~~~~

Globbing refers to using special characters to match multiple files. An example is "\*.py" which means "all files with the extension .py in the current directory". For example, to find out the number of lines in Python files:

.. code-block:: bash

    $ wc -l *.py
     156 conf.py
       8 gen.py
       4 rand.py
       3 with.py
     171 total

Seq
~~~

The command "seq" simply outputs a sequence of numbers:

.. code-block:: bash

    $ seq 1 5
    1
    2
    3
    4
    5

This might not be very useful by itself but can be handy when combined with other tools.

Find
~~~~

Find is useful for finding files, and optionally performing operations on them.

For example, let's assume you have a directory with subdirectories, with the directory and subdirectories having lots of Python files. Let's further assume you had used the Python "with" statement in some of the files and would like to see how, but you can't remember which files exactly use "with". Find and grep to the rescue:

.. code-block:: bash

    $ find . -name '*.py' -exec grep with {} +
    ./conf.py:# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
    ./with.py:with open('test.txt', 'w') as f:

Let's go through this point by point:

* We execute find with several parameters
* The first parameter is '.', i.e. search in the current working directory (as well as subdirectories)
* Search for files with the extension '.py'
* For each found file, run "grep with $filename". The notation {} means the found file name will be used here, and the final '+' means the grep command will be run for once with all the files as parameters. For example, if the find command found three Python files, ./a.py, subdir/b.py and subdir2/c.py, it would execute "grep with ./a.py subdir/b.py subdir2/c.py".

The output has two lines: one with grep matching in conf.py, where a comment using the word "with", and another in with.py where the Python with statement was used.

If we only wanted to find the files with the Python extension without grepping, we simply leave out the -exec part:

.. code-block:: bash

    $ find . -name '*.py'
    ./tmp/config.py
    ./conf.py
    ./my_project/hello.py
    ./guess/guess.py
    ./with.py
    ./rand.py

