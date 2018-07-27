Unix shell scripting
--------------------

  Those who don't understand Unix are condemned to reinvent it, poorly.

  -- Henry Spencer

Redirecting
===========

You can always write the output of any command to a file by redirecting, i.e. using the '>' character:

.. code-block:: bash

    $ awk '{print $1}' test.txt > output.txt

This will create a file called output.txt, overwriting any previous contents, with the output of the 'awk' command.

You can also append to the end of an existing file, by using '>>':

.. code-block:: bash

    $ awk '{print $1}' test.txt >> output.txt

Redirecting will allow us to simplify writing our own software. For example, it might not be necessary to open a file for writing in Python, so instead of this:

.. literalinclude:: ../material/py/with.py
   :language: python

...we can simply write this in Python:

.. code-block:: python

    for i in xrange(5):
        print "%f %f\n" % (0.5, 0.2)

...and then run it like this:

.. code-block:: bash

    $ python my_file.py > test.txt

This has the added flexibility that we don't have to hard code the output file name.

If necessary, you can also discard the output by redirecting it to the special file /dev/null, which has the sole purpose of consuming and discarding all its input:

.. code-block:: bash

    $ python my_file.py > /dev/null

*Exercise*: rewrite your program that generates the 10,000 functions file to write to standard output.

If you have a program that reads from standard input (stdin), you can have it read a file with a redirection:

.. code-block:: bash

    $ python script.py < myfile.txt

Shell scripts
=============

Simple for loops
~~~~~~~~~~~~~~~~

In most Unix shells including bash you can define for loops. For example, let's say you wanted to run your number generation program three times, and store the output to different files:

.. code-block:: bash

    $ for i in 1 2 3; do python gen.py > functions_${i}.txt ; done

This will generate three files named functions_1.txt, functions_2.txt and functions_3.txt.

If we wanted to generate a hundred files, typing in the number would get tedious. We can use "seq" instead:

.. code-block:: bash

    $ for i in $(seq 1 100); do python gen.py > functions_${i}.txt ; done

This will generate a hundred files. The notation $(...) allows capturing the output of a command within another command. By using curly braces ({}) around the 'i' variable we ensure the variable name gets correctly understood by the shell and not confused with the file name template.

if and branches
~~~~~~~~~~~~~~~

The shell built-in command "if" allows branching. There are other built-ins for comparing values against each other. For example:

.. code-block:: bash

    $ grep -q 2345 test.txt ; return_value=$?; if [ $return_value -eq 0 ]; then echo "found"; else echo "not found"; fi

This command will output "found" if the string was found in the input file, and "not found" otherwise. (The "-q" switch to grep suppresses output.)

Storing scripts in a file
~~~~~~~~~~~~~~~~~~~~~~~~~

It's often practical to store scripts to a file. Here's the previous example stored in a file:

.. code-block:: bash

    #!/bin/bash

    grep -q 2345 test.txt
    return_value=$?
    if [ $return_value -eq 0 ]; then
        echo "found"
    else
        echo "not found"
    fi

The script behaves similarly as the one-liner, but the script has one more line: it starts with "#!/bin/bash". This is also called shebang, a character sequence at the beginning of a script indicating which tool should be used to interpret the script (in this case bash).

You can store the above in a file, and then execute it by passing it to bash explicitly:

.. code-block:: bash

    $ bash ./test.sh
    found

Or you can simply make it executable, in which case it can be directly interpreted using bash:

.. code-block:: bash

    $ chmod +x test.sh
    $ ./test.sh

(The command "chmod" modifies the access rights to a file, and passing it "+x" means the file is allowed to be executed.)

Process handling
================

Process handling refers to managing how and when programs (processes) start and end.

Typically, unless overridden by the program, hitting ctrl+c sends SIGTERM (signal to terminate) to the current program. SIGTERM is a signal that tells a program it should terminate, and by default will kill the program. However programs are allowed to install a *signal handler* for SIGTERM so they can perform any necessary cleanup before terminating, for example closing and saving any files that have been modified.

Another useful key combination is ctrl+z which stops the program execution and puts it in the background. This means that you can start a process, for example "less test.txt", then exit the program but keep it running by pressing ctrl+z which will drop you back to the shell. Let's go through an example usage:

