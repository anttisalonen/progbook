PNG files
---------

PNG (Portable Network Graphics) is a common file format for storing image data. Let's write a parser to parse some parts of PNG files in C. This will help teach us the following concepts:

* Working with binary data
* Data driven programming in C - or, how to structure C programs nicely

On the side, we'll learn about the following smaller but important C concepts:

* C macros
* Command line options
* The switch-case statement
* typedef

We'll end up with a program that can parse some parts of a PNG file and display what it parsed. The program will be around 150 lines of code. The reader should ideally be able to finish the exercises within 40 hours.

Introduction to PNG
===================

PNG file format is described online at e.g. Wikipedia but also at the RFC (Request for Comments) 2083 which describes the format in detail. In a nutshell:

* PNG starts with a *signature*, a sequence of bytes which can be used to identify a file as a PNG file
* The signature is followed by multiple *chunks*, whereby each chunk contains different kind of information about the file, including the actual image data
* Each chunk is identified by four characters and includes, apart from the identifier, the length, the actual data, and the *CRC*, or *cyclic redundancy check* - a checksum used to ensure the contents of the buffer are valid

*Exercise*: Look up RFC 2083 online. You'll be needing it later.

The signature is eight bytes long and contains the following values, in hex format:

+----+----+----+----+----+----+----+----+
| 89 | 50 | 4E | 47 | 0D | 0A | 1A | 0A |
+----+----+----+----+----+----+----+----+

In ASCII, the bytes 2-4 (starting from 1) are "PNG"; bytes 5-6 are "\\r\\n" (CRLF) and byte 8 is "\\n".

Let's write a program that loads a given input file to a buffer. For this, we'll allocate a buffer of 16 megabytes. If the user attempts to open a larger file, we exit. This is naive but works well enough for our purposes.

.. literalinclude:: ../material/png/png1.c
   :language: c
   :linenos:

Let's go through this line by line:

* Line 5: We define a *macro* that contains the maximum size of the file we can open (16 MB). Because we need this value in several places, it's best to define it in one place to keep the uses in sync.
* Line 9-13: We check the command line arguments. In C, we can either define the main function to take no parameters (void) or "int argc, char \*\*argv". If we use the latter, the variable "argc" contains the number of arguments and the variable "argv" the arguments themselves. If argc is 1, then the only argument (found at argv[0]) is the program name itself. We want to accept one additional argument so we ensure "argc" is 2.
* Line 18: We open the given file.
* Line 24: We read the whole file into our buffer.

Now, that program doesn't do very much so let's change that. We can, after opening and reading in the file, add a function call to a new function "check_header", which receives our buffer as the input parameter:

.. code-block:: c

    void validate(int val, const char *msg)
    {
        if(!val) {
            fprintf(stderr, "Invalid file: %s\n", msg);
            exit(1);
        }
    }

    void check_header(const char *buf)
    {
        validate((unsigned char)buf[0] == 0x89, "header byte 1");
        validate((unsigned char)buf[1] == 'P',  "header byte 2");
    }

Here, our function "check_header" calls "validate" twice, for the first two bytes in the buffer. The function "validate", defined above, checks if the condition is true, and if not, prints out an error and exits the program.

.. topic:: Digression: what is "unsigned"?

  In C, there are several integer data types; e.g. char (one byte), int (one word, e.g. 4 bytes, but depends on the system), *long* (system dependent but at least 4 bytes) and others. In addition, each data type can either be *signed* or *unsigned*. Signed means they can have a minus sign, i.e. can be negative. Typically, as some space e.g. one bit is used for the sign, the unsigned data can have a larger range. For example, a signed char has a range of at least from -127 to 127 while an unsigned char has a range from at least 0 to 255.

  In the above, the byte values in the PNG header can be more than 127, e.g. the value 0x89 is 137 in decimal, hence the cast to unsigned char.

.. topic:: Digression: stderr

  The function "validate" prints out the error using *fprintf*, which is similar to printf but allows defining the output file for writing. We define the output to go to *stderr*, or "standard error", which is very similar to standard in but is another file. The main use case is redirecting: if we were to direct the output of our program to a file, what gets written to stderr actually gets written out in the terminal, not in the output file. E.g:

  .. code-block:: bash

    $ ./png file.c > out.txt
    Invalid file: header byte 1
    $ cat out.txt
    $

  If you want to redirect stderr, you can use "2>", e.g. "./png file.c > out.txt 2> stderr.txt".

  The function call 'printf("abc\\n");' is equivalent to 'fprintf(stdout, "abc\\n");'.

*Exercise*: Expand the program to check for all eight bytes of the header. Find a PNG file to test your program on.

.. only:: html

  You can download a sample PNG file here - a screenshot from another chapter (e.g. right click and "save as"):

  .. image:: ../material/js/guess.png

.. only:: not html

  You can download a sample PNG file from the book web page - a screenshot from another chapter.

You'll need to use either this exact image or another image with the same chunks for later exercises in this part of the book.

Now we have a program that checks, for a given input file, whether it has the correct PNG signature or not. Let's see if we can expand it to read all the headers in a PNG file. You may have read from the RFC or Wikipedia that after the signature, the series of chunks begins, with each chunk having the following format:

+---------+------------+----------------+---------+
| Length  | Chunk type | Chunk data     | CRC     |
+---------+------------+----------------+---------+
| 4 bytes | 4 bytes    | /length/ bytes | 4 bytes |
+---------+------------+----------------+---------+

