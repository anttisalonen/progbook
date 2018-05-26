Unix way - merge
----------------

Now we have all pieces of our software except merge. Merge is expected to take the outputs of both sched and parse_gps and produce the input for display. The output will need to show the estimated bus arrival time where possible and the scheduled bus arrival time otherwise, and sort the buses based on the arrival time, i.e. interleave the estimated and scheduled arrival times as needed. Have a think about how you would design this software.

As before, let's break this down:

* Parse the command line parameters and read in the input data, either to a list of tuples or a list of classes
* Implement the core logic to handle the input data

  * If a bus is scheduled but the GPS data says it's already passed, don't show it
  * If a bus is scheduled and there is no GPS data, include the scheduled time in the output
  * If there is GPS data for a bus, include the estimated time in the output
  * Sort the output by time difference to current time (given as command line parameter)

* Output the final result according to the specified data format

There are a few interesting tidbits that arise during the implementation:

* The output will need to include the actual bus route names. The names for the routes are defined in the requirement specification but we haven't used them in our code before. An easy way to go about this is to define a Python dictionary where the keys are route numbers and values are route names. This way we can easily convert a route number to its name when needed.
* Because we need to sort the output by the arrival time, we'll be needing to calculate the time difference between two time stamps again. We already implemented this as part of sched. Instead of implementing this again, we can *reuse our old code* by running e.g. "import sched" and then calling the function e.g. "sched.diff_time".
* As part of the implementation, we'll need to ask the question "has a bus with this route number and start number already passed our bus stop?" Such questions can be dealt with by defining a Python set of buses (e.g. route number and start number tuples) e.g. with "my_passed_set = set(l)" where l is our list of buses that have passed the bus stop. One can then check whether a tuple exists in the set by e.g. using "(routenr, startnr) in my_passed_set". This expression will return true if the tuple exists in the set.
* The type of data entry (GPS based data, scheduled data, or passed) is a good candidate for an *enum*. An enum (short for enumeration) is a simple data type which simply assigns a name to a set of constants. In our case, the constant 0 means GPS based data, 1 means scheduled data and 2 means passed data. We can construct an enum in Python using the following syntax:

.. code-block:: python

    class Kind(object):
        GPS = 0
        Schedule = 1
        Passed = 2

Here, we define a class with three class variables. Now, we can simply e.g. type Kind.GPS in our code which means 0. This has the benefit that instead of having code where we have numbers like 0, 1 and 2 littered around, by typing e.g. Kind.Passed instead the code becomes easier to understand when reading it.

* We'll need to output the time stamps in proper time format. Python has support for this; e.g.:

.. code-block:: python

    print '%02d:%02d' % (hour, minute)

This print statement will print two integers with a colon in between, but if either of the integers are less than 10 it will *pad* the output by leading zeroes, such that e.g. the number 5 will be printed as 05.

Now we should have what we need to implement this program.

*Exercise*: Put together some mock-up data for merge. You can create this by running the sched and parse_gps programs. Modify the inputs of these programs such that you have a good input data set: a bus that is included in the schedule but has already passed; a bus that's included in the schedule but GPS data for a prediction exists as well; a bus that's included in the schedule but no GPS data for it exists.

*Exercise*: Implement merge as planned.

Gluing everything together
==========================

We now have a few programs where we can take inputs, generate outputs, provide these as inputs again etc. Let's put together a simple shell script that we can use to save us from repetitive work, and also to help document the flow. One way to go about this is to copy-paste all the commands you run to a file, add any boilerplate at the top, and then replace any repetitions with variables. You might end up with something like this:

.. literalinclude:: ../material/bus/run_ex.sh
   :language: bash
   :linenos:

Let's go through this line by line:

* Line 1: Shebang
* Lines 3-4: We set some bash settings to make our code safer: set -e causes the script execution to terminate immediately when a command returns with a non-zero exit code. This means that if we have a bug and a Python script raises an exception, the following commands will not be executed. This is often practical as the following commands may not be sensical if a previous command failed, making it more difficult to spot the actual issue. set -u causes bash to terminate the execution if a variable is used before it is set.
* Lines 6-9: We define some variables that reduce redundancy in our script.
* Lines 11-14: We run our program one after the other, resulting in the display program being run.

*Exercise*: Write a shell script or modify the above one to fit your environment so that you can run all your code in one command.

Makefiles
=========

Make is a traditional Unix program that runs commands in a sequence based on whether a file has been updated or not. It's often used to run a compiler to compile source files to an executable after source code changes. It can also run tests or anything else, depending on what you tell it to run. Here, we'll use it to compile our C++ program and run the script we just wrote.

You execute make by running "make" in your shell:

.. code-block:: bash

    $ make
    make: *** No targets specified and no makefile found.  Stop.

Make looks for a file called "Makefile" in your current directory and executes commands specified in it. As we have no makefile it won't do anything. Let's fix this by writing a makefile:

.. literalinclude:: ../material/bus/Makefile_ex
   :language: make
   :linenos:

Here, we define two *targets*. Normally, defining a target means that a file with the name of the target will be created by running the given command or commands. Our second target is a target called "bus": by running the compiler (g++), a file called bus is created. This is our C++ executable. We also tell Make that the file depends on sdl_sched2.cpp; this is our source file.

A caveat on Makefiles is that the whitespace before the commands *must* be a tab. Spaces are not allowed.

Our first target (the first target is also the default target for Make) is called "default". It depends on the bus executable and simply runs our shell script.

Now, with this makefile, running "make" gives the following:

.. code-block:: bash

    $ make
    g++ -Wall -g3 -O2 -I/usr/include/SDL2 -D_REENTRANT -L/usr/lib -lSDL2 -lSDL2_ttf -o bus sdl_sched2.cpp
    ./run_ex.sh
    $

In other words, it runs the C++ compiler to generate the executable "bus", followed by running our shell script, provided the compilation succeeds.

Now, if you were to run make again without making changes to the C++ source file, you get this:

.. code-block:: bash

    $ make
    ./run_ex.sh
    $

In other words, Make detects that recompiling the C++ file is not necessary and simply runs the script.

If you do want to run the C++ compiler without making changes to the source file, you can either simply save the file, or use the Unix command "touch", e.g. "touch sdl_sched2.cpp". Either action will update the file modification timstamp which Make uses to detect whether the command to generate the target needs to be re-run or not.

Make can be used for lots of things and there's *a lot* more to Make than this short section might suggest; but some important properties of Make were introduced here. The main takeaway is that Make can be used to drive the build and test process.

*Exercise*: Put together your Makefile to compile your C++ code as well as for running your Python and C++ programs.
