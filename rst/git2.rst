Working with other git repositories
-----------------------------------

We've learned how to create a git repository and commit changes to it. Git allows collaboration between different people by having the functionality to *push* commits to and *pull* commits from another repository. (Git is a *distributed* version control system, which means that each repository is equal; there is no server-client relationship like with some other version control systems such as Subversion.)

Let's practice this a bit. We created a repository the last time by running "git init" in a directory. We can *clone* that directory by running "git clone", e.g.:

.. code-block:: bash

    $ mkdir my_clone
    $ cd my_clone
    $ git clone ../my_project
    Cloning into 'my_project'...
    done.
    $ ls
    my_project
    $ cd my_project
    $ git log
    commit 43130e10f89232f5ce542c4d864ff78e0a171796
    Author: Antti Salonen <ajsalonen@gmail.com>
    Date:   Fri Feb 9 00:42:07 2018 +0100

        add printing hello world again

    commit b279b4fb109844ab0337bc906897f6e48a3c18cf
    Author: Antti Salonen <ajsalonen@gmail.com>
    Date:   Fri Feb 9 00:35:05 2018 +0100

        initial commit

What happened here is that by running "git clone" git creates an identical copy of the existing repository. It's not very interesting on your own machine but you can also tell git to clone a remote repository, i.e. over the Internet.

Collaboration
=============

Now, let's make a change in our clone:

.. code-block:: bash

    $ echo "print 'hello world'" >> hello.py
    $ git commit hello.py -m "add third print"
    [master bc6ef9e] add third print
     1 file changed, 1 insertion(+)

We now have one more commit in our clone than in our original repository.

The typical flow, e.g. when working with a repository in github or generally a central repository, is to push the change to the *origin*, i.e. to the repository that was used for creating the clone:

.. code-block:: bash

    $ git push origin master

Here, we tell git to push the changes to the *master branch* on the origin. Branches generally speaking allow you to have different developments in different branches, and the master branch is the default branch. You can try the command above but because we work with local repositories, git doesn't by default allow pushing to it.

What we could do would be to change the directory back to the origin repository and run "git pull ../my_clone/my_project" which would fetch and merge the changes from the clone back to the original repository.

Branches
========

Let's create, add and commit a new file with some more interesting contents:

.. code-block:: python

    with open('test.txt', 'w') as f:
        for i in xrange(5):
            f.write("%f %f\n" % (0.2, 0.5))

This simple Python script generates a new file with some numbers in it.

Now, let's make two branches from master, such that both branches have some changes to this file:

* The first branch should be called "values" and change the values being written to a file
* The second branch should be called "three" and write three values in the file instead of two

Let's then merge the two branches in master. As there's no obvious way to merge the two branches we'll end up in conflict which we'll have to resolve manually.

Why would you do this? This could happen because we're confused with what we're doing, or because we're working in a team and making conflicting changes, or because we want to pick different changes from different versions to create a new version. While we'll do this work on local branches, in general the principle is the same when working with remote code, e.g. code from other people.

Let's then create a new branch where we want to change the values that are being saved in the file such that they're 0.0 and 1.0:

.. code-block:: bash

    $ git branch values
    $ git checkout values
    Switched to branch 'values'

What we did here was create a new branch "values", then checked it out, meaning we changed the current branch to it. We can check which branch we're in by running "git branch":

.. code-block:: bash

    $ git branch
      master
    * values

Now, let's modify the file. By running "git diff" before adding the changes to the staging area or committing them we can see changes in the current checkout:

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

Here, git shows us the changes we've made. We can now commit the changes.

What our repository now looks like is this:

.. code-block:: bash

    $ git log --graph --decorate --pretty=oneline --abbrev-commit --all
    * 4cf7d38 (HEAD -> values) change values to be 0 and 1
    * 91abbc4 (master) add with
    * bc6ef9e add third print
    * 43130e1 add printing hello world again
    * b279b4f initial commit

This means:

* We have the latest commit which is the current working directory state (HEAD) and the head of the "values" branch which has the commit with hash 4cf7d38 where we changed the values to be 0 and 1
* The head of the "master" branch is 91abbc4 where we added the original with.py

You can change between branches by using "git checkout":

.. code-block:: bash

    $ git checkout master
    Switched to branch 'master'
    $ grep write with.py
            f.write("%f %f\n" % (0.2, 0.5))
    $ git checkout values
    Switched to branch 'values'
    $ grep write with.py
            f.write("%f %f\n" % (0.0, 1.0))

Merging and conflicts
=====================

Let's then create the branch "three" and create a commit there:

.. code-block:: bash

    $ git checkout master
    Switched to branch 'master'
    $ git branch three
    $ git checkout three
    Switched to branch 'three'
    $ vim with.py
    $ git diff
    diff --git a/with.py b/with.py
    index f61db97..444a55f 100644
    --- a/with.py
    +++ b/with.py
    @@ -1,3 +1,3 @@
     with open('test.txt', 'w') as f:
         for i in xrange(5):
    -        f.write("%f %f\n" % (0.2, 0.5))
    +        f.write("%f %f %f\n" % (0.2, 0.5, 0.8))
    $ git commit -m "write out three values" with.py 
    [three dd6c856] write out three values
     1 file changed, 1 insertion(+), 1 deletion(-)

