JSON
----

JSON stands for JavaScript Object Notation but is used a lot outside Javascript.

It's a data format for storing any basic data, is fairly simple and is often used for exchanging data between different programs (often implemented in different programming languages) or more generally storing the output of a program.

Here's a simple JSON file:

.. code-block:: js

    {
        "key": "value"
    }

This JSON file contains one key named "key" which has the value "value". JSON can store objects (collections of key-value pairs) like above, numbers, strings and lists. Here's a more complex JSON file:

.. code-block:: js

    {
         "my_list": [
             1,
             2,
             3,
             4
         ],
         "alphabet": {
             "a": 1,
             "b": 2,
             "c": 3
         }
    }

Here we have an object with two keys: my_list and alphabet. The item my_list is a list with four numbers as elements. The item alphabet is an object with three key-value pairs. Because objects and lists can include lists or other objects, JSON is very flexible and can support arbitrarily complex data. The top level item in JSON must be either an object or a list. In professional settings, it's not uncommon to work with JSON files with thousands of entries.

One of the main strengths about JSON is that pretty much all main languages have support for reading and writing JSON. Here's an example Python code to read and use the above JSON file:

.. code-block:: python

    import json

    with open('data.json', 'r') as f:
        data = json.load(f)
    print len(data['my_list'])
    print data['alphabet']['c']

The above will print 4 and 3 (the length of the list and the value for key 'c' respectively). Here's an example Javascript code to read and use the same JSON file:

.. code-block:: js

    var parsed = JSON.parse(data);
    console.log(parsed['my_list'].len);
    console.log(parsed['alphabet']['c']);

Here's an example Python code to generate this JSON file:

.. code-block:: python

    import json

    data = dict()
    data['my_list'] = [1, 2, 3, 4]
    data['my_alphabet'] = {'a': 1, 'b': 2, 'c': 3}
    print json.dumps(data, indent=4)

Or in Javascript:

.. code-block:: js

    var data = {};
    data.my_list = [1, 2, 3, 4];
    data.alphabet = {'a': 1, 'b': 2, 'c': 3};
    console.log(JSON.stringify(data));

JSON vs. delimiter separated data
=================================

In our code where we read in the 10,000 functions we used delimiter separated data (delimiter being the space character in our case). In other words, our data looked like this:

.. code-block:: bash

    7.269553 3.427526 6.633603
    1.980206 -3.655827 -2.629755
    -8.687820 -6.930905 -8.731439
    -0.608791 -8.126272 -8.652504
    [...]

This is fine for many cases but it has some pros and cons when compared to JSON, namely:

* Delimiter separated data is easier to work on with standard Unix tools than JSON
* JSON provides more flexibility in terms of nesting data or variable length lists
* If no JSON library is available, parsing delimiter separated data is usually easier

If we were to store this data in JSON, we could use for example:

.. code-block:: js

    [
        {
            'a': 7.269553,
            'b': 3.427526,
            'c': 6.633603
        },
        {
            'a': 1.980206,
            'b': -3.655827,
            'c': -2.629755
        },
        ...
    ]

That is, a list of objects, whereby each object has the keys a, b and c. If we wanted to parse and use this in Python we could e.g. do this:

.. code-block:: python

    import json

    with open('data.json', 'r') as f:
        data = json.load(f)    # data is now a list with 10,000 elements
    print len(data) # would print 10000 for 10,000 objects in the list
    for function in data:
        print function['a'], function['b'], function['c'] # prints out the values for each function

Another way to store the same data would be:

.. code-block:: js

    [
        [7.269553, 3.427526, 6.633603],
        [1.980206, -3.655827, -2.629755],
        ...
    ]

That is, a list of lists, whereby each inner list always has three entries. We could then use this data in Python e.g. like this:

.. code-block:: python

    import json

    with open('data.json', 'r') as f:
        data = json.load(f)    # data is now a list with 10,000 elements
    print len(data) # would print 10000 for 10,000 elements in the list
    for function in data: # function is a list of three floats
        print function[0], function[1], function[2] # prints out the values for each function

Whichever way the data is structured is mostly a matter of personal preference, though the latter in some ways has less room for error as there are no keys to type.

*Exercise*: Rewrite the Python program that generates 10,000 functions to generate JSON instead. You can simply put your data in a list, which you'd then need to write out using json.dumps(my_list, indent=4). You can initialise a list e.g. by stating my_list = list(), and append to a list using my_list.append(item). You can also create a dictionary e.g. by stating my_dict = dict(), and setting a value by stating my_dict['key'] = value.

*Exercise*: Modify your Python program that reads in 10,000 functions to read from your JSON file instead.


