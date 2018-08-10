Array, stack, queue
-------------------

For reasons that will soon become apparent, it's useful to have a basic understanding of data structures. This section will cover arrays, stacks and queues.

Arrays
======

We've already used *arrays* in C: Array is a data structure which contains multiple elements of data, such that each element uses the same amount of memory and they're allocated contiguously:

.. verb::

  +---+---+---+
  | 1 | 2 | 3 |
  +---+---+---+

As you can possibly imagine, accessing an element in an array is fast (O(1)) because you can calculate the address of an element using the following formula:

.. code-block:: c

    address(element_index) = base_address + element_index * element_size

However, if you wanted to insert an element in an array, you have to move all elements after the new element one slot to the right, making inserting a relatively complex (O(n)) operation. Similarly, were one to ask "do I have value x in the array", one would have to traverse the whole array, making this an O(n) operation as well.

You can define an array in C with the following syntax (for example, an array with 5 ints):

.. code-block:: c

    int my_array[5];

Lists in Python are technically also arrays but each elements holds a pointer to arbitrary data, such that in Python, each element in a list can be of different type.

Stack
~~~~~

Typically the memory layout for a stack is the same as for an array but the usage pattern is different.

You can imagine stack as an array where you only have visibility over the last element in the array: you can read it, pop it (redefine "last element" as one to the left), and push an element (redefine "last element" as one to the right, and set its value). 

Stack is also called "LIFO" - last in, first out. Here's an example "animation" of using a stack:

.. verb::

  +---+---+---+
  | 1 | 2 | x | # push(3)
  +---+---+---+
        ^ end of stack

  +---+---+---+
  | 1 | 2 | 3 | # pop() - returns 3
  +---+---+---+
            ^ end of stack

  +---+---+---+
  | 1 | 2 | 3 |
  +---+---+---+
        ^ end of stack

* Initially, in this example, we have two elements in the stack (1 and 2). In total our array has three elements, such that the stack can't hold any more. If we try to push any more in the stack then we get a *stack overflow*. ('x' denotes unknown memory.) The caret ("^") represents a pointer to the end of stack.
* We then push 3 in the stack, meaning we set the value of the third element to 3 and note that the end of the stack is now on the third element.
* We then pop an element from the stack, meaning we set the end of the stack to the second element. The 3 is still in the memory but not treated as part of the stack.

.. topic:: I've heard of stack overflow before

  In computers (mobile phones, what have you) there's normally another, implicit stack: whenever you declare a variable in C, it's typically put in *the stack*, which is memory allocated by the compiler for the program run time for things like local variables. Also, when you call a function, the function parameters as well as the return address are typically put in the stack.

  An issue arises when you overflow *this stack* - it typically has a fixed amount of memory allocated to it, for example eight megabytes, and if you have very large local variables (for example an array with tens of millions of elements) or very deep recursion (millions of recursive function calls) then your program will overflow the stack, the operating system will notice this and kill your application. Apart from having your application crash, stack overflows can be security holes and therefore used to hack into systems.

Queue
~~~~~

Queue is similar to stack but the opposite. It typically also has the same memory layout as an array but with a different usage pattern.

You can imagine queue as an array where you only have visibility over the first element in the array: you can read it, pop it (redefine "first element" as one to the right), and push an element (redefine the last element as one to the right, and set its value).

Queue is also called "FIFO" - first in, first out. Here's an example "animation" of using a queue:

.. verb::

  +---+---+---+---+---+
  | 1 | 2 | x | x | x | # push(3)
  +---+---+---+---+---+
    ^   ^ end of queue
    start of queue

  +---+---+---+---+---+
  | 1 | 2 | 3 | x | x | # pop() - returns 1
  +---+---+---+---+---+
    ^       ^ end of queue
    start of queue

  +---+---+---+---+---+
  | 1 | 2 | 3 | x | x |
  +---+---+---+---+---+
        ^   ^ end of queue
        start of queue

* Initially, in this example, we have two elements in the queue (1 and 2). In total our array has five elements, such that we can't push more than five elements in the queue ever without reallocating.
* We then push 3 in the queue, meaning we set the value of the third element to 3 and note that the end of the queue is now on the third element.
* We then pop an element from the queue, meaning we set the beginning of the queue to the second element. The 1 is still in the memory but not treated as part of the queue.

More buffers
~~~~~~~~~~~~

A ring buffer is similar to a queue but, in case the buffer allocated to the queue is full, a new element replaces the oldest element in the queue, such that the oldest element is *overwritten*. This can be useful to e.g. keep track of the last N integers, or pointers, or any other data.

A double-ended queue, or *deque*, is, like the name suggests, a queue where one can append or remove elements at either end of the queue. This can often be implemented as a kind of a ring buffer.
