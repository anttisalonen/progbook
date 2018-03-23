Version control
---------------

Have you ever had the feeling that you want to make large scale changes to your code, but are afraid of breaking something? Has the thought crossed your mind that you could copy your source file or files to another directory, so that you have a backup in case something goes wrong?

If so then that's perfectly normal. You might, however, imagine that if you were to work on a project for a longer time, say, several weeks, the number of backup directories would become difficult to have an overview of, and if you actually had to revert some changes from the past, it might be difficult to remember which directory contained which changes.

Version control systems (VCS) have been implemented to help solve these issues, as well as help developers collaborate on the same source code.

Version control systems have a long history, and I won't go through all of that. Suffice to say, there are lots of different version control systems, and many developers have their own personal favorite.

This book will introduce the reader to one of the most useful ones, named git. Git was originally developed in 2005 by Linus Torvalds for the Linux kernel development, and has since become one of the most widely used version control systems worldwide.

Git has several pros, and a significant one is how it scales: it's very practical to use in hobby projects of individual developers, but is also being used to version control some of the world's largest source code repositories.

There is lots that can be learned about git; this book will cover the basic fundamentals.

Creating a git repository
=========================

Let's assume you have a directory which is otherwise empty except a file you've written, hello.py, which contains "print 'hello world'". Let's turn this directory into a git repository:

.. code-block:: bash

    $ git init
    Initialized empty Git repository in /home/antti/book/my_project/.git/

Git has now initialised an empty repository. What this means in practice is that git has generated a new hidden directory called .git which we can see by running the command "ls -la":

.. code-block:: bash

    $ ls -la
    total 16
    drwxr-xr-x 3 antti users 4096 Feb  9 00:30 .
    drwxr-xr-x 8 antti users 4096 Feb  9 00:30 ..
    drwxr-xr-x 7 antti users 4096 Feb  9 00:30 .git
    -rw-r--r-- 1 antti users   20 Feb  9 00:30 hello.py

What we want to do next is store the file hello.py in the git repository. After that it'll be version controlled. We can do this by running the following:

.. code-block:: bash

    $ git add hello.py

We can then run "git status" to see the current status:

.. code-block:: bash

    $ git status
    On branch master

    Initial commit

    Changes to be committed:
      (use "git rm --cached <file>..." to unstage)

    	new file:   hello.py

What this means is that if we were to commit our changes, i.e. create a new version, the new version would include a new file, namely hello.py. Let's do just that:

.. code-block:: bash

    $ git commit -m "initial commit"
    [master (root-commit) b279b4f] initial commit
     1 file changed, 1 insertion(+)
     create mode 100644 hello.py

The "-m" switch to git commit allows the user to enter the commit message, which should include a short description about what the commit is about. Since this is our initial commit, we label it as such.

After running the command, git tells us it's created a new commit with the new file included. What happens behind the scenes is that git stores the necessary information about your file in the .git directory, such that it can be indexed and retrieved again when needed.

If we wanted to see the status of our repository now, we can run "git status" again:

.. code-block:: bash

    $ git status
    On branch master
    nothing to commit, working tree clean

This means that the latest version git has stored matches the contents of our files (in this case, hello.py).

Further commits
===============

Let's now assume we want to make a change to our hello.py, by appending "print 'hello world'" to it:

.. code-block:: bash

    $ echo "print 'hello world'" >> hello.py

Now that we've modified our file, we can check status again:

.. code-block:: bash

    $ git status
    On branch master
    Changes not staged for commit:
      (use "git add <file>..." to update what will be committed)
      (use "git checkout -- <file>..." to discard changes in working directory)

    	modified:   hello.py

    no changes added to commit (use "git add" and/or "git commit -a")

The status now tells us that we've modified our file, and that the modifications aren't included in any version controlled by git.

We can view the changes we've made by running "git diff":

.. code-block:: bash

    $ git diff
    diff --git a/hello.py b/hello.py
    index a968078..01283b8 100644
    --- a/hello.py
    +++ b/hello.py
    @@ -1 +1,2 @@
     print 'hello world'
    +print 'hello world'

The output suggests that there's been a new line added to the end of hello.py.

We can now commit this change:

.. code-block:: bash

    $ git add hello.py
    $ git commit -m "add printing hello world again"
    [master 43130e1] add printing hello world again
     1 file changed, 1 insertion(+)

Now we already have two commits. We can see the commit log by running "git log":

.. code-block:: bash

    $ git log
    commit 43130e10f89232f5ce542c4d864ff78e0a171796
    Author: Antti Salonen <ajsalonen@gmail.com>
    Date:   Fri Feb 9 00:42:07 2018 +0100
    
        add printing hello world again
    
    commit b279b4fb109844ab0337bc906897f6e48a3c18cf
    Author: Antti Salonen <ajsalonen@gmail.com>
    Date:   Fri Feb 9 00:35:05 2018 +0100
    
        initial commit

The log will show the summary of each commit as well as the *commit hash*, which uniquely identifies each commit.

git reset
=========

Now comes the interesting part: let's say we want to go back to the previous version, before we added the second "print 'hello world'" to the end of hello.py. One way to do this is the following:

.. code-block:: bash

    $ git reset --hard b279b4f
    HEAD is now at b279b4f initial commit

The command asks git to reset the state of the current working tree to the commit b279b4f (the first few characters of the commit hash we're interested in). Git does this, and as part of that, replaces our hello.py with the old version:

.. code-block:: bash

    $ cat hello.py
    print 'hello world'

If we look at the log, we see the previous commit is no longer there:

.. code-block:: bash

    $ git log
    commit b279b4fb109844ab0337bc906897f6e48a3c18cf
    Author: Antti Salonen <ajsalonen@gmail.com>
    Date:   Fri Feb 9 00:35:05 2018 +0100

        initial commit

If we wanted to get our changes back, we can, because we noted the commit hash:

.. code-block:: bash

    $ git reset --hard 43130e10
    HEAD is now at 43130e1 add printing hello world again
    $ cat hello.py
    print 'hello world'
    print 'hello world'

Staging area
============

If you're about to commit changes to git, but want to review the changes before, this is possible too. First, mark the changes you want to add with "git add":

.. code-block:: bash

    $ git add hello.py

You can then run "git diff --cached" to see the changes that are in the *staging area*, i.e. marked to be committed:

.. code-block:: bash

    $ git diff --cached
    diff --git a/hello.py b/hello.py
    index 01283b8..4c80684 100644
    --- a/hello.py
    +++ b/hello.py
    @@ -1,2 +1,3 @@
     print 'hello world'
     print 'hello world'
    +print 'hello world'

You can also check the status to get an overview of the files about to be committed:

.. code-block:: bash

    $ git status
    On branch master
    Changes to be committed:
      (use "git reset HEAD <file>..." to unstage)

    	modified:   hello.py

Note that git also helpfully provides some tips about which commands to run.

This was a very short introduction to git. There will be more covered in this book later; you can also check the built in documentation for git by running "git --help" or the help for specific commands, for example "git status --help".

*Exercise*: Create a git repository and repeat the above commands yourself.

*Exercise*: Use git to version control all your previous and future software development projects.