Now, let's see where we are:

.. code-block:: bash

    $ git log --graph --decorate --pretty=oneline --abbrev-commit --all
    * 4cf7d38 (values) change values to be 0 and 1
    | * dd6c856 (HEAD -> three) write out three values
    |/
    * 91abbc4 (master) add with
    * bc6ef9e add third print
    * 43130e1 add printing hello world again
    * b279b4f initial commit

* "values" is still where it is - one commit ahead of master, namely 4cf7d38
* "master" is still where it is - but now it has two branches ahead of it
* HEAD, our current checkout, is at the head of "three", which is a new commit, dd6c856

Now, let's try to switch to "master" and *merge* the changes from both "values" and "three" to it:

.. code-block:: bash

    $ git checkout master
    Switched to branch 'master'
    $ git merge values
    Updating 91abbc4..4cf7d38
    Fast-forward
     with.py | 2 +-
     1 file changed, 1 insertion(+), 1 deletion(-)

Here, we run "git merge" to merge two branches. ("git pull" does "git fetch", i.e. downloading the status of a remote repository, followed by "git merge", i.e. merging the status of the remote repository with ours.) It works out well and git modifies our with.py to include the changes from "values". We can check the status using "git log":

.. code-block:: bash

    $ git log --graph --decorate --pretty=oneline --abbrev-commit --all
    * 4cf7d38 (HEAD -> master, values) change values to be 0 and 1
    | * dd6c856 (three) write out three values
    |/
    * 91abbc4 add with
    * bc6ef9e add third print
    * 43130e1 add printing hello world again
    * b279b4f initial commit

Here, our current state (HEAD) is the head of master, which is also the head of values, and three is a separate branch that doesn't have commit 4cf7d38. It does however have the commit dd6c856 so let's try to merge that to master as well:

.. code-block:: bash

    $ git merge three
    Auto-merging with.py
    CONFLICT (content): Merge conflict in with.py
    Automatic merge failed; fix conflicts and then commit the result.

Now, because we've modified the same location in the same file in two different commits which we try to merge, git doesn't know how to merge these automatically and tells us to fix it ourselves. Let's now take a look at with.py:

.. code-block:: bash

    $ cat with.py
    with open('test.txt', 'w') as f:
        for i in xrange(5):
    <<<<<<< HEAD
            f.write("%f %f\n" % (0.0, 1.0))
    =======
            f.write("%f %f %f\n" % (0.2, 0.5, 0.8))
    >>>>>>> three

What this tells us is:

* In HEAD, we have code writing out 0.0 and 1.0
* In the other branch (three), we have code writing out 0.2, 0.5, 0.8

We can check the status using "git status":

.. code-block:: bash

    $ git status
    On branch master
    You have unmerged paths.
      (fix conflicts and run "git commit")
      (use "git merge --abort" to abort the merge)

    Unmerged paths:
      (use "git add <file>..." to mark resolution)

            both modified:   with.py

    no changes added to commit (use "git add" and/or "git commit -a")

We'll then have to fix the code manually:

.. code-block:: bash

    $ vim with.py
    $ git add with.py
    $ git status
    On branch master
    All conflicts fixed but you are still merging.
      (use "git commit" to conclude merge)
    
    Changes to be committed:
    
    	modified:   with.py
    
    $ git diff --cached
    diff --git a/with.py b/with.py
    index d63b0bf..ae90c0d 100644
    --- a/with.py
    +++ b/with.py
    @@ -1,3 +1,3 @@
     with open('test.txt', 'w') as f:
         for i in xrange(5):
    -        f.write("%f %f\n" % (0.0, 1.0))
    +        f.write("%f %f %f\n" % (0.0, 0.5, 1.0)
    
...and commit:

.. code-block:: bash

    $ git commit -m "merged"
    [master ab7a9a6] merged with
    $ git log --graph --decorate --pretty=oneline --abbrev-commit --all
    *   ab7a9a6 (HEAD -> master) merged with
    |\
    | * dd6c856 (three) write out three values
    * | 4cf7d38 (values) change values to be 0 and 1
    |/
    * 91abbc4 add with
    * bc6ef9e add third print
    * 43130e1 add printing hello world again
    * b279b4f initial commit
    
Now, we see git visualise our repository again:

* The branch "values" has commit 4cf7d38
* The branch "three" has commit dd6c856
* The branch "master", which is also our current working directory (HEAD), we have a commit that merges both threads

In our case, we had a conflict between two local branches, but the process is the same if there are remote branches involved.

Note that in most cases, when two commits have changes in the same files, git is still usually able to merge them without issues. Conflicts only arise when no obvious automatic merge is possible.

In general, there are many different workflows one can use with git, but typically, when working with other people, one fetches and merges the code from others with "git pull", has to resolve any merge conflicts if any arise, and push any local commits with "git push".
