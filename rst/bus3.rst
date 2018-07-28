Monolithic way - scheduled arrivals and GPS data
------------------------------------------------

In the previous section we wrote the code to read in all the necessary data. Now let's use some of it.

We want to write the logic that takes two things as input:

* The scheduled arrivals
* Current time of day

And, like sched in our Python version, outputs the next 50 buses that will be arriving at the bus stop. The output will be merged with the output of the GPS data analysis.

To start with, let's define the output data type. We've already defined a few - schedule entries, historical data, current GPS data - but none of them seem to match the requirement of encoding the arrival time, route number, start number and kind, i.e. the fact that this data results from the bus schedule (or indeed arriving or passed bus based on GPS data). While we could organise this differently, by e.g. using the schedule entry data type, as per our original architecture diagram we can simplify the merging logic by ensuring the data format for both GPS and schedule data is the same.

*Exercise*: Write a class that includes the arrival time, route number, start number and kind as member variables.

Filtering
=========

We can now write a function that does three things:

* From a list of all scheduled arrivals, filter only those that arrive e.g. in the next 12 hours
* Sort this filtered list by how long it takes for the bus to arrive
* Keep only the first 50 elements of the list (slice)

In Python, this could look e.g. something like the following:

.. code-block:: python

    l = [entry for entry in l if time_diff(current_time, entry.time) > 0]
    l = sorted(l, key=lambda entry: entry.time)
    l = l[:50]

In C++, we can conceptually do the same, albeit with a bit more typing.

We can filter in C++ by #including <algorithm> and using std::remove_if, e.g.:

.. code-block:: cpp

    /* my_array is the input list */
    /* curr_time is the current time */
    std::remove_if(my_array.begin(), my_array.end(),
            [&](const auto& bi) {
                return time_diff(curr_time, bi.time) < 0;
            });

Here, we call the function remove_if with three parameters:

1. The start iterator - beginning of our array (or list, or any collection)
2. The end iterator - end of our array
3. The lambda function which works as the filter. The lambda function itself consists of three elements:

   1. [&] - this tells the compiler which local variables to capture, and how. In this case we need to capture the curr_time as it's a local variable used in the lambda function, and we don't wish to copy it but instead refer to it, which is expressed with [&].
   2. (const auto& bi) - this is the parameter to our lambda function. It's the same type as the elements in our array my_array. We can use the keyword "auto" instead of explicitly naming the type.
   3. "return time_diff(curr_time, bi.time) < 0;" - the function body itself. This will result in removing all elements where our function time_diff returns a value less than zero.

*Exercise*: Look up the C++ reference for std::remove_if.

Alternatively, primitives such as for-loops and vector::push_back() can be used instead.

*Exercise*: Write the beginning of our sched function - a function that takes the schedule data that was parsed and a timestamp, and returns bus arrivals, namely those arriving next. The result doesn't yet need to be sorted. Run your code.

Sorting
=======

We haven't yet sorted data in C++ so let's fix that. Sorting can be done e.g. like this:

.. code-block:: cpp

    std::sort(my_array.begin(), my_array.end(),
        [](const auto& b1, const auto& b2) {
            return time_diff(b2.time, b1.time) < 0;
        });

Again we provide three parameters: two iterators for the begin and the end, and the lambda function that defines the sorting order. Here, we sort based on the time difference between entries.

*Exercise*: Look up the C++ reference for std::sort.

*Note*: std::sort assumes the function parameter describes the order as strict weak ordering. If this is not the case then undefined behaviour will occur. This means that e.g. in the above, if the operator <= was used instead of <, then the code may crash when executed.

*Exercise*: Include sorting in your sched function.

Slicing
=======

Now, let's tackle the final hurdle of only keeping the first 50 elements of our array (or list). We already touched upon the erase-remove idiom with our Sudoku exercises, and keeping the first elements is similar:

.. code-block:: cpp

    if(my_array.size() > 50)
        my_array.erase(my_array.begin() + 50, my_array.end() - 1);

Here, we first ensure the iterator my_array.begin() + 50 is valid, and if so, we pass the range of elements we want to remove to std::vector::erase, namely the start and end iterator. If your code uses std::list, the above snippet is still valid but will call std::list::erase instead.

