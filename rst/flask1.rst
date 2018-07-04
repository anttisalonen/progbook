Creating a simple web page
--------------------------

Now that we have written some Python code, let's see what we can do by *leveraging* existing libraries.

To do this we'll come up with a goal. Let's imagine we have the following use case:

* We have a computer which is connected to speakers and can hence produce audio
* We'd like this computer to play *internet radio*
* Furthermore, we'd like to stop or start the playback using a smartphone

In other words, we have a computer serving as a very simple media center, and we want to turn the music on or off without having to get up from the couch. How would we accomplish this?

We'll go through the following steps:

* Setting up the computer for serving web pages - by installing *external libraries*
* Putting together a simple web page with the stop/start buttons
* Setting up the computer for playing music from a command line
* Figuring out how to control the command line from Python
* Putting it all together, i.e. connecting the buttons to the command line music player

External libraries in Python
============================

Because writing all the code needed to serve a web page is a lot of work, we want to install an external library that takes care of most of this for us. In this chapter we'll be using *Flask* - a microframework for Python that allows relatively easy creation of web applications.

Python has a tool called *pip* which can download and install libraries. You can find out if you already have pip by running "pip" in the terminal. Most Unix systems include pip as part of the Python installation. If you don't have it, please consult the documentation of pip and your operating system online on how to install it.

Flask has fairly straightforward installation instructions; at the time of writing, running "pip install Flask" might be enough.

*Exercise*: Look up the webpage for Flask, the Python microframework.

There are several ways to install external libraries in Python. One very convenient way is to use *virtualenv*, or *virtual environment*: this tool allows you to install libraries without having to install them system-wide, potentially greatly simplifying installation.

Virtualenv is a software that can be downloaded from the virtualenv web page. There are several ways to install it; at the time of writing, running "pip install virtualenv" might be enough.

*Exercise*: Look up the webpage for virtualenv. Install it.

Once you have virtualenv installed, you need to create a virtual environment and activate it. Once you've activated it, changes to Python libraries such as installing new ones will only affect the virtual environment. This means that installing libraries might be easier, but in order to use them you'll need to have activated the virtual environment.

At the time of writing, the command "virtualenv myve" creates a virtual environment to directory "myve". This directory will be created as a subdirectory to the current working directory. After creating the virtual environment, it can be activated by running "source bin/activate" in the "myve" directory.

*Exercise*: Create a virtual environment and activate it. Look up instructions from the virtualenv webpage if you get stuck. Install Flask in your virtual environment.

Let's test Flask to ensure you've installed it correctly. At the time of writing, it should be enough to create the following Python file:

.. code-block:: python

    from flask import Flask
    app = Flask(__name__)

    @app.route("/")
    def hello():
        return "Hello World!"

...and run "FLASK_APP=hello.py flask run" (where hello.py is the Python file that was just created). After this, directing your browser to "http://localhost:5000/" should display the text "Hello World!"

*Exercise*: Test Flask to ensure you've installed it correctly. If you get stuck, consult the documentation on the Flask webpage.

Now that we have Flask installed and can serve webpages, we can put together a simple page, though at this stage it won't do anything.

HTML
====

We'll go more into details around HTML is later in this book. For now, suffice to say, HTML defines what will be shown to the user visiting a web page. We'll create a very simple page with two buttons labeled "Start" and "Stop", and nothing else.

As per Flask documentation, in order to display an HTML page it must reside in a subdirectory called "templates".

*Exercise*: Create this subdirectory in the directory where your hello.py is by running "mkdir templates".

We can now add an HTML file in this directory. We'll later tell Flask to send this HTML file to anyone visiting our site.

Here's a sample HTML file:

.. literalinclude:: ../material/flask/templates/radio0.html
   :language: html
   :linenos:

Let's go through this line by line:

* Line 1 indicates to the browser which HTML version is used.
* Line 2 makes the page more readable on smartphones, as required by our use case.
* Line 3 *opens* the *html tag*. This tag is closed on line 8.
* Line 4 sets the page title.
* Lines 5-7 define our *form*: a button. By having a form (with POST method) the browser can submit information about button presses to our Python code.
* Line 6 creates a button with the text "Start". The "<br />" marks a line break; this causes any following items to be below the button.
* Line 7 ends our form.

*Exercise*: Type (don't copy-paste) the above HTML file to a file. Save the file as "radio.html" in the "templates" directory.

We can now ask Flask to display this HTML page. As per the Flask documentation, the following code should do the trick:

.. literalinclude:: ../material/flask/flask1.py
   :language: python
   :linenos:

This has a few changes compared to the "Hello world" page:

* We import a few more identifiers from the "flask" package, namely "render_template" and "request".
* We've added another parameter "methods=['POST', 'GET']" to the *decorator* of our function.
* Instead of returning "Hello world", we return the return value of the render_template() function, and pass this function the file name "radio.html".

*Exercise*: Modify your hello.py or create a new Python file with the above contents. Open your page in your browser. You should be able to see one button labeled "Start". Clicking it shouldn't do anything.

We now have one button - but we wanted two.

*Exercise*: Add a button labeled "Stop" below the first button.

Now, you should be able to access the page from your local computer - but if you tried from another computer (or a smartphone), you might see the access will be denied. This is because by default Flask only allows connections from the local computer as in debugging mode, any Python code could be executed. Assuming you can trust the computers in your network, as per Flask documentation, you can enable Flask to allow connections from all public IPs by adding the parameter "--host=0.0.0.0" to your "flask run" command.

*Exercise*: If you have another computer or a smartphone available in the same network as your development computer and trust the computers in your network, restart Flask such that it allows connections from your other device. Find out the IP address of your development computer (e.g. by running "ifconfig" on Linux; see your system documentation). (The IP address you need may be the one from your local network, i.e. starting with e.g. 10.0.* or 192.168.*.) Connect to your web page from your other device.

Now that we're able to display a web page with buttons that don't do anything, let's see how we can make the buttons work.

