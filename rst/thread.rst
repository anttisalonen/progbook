Concurrency
-----------

Concurrency refers to multiple tasks being executed, at least seemingly, the same time.

In the section "Unix shell scripting" we already touched upon the concept of a *process* - a program running on a computer. Modern OSes support having lots of processes at the same time; you may have e.g. the web browser and a terminal open at the same time, and the terminal may have several windows e.g. the shell, the editor and the compiler. Furthermore, modern OSes support running several processes at once; you may e.g. start the compiler which could run for a while, and at the same time use the editor without the computer locking up.

Typically, each process, when running, runs on a *CPU core*; modern CPUs may have multiple cores, more or less independent from each other, each one being able to process instructions individually. Hence, if your CPU e.g. has four cores, it is able to run four processes at the same time (*in parallel*).

.. topic:: Digression: What if I want to run even more processes at the same time? What if I only have one CPU core?

    More processes can be run concurrently than a computer has CPU cores available. However, they won't run in parallel - they won't *really* run at the same time. Instead, modern OSes will change the process running on a CPU core every few milliseconds, creating *the appearance* that more processes than CPU cores are being executed at once, even though in reality the execution is interleaved. This changing of the process that is being executed on a CPU core is called a *context switch*.

Conversely, if you have certain computations you want the CPU to process, and your CPU has several cores which you would like to use, you need to somehow be able to split your work across the different CPU cores.