*Exercise*: Include the code to remove excessive elements from your container in your sched code.

We now have the code in place to find the next buses to arrive at our bus stop, according to the bus schedule.

Handling GPS data
=================

For GPS data, as with our Python code, we need to do the following:

* For all current GPS data, find the relevant historical data points - i.e. data points with the same route and within 100 meters of the current position
* For all the relevant historical data points, calculate the average time it took for the bus to arrive, and associate this time with the current GPS data point
* Sort the current GPS data by the average time, and keep the first 20 points
* In addition to the buses that seem to arrive soon, output the list of buses that have already passed according to the GPS data

Now, the main new thing is *associating* a time with the GPS data point. We already have a class representing a current GPS data point. We could define a new class that e.g. includes an object of such a class as a member variable, and has the average time based on historical data as another member variable. Another way to do this is to use *tuples*: ad-hoc combinations of multiple data types in one. Here's an example of using tuples in C++):

.. code-block:: cpp
    :linenos:

    #include <iostream>
    #include <tuple>

    class C {
        public:
            C(int a, int b) : m_a(a), m_b(b) { }
            int m_a;
            int m_b;
    };

    int main()
    {
        std::tuple<C, int> c = std::make_tuple(C(1, 2), 3);
        std::cout << std::get<0>(c).m_a << "\n"; // prints 1
        std::cout << std::get<1>(c) << "\n";     // prints 3
    }

Let's see what we have:

* Line 2: We #include <tuple> as is needed when working with tuples
* Lines 4-9: We define a class called C. It has one constructor which takes two parameters, a and b. It also has two member variables, m_a and m_b. They are initialised when the constructor is run. Because it has a constructor defined, it has no *default constructor*, i.e. the two values need to be passed to the constructor whenever an object of class C is instantiated.
* Lines 11-16: We define the main function.
* Line 13: We define our tuple. It's a tuple of C and int. We create it using the std::make_tuple() function, which takes an object of type C and an int as is required from the tuple type definition. We instantiate an object of type C with values 1 and 2.
* Line 14: Using std::get<0> we can access the first element in the tuple, i.e. the value of type C. We can then access its public member variable using ".m_a", hence printing 1.
* Line 15: We can access the second element in the tuple using std::get<1>.

Similarly to any other type, you can have a vector of tuples. This could be defined and used e.g. like this (after #including <vector>):

.. code-block:: cpp

    std::vector<std::tuple<C, int>> my_array;
    my_array.push_back(std::make_tuple(C(4, 5), 6));
    std::cout << std::get<0>(my_array[0]).m_b << "\n"; // prints 5

*Exercise*: Look up the C++ reference for tuple.

*Exercise*: Implement a function that takes a current GPS data point and the historical GPS data and will return the average time of the historical data points within 100 meters of the current GPS data point, or a sentinel value (e.g. -1.0) if no historical data points within 100 meters were found.

*Exercise*: Use the above function to capture, for each current GPS data point, what the predicted time until arrival is, or whether the bus has already passed.

*Exercise*: Sort your resulting data based on the time until estimated arrival. Convert your data to the same data type as your code that works with the scheduled arrivals. You can do this by adding the time until estimated arrival with the current time of day. Also include the passed buses in your output.

Digression: parameters and god classes
======================================

We've now written a few functions which take several different kinds of data as input and return several kinds of data as output. For example, the above GPS analysis function will need, in addition to the current and historical GPS data, the current time of day, and will return an array of incoming bus data. While it's typically no problem writing such functions, there's another way to organise such code: instead of passing all the data as parameters we can define a class which has all the necessary data as member variables and write the logic as member function or functions. With this scheme, all the functions always have access to the data, making passing or returning data unnecessary.

This has the benefit of potentially simplifying the code, but the downside of breaking code modularity - because all code has access to all data, it may quickly become unclear what each function does and how it depends on other functions without inspecting the code of other functions. Because of this, it's generally better practice to isolate the different functions from each other and not have so called "god classes" - so called because they see and have access to everything. Never the less, in some cases writing a god class may be easier and quicker than splitting all the logic to isolated functions.

We now have the code to read in all our schedule and GPS data. What's left is merging this data and final touches regarding command line option parsing and label output.
