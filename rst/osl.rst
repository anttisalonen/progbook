Software licenses
-----------------

As a software engineer it's often useful to have some understanding of the different software licenses. This may be useful not only to understand whether you can use certain software, or what the limitations are, but also something to consider when writing your own software and making it available to others. This section provides an overview. Note that none if this is legal advice. If necessary please consult a lawyer.

Proprietary software
====================

Proprietary software, or non-free software, refers to software where another entity than the user, such as the software publisher, retains some rights. This typically means that the user may not be allowed to e.g. copy, modify, distribute or sell the software, or receive the source code of the software.

Open source licenses
====================

We've used lots of different open source software in this book, from the compilers and interpreters (e.g. Python, gcc, clang and all main JavaScript engines) to various libraries (SQLite, SDL2, Flask) and other tools (most shells, Make, git, vim). In addition, several components of modern operating systems may be open source, from the kernel to the file system utilities (commands such as "ls", "grep" etc.) and commonly used desktop software such as the browser. There are several different open source licenses and here only some of the most popular ones are covered.

A note on terminology: open source and free software are used interchangeably in this section. They're often treated as synonyms of each other although "free software" emphasises freedom (as in speech, not as in beer) while "open source" emphasises the practicality of being able to access the source.

Permissive licenses
~~~~~~~~~~~~~~~~~~~

*Permissive* free software licenses are licenses that have very little requirements on the usage of the software. Most common permissive free software licenses include the *MIT license* and the *BSD licenses*. Typically, permissive licenses allow the use, copying, modification, distribution and selling of the software as long as the relevant copyright notice is stated, i.e. the original author is credited.

E.g. clang, the main Python interpreter, SDL2 and Flask are licensed under a permissive free software license.

GNU General Public License
~~~~~~~~~~~~~~~~~~~~~~~~~~

GNU General Public License (GPL) is a *copyleft* license. This means that, similarly to the permissive license, the license allows the use, copying, modification, distribution and selling of the software, but under the condition that any software distribution of the original or modified software must retain the same license terms. This means that were one to e.g. obtain the source code of a program licensed under GNU GPL, and modify it, any recipient of the modified software has the right to access the modified source code. Similarly, were one to obtain a program licensed under GNU GPL in binary form (i.e. only the machine code but not the source code), the recipient has the right to receive the source code for the received machine code.

E.g. gcc, the Linux kernel and git are licensed under GNU GPL.

Public domain
~~~~~~~~~~~~~

Software under public domain or licensed under a public domain equivalent license allows the use, copying etc. of the software without any limitations.

E.g. SQLite is placed under public domain.

Open source license compatibility
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

One may ask the question: "what happens when one incorporates multiple pieces of software licensed under different terms?" Generally speaking, it may make things tricky from legal perspective, but often it is possible to check the different licenses for *compatibility*.

License compatibility refers to whether different software components licensed under different license terms can be combined. It's typically allowed to incorporate software licensed under more permissive license terms under less permissive license. For example, one could take various pieces of software that are all placed under public domain and license them under the MIT license which is a permissive license. Another example is combining various pieces of software licensed under permissive licenses with software licensed under GNU GPL, and license the final product under GNU GPL. This is because a permissive license may be *compatible* with GNU GPL but not the other way around. In other words, one may not e.g. license software originally licensed under GNU GPL using the MIT license as this would conflict with the GNU GPL license terms.
