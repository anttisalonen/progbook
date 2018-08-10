Finishing our simple PNG parser
-------------------------------

We're now able to do some preliminary parsing of a PNG file, but we're still lacking the parsing of the actual blocks.

We won't parse all blocks - most notably, we won't parse the actual image data - but we can get some information out of some of the other blocks. For example, the header block (IHDR), the background colour block (bKGD) and last modification block (tIME) give us some good cases for practising our programming skills. For such blocks, we can set the goal of simply printing out the contents to stdout.

Data driven programming
=======================

Now, we have code to tell when we've stumbled upon a block, we know how long a block is in bytes, and we know where to find the contents of the block. What we could do is simply check the type of the block and then have some logic to react to it, i.e.:

.. code-block:: c

    printf("chunk: %s - len: %d (%d)\n", chunkbuf, len, size - (pos + len + 12));
    if(!strcmp(chunkbuf, "IHDR")) {
        printf("Image width: ...");
        /* more printfs */
    } else if(!strcmp(chunkbuf, "tIME")) {
        printf("Year: ...");
        /* more printfs */
    } else if(...) /* and so on */

However, this is an excellent use case to simplify the logic (i.e. reduce the number of if's) and try *data driven programming*, where we capture the different functionality in *tables*, or arrays with an ID and a function pointer designating what to do.

What we want to have is the following:

1. We'll have one function for each type of chunk
2. We'll create an array of structs, whereby each struct has a string (const char pointer) which is the type of the chunk, and a function pointer pointing to the function that handles the chunk
3. We'll have a for loop looping through this array instead of the if-else if chain, calling the relevant function for each found block

Let's illustrate this.

Typedefs
========

First of all, when working with function pointers in C, it's often practical to define the type of the function in a *typedef*. This is mainly because the syntax of declaring function pointers in C can be seen as a bit complex, and with a typedef we can capture this complexity in one place. What typedef does is simply assign another name to a type. In our case, our type is a function pointer.

We want a separate function for each type of chunk, and our typedef should describe the type for such a function. Our function should return nothing (void), and it should take a const pointer to an input buffer plus its length as parameters. Here's an example declaration of such a function:

.. code-block:: c

    void time_handler(const char *buf, int len);

A typedef for function pointers that could point to such functions would be:

.. code-block:: c

    typedef void (*handler_ptr)(const char *buf, int len);

Now, when we want to have a variable which points to a function, we can use "handler_ptr" as the type for that variable. In fact, let's do just that, and declare such a variable as part of a struct:

.. code-block:: c

    struct handler {
        const char  *type;
        handler_ptr func;
    };

Here we have a struct with a const char pointer and a function pointer.

Now we can define a global variable which is an array of such structs. While generally global variables aren't great, our array will be constant and as such not lead to confusing code even though it's global:

.. code-block:: c

    const struct handler handlers[] = {
        { "IHDR", header_handler },
        { "tIME", time_handler },
        { NULL,   NULL }
    };

Here we have an array of total three structs, such that the const char pointer identifies the type of the chunk and the second variable in the struct refers to a function to call for that chunk. We have the last value only contain NULL pointers to mark the end of the array which will be useful soon.

We can now define each function - they can be empty for now:

.. code-block:: c

    void header_handler(const char *buf, int len)
    {
    }

    /* repeat for time_handler */

Finally, we can add the loop in our main function to call each function:

.. code-block:: c
    :linenos:

    for(int i = 0; handlers[i].type != NULL; i++) {
        if(!strcmp(chunkbuf, handlers[i].type)) {
            handlers[i].func(buf + pos + 8, len);
            break;
        }
    }

Let's go through this line by line:

* Line 1: We loop through our array. The terminating condition checks for NULL value for the type: if the type pointer is NULL, we know we've reached the end of the array and terminate the loop. This is a convention we've agreed to by ourselves.
* Line 2: We check if the current element in the array matches the chunk type we've found in the file.
* Line 3: We call the function pointer pointed to by the struct in the array, and pass it the data from the file.
* Line 4: As we've found the correct handler and called it, we can terminate the loop.

*Exercise*: Put all of the above together. Your handlers don't need to do anything with the data yet but your code will need to compile.

Parsing the chunks
==================

We can now implement our first chunk handler - the handler for the header. We simply need to validate the input to ensure we're not reading past the end of the buffer, then call "printf" for the relevant data as per the file format specification. The chunk handler can start like this:

.. code-block:: c
    :linenos:

    void header_handler(const char *buf, int len)
    {
        validate(len == 13, "header must be 13 bytes");
        printf("Width:              %d\n", get_big_endian(buf));
        printf("Height:             %d\n", get_big_endian(buf + 4));
        printf("Bit depth:          %d\n", (unsigned char)buf[8]);
        ...
    }

*Exercise*: Finish the header handler. Look up from the spec what the contents of the header are expected to be.

We can then proceed to the time handler (type "tIME"). If you check the spec you'll see that it has a field with two bytes. If it was one, we could simply cast to unsigned char. If it was four, we could use our "get_big_endian" function. Since it's two, we need another function to parse it correctly.

*Exercise*: Implement the time handler. Read in the year using a function "get_2_byte_big_endian" which you need to define yourself. It should work the same as "get_big_endian" but only read in two bytes.

*Exercise*: Add another handler to handle the "pHYs" block. Look up from the spec what the format is.

Finally, for this section, we'll need to implement the background colour handler. This is interesting because, if you look at the spec, you'll see that how to parse it depends on the colour type which was defined in the header. This means that we'll have to store the colour type in a variable in the header handler and have it accessible for the background colour handler.

While we could in theory use a global variable for this, such generally leads to confusing code, and we can do better. A nice way to go about this is to add another parameter to the handler functions which is shared among all functions. Generally it's best to define a struct for all the data you need to thread through the functions, e.g.:

.. code-block:: c

    struct png_data {
        int color_type;
    };

Now, we need to change the function pointer typedef to include a pointer to "struct png_data", and add this parameter to all the handler functions. We can then define a variable of this type in main, clear its memory, and pass a pointer to it to the handler function being called.

Once we have this, we can set the "color_type" variable in the header handler, and read its value in the background colour handler. (As we'll pass a pointer to the struct to the functions, we can access it with the -> syntax, e.g. png->color_type.)

*Exercise*: Add the background colour handler function to your program. Depending on the file, it should print either the palette index, grey colour value or the red, green and blue background colour values.

Slight digression: switch-case
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

You may have implemented the background colour handler using something like this:

.. code-block:: c

    if(png->color_type == 3) {
        ...
    }
    else if(png->color_type == 0 || png->color_type == 4) {
        ...
    } ...
    else {
        printf("unknown color type\n");
    }

This works, but C provides another way to implement this kind of a pattern, namely the *switch-case* syntax. The above could be translated to switch-case like this:

.. code-block:: c

    switch(png->color_type) {
        case 3:
            /* code here */
            break;

        case 0:
        case 4:
            /* code here */
            break;

        /* more cases here */

        default:
            printf("Unknown background color type\n");
            break;
    }

In other words, we can define the points to jump to depending on the input variable value. We'll need to add break statements at the end of each case as otherwise the execution would continue to the next case. The "default" case is jumped to if no other case matched.

*Exercise*: Rewrite your background colour handler to use switch-case.

One thing we didn't discuss here was checking the CRC to ensure the data hasn't been corrupted. However, RFC 2083 includes C code to calculate the CRC which we can reuse.

*Exercise*: Copy the code from RFC 2083 to your program. Using the sample code, calculate the CRC of each block and compare it to the CRC stored in the file (they should match). Note that the CRC needs to be calculated over the data field *and* the chunk type field. You may need to make some modifications to the sample code to account for const and different signs.

After these exercises, for the sample image, the output should be something like this:

.. verb:: 

    chunk: IHDR - len: 13 (15154)
    Width:              732
    Height:             150
    Bit depth:          8
    Color type:         6
    Compression method: 0
    Filter method:      0
    Interlace method:   0
    CRC correct: 1
    chunk: bKGD - len: 6 (15136)
    R: 255
    G: 255
    B: 255
    CRC correct: 1
    chunk: pHYs - len: 9 (15115)
    pixels/unit, x: 2835
    pixels/unit, y: 2835
    unit:           1
    CRC correct: 1
    chunk: tIME - len: 7 (15096)
    Year:   2018
    Month:  2
    Day:    4
    Hour:   22
    Minute: 44
    Second: 30
    CRC correct: 1
    chunk: iTXt - len: 29 (15055)
    CRC correct: 1
    chunk: IDAT - len: 8192 (6851)
    CRC correct: 1
    chunk: IDAT - len: 6827 (12)
    CRC correct: 1
    chunk: IEND - len: 0 (0)
    CRC correct: 1

Congratulations, you now have your own simple (partial) PNG file parser. 
