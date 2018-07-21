Regular expressions
-------------------

  Some people, when confronted with a problem, think "I know, I'll use regular expressions." Now they have two problems.

  -- Jamie Zawinski

A lot can be said about regular expressions. I'll stick to the essentials, bare minimum in this section.

Regular expressions are an extension to the usual search and replace. For example, you might want to find any line in a file that ends with the digit 9:

.. code-block:: bash

    $ grep "9$" test.txt | head
    -8.687820 -6.930905 -8.731439
    -6.532205 -8.706914 1.642039
    3.052381 2.971625 -0.620359
    5.722527 -7.593573 -8.653239
    [...]

'grep' by default assumes the search string provided to it is a regular expression. The string "9$" consists of two characters: 9 and $. $ is called an anchor character: it designates the end of the line. Hence, searching for "9$" means that grep will output all lines where 9 is the last character in the line.

A more complex example is wanting to search any negative numbers in the file between -1.0 and -1.3:

.. code-block:: bash

    $ grep "\-1\.[012][0-9]*" test.txt | head
    0.094081 -6.537027 -1.189798
    4.145320 -1.276905 -6.513201
    -1.240556 -2.562810 -8.189995
    -1.114494 -4.974068 -5.414513
    [...]

This command outputs any line in which a number between -1.0 and -1.3 was found. Let's go through the regular expression "\\-1\\.[012][0-9]*" piece by piece:

\\- means "match the minus character". We need a backslash before the minus character because otherwise grep would confuse it with a switch. The backslash is the escape character; in this case it means "treat the following character as the literal minus character".

1 means we search for a 1.

\\. means the period character, i.e '.'. This needs to be escaped using backslash because in regular expressions, the period character is short hand for "any character". However in this case we mean the actual period character, hence the backslash.

[012] means that after the period character we search for either a 0, 1 or 2.

[0-9] means that after the previous character (0, 1 or 2) we search for any digit.

\* means that the previous search must be repeated as often as possible, i.e. for any digits until some other character is found.

Hence, the regular expression "\\-1\\.[012][0-9]*" means "search for a string that starts with a minus character, has a 1, followed by a period, followed by either 0, 1 or 2, followed by zero or more digits".

It may become more obvious if we pass the switch "-o" to grep, which means that only the matching strings are output, not the full line of a match:

.. code-block:: bash

    grep -o "\-1\.[012][0-9]*" test.txt | head
    -1.189798
    -1.276905
    -1.240556
    -1.114494
    [...]

Now, grep only outputs the strings that were matched, i.e. the numbers between -1.0 and -1.3.

As unplausible as it may sound, experienced programmers are often able to derive a correct regular expression for a certain use case with ease, though sometimes, especially with more complex expressions, some debugging may be needed. This comes with practice; an experienced programmer often uses regular expressions several times per day.

Regular expressions aren't limited to grep, but are available in most programming languages as well as other Unix tools such as sed. To get a refresher on sed, you might, for example, use sed to search and replace the string "123" with "456":

.. code-block:: bash

    $ sed -e 's/123/456/g' test.txt > new_file.txt

Now, let's say you had a more advanced use case, for example replacing any negative number with 0.0. This can be achieved using regular expressions with 'sed':

.. code-block:: bash

    $ sed -e 's/-[0-9.]*/0.0/g' test.txt
    7.269553 3.427526 6.633603
    1.980206 0.0 0.0
    0.0 0.0 0.0
    0.0 0.0 0.0
    0.0 6.908667 4.418877
    [...]

Let's go through this piece by piece.

* 's/-[0-9,]*/0.0/g' - four components, delimited by '/'. The first one, 's' means search and replace.

The second component is -[0-9.]*:

* \- First find a minus character
* [0-9.] Then find any character that is a number (0-9), or a period. Square brackets denote "any of".
* \* Repeat the previous as many times as possible. Hence, of the string "-1.234 2.000", the part "-1.234" would be matched because it starts with a minus character, followed by several digits and a period. After reaching the space character the search terminates.

The third component is 0.0: Whatever was matched in the search will be replaced by this.

The fourth component is 'g' again, i.e. apply this search as many times as possible for each line. This means that for example for line "-1.234 -2.000 1.23" the both matching instances will be replaced (both "-1.234" and "-2.000" will be replaced by "0.0").

*Exercise*: Using your generated file of 10,000 functions, write a 'sed' command to replace all digits after the decimal point with a 0. For example, the line "1.980206 -3.655827 -2.629755" should become "1.0 -3.0 -2.0".

The man page for GNU grep includes a good reference on regular expressions, although this might not be available on non-GNU systems like some BSDs.

.. topic:: GNU? BSD? What's going on?

    Unix has a long and complex history. To make it short, BSD (Berkeley Software Distribution, originating from the University of California, Berkeley) is a family of Unix implementations nowadays consisting of a few operating systems including OpenBSD and macOS. The BSD operating systems have the typical Unix tools such as grep implemented and documented. GNU (GNU's Not Unix - implying GNU doesn't contain any original Unix code) is *another* implementation of Unix and hence *also* has typical Unix tools such as grep implemented and documented - but implemented and documented separately and hence slightly differently. While Unix has been standardised, such that one can expect tools such as grep generally behave similarly across various Unix implementations, the different implementations can include additional features in their implementations that others might not have. Or indeed include a reference on regular expressions in their grep man page.

    Linux distributions are the most common operating systems that include GNU tools.

*Exercise*: Look up a regular expression reference to see what character classes and expressions are available.
