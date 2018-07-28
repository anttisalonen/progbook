Making our web page work
------------------------

So we have the buttons, and we have some Python code that doesn't really do much. We can improve things by writing some Python code that reacts to the button presses by starting or stopping an mp3 stream replay. To be able to do this, we should first understand how we can start or stop an mp3 stream replay on the command line.

Streaming mp3 files on the command line
=======================================

In order to stream mp3 files on the command line, we need two things:

* An mp3 stream
* An mp3 player

For the first one you can try to find a local, interesting radio station who provide an HTTP link to an mp3 stream. If you're out of luck, you can also use one I found: the Norwegian public broadcasting company provides exactly this kinds of links for their radio stations, and some stations are also allowed to be heard and played from anywhere in the world. Here's one such a link: `http://lyd.nrk.no/nrk_radio_p1_trondelag_mp3_h <http://lyd.nrk.no/nrk_radio_p1_trondelag_mp3_h>`_

If you try accessing this link from your browser, and have the audio replay set up, you may be able to hear some Norwegian local radio.

Now that we have an mp3 stream, how about the player? There are several mp3 players for various Unix systems that are able to download and replay an mp3 stream via HTTP. Here's an incomplete list:

* mpg123 (or mpg321)
* mplayer
* cvlc
* ffmpeg

Typically, one could use one of these to replay an mp3 stream by passing the URL as the first parameter, e.g.:

.. code-block:: bash

    $ mpg123 http://lyd.nrk.no/nrk_radio_p1_trondelag_mp3_h

You can stop the replay by sending the signal SIGTERM i.e. signal 15, to the replayer, e.g. by hitting ctrl+c on most systems.

However, some may not be able to read the mp3 stream via HTTP. If provided with the data, they could still be able to replay the mp3 though. Tools like "curl" come into rescue here:

.. code-block:: bash

    $ curl http://lyd.nrk.no/nrk_radio_p1_trondelag_mp3_h | mpg123 -

What we have here is a nice use of the Unix pipe: Curl is a program that can download files over the network e.g. using the HTTP protocol. We output the curl output to a pipe, such that on the other side the data is received by our mp3 player. The dash ("-") at the end is the input "file" for the player; the dash generally means "standard input", i.e. the output of curl in our case. This causes curl to download the stream, and the player to replay it.

*Exercise*: Install one of the above, or some other suitable mp3 player on your system. Replay the mp3 stream from the command line. Install curl if necessary.

Now that we're able to start and stop the playback on the command line, we can try to hook up our HTML buttons to these actions. But how can we use the command line from Python?

Python subprocess
=================

Python provides a module called "subprocess" which allows us to run shell commands from Python. Here's a simple example:

.. code-block:: python

    import subprocess

    subprocess.call(['ls', '-l'])

This would run the command "ls -l" from Python and write the output to stdout.

There's a lot that can be written about subprocess.

*Exercise*: Look up the reference for "subprocess" online, or by running "help(subprocess)" in the Python interpreter.

However, our use case is fairly simple: we want to be able to start an mp3 player in the background, and kill it.

As per the subprocess documentation, a special optional parameter called "shell" exists. If set to true, the command will be interpreted by our shell. This can simplify our code when we want our command to run in the background, and can also be used for redirecting the command output. (In a real software project, as per the subprocess documentation, it might be better to refrain from using "shell=True" and instead use the subprocess methods for running commands in the background or capturing command output.)

Using "shell=True" also implies that providing the command as a list of parameters to subprocess.call() is no longer necessary. This means that if we wanted to e.g. run "ls -l" and redirect the output to a file called "out.txt", we could write this:

.. code-block:: python

    subprocess.call('ls -l > out.txt', shell=True)

In order to start a command and keep it running in the background, we can add an ampersand ("&") at the end of the command. For example, to start the mp3 player mpg123 and keep it running in the background:

.. code-block:: python

    subprocess.call('mpg123 http://lyd.nrk.no/nrk_radio_p1_trondelag_mp3_h &', shell=True)

We can use a Unix command to send a signal to a process to end it. For example, most Unix systems have a command "killall" which will send a signal (by default SIGTERM) to all processes with a given name. For example, the command "killall mpg123" will stop all mpg123 processes in the system (that the current user is permitted to stop). If killall is not available, another possible command to use is "pkill" which for this use case is equivalent with "killall".

*Exercise*: Look up the man page of either "killall" or "pkill".

*Exercise*: Try starting your mp3 player on one terminal. Kill it from another terminal using e.g. "killall".

We now know how to start and stop the player in Python. How will we know under which conditions our code to start and stop should be run?

Controlling playback in Flask
=============================

When we press the button "Start" or "Stop" on our HTML page, our Python code gets executed. This is done by Flask.

In our Python code, we can find out whether a button has been pressed or whether the user simply entered the page, and if a button was pressed, which button it was.

If a user simply opens the page, the browser sends a "GET" request. If a button was pressed, a "POST" request is sent instead. We can check whether a POST request was sent in our Python code with the following if statement:

.. code-block:: python

    if request.method == 'POST':

This is because our form in HTML states the method to use is POST.

We can find out which button the user pressed using e.g. the following if statement:

.. code-block:: python

    if request.form['submit'] == 'Start':

This line would evaluate to True if the button labelled "Start" was pressed. We know this because our HTML knows this. (The variable "request.form['submit']" is set to what we wrote in the "value" attribute for each button in our HTML.)

For example, the following would print "Start pressed" whenever the user pressed the "Start" button, and would in any case render the HTML we wrote:

.. code-block:: python

    def hello():
        if request.method == 'POST':
            if request.form['submit'] == 'Start':
                print "Start pressed"
        return render_template('radio.html')

*Exercise*: Add code to your Python function such that if the "Stop" button was pressed, kill all existing music playing processes. The function should return "render_template('radio.html')" in any case (i.e. whether a GET or POST request was received.) Try your function out. If no music players were found, you should see a message along the lines of "mpg123: no process found".

*Exercise*: Add code to your Python function such that if the "Start" button was pressed, kill all existing music playing processes (in case any exist) and start a new one.

If everything worked out well, congratulations! You now have a music player controllable over a web page.
