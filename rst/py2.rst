Object oriented programming
---------------------------

Object oriented programming is fairly common in Python, and we've used it pretty much from the beginning. See for example this code:

.. literalinclude:: ../material/py/with.py
   :language: python

Here, "f" is a file object and "write" is a *member function* of the file class.

In general, in many languages including Python, if you call a function such that the function syntactically appears to "belong" to a variable, like with "f.write()", then you're probably calling a member function of a class.

Defining your own data types
============================

In Python, data types are defined using the "class" keyword:

.. literalinclude:: ../material/py/cl1.py
   :language: python
   :linenos:

Let's go through this line by line:

* Line 1: We define a *class* named A. At a high level, a class is a data type, describing data types and functions common to the class.
* Line 2: We define the *init function*, or *constructor*: a function that will be automatically called when an object of this class is created.
* Line 3: The constructor will create a *member variable* named foo - a variable that's contained within the object.
* Line 5-6: We define a *member function* - a function that's part of a class.
* Line 8-9: This member function produces some output and also uses the member variable value.
* Line 11: We create (or *instantiate*) one object of class A. Class A has a constructor that requires one parameter: we pass in 42.
* Line 13: We call the member function "call_me" for class A.
* Line 14: We call the member function "add_one" for class A.
* Line 15: We call the member function "call_me" for class A.

Executing this will output:

.. code-block:: bash

    $ python2 cl1.py
    A called - my foo is: 42
    A called - my foo is: 43

Duck typing revisited
~~~~~~~~~~~~~~~~~~~~~

Here's a bit larger example:

.. literalinclude:: ../material/py/cl.py
   :language: python
   :linenos:

Let's go through this line by line:

* Lines 1-10: As in the example above.
* Line 11: We define another class named B.
* Line 12-13: We define a member function for class B - with the same name as we did for class A.
* Line 15: We define a list with two elements, and create (or *instantiate*) one object of each class; one of class A, one of class B. Class A has a constructor that requires one parameter: we pass in 42.
* Line 17: We iterate through our list.
* Line 18: We call the member function "call_me" for each element in the list, i.e. once for our object of class A, once for our object of class B.

Executing this will output:

.. code-block:: bash

    $ python2 cl.py
    A called - my foo is: 42
    B called

We see an example of duck typing here: while classes A and B technically have nothing to do with each other, they do share the same *interface*, namely calling the function "call_me". We also see that the classes in Python are more powerful than the structs in C: in Python we can combine multiple variables in one object, but in addition we can have member functions and use these to define an interface on interacting with the data.

