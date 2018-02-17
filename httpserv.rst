Writing a toy web server
------------------------

After using Flask a bit, you might have wondered "what's actually happening?"

In essence, Flask receives HTTP requests from the browser over the network, and sends HTML data back to the browser which then displays it. But we should dig a bit further here.

OSI model
=========

The OSI model is a way to structure the network activity in these kinds of situations. There are total seven layers which make up the model:

+-------+--------------------+-----------------------------------------------------------+
| Layer | Name               | Protocol used in our case                                 |
+=======+====================+===========================================================+
| 7     | Application layer  | HTTP                                                      |
+-------+--------------------+-----------------------------------------------------------+
| 6     | Presentation layer | -                                                         |
+-------+--------------------+-----------------------------------------------------------+
| 5     | Session layer      | -                                                         |
+-------+--------------------+-----------------------------------------------------------+
| 4     | Transport layer    | TCP                                                       |
+-------+--------------------+-----------------------------------------------------------+
| 3     | Network layer      | IP                                                        |
+-------+--------------------+-----------------------------------------------------------+
| 2     | Data link layer    | Wifi or Ethernet, depending on your network configuration |
+-------+--------------------+-----------------------------------------------------------+
| 1     | Physical layer     | E.g. DSL, depending on your network configuration         |
+-------+--------------------+-----------------------------------------------------------+

The layers 1 and 2 are set up depending on your network configuration.

The layers 3 and 4 are implemented by your OS; Windows, Mac and Linux implement the TCP/IP stack in the kernel.

The layers 5, 6 and 7 are all summarised by HTTP. When we ran Flask, we ran its built in HTTP server.

Now, we've used OS functions before: When we open a file in C, we call the open() or fopen() functions, which end up calling kernel functions. The kernel functions typically interact with the actual hardware; In case of opening a file, the kernel functions would read data from the physical hard drive in the computer. Similarly, when writing a HTTP server, "all" we need to do is call the relevant kernel functions that trigger TCP/IP communications. Let's try this out.

BSD sockets API
===============

The API for accessing the TCP/IP stack is called the BSD sockets API. (BSD stands for "Berkeley Software Distribution"; the API originates from the University of California, Berkeley.) Here's a simple example using it:

.. literalinclude:: sock1.c
   :language: c
   :linenos:

Let's walk through this step by step:

* Line 14: We define a variable with the type struct sockaddr_in. This is a type that's defined as part of the BSD sockets API.
* Line 15: We clear all the memory allocated for this variable using the standard C library memset() function.
* Lines 16-18: We set some values in that struct, which basically mean that we want to open the TCP/IP port 1234.
* Line 20: We use the first function of the BSD sockets API: socket(). This function should return a socket, i.e. something we can use for communications. 
* Lines 21-24: We check the value of our socket. -1 means something went wrong. In this case we output an error string with perror() and stop the program execution.
* Lines 26-36: We use the API to configure our socket such that it listens to incoming connections, i.e. as a server. It listens to TCP port 1234 and will allow up to five connections to line up.
* Line 38: We run a for-loop which initialises nothing, has no condition for termination, and does nothing on each iteration. In other words, an infinite loop.
* Line 39: We call accept(), another BSD sockets API function. This function call will block, i.e. our program execution will not proceed, until someone connects to the server.
* Line 47: We call the function handle_server() which is a function that we will need to write ourselves. This defines what the server actually does.