In other words, each chunk starts with four bytes indicating the length of the chunk data in bytes, followed by the chunk type (4 bytes), the chunk data (arbitrary length), and finally the CRC which is 4 bytes and can be used to check the data for correctness.

Now, let's see if we can parse this. Let's add the following code after we've checked the header:

.. code-block:: c

    int pos = 8;
    while(1) { /* FIXME: infinite loop */
        char lenbuf[4];
        memcpy(lenbuf, buf + pos, 4);
        int len = get_big_endian(lenbuf);
        char chunkbuf[5];
        /* TODO: fill chunkbuf */
        printf("chunk: %s - len: %d (%d)\n", chunkbuf, len, size - (pos + len + 12));
        /* TODO: increment pos correctly */
    }

The code is a bit incomplete but a nice skeleton to start with:

* We define the variable "pos" which refers to our position in the full buffer.
* We then loop over all the chunks in the file (and more).
* We first copy the bytes designating the length of the buffer to a 4-byte buffer.
* We then convert that to an integer using the function "get_big_endian" (to be defined).
* We fill in the chunkbuf, i.e. a buffer containing the name of the chunk. We allocate a buffer of five bytes for this for convenience - the four bytes which are the chunk name are all ASCII so by allocating five bytes and finishing the buffer with a zero we can treat the buffer as a string and pass it to printf directly.
* We then call printf telling us which chunk we've found, what the length of the chunk is, and how much of the file is still left after this chunk.

Before we fill in the blanks, let's go to the details around "get_big_endian".

Endianness, and bit level operations
====================================

You may have noticed from the RFC or other sources that some values in the PNG file are stored as *big endian*. What does this mean? Let's first have a refresher on binary numbers. Imagine we have the number 135,200 and we want to store this. The number 135,200 is in decimal - in binary it is (using the Python interpreter to check):

.. code-block:: python

    >>> bin(135200)
    '0b100001000000100000'

In other words - each of the four bytes in binary and hex:

+----------+----------+----------+----------+
| 00000000 | 00000010 | 00010000 | 00100000 |
+----------+----------+----------+----------+
|  0x00    |  0x02    |  0x10    |  0x20    |
+----------+----------+----------+----------+

The first byte has no bits set. Each of the other bytes has one bit set each.

Now, the way the table is here is in *big endian* - the most significant byte is the first one from the left, i.e. the bytes are ordered from the big end.

This means that in our buffer, if the length was 135,200 bytes, we'd have the following contents:

+--------------+--------------+--------------+--------------+
| buf[pos + 0] | buf[pos + 1] | buf[pos + 2] | buf[pos + 3] |
+--------------+--------------+--------------+--------------+
|  0x00        |  0x02        |  0x10        |  0x20        |
+--------------+--------------+--------------+--------------+

Now the question is, how would we convert these four bytes (char values) to an integer containing the value 135,200. This is the purpose of the function "get_big_endian".

To make it short, here's the function definition:

.. code-block:: c
    :linenos:

    int get_big_endian(const char *buf)
    {
        return ((unsigned char)buf[0] << 24) |
               ((unsigned char)buf[1] << 16) | 
               ((unsigned char)buf[2] << 8)  | 
                (unsigned char)buf[3];
    }

This introduces two new operators: *bit shifting* and *bitwise or*.

Bit shifting shifts bits within a variable. Let's assume we the following code:

.. code-block:: c

    int x = 4;
    x = x << 1;
    printf("%d\n", x); // prints "8"

Here, "x" originally had the value 4, or "00000100" in binary (skipping some 0's at the front). We then *shift the bits to the left* by one on the second line, ending up with "00001000", or 8. This is bit shifting.

Bitwise or then:

.. code-block:: c

    int x = 3;
    int y = 6;
    int z = x | y;
    printf("%d\n", z); // prints "7"

Here we "or" the bits of x and y, bitwise:

+-------+----------+
| x     | 00000011 |
+-------+----------+
| y     | 00000110 |
+-------+----------+
| x | y | 00000111 |
+-------+----------+

...and 00000111 is 7.

Where does that leave us with get_big_endian? Let's take another look:

.. code-block:: c

        return ((unsigned char)buf[0] << 24) |
               ((unsigned char)buf[1] << 16) | 
               ((unsigned char)buf[2] << 8)  | 
                (unsigned char)buf[3];

We take the first byte, shift its value by 24 bits to the left. We take the second byte, shift its value by 16 bits to the left, and 'or' the two. We take the third byte, shift its value 8 bits to the left, and 'or' the three. We take the fourth byte and 'or' it with the rest. Hence constructing the representation of an int from the four bytes.

Now we have everything to fill the gaps in our loop to go through all the chunks in a PNG file.

*Exercise*: Implement the missing bits. Terminate the loop when you reach the end of the file, as identified by the "size" variable. Increment pos by the chunk header size plus the length. Initialise the chunk string appropriately. Your program should output something like this (for guess.png):

.. verb::

    chunk: IHDR - len: 13 (15154)
    chunk: bKGD - len: 6 (15136)
    chunk: pHYs - len: 9 (15115)
    chunk: tIME - len: 7 (15096)
    chunk: iTXt - len: 29 (15055)
    chunk: IDAT - len: 8192 (6851)
    chunk: IDAT - len: 6827 (12)
    chunk: IEND - len: 0 (0)

