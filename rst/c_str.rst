C and strings
-------------

  The most effective debugging tool is still careful thought, coupled with judiciously placed print statements.

  -- Brian Kernighan

This section goes in depth around strings (and buffers in general) in C.

C string handling
=================

Before we continue with C, there are a couple of building blocks regarding string handling that you should know about. Let's first cover through some basics about strings in C.

C doesn't really have strings. What it has are buffers (allocation of continuous memory), chars or characters (one byte, representing one ASCII character, meaning English letters, numbers or other symbols) and pointers. Mixing these conceptually results in something like a string. See the below diagram which is a continuous buffer i.e. array with eight slots, each holding one byte (character):

.. code-block:: c

    +-----+-----+-----+-----+-----+------+---+---+
    | 'H' | 'e' | 'l' | 'l' | 'o' | '\0' | ? | ? |
    +-----+-----+-----+-----+-----+------+---+---+

.. topic:: Digression: what is ASCII?

  ASCII is the name of a mapping (encoding) between numbers and characters. ASCII defines that, for example, the letter 'H' is equivalent to the number 72, and vice versa. The letter 'h' (lower case) is equivalent to 104. ASCII also includes non-printable characters, e.g. line feed ("\\n") which is equivalent to number 10, or null ("\\0") which is equivalent to number 0. You can find the full encoding online. If you need non-English characters then other encodings exist, such as UTF-8.

This is a buffer with eight slots containing the string "Hello". The sixth slot is a 0, or '\\0', which indicates the end of a string. The value of this is in fact 0 but this has two representations: 0 (the number) or '\\0' (character). The last two slots are *undefined* and reading them results in *undefined behaviour* like crashing, garbage, nothing, or anything.

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
    snprintf(my_array, 8, "Hello");

* Line 1: We allocate the buffer in stack.
* Line 2: We clear the buffer, such that all the values in the buffer are 0. This way, as long as we keep the last character to 0 in the buffer (remember 0 indicates end of a string), we shouldn't be either reading or writing past the end of our buffer. (You will need to #include <string.h> to declare memset().)
* Line 3: We write the string "Hello" to the buffer using *snprintf*, which takes as a parameter the maximum number of bytes to write. We tell it to write maximum eight characters such that the last one will always be 0.

Now, while array in general is not the same thing as a pointer, for strings the two are sometimes interexchangable. For example, let's assume you want to pass your array as a parameter to another function. You can't because arrays are always passed by reference in C. This means that the array will *decay* into a pointer:

.. code-block:: c
    :linenos:

    void my_function(const char *str, int len) {
        /* sizeof(str) will NOT give an answer as to how long the buffer is */
        snprintf(str, len, "Hello");
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
    snprintf(my_array, 8, "Hello");
    my_array[0] = 'J'; /* my_array is now "Jello"; */
    my_array[4] = 'y'; /* my_array is now "Jelly"; */

How would we do this if we only had a char pointer, not the array itself? We can use *pointer arithmetic*:

.. code-block:: c

    void my_function(char *str) {
        *str = 'J';
        *(str + 4) = 'y';
        /* or equivalently: str[4] = 'y';         */
        /* or equivalently: str += 4; *str = 'y'; */
    }

    int main(void) {
        char my_array[8];
        memset(my_array, 0x00, sizeof(my_array));
        snprintf(my_array, 8, "Hello");
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

Digression: debugging
=====================

Let's assume you try to run your program, and it crashes. What's going on?

There are a few ways to find out. In the worst case, you simply get a segmentation fault, i.e. tried to access memory your program didn't have access to. There are a few ways to debug this:

1. Code inspection and hardening - going through the code and adding useful *assertions* where necessary.
2. Debug printf - inserting printf calls to various places in your code, seeing which one gets executed, allowing you to pinpoint the line that is the cause for the crash.
3. Using a *debugger* to show the root cause of the crash and the state of the program at the time of crash.

Assertions seem like going through in more detail. For example, if you have an int variable named "foo", and you assume it should always be between 0 and 5, you can use this code (after #including <assert.h>):

.. code-block:: cpp

    assert(foo >= 0 && foo <= 5);

Now, what happens is the program will always check, when executing the statement, whether your statement is true and if not, will immediately crash the program. This is helpful for detecting cases where your assumptions were wrong.

Finally, *debuggers* are programs which execute your program in a controlled environment with the ability to track and stop the program executiuon when necessary. One potentially useful debugger is *gdb* (or its clang counterpart, *lldb*). There are many ways to use it but one way is to get a *backtrace* of the function calls leading to the crash, i.e. all the function calls in the stack at the time the crash occurred. This can be achieved by following these steps:

  * Compile the program with "-g3" to get include debug data in the program which will be used by the debugger e.g. to display line numbers
  * Possibly do not compile with optimisations, i.e. do not compile with "-O2" as this may cause the debugger output to be very different
  * Instead of running the application with simply "./program abc", run "gdb --args ./program abc". This will launch gdb (assuming it's installed)
  * gdb will display a prompt, allowing you to enter commands. Simply enter the command "r" (for "run") and hit enter. This will run the program.
  * If the program crashes, gdb will let you know and also show the line that caused the crash. With the command "bt" ("backtrace") you can see the function stack leading up to the call.
  * You can exit gdb with "q".
      
Here's an example gdb session:

::

    Reading symbols from ./segv...done.
    (gdb) r
    Starting program: ./segv 10 10000

    Program received signal SIGSEGV, Segmentation fault.
    0x0000000000400825 in run (size=10, loop_size=10000) at segv.c:9
    9	        array[i] = i + array[i - 1];
    (gdb) bt
    #0  0x0000000000400825 in run (size=10, loop_size=10000) at segv.c:9
    #1  0x00000000004008c1 in main (argc=3, argv=0x7fffffffe7d8) at segv.c:18
    (gdb) 

Here we can see the program crashed at line segv.c:9, in function "run", which was entered from function "main" at segv.c:18.

Debuggers can do a lot more, e.g. set breakpoints, display variable names, and more.

*Exercise*: Write a function that will determine the length of a string. You can detect the end of a string by comparing a character in a string against 0, or '\\0': if it is 0 then it denotes the end of the string. (This exercise exists for educational purposes; the C standard library includes functions "strlen" and "strnlen" for this.)

*Exercise*: Write a function to count the number of occurrences of the character 'a' in a given input string.

*Exercise*: Extend your function from the previous exercise such that the character to count occurrences for is given as an additional input parameter.

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

If you want to compare only parts of a string, strncmp can do this too. Let's say you have a buffer, and you know its first letters are "HTTP/1.1 " but you want to know whether they are followed by the letters "200". You can do e.g.:

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
    buf[3] = '\0'; /* ensure string termination */
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

Another potentially useful function is strtok(). Here's an example of its usage:

.. code-block:: c
    :linenos:

    char *str = "this is a string.\n";
    char *p = strtok(str, " "); // p now points to "this"
    p = strtok(NULL, " ");      // p now points to "is"
    p = strtok(NULL, " ");      // p now points to "a"

Finally, the functions "strcat" and "strncat" append a string to an existing string:

.. code-block:: c

    char buf[256];
    memset(buf, 0x00, sizeof(buf));
    strncat(buf, "hello ", 255);
    strncat(buf, "world\n", 249);
    printf("%s", buf);

*Exercise*: Let's assume you have 50 words with five letters each and you append each word to a buffer using strncat, one after another. (The buffer is assumed to be large enough.) In terms of big O notation, what's the run time of this algorithm? In order to know where to append to, strncat() iterates through the destination buffer to find the end of the string every time it is called.