Processes are one way to do this; you may be able to run multiple processes, each doing part of the work. You may need your processes to communicate with each other ("*Interprocess communication*, or IPC); we've seen a few different ways processes can communicate with each other, such as files, sockets (even on a single computer) or the Unix pipe.

Apart from processes, you can use *threads* to split the work. Threads work such that you may start a new thread in your code, such that, as a result, you now have two threads running your code at the same time. They share the same memory space, e.g. variables, so communication between threads can be relatively simple.

.. topic:: Digression: What is memory space?

    When you check the pointer value of a variable, e.g. 0x7fffe7d8, this address, on a modern OS, does not really refer to the *physical address* in your memory. Instead, modern OSes typically employ *virtual addresses* - they let each program have their own *virtual address space*, and whenever the program tries to access memory, the CPU performs a *translation* from virtual address to the physical address on the physical memory. This way it's usually not possible for processes to read each others' memory. Indeed one of the main distinctions between a thread and a process is that when a thread is created, it shares the virtual address space with the thread that created it, while a process receives its own virtual address space from the OS on creation.

Simple example
==============

Let's put together a simple threading example by writing a program that has two threads, both of them writing some characters to stdout. A common C API to manage threads is the *pthreads* API. It provides e.g. the following function:

.. code-block:: c

    int pthread_create(pthread_t *thread,
                       const pthread_attr_t *attr,
                       void *(*start_routine) (void *),
                       void *arg);

This function takes four parameters:

* thread: a *handle* to a thread. We simply need to create a local variable of type pthread_t and pass its address to the function. The function will fill its contents such that if we need to refer to the thread later, we can use this variable.
* attr: attributes of the thread. We can simply pass NULL which causes pthreads to use the default attributes.
* start_routine: this is a function pointer to the function that the thread should start executing. The function must take one parameter of type void pointer (pointer to anything) and return a void pointer.
* arg: the parameter to pass to start_routine when the thread runs.

pthreads also allows us to wait until a thread has finished:

.. code-block:: c

    int pthread_join(pthread_t thread, void **retval);

* thread is the thread handle.
* retval is a pointer to a void pointer. With this pointer we can retrieve the return value of the function that the thread executed.

Now, let's create two threads, one of which printing 100,000 lines filled with the character 'x', the other printing 100,000 lines filled with the character 'y':

.. literalinclude:: ../material/close/th1.c
   :language: c
   :linenos:

Let's go through this line by line:

* Line 3: We #include <pthread.h>, declaring the functions from the pthreads API.
* Lines 5-15: We define our function that the threads will execute. It will use printf() to write characters to screen. It takes a void pointer, which is in this case a pointer to char, which is the character we want to print. On line 7 we cast the void pointer to a char pointer, and dereference it to obtain our char.
* Lines 19-20: We define our variables of type pthread_t which are our handles to our two threads.
* Lines 24-25: We define our characters, the addresses of which we pass as parameters to the thread function.
* Lines 27-36: We start our two threads using pthread_create. We want both of them to run the same function, and we pass them the addresses to characters 'x' and 'y' respectively. The threads will start running directly after pthread_create is called.
* Lines 38-47: We wait until both threads are finished by calling pthread_join which will block until the function call finishes.

Note that although the two threads can share variables, they both have their own stacks. This means that the local variable e.g. in the function print_text() is *not* shared between the two threads. However, if you have variables with are pointers, they might point to the same memory.

When compiling this program the compiler flag "-pthread" needs to be passed to the compiler (at least in case of gcc and clang; e.g. "gcc -pthread -O2 -Wall -o th1 th1.c"). This enables pthread support during compilation.

*Exercise*: Compile and run the above program. Bonus points if you type it instead of copy-pasting. Run it multiple times. What output would you expect? Is the output always the same?

When running the program you may see characters "x" fill the screen. When this happens, the first thread is running and writes its output to the screen. Other times you may see characters "y" on the screen. Although both threads may be running at the same time, only one of them is allowed to write to the screen at once (this is typically enforced by the OS).

Incrementing
============

Now, let's assume we have a task: we need to increment an integer very many times. To make things interesting, we use two threads to increment the integer.

While this example is hardly very realistic, it does demonstrate some key concepts around threads. Generally, if we had multiple integers to increment, i.e. if our problem was *parallelisable*, using threads might make sense. In this case, because we only have one integer, using multiple threads won't help performance, but we can anyway see what happens.

Take a look at the following program:

.. literalinclude:: ../material/close/th2.c
   :language: c
   :linenos:

* Lines 5-8: We define a struct which holds the data we want to pass to our two threads. It holds the number of increments we want each thread to perform, and the pointer to the one integer that should be incremented.
* Lines 10-17: We define the function the threads will run. We cast the void pointer to a pointer to thread_param, and then have our loop where we perform a number of increments to the integer in question.
* Line 29: We define our integer that needs to be incremented.
* Lines 33-40: We define the contents of the parameters we pass to our threads. The number of increments are read from command line parameters for each thread. We use the C standard library function atoi() to convert a char pointer to an int, and ensure the threads receive the address of our one integer.
* Line 64: After running our threads and waiting until they've finished, we print out the final number.

This program could be used e.g. like the following:

.. code-block:: bash

    $ ./th2 1000 1000
    Final value: 2000

Here, we ask both threads to increment the integer by 1,000. As expected, the final value is 2,000.

*Exercise*: Compile and run the above program. Bonus points if you type it instead of copy-pasting. Run it multiple times. Try with large values, e.g. 100,000 for both threads. What output would you expect? Is the output always the same?

What happens here is that because both threads access the same memory, i.e. the same variable "our_value" from main, the result of one thread may interfere with the other. The x86-64 assembly for the incrementing could look e.g. like the following, with compiler optimisations disabled for clarity (output in the output file by the compiler after passing the "-S" flag):

.. code-block:: bash

    movl    (%rax), %edx
    addl    $1, %edx
    movl    %edx, (%rax)

Here, the first instruction loads the value from the address stored in the register rax (param->variable_to_increment) to the register edx. The second instruction increments edx by one. The third instruction moves the value from register edx to the address stored in rax (param->variable_to_increment). In other words, because the addition instruction works with data stored in registers, the data is fetched from memory for the addition, incremented, and then written out again, in a loop.

(The assembly code using compiler optimisation flags looks different but has the same effect in principle.)

Now, because we have two threads accessing the same memory concurrently, the end result may be incorrect. It could be that the first thread performs all of its calculation, then the second thread kicks in and clears the result of the first thread, such that the final result is only affected by the calculations of the second thread. This could happen, for example, if one of the threads performs the addition but is then *preempted*, i.e. temporarily moved out from a CPU core to let another thread run, and then later moved back to the CPU core. In this case the thread would then perform the move of the (now obsolete) data to memory. This is called a *race condition*. However, we can solve this.

Incrementing with mutual exclusion
==================================

pthreads offers *mutexes*, a portmanteau from "mutual exclusion", as a synchronisation mechanism. Mutex, also called a *lock*, can be used to limit the access to a *critical section*, i.e. code where shared data is being accessed and modified. A thread can lock a mutex, perform the increment, and then unlock the mutex. If another thread at the same time attempts to lock the mutex, it will block until the mutex has been unlocked. Hence, only one thread will be performing the increments at once.

To do this, we should define a mutex. It needs to be defined in main because the mutex must be shared between threads. A mutex can be defined like this:

.. code-block:: c

    pthread_mutex_t our_mutex = PTHREAD_MUTEX_INITIALIZER;

Now that we have a mutex, we should ensure our threads have access to it. We can do this by adding a field in the struct definition:

.. code-block:: c

    struct thread_param {
        int num_increments;
        int *variable_to_increment;
        pthread_mutex_t *mutex;
    };

Here, the last field in the struct is a pointer to a mutex handle. We can then, in main, before starting our threads, set this field to the address of our mutex:

.. code-block:: c

    param1.mutex = &our_mutex;
    param2.mutex = &our_mutex;

Finally we can use the mutex in our function that the threads will run:

.. code-block:: c

    int ret = pthread_mutex_lock(param->mutex);
    if(ret) {
        fprintf(stderr, "Could not lock\n");
        return NULL;
    }
    (*param->variable_to_increment)++;
    ret = pthread_mutex_unlock(param->mutex);
    if(ret) {
        fprintf(stderr, "Could not unlock\n");
        return NULL;
    }

Here, we lock the mutex before the increment and unlock it directly after, within our loop. Locking and unlocking a mutex in an inner loop like this may ruin our performance because mutex locking and unlocking could be a heavy operation, requiring relevant kernel code to be executed - a better way would be to perform the lock and unlock before and after the loop - but for demonstration purposes that'll do.

*Exercise*: Make the required changes to the previous program such that the critical code is protected by a mutex. Does the output of the program change?

*Exercise*: What would happen if a thread forgot to unlock a mutex? This might not happen in this case but could be more plausible in code with several branches, or multiple return points from a function, or C++ exceptions.

While our use case for using threads was somewhat non-realistic, better uses of threads do exist. For example, an HTTP server may have several threads serving the various clients that connect to the server, such that a client doesn't need to wait until another client has been served. Another example is splitting the work: for example, our Sudoku solver solved a series or Sudoku puzzles in series. To improve performance, it could be rewritten to use threads, such that each thread picks one of the unsolved puzzles and solves it. This way, the run time of solving a set of puzzles could be dramatically decreased. While pthreads could be used with C++, C++ (like most modern programming languages) also provides its own API for managing threads.
