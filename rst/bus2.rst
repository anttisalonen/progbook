Monolithic way - parsing
------------------------

Now that we're to a large part done with the Unix way, let's shift focus on the monolithic way where we implement the functionality of our three Python programs in one C++ program.

The basic principle of designing the software top down still applies. Let's start with writing down all the things we need to do:

* Parse input files

   * With GPS data, as with the Python version, we'll need to be able to calculate the distance between two points

* Merge the GPS estimates and the scheduled times

   * This includes writing some code to handle timestamps, more specifically, to calculate the difference between timestamps

* Pass on the text labels to the SDL code that actually renders them

It may also make sense to think about what data types we will have. We'll have at least the following:

* Timestamp
* Historical GPS data
* Current GPS data
* Schedule entry
* Arriving bus entry (either estimated via GPS or scheduled)
* Group of text labels

In C++ (and in many other languages) it often makes sense to think of each kind of data type as a class. For example, we could have a class for timestamp, with member variables "hour" and "minute" (both integers), and a function that returns the difference in minutes between two given timestamps.

*Exercise*: Think about the data types listed above. What data (member variables) would they hold? What operations would exist for them?

Timestamps
==========

We can then go about implementing each of these classes, one by one, along with the relevant logic. For example, for a timestamp, we could come up with the following class definition:

.. code-block:: cpp

    class Time {
        public:
            Time();
            Time(int h, int m);
            int hour;
            int minute;
    };

Here we define the Time class with two public member variables (as they're simple data, there's arguably no need to hide the data by making it private) and two constructors. The first constructor should simply generate a timestamp object with both hour and minute set to 0, and the second constructor should set the hour and minute as given. The functions for this class could be defined e.g. as the following:

.. code-block:: cpp

    Time::Time()
        : hour(0),
        minute(0)
    {
    }

    Time::Time(int h, int m)
        : hour(h),
        minute(m)
    {
    }

We now have our (arguably very simple) first class. We already touched upon a function to calculate the difference between two timestamps; while we could have such a function as a member function for the Time class, it's also possible to have this as a free standing function. If we implement it as such, it would have the following declaration:

.. code-block:: cpp

    int time_diff(const Time& t1, const Time& t2);

In other words, a function that takes two constant Time objects and returns an integer.

*Exercise*: Implement this function and test it.

Schedule entry
==============

The requirements spec defined the schedule entry to have a route number, a start number and a timestamp. We've identified one task as parsing the schedule information, in other words, reading the relevant file contents and generating a list (std::vector) of schedule entries. For this we need to do the following:

* Declare the schedule entry class
* Write the functionality to convert a text line to a schedule entry class
* Read in a file and call use abovementioned functionality in a loop to generate an std::vector of schedule entries

Let's start with the first one.

*Exercise*: Declare the schedule entry class. It should have a route number, a start number and a timestamp as member variables. The first two can be integers while we should instantiate a Time object for the last one.

Now, converting a string which contains space delimited numbers to integers is funny business in C++. A quick online search shows us one way to do this:

.. code-block:: cpp
    :linenos:

    #include <string>
    #include <sstream>

    void func()
    {
        std::string line;
        // TODO: set contents of line appropriately
        std::istringstream iss(line);
        int route_nr;
        int start_nr;
        int hour;
        int minute;
        if(!(iss >> route_nr >> start_nr >> hour >> minute)) {
            throw std::runtime_error("Could not parse data");
        }
        // TODO: use our integers here
    }

Let's go through this line by line:

* Lines 1-2: include string and sstream (stringstream) which we'll be needing shortly.
* Line 6: the variable line contains our input data.
* Line 8: We convert the data from an std::string to std::istringstream. Istringstream allows reading integers from it fairly easily (using the >> operator).
* Lines 9-12: We define our variables which will hold the integers we'll read in.
* Line 13: We use the >> operator to read in four integers. The operator will return false if something went wrong during parsing (for example, the input data contained letters). In this case, we throw an exception.

How would you get each line from a file in C++? Another online search reveals a way:

.. code-block:: cpp
    :linenos:

    #include <iostream>
    #include <string>
    #include <fstream>

    void func()
    {
        std::ifstream infile("input_file.txt");
        std::string line;
        while(std::getline(infile, line)) {
            // TODO: use line here
        }
    }

Let's go through this line by line as well:

* Lines 1-3: Import iostream, string and fstream (file stream).
* Line 7: Instantiate an object of type std::ifstream named infile. We pass the file name to its constructor as the parameter. This object represents an input file we can read data from.
* Line 8: Declare an std::string which will hold each line of the file as we read them in.
* Line 9: Use the C++ standard function std::getline() which takes two parameters: the input file stream and an std::string. The contents of the next line will be read from the input stream and stored in the string. The function will return false when the whole file was processed, allowing us to use it in a while loop.

We should now have everything we need in order to read in the schedule data and generate an std::vector of schedule entry objects.

*Exercise*: Generate an std::vector of schedule entry objects by reading in the schedule data file. Test your code.

While we're in the parsing business, let's go ahead and repeat this for the historical GPS data file.

*Exercise*: Implement a class to hold the historical data. It will need to have some floats as member variables to hold the time it took the bus to reach the bus stop as well as the X and Y coordinate data. Read in a historical GPS data file to an std::vector of historical data objects.

As per our requirements specification, the only file type we aren't yet able to parse is the current GPS data. This data is interesting because it has the integer representing whether the bus has already passed our bus stop or not (0 if not, 2 if passed). As with our Python code, it seems like a good way to make our code clearer to read if we use an enumeration for this. Enums in C++ can e.g. be defined as the following:

.. code-block:: cpp

    enum class Kind {
        GPS,
        Schedule,
        Passed
    };

This defines a new data type called Kind which is an enumeration: it can only have a value GPS, Schedule, or Passed. (In C, the keyword "class" would have to be left out; it can also be left out in C++ but including it improves type safety by prohibiting implicit conversions between the enum and int, potentially reducing bugs).

You can then define and set a variable of this type e.g. with the following:

.. code-block:: cpp

    Kind my_variable = Kind::GPS;

You can also convert an integer to Kind. For example:

.. code-block:: cpp

    int val = 1;
    Kind my_variable = Kind(val);
    // my_variable is now Kind::Schedule

By default, with our above definition of the data type, the value 0 is converted to Kind::GPS, value 1 to Kind::Schedule, and value 2 to Kind::Passed.

We should now be able to define a class to hold the current GPS data, and parse a file holding such data.

*Exercise*: Define a class for current GPS data. It should include a member variable of type enum class Kind. Write code to read in a file of current GPS data to generate an std::vector of objects of your class, and test your code.

Now that we're able to parse in all the data we need, it seems that, apart from any necessary glue code, the actual logic we need to implement is reduced to:

* Calculating the estimated arrival times from GPS data
* Reading in the scheduled bus arrivals based on the current time
* Merging the estimated (and already passed) bus arrivals with the scheduled arrivals
* Parsing the command line arguments - including parsing time from a string "hh:mm" to a timestamp
* Converting timestamps to strings of format "hh:mm" for displaying purposes and calling the relevant function to display the labels

We'll address these in the next section.
