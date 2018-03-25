Some related Unix tools
-----------------------

Hashing
=======

You may have wondered what the hashes are about that git uses.

In general, a hash function takes any data as input and creates a hash value of the data. You can try this yourself by e.g. running the following command on Unix:

.. code-block:: bash

    $ sha1sum with.py
    4f2fb68a29c3a1f9978be115a1798371a57e9ae9  with.py

Here, we run the command sha1sum which calculates the SHA-1 hash of the file with.py. If you don't have sha1sum you may try e.g. md5sum. Hash functions can have the following properties:

* Changing a file insignificantly (e.g. by adding one byte) may significantly change the hash (e.g. result in a completely different hash)
* The hash function may be *cryptographically secure* - i.e. it is difficult or impossible to modify the input data such that the resulting hash would still be the same

In general, if you know the hash of a file, you can calculate the hash to check whether the file has been modified or corrupted. git uses hashes to uniquely identify commits and to protect against data corruption.

*Exercise*: Look up the definition of SHA-1 hash function online.

diff and patch
==============

"git diff" gives a practical output of a difference between a file before and after a change:

.. code-block:: bash

    $ git diff
    diff --git a/with.py b/with.py
    index f61db97..d63b0bf 100644
    --- a/with.py
    +++ b/with.py
    @@ -1,3 +1,3 @@
     with open('test.txt', 'w') as f:
         for i in xrange(5):
    -        f.write("%f %f\n" % (0.2, 0.5))
    +        f.write("%f %f\n" % (0.0, 1.0))

In general, you can *diff* any two files by running the utility "diff". Conventionally the switch "-u" is used to display the output in *unified form*, which is also the default git uses:

.. code-block:: bash

    $ diff -u with2.py with.py
    --- with2.py	2018-03-25 22:34:47.530840487 +0200
    +++ with.py	2018-03-25 22:05:25.477035716 +0200
    @@ -1,3 +1,3 @@
     with open('test.txt', 'w') as f:
         for i in xrange(5):
    -        f.write("%f %f\n" % (0.0, 1.0)
    +        f.write("%f %f %f\n" % (0.0, 0.5, 1.0)
    
What can be useful is redirecting diff output to a file. There's another utility called *patch* which takes the output from diff to actually make changes to a file, i.e. patch them. Let's say someone sent us the above diff output and we had our file with.py which we wanted to patch:

.. code-block:: bash

    $ patch -p0 < with.diff
    patching file with.py

Here, "patch" will modify our with.py according to the diff.
