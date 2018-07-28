Redis
-----

Redis is a NoSQL database that we'll use for our high score list.

Let's do the following:

* Download Redis source, compile it and run it - it, like most databases, runs as a server, which means we'll just leave it running in a terminal
* Install a Python client to Redis and test it
* Figure out how to use Redis with some examples

What is a database?
===================

In general, database is a program that allows other programs to perform data operations. The typical data operations are CRUD:

* Create, i.e. add some data in the database
* Read, i.e. fetch some data from the database
* Update, i.e. modify some existing data in the database
* Delete, i.e. remove some data from the database

So in essence, a database needs the following:

* Some storage, typically as a file or files on a hard disk, where it stores the data in some format it deems best
* Some interface for other programs to interact with it

The interface is typically, though not always, a TCP/IP socket, such that a program can connect to it over web (or from the same machine as the database is running on), and then send commands and data to the database, and receive data.

In some cases, like our programs that worked with 10,000 quadratic equation definitions, as databases can be used to store data persistently, a database can be used instead of storing data in a file. The program would then read the data from the database instead of a file.

SQL vs. NoSQL
=============

SQL is a wide-spread class of databases. There are several SQL databases, for example the open source MySQL and PostgreSQL as well as Microsoft SQL. The main common characteristics is that the interface to them is defined by the SQL language which standardises many of the common database operations. SQL databases are also relational, meaning that relationships between data can be explicitly described. Typically, the data tables, i.e. the format in which the data can be stored, must also be defined explicitly, and will to some extent be verified by the database.

NoSQL databases generally mean any database that don't have SQL as the interface for performing database operations, however the term is often used for a certain type of databases which often aren't relational and don't expect the user to define the tables beforehand.

There have been a lot of discussions about pros and cons of SQL and NoSQL. The following is mostly my opinion which, like many hot topics in software development, won't be shared by most other developers - caveat emptor. In general, SQL, forcing the user to structure the data formats beforehand, is suitable when the user benefits from the database ensuring the user is doing the right thing. In other words, if you have or will have lots of code working with the database, it's beneficial to use an SQL database because the database will help you do the right thing. On the other hand, for simpler projects with little code touching the data, it can be more effective to use a NoSQL database as it's often faster to set up and use. As this project is about introducing the concepts around web development and the amount of code is relatively little, we'll use a NoSQL database. We'll be using an SQL database later on.

Setting up Redis
================

Redis is one of the more popular NoSQL databases: it's relatively simple and small (about 16,000 lines of C), and easy to set up. It's a key-value database, meaning the user can store data simply by defining the key and the value, and read data using the key. In the simplest case, both key and value are strings so one can e.g. store the value "bar" with the key "foo", and then retrieve the value "bar" by querying for key "foo".

While Redis could be installed a more traditional way (e.g. package manager on Linux), we can use this as an opportunity to compile other people's code from source. 

If you go to the Redis website, you'll see a download link and some instructions on how to set Redis up. At the time of writing these are the instructions:

.. code-block:: bash
    :linenos:

    $ wget http://download.redis.io/releases/redis-4.0.8.tar.gz
    $ tar xzf redis-4.0.8.tar.gz
    $ cd redis-4.0.8
    $ make

This introduces us some important concepts:

* Line 1: We use wget to download a tarball (file with a .tar.gz ending). wget is a tool for downloading files from the web (similar to curl), and tar is a common way in Unix to package several files into one for easier handling.
* Line 2: We extract the contents of the tarball. tar takes three switches:

  * 'x' means "extract"
  * 'z' means "decompress gzipped data" - the file extension .tar.gz implies the files have been packaged using tar, and compressed using gzip
  * 'f' means read from a given file

* Line 3: We then change directory to the directory that was extracted. The convention is that when extracting a tarball, it should only create one directory with the name of the tarball as not to mix up with files in the current directory.
* Line 4: We run "make", which is a standard Unix tool for compiling code. What it does is software specific; make as such only executes what's been defined in the Makefile. In case of Redis, the Makefile starts compiling a bunch of C code.

Running these commands should result in your system compiling Redis and all its dependencies. If something goes wrong then your C compiler might not be correctly set up.

.. topic:: Software versioning

  The Redis package downloaded in the example was version 4.0.8. The typical convention is that the number '4' in this version denotes the *major* version number, '0' is the *minor* version number and '8' is the *patch level* version number. As Redis is software we'll write software to work against and hence has an API, it's important to note which version is being used. When we use Redis we assume that the documentation matches its behaviour - but this might change in future Redis versions. Typically, in new versions which only have the patch level changed, e.g. 4.0.9, all changes made will be *backwards compatible* - that means, all the code that was written against 4.0.8 will work for 4.0.9. This is typically also the case for minor versions, but a major version change can be expected to introduced changes that *break* backwards compatibility. That means, if we were to update our Redis server years later, it might end up e.g. as version 6.0.0, and if changes that break backwards compatibility were introduced and we tried running our old code with that version we'd get errors.

  Code that's ended up not working due to software it's dependent on having moved on to break backwards compatibility is said to have *bitrot*. Somewhat related, code that has been written over a longer time without consideration over maintainability or proper structure is said to have *organically grown*. Code for which the execution flow is hard to follow due to various branches and lack of structure is called *spaghetti code*.

As per the documentation, once the compilation is done, you can then start the Redis server by running "src/redis-server". This is the binary that resulted from the compilation. You should then see a bunch of text running through, ending with something along the following lines:

.. code-block:: bash

    16870:M 13 Feb 23:32:25.161 * DB loaded from disk: 0.000 seconds
    16870:M 13 Feb 23:32:25.161 * Ready to accept connections

Redis should now be running and accepting TCP/IP connections. You can leave it running as we try to connect to it using Python.

*Exercise*: Download, compile and start Redis. Feel free to take a look at the code while you're at it.

Python Redis client
===================

The main Python Redis client can be found online (at the time of writing, in GitHub). There are a few ways you could install this, but pip is probably the most straightforward:

.. code-block:: bash

    pip install redis

*Exercise*: Install the Python Redis client.

It's probably best to install it in your virtualenv, but you may want to install it system wide (with sudo) instead.

If the installation was successful, you should now be able to connect to your Redis server and store and fetch some data by running the following:

.. code-block:: bash

    $ python2
    Python 2.7.13 (default, Dec 21 2016, 07:16:46)
    [GCC 6.2.1 20160830] on linux2
    Type "help", "copyright", "credits" or "license" for more information.
    >>> import redis
    >>> r = redis.StrictRedis(host='localhost', port=6379, db=0)
    >>> r.set('foo', 'bar')
    True
    >>> r.get('foo')
    'bar'

If you get an error when importing redis, the client isn't installed correctly. If you get an error when connecting, the Redis server isn't running.

If you were able to import and connect, the line "r.set('foo', 'bar')" sets the value for key "foo" to "bar", and the line "r.get('foo')" retrieves the value for key 'foo'.

*Exercise*: Try out your Python Redis client.

Now, we should be all set to go.

Using Redis
===========

There are several ways to store data in Redis. While Redis can store simple string key-value pairs, it can do more, for example lists and sets. The best way depends on how you access and modify the data, but for getting a bit familiar with Redis we can envision the following exercises.

*Exercise*: Write a Python program to read in your JSON file with 10,000 quadratic equations, and store it as a value, as a string, in Redis. The key isn't very important for this exercise.

*Exercise*: In your Python program that reads 10,000 quadratic equations from a file, add functionality to read the data from the Redis database instead. You'll need to parse the JSON string.
