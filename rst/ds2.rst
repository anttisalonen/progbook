More data structures
--------------------

Previously we had a section on arrays. Recall that arrays are a continuous block of memory and have fast random access but slow insertion. This section goes through some more interesting data structures.

Linked lists
============

Linked lists solve the problem of inserting an element in the middle. They're constructed such that each element in a list holds the data as well as a pointer to the next cell in the list, whereby each cell is individually allocated.

.. code-block:: c

    +--------+--+      +--------+--+      +--------+---+
    | data 1 | -+----->| data 2 | -+----->| data 3 | x |
    +--------+--+      +--------+--+      +--------+---+
    .    ^
    .   Head 

The "head" pointer points to the beginning of the list. The last element will have the "next" pointer point to NULL (denoted by "x" in the diagram above), signalling the end of the list. If one were now to add an element in the list, this can be done by changing the "next" pointer of the previous cell in the list to point to the new element and the "next" pointer of the new element to the cell that was previously pointed to by the previous cell.

In practice, due to the overhead of allocating memory for individual cells and the performance issues caused by memory fragmentation and poor cache locality, linked lists rarely show any performance benefit over arrays.

Apart from *singly linked lists* like above, there are also *doubly linked lists*, where each element not only has a pointer to the next element but also to the previous one. These allow traversing in both directions in the list for the extra cost of storing a pointer, and maintaining this on element insertion and deletion.

Sets
====

A set is a data structure that can hold different values of data and efficiently answer the question whether a value is contained in the set or not. A typical way to achieve this is to implement a *balanced binary tree*, i.e. a tree structure which can be traversed top-down when doing basic operations such as looking up or inserting data.

.. code-block:: c

  .             -------------
  Root node --> | 4 | . | . |
  .             ------|---|--
  .                   |   |
  .        ------------- -------------
  .        | 1 | x | . | | 5 | x | x |
  .        ----------|-- -------------
  .                  |
  .        -------------
  .        | 3 | x | x |
  .        -------------

Here, the root node is the entry point to the tree. Each node has payload data as well as two pointers, one to the left and one to the right. The pointer to the left points to a node where the payload data is less than in the current node. The pointer to the right points to a node where the payload data is more than in the current node. In this example set we have stored number 1, 3, 4 and 5. If one were now to ask, "do we have number 3 stored in the set", a function answering this would do the following:

1. Root node value is 4, which is more than 3, so enter the left node (as it is not NULL).
2. Our next node value is 1, which is less than 3, so enter the right node (as it is not NULL).
3. Our next node value is 3, which is the value we were looking for, so the function can return *true*.

If we were to ask whether the number 6 is in the set, the function would do the following:

1. Root node value is 4, which is less than 6, so enter the right node (as it is not NULL).
2. Our next node value is 5, which is less than 6, but the right node pointer is NULL, hence the value 6 is not included, so the function must return *false*.

Inserting a number is trickier but, similarly to lookup, can be performed in O(log n) time. (To support efficient insertion, a *self balancing binary search tree* such as a red-black tree is required.)

C doesn't have built in support for sets (although C++ does). In Python, sets can be defined and used in the following manner:

.. code-block:: python

    >>> my_set = set()
    >>> my_set.add(1)
    >>> my_set.add(3)
    >>> my_set.add(4)
    >>> my_set.add(5)
    >>> 3 in my_set
    True
    >>> 6 in my_set
    False

*Exercise*: Look up the definition of a red-black tree online.

Dictionaries
============

Dictionary, also called a *hash map*, is similar to a set but has a *value* associated with each *key* stored in the map, with the key playing the same role as the payload data did for sets.

Hence it can have a similar internal structure to a set, but with another pointer in each cell indicating the value for the key.

Apart from a binary search tree, another way to implement dictionaries is to use a *hash function* to hash the data, i.e. generate an index (or *bucket*) for each data point and use this index to retrieve the data. For example, if we have keys 1, 3, 4 and 5 in our dictionary, we could *hash* these to indices 0, 1, 2 and 3 of an array. Now, when the user asks for the value for the key 1, we access our array at index 0 and return the corresponding data.

(As an aside, technically, as sets are very similar to dictionaries - the only difference being that sets don't have a value associated with each key - sets can also be implemented using a hash function instead of a binary search tree.)

In practice, the *hash function*, i.e. the function which generates this mapping from keys to indices, isn't perfect (unless all keys are predefined) and there will need to be more indices in the array than keys, and two or more keys may use the same index, requiring the implementation to handle this case (*hash collision*), for example by storing a linked list for each index, with each element in the linked list corresponding to one key-value pair. These complexities lead to the worst case insertion (where all indices have to be regenerated) to have O(n) runtime. Search can also have O(n) worst case runtime in the case where all keys end up in a single index, such that the search degenerates to a search in a linked list.

C doesn't have built in support for dictionaries (although C++ does). In Python, dictionaries can be defined and used in the following manner:

.. code-block:: python

    >>> my_dict = dict()
    >>> my_dict['a'] = 1
    >>> my_dict['b'] = 2
    >>> 'a' in my_dict
    True
    >>> my_dict['a']
    1
    >>> my_dict.get('c', -1) # for get(), the last parameter is the default if the key is not found
    -1
    >>> del my_dict['b']
    >>> 'b' in my_dict
    False
    >>> try:
    ...     print my_dict['d']
    ... except KeyError:
    ...     print 'not found'
    ...
    not found

(This example also demonstrates Python exception handling and the Pythonic EAFP ("easier to ask for forgiveness than permission") principle as well as exceptions: it's typically cleaner code to try to access a key in a dictionary and handle the error if the key is not found than check beforehand whether the key is in a dictionary and only access it if it is.)

Priority queues
===============

A priority queue is a data type where each element added to it has a priority (e.g. an integer), and retrieving the element with the highest priority is typically a fast operation. It supports adding elements with a given priority and removing the element with the highest priority. Priority queues are often implemented as *heaps*, which are a kind of a tree data structure (often a binary tree) with the element with the highest value as the root of the tree.

Summary
=======

Finally, here's a summary table of the performance of the different operations:

+--------------------------------------------------------------+----------------------------------------------+------------------------------------------+
| Data type                                                    | Access                                       | Insertion                                | 
+==============================================================+==============================================+==========================================+
| Array                                                        | O(1)                                         | O(n)                                     |
+--------------------------------------------------------------+----------------------------------------------+------------------------------------------+
| Linked list                                                  | O(n)                                         | O(1)                                     |
+--------------------------------------------------------------+----------------------------------------------+------------------------------------------+
| Set or a dictionary (implemented using a binary search tree) | O(log n)                                     | O(log n)                                 | 
+--------------------------------------------------------------+----------------------------------------------+------------------------------------------+
| Set or a dictionary (implemented using hashing)              | O(1) on average (O(n) in the worst case)     | O(1) on average (O(n) in the worst case) |
+--------------------------------------------------------------+----------------------------------------------+------------------------------------------+
| Priority queue (implemented using a binary tree)             | O(1) (only for the highest priority element) | O(log n)                                 |
+--------------------------------------------------------------+----------------------------------------------+------------------------------------------+

