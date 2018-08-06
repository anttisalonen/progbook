Writing a toy web server
------------------------

After having used Flask a bit, you might have wondered "what's actually happening?"

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

The layers 5, 6 and 7 are all summarised by HTTP. When we ran Flask, we ran its built-in HTTP server.

Now, we've used OS functions before: When we open a file in C, we call the open() or fopen() functions, which end up calling kernel functions. The kernel functions typically interact with the actual hardware; in case of opening a file, the kernel functions would read data from the physical hard drive in the computer. Similarly, when writing a HTTP server, "all" we need to do is call the relevant kernel functions that trigger TCP/IP communications. Let's try this out.

BSD sockets API
===============

The API for accessing the TCP/IP stack is called the BSD sockets API. (BSD stands for "Berkeley Software Distribution"; the API originates from the University of California, Berkeley.) Here's a simple example using it:

.. literalinclude:: ../material/httpserv/sock1.c
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

  What the error tells us is that the function bind() expects a pointer to *struct sockaddr* but we pass it a pointer to *struct sockaddr_in*. Because of the way the API is specified (in the case of IP communication it actually required *struct sockaddr_in* despite what the function declaration says), we cast the type to *struct sockaddr*. Type casting basically means telling the compiler "please pretend this variable has a different type than what it actually has". Having this possibility in C makes C a weakly typed language.

.. topic:: perror?

    Most if not all standard C library functions, when they fail, return -1 and set a *global* variable called "errno" (error number). The user is expected to check if the return value is -1 and, if so, the variable errno can be used to obtain more information about the error. What the function perror() does is simply write out a textual description of this variable.

The code listing above can be compiled e.g. with the following:

.. code-block:: bash

    $ gcc -Wall -Werror -o sock1 sock1.c && ./sock1
    /tmp/ccf2I2NV.o: In function `main'
    sock1.c:(.text.startup+0x73): undefined reference to `handle_client'
    collect2: error: ld returned 1 exit status

Compilation succeeds but linking fails because the function "handle_client" isn't defined. If we implement that function correctly then we have a simple web server.

*Exercise*: Check out a man page of one of the BSD sockets API, for example by running "man shutdown".

Reading from a socket
=====================

Here's a short and simple implementation of the handle_client function:

.. literalinclude:: ../material/httpserv/sock_read.c
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

The full HTTP 1.1 protocol is described in RFCs 7230-7237. RFCs (Request for Comments) are documents maintained by W3C (World Wide Web Consortium).

*Exercise*: Look up RFC 7230 online. Don't read all of it, but try to get an overview of the HTTP 1.1 architecture. Especially note the example message exchange for a GET request.

Now that you've seen RFC 7230, you can probably tell that a simple server response has a few lines of text, beginning with a line such as "HTTP/1.1 200 OK", with the actual data for the user at the end.

Refresher on C string handling
==============================

Here's a snippet that puts some of the C string handling together:

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

* Lines 1-2: In C, you can declare strings over multiple lines. The literal quotes will be simply concatenated at compile time. Also note the character sequence \\r\\n which means CRLF, or "carriage return, line feed".
* Line 4: The function strncmp() checks whether two strings are the same.
* Line 8: The function strlen() determines the length of a string.
* Line 9: Check that we won't be reading past the end of a string to avoid undefined behaviour or creating security holes.
* Line 10: By adding a number to a pointer (string), you can effectively start reading from a later point in a string.
* Lines 15-18: For turning an integer value to a string (character buffer), allocate a buffer large enough, clear it using memset() and finally use the sprintf() function to write the integer value as the contents of the buffer.

With the above knowledge it should be possible to finish the next exercise. You may also find it interesting to take a look at the various man pages of the different functions.

*Exercise*: Modify the function handle_client to check if the client connecting appears to make a HTTP 1.1 GET request. In this case, respond with a valid HTTP 1.1 200 response, replying with a short message such as "Hello world". Make sure you set the Content-Length part of the response correctly. Connect to your server using your web browser to ensure you send the correct data. You can write data to the client by using the write() function, e.g. 'write(fd, "hello", 5);'. You can also write data piece by piece, by calling write() multiple times.

As you can now see, the browser will be able to render your text, which means you have the beginnings of a web server. To make things more interesting, let's have another exercise.

*Exercise*: Create two HTML pages, with the first one linking to the second. (You can create a link in HTML by using the <a> tag; for example, the following creates a link to a page called two.html: <a href="two.html">link</a>.) For the request to /, serve the first HTML page by reading its contents to a buffer and then sending the buffer contents as part of the response. Again, make sure you set the Content-Length part of the response correctly. Parse the request path in detail, such that you'll be able to serve the second page when the browser requests it. Note: in order to have the browser display HTML properly, you'll need to set the Content-Type field to text/html.

At this stage our implementation doesn't respect the whole specification, but it's able to serve some web pages. If you made it here, congratulations.