.. code-block:: bash
    :linenos:

    $ cp -i big_file.tgz /mnt
    ^Z
    [1]+  Stopped                 cp -i big_file.tgz /mnt
    $ bg
    [1]+ cp -i big_file.tgz /mnt &
    $ ls
    Makefile  dep.rst   ex_js2.rst
    $
    [1]+  Done                    cp -i big_file.tgz /mnt

Let's assume we have a very large file we want to copy somewhere. Copying large files can take time, and cp by default doesn't output anything while copying. We start the copy on line 1.

After waiting for some time, we get bored and want to do something else so we press ctrl+z. This is shown in the terminal with ^Z (line 2). Pressing this key combination will tell us that the copy command has been stopped (line 3), and drops us back in our shell (line 4).

We then issue the command "bg" which means the previous command should run in the *background*, meaning it can run but should not prevent us from using the shell. Issuing this command tells us on line 5 that the copy command continues to run again, pointed out by the & sign at the end of the command.

On line 6 we can then do what we want, e.g. run "ls".

On line 8, we have the prompt again and wait for some time, until we expect the copy command to have finished. We can check if this is indeed the case by hitting enter (doing nothing). Bash uses this opportunity to tell us that the copy operation indeed has finished (line 9).

If you're running a command and you want to start it on the background right away, you can add the & sign to the end of the command:

.. code-block:: bash

    $ cp -i big_file.tgz /mnt &
    [1] 23904
    $

If you have a command running in the background but want to run it on the foreground again, run "fg":

.. code-block:: bash

    $ cp -i big_file.tgz /mnt &
    [1] 23904
    $ fg
    cp -i big_file.tgz /mnt
    $

If you want to see which commands you have running in the background in the current terminal, run "ps":

.. code-block:: bash

    $ ps
      PID TTY          TIME CMD
     5898 pts/8    00:00:01 bash
    23904 pts/8    00:00:00 cp
    23905 pts/8    00:00:00 ps

If you want to send a SIGTERM signal to another process than what's currently running in the foreground, you can use the "kill" command with the PID (process ID) of the process you want to kill, for example:

.. code-block:: bash

    $ cp -i big_file.tgz /mnt &
    [1] 23904
    $ ps
      PID TTY          TIME CMD
     5898 pts/8    00:00:01 bash
    23904 pts/8    00:00:00 cp
    23905 pts/8    00:00:00 ps
    $ kill 23904
    [1]+  Terminated              cp -i big_file.tgz /mnt
    $

Note that bash tells you the PID of the newly started process when you start it on the background using &.

Because a program can install a signal handler for SIGTERM, it's possible that sending SIGTERM to a program won't kill it. To force kill a program you need to send the signal SIGKILL:

.. code-block:: bash

    $ cp -i big_file.tgz /mnt &
    [1] 23937
    $ kill -KILL 23937
    $
    [1]+  Killed                  cp -i big_file.tgz /mnt
    $

*Exercise*: Start "less" to browse a file, hit ctrl+z to move it to the background, then run "fg" to bring it to foreground again.

Cron
====

Cron is a software in Unix-like operating systems which allows the user to configure running a command at given time intervals.

For example, running the command "ls > out.txt" in the home directory once per day, i.e. to store the listing of the files in the home directory in a file, can be done using cron. To do this, one would run the command "crontab -e", which would open the *cron table* in the user's default editor. The cron table is a file where each row describes what command to run periodically, and how often. E.g.:

.. code-block:: bash

    15 3 * * * ls > out.txt

This line would cause cron to run the command at 3:15 AM every day. The columns on the table are minutes, hours, day of month (1-31), month (1-12) and day of week (0-6 where 0 is Sunday and 6 is Saturday). The asterisk ("\*") is a placeholder meaning "every". If you want to use cron in your system, you need to ensure it is running (e.g. by running "ps aux | grep crond"; "ps" lists the processes running in the system and crond means *the cron daemon*; daemon being a program that runs in the background). If cron is not running, you can find out how to start it from the documentation of your operating system.