.. topic:: Digression: type casting

  Line 26 is also interesting because we perform a *type cast* here. The line without the type cast would look like this:

  .. code-block:: c

    if (bind(server_fd, &sa, sizeof sa) == -1) {

  That is, we simply pass the address of our struct sockaddr_in to the bind() function. If we were to try this then we would get a compiler warning, turned error if the "-Werror" switch is used:

  .. code-block:: bash

      sock1.c: In function 'main'
      sock1.c:26:22: error: passing argument 2 of 'bind' from incompatible pointer type [-Werror=incompatible-pointer-types]
        if (bind(server_fd, &sa, sizeof sa) == -1) {
                      ^
      In file included from sock1.c:2:0:
      /usr/include/sys/socket.h:123:12: note: expected 'const struct sockaddr *' but argument is of type 'struct sockaddr_in *'
       extern int bind (int __fd, __CONST_SOCKADDR_ARG __addr, socklen_t __len)
                  ^~~~

  What the error tells us is that the function bind() expects a pointer to *struct sockaddr* but we pass it a pointer to *struct sockaddr_in*. Because of the way the API is specified (in the case of IP communication it actually required *struct sockaddr_in* despite what the function declaration says), we cast the type to *struct sockaddr*. Type casting basically means telling the compiler "please pretend this variable has a different type than what it actually has". Having this possibility in C makes C a weakly typed language.

The code listing above can be compiled e.g. with the following:

.. code-block:: bash

    $ gcc -Wall -Werror -O2 -o sock1 sock1.c && ./sock1
    /tmp/ccf2I2NV.o: In function `main'
    sock1.c:(.text.startup+0x73): undefined reference to `handle_client'
    collect2: error: ld returned 1 exit status

Compilation succeeds but linking fails because the function "handle_client" isn't defined. If we implement that function correctly then we have a simple web server.

*Exercise*: Check out a man page of one of the BSD sockets API, for example by running "man shutdown".

Reading from a socket
=====================

Here's a short and simple implementation of the handle_client function:

.. literalinclude:: sock_read.c
   :language: c
   :linenos:

This function reads up to 1,023 bytes at once from the client and prints them out to stdout.

Because we now have a server which listens to connections to port 1234, we can try connecting to it using a web browser. We should then see what the web browser sends our program.

*Exercise*: Put together all the above server code and connect to 127.0.0.1:1234 with your browser. 127.0.0.1 means localhost, meaning the computer you're running.

HTTP
====

After doing the above exercise you should see a nice set of text that the web browser has sent our program. On my setup, the first two lines are these:

.. code-block:: bash

    GET / HTTP/1.1
    Host: 127.0.0.1:1234

The first line means that the browser would like to retrieve any data from the address / using the HTTP 1.1 protocol. The second line means that from the browser's point of view, it's connecting to 127.0.0.1:1234.

The full HTTP 1.1 protocol is described in RFCs 7230-7237. RFC's (Request for Comments) are documents maintained by W3C (World Wide Web Consortium).

*Exercise*: Look up RFC 7230 online. Don't read all of it, but try to get an overview of the HTTP 1.1 architecture.

Now that you've seen RFC 7230, you can probably tell that a simple server response has a few lines of text, beginning with a line such as "HTTP/1.1 200 OK", with the actual data for the user at the end.

C string handling
=================

Before we continue with implementing our web server, there are a couple of building blocks regarding string handling in C that you should know about. Let's first cover through some basics about strings in C.

C doesn't really have strings. What it has are buffers (allocation of continuous memory), chars or characters (one byte, representing one ASCII character, meaning English letters, numbers or other symbols) and pointers. Mixing these conceptually results in something like a string. See the below diagram which is a continuous buffer i.e. array with eight slots, each holding one byte (character):

.. code-block:: c

    +-----+-----+-----+-----+-----+------+---+---+
    | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' | ? | ? |
    +-----+-----+-----+-----+-----+------+---+---+

This is a buffer with eight slots containing the string "Hello". The sixth slot is a 0, or '\0', which indicates the end of a string. The last two slots are *undefined* and reading them results in *undefined behaviour* like crashing, garbage, nothing, or anything.

You can create such a buffer by e.g. doing the following:

.. code-block:: c
    :linenos:

    char my_array[8];
    sprintf(my_array, "Hello");

* Line 1: We allocate the buffer in stack.
* Line 2: We write the string "Hello" to the buffer.

In general, however, you may write something into a buffer where you don't know the length of the string in advance. What you'll need to do in any case is allocate a buffer large enough for your needs, but in addition it's always best practice to *clear* the memory in advance, to avoid undefined behaviour. You must also ensure you don't write *past the end* of the buffer because this will typically overwrite some of your other variables, probably crashing your program and also possibly creating a security hole. It's typically better to do this:

.. code-block:: c
    :linenos:

    char my_array[8];
    memset(my_array, 0x00, sizeof(my_array)); /* sizeof(my_array) will return 8 */
    snprintf(my_array, 7, "Hello");

* Line 1: We allocate the buffer in stack.
* Line 2: We clear the buffer, such that all the values in the buffer are 0. This way, as long as we keep the last character to 0 in the buffer (remember 0 indicates end of a string), we shouldn't be either reading or writing past the end of our buffer.
* Line 3: We write the string "Hello" to the buffer using *snprintf*, which takes as a parameter the maximum number of bytes to write. We tell it to write maximum seven characters such that the last one will always be 0.

Now, while array in general is not the same thing as a pointer, for strings the two are sometimes interexchangable. For example, let's assume you want to pass your array as a parameter to another function. You can't because arrays are always passed by reference in C. This means that the array will *decay* into a pointer:

.. code-block:: c
    :linenos:

    void my_function(const char *str, int len) {
        /* sizeof(str) will NOT give an answer as to how long the buffer is */
        snprintf(str, len - 1, "Hello");
    }

    int main(void) {
        char my_array[8];
        memset(my_array, 0x00, sizeof(my_array)); /* sizeof(my_array) returns 8 */
        my_function(my_array, sizeof(my_array));
    }

Here, "my_function" cannot by itself know how long the string (character buffer) pointed to by "str" is, and must take a second parameter "len" which must have this information. The "sizeof" operator only tells the size of the array (in bytes) at the site where the array is allocated, not where only a pointer is available.

Pointer arithmetic and substrings
=================================

Let's say we want to modify the array by a character. We can do this:

.. code-block:: c

    char my_array[8];
    memset(my_array, 0x00, sizeof(my_array));
    snprintf(my_array, 7, "Hello");
    my_array[0] = 'J'; /* my_array is now "Jello";
    my_array[4] = 'y'; /* my_array is now "Jelly";

How would we do this if we only had a char pointer, not the array itself? We can use *pointer arithmetic*:

.. code-block:: c

    void my_function(char *str) {
        *str = 'J';
        *(str + 4) = 'y';
    }

    int main(void) {
        char my_array[8];
        memset(my_array, 0x00, sizeof(my_array));
        snprintf(my_array, 7, "Hello");
        my_function(my_array);
    }

By *dereferencing* the pointer "str" with \* we can access individual characters in the buffer, and also assign to them.

By adding a number n to a pointer the resulting pointer points to data n elements after the first element, and by dereferencing it we can also assign to it.

As a diagram it looks like this:

.. code-block:: c

    +-----+-----+-----+-----+-----+------+---+---+
    | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' | ? | ? |
    +-----+-----+-----+-----+-----+------+---+---+
    .  ^                       ^
    . str                    str + 4

If one were to pass a char pointer to my_function which pointed to less than five bytes of allocated memory, *my_function* would cause undefined behaviour.

As my_function modifies "str", the parameter can't have the const qualifier.

String comparisons
==================

You can check if two strings are the same by using the "strncmp" function:

.. code-block:: c

    char *a;
    char *b;
    /* set a and b somehow */
    if(!strncmp(a, b, 20)) {
        printf("a and b are the same (at least the first 20 characters).\n");
    }

(You'll need to #include <string.h> for strncmp as well as most of the other string utility functions, including memset().)

If you want to compare only parts of a string, strncmp can do this too. Let's say you have a buffer, and you know its first letters are "HTTP/1.1 " but you want to know whether the status code is "200". You can do e.g.:

.. code-block:: c

    char *input_string = ... ;
    if(!strncmp(input_string + 9, "200", 3)) {
        printf("The status code is 200.\n");
    }

What happens here is that we use pointer arithmetic to skip the first nine characters ("HTTP/1.1 "), then compare the next three (and only three) characters with the string "200". strncmp() returns 0 if the strings matched for the given number of characters.

Another option would be to copy the relevant substring to its own buffer (assuming we don't want to modify the input string):

.. code-block:: c

    char *input_string = ... ;
    char buf[4];
    buf[3] = '\0'; /* ensure NULL termination */
    strncpy(buf, input_string + 9, 3);
    if(!strncmp(buf, "200", 3)) {
        printf("The status code is 200.\n");
    }

The function "strncpy" copies n bytes from a source buffer to a destination buffer.

Since it's only three characters were checking, we could also check them manually:

.. code-block:: c

    char *input_string = ... ;
    if(*(input_string + 9)  == '2' &&
       *(input_string + 10) == '0' &&
       *(input_string + 11) == '0') {
       printf("The status code is 200.\n");
    }

Finally, here's a snippet that puts some of this together:

.. code-block:: c
    :linenos:

    const char *my_string = "HTTP/1.1 200 OK\r\n"
                            "Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n";

    int ret = strncmp(my_string, "HTTP/1.1", 8);
    if(ret == 0) {
        printf("my_string begins with \"HTTP/1.1\"\n");
    }
    int my_string_len = strlen(my_string);
    if(my_string_len > 14) {
            ret = strncmp(my_string + 9, "200", 3);
            if(ret == 0) {
                printf("my_string has the characters \"200\" nine characters after the start.\n");
            }
    }
    int value = 42;
    char buf[8];
    memset(buf, 0x00, sizeof(buf));
    sprintf(buf, "%d", value);

Let's see what we have...

* Lines 1-2: In C, you can declare strings over multiple lines. The literal quotes will be simply concatenated at compile time. Also note the character sequence \r\n which means CRLF, or "carriage return, line feed".
* Line 4: The function strncmp() checks whether two strings are the same.
* Line 8: The function strlen() determines the length of a string.
* Line 9: Check that we won't be reading past the end of a string to avoid undefined behaviour or creating security holes.
* Line 10: By adding a number to a pointer (string), you can effectively start reading from a later point in a string.
* Lines 15-18: For turning an integer value to a string (character buffer), allocate a buffer large enough, clear it using memset() and finally use the sprintf() function to write the integer value as the contents of the buffer.

Another potentially useful function is strtok(). Here's an example of its usage:

.. code-block:: c
    :linenos:

    char *str = "this is a string.\n";
    char *p = strtok(str, " "); // p now points to "this"
    p = strtok(NULL, " ");      // p now points to "is"
    p = strtok(NULL, " ");      // p now points to "a"

With the above knowledge it should be possible to finish the next exercise. You may also find it interesting to take a look at the various man pages of the different functions.

*Exercise*: Modify the function handle_client to check if the client connecting appears to make a HTTP 1.1 GET request. In this case, respond with a valid HTTP 1.1 200 response, replying with a short message such as "Hello world". Make sure you set the Content-Length part of the response correctly. Connect to your server using your web browser to ensure you send the correct data. You can write data to the client by using the write() function, e.g. 'write(fd, "hello", 5);'. You can also write data piece by piece, by calling write() multiple times.

As you can now see, the browser will be able to render your text, which means you have the beginnings of a web browser. To make things more interesting, let's have another exercise.

*Exercise*: Create two HTML pages, with the first one linking to the second. (You can create a link in HTML by using the <a> tag; for example, the following creates a link to a page called two.html: <a href="two.html">link</a>.) For the request to /, serve the first HTML page by reading its contents to a buffer and then sending the buffer contents as part of the response. Again, make sure you set the Content-Length part of the response correctly. Parse the request path in detail, such that you'll be able to serve the second page when the browser requests it. Note: in order to have the browser display HTML properly, you'll need to set the Content-Type field to text/html.

At this stage our implementation doesn't respect the whole specification, but it's able to serve some web pages. If you made it here, congratulations.
