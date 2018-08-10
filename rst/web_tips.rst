Some more tips and tricks
-------------------------

Our simple web app is now fully functional. This page includes some tips related to it.

Using our new API outside the browser
=====================================

As we wrote a new GET handler in Flask to send the high score data, we created an API for the high score that can also be used outside the browser. To demonstrate this, you can e.g. run the following command in your shell:

.. code-block:: bash

    $ curl -s localhost:5000/guess/hiscore?max_value=25

(The "-s" switch reduces the output of curl such that only the response from the server is output. You may need to change the URL and the parameter to match your code.)

If you run this, you should get a JSON string back representing your high score list. This is practical because you can do all kinds of things with this API, such as:

* Write a script that gets the JSON files for all max_values, and display statistics over:

  * Which max_values have been played and which haven't
  * Who are the most active (or best) users of the game
  * How do the high score lists evolve over time

* Visualise the high score list in some other way, e.g. as a 3D scene
* Fetch the high score list periodically and automatically email an alert when the list has changed

It's often beneficial to expose such an API for reasons like this.

CSS, or making web pages prettier
=================================

CSS (Cascading Style Sheets) is a standard for defining how web pages look like. It's typically used together with HTML and JS; HTML controls the layout, JS defines the logic and CSS defines the looks. As this book isn't a web design book we won't go to the details on CSS but here's an overview:

* You create a .css file which describes e.g. what colour or fonts different tags should use, for example headings or paragraphs. CSS looks something like this:

.. code-block:: css

    body {
        background-color: #ff0000;
        color: #00ff00;
    }

(Colours are defined from 0 to 255 for red, green and blue separately, in hex notation, i.e. ff stands for 255. In other words, the above sets the background colour to strong red and the text colour to strong green.)

* You can also define a *class*: a style that applies to multiple elements.

* Once you've defined your CSS file you reference it in your HTML by including something like the following tag in your head section:

.. code-block:: html

    <link href="my_file.css" rel="stylesheet">

* If your CSS defines styles for a class, then you need to define which class your element uses. For example:

.. code-block:: html

    <table id="hiscore" border="1" class="my-table-style">

For those with little eye for design, there are several CSS libraries available online that can be used and downloaded by your web page such that you don't have to specify the style yourself. After searching online for a while you might find some. One of the popular ones is Pure.

*Exercise*: Find the Pure CSS website.

Pure CSS modules
~~~~~~~~~~~~~~~~

If you take a look at the usage section on Pure, you'll find the URL you can use to reference in your HTML such that when opening your page, the browser will download the Pure CSS modules in addition and use those if you define any of your elements to use the Pure styles. At the time of writing, the Pure page instructs you to do this:

.. code-block:: html

    <link rel="stylesheet" href="https://unpkg.com/purecss@1.0.0/build/pure-min.css" integrity="sha384-nn4HPE8lTHyVtfCBi5yW9d20FjT8BJwUXyWZT9InLYax14RDjBj46LmSztkmNP9w" crossorigin="anonymous">

This includes the URL to the Pure css file (at unpkg.com) as well as a hash of the file to ensure the file hasn't been modified by a malicious party.

Once we have this css defined, we can set the class of some of our elements such that they use Pure. As per Pure documentation, you can define the class for your high score table using the following:

.. code-block:: html

    <table id="hiscore" border="1" class="pure-table">

That is, you can otherwise keep it the same but include the "class" attribute to refer to "pure-table".

*Exercise*: Modify your HTML to use Pure. You can start with the table, but if you're interested you can also modify the form in your page for starting the new game to use Pure. You'll need to use a couple of extra HTML tags to use all of Pure's features.

Accessing your page from other devices
======================================

By default Flask only accepts connections from the computer Flask itself is running on. As per Flask documentation you can override this by passing the --host parameter to Flask, e.g.:

.. code-block:: bash

    $ FLASK_APP=guess.py flask run --host=0.0.0.0

This will allow connections from all devices in your network. (If you've enabled Flask debugger and don't trust all users in your network or expose your computer to the Internet directly then don't run this as in debug mode Flask allows users to execute code on your computer directly.)

In order to connect to your page from another device, you'll need to know the IP address where to connect. How to find this out is operating system dependent; on Linux, for example, you can typically run either "ifconfig" or "netcfg". You'll typically have multiple IP addresses (at least 127.0.0.1 and one more if you're connected to Internet) - pick one that makes sense for your network.

*Exercise*: Connect to your page from another device. Look up online how to find out your IP address if necessary. By default Flask serves your pages at port 5000 so you'll need to include that in your URL.

