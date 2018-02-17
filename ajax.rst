AJAX
----

AJAX stands for Asynchronous Javascript and XmlHttpRequest which is quite a mouthful. Before we go through what it is, let's explain why we need it.

Remember that what we wanted to do was have a high score list for our guessing game. We have the following constraints:

* The client (browser) displays HTML and executes JavaScript
* The server has direct access to the database while the client does not

These constraints imply the following:

* Once the user has guessed the correct number, as we want to store this in the database, the client will need to send this information to the server
* In order to update the high score table on the page, the server will need to send the information about the high score list to the client
* The above should happen without having to refresh or reload the page

From a high level software architecture point of view, then, this is how the flow is expected to work:

1. User opens the relevant URL in his or her browser and has some means to enter his or her name on the page
2. Our web server serves the HTML and Javascript that is our guessing game
3. The user plays the game happily in the browser, executing Javascript, without the server being aware
4. Once the user guesses the right answer, Javascript sends the number of guesses and the player name to the server
5. The server stores the number of guesses and the player name in the database
6. The server fetches the high score list from the database, and send it to the client (Javascript)
7. The client displays the high score list

Seems simple, doesn't it?

Apart from entering the name, we've already covered steps 1-3 and to some extent step 5. In this chapter we'll cover steps 4 and 6. They're basically what AJAX is about.

Before we get to AJAX in detail, you might have noticed that we talk about server and client here, however if we simply load our HTML guessing game in the browser there is no server, only client. This means that we will have to pull up Flask, our web microframework again, as it has the capability of running a web server which we need going forwards.

*Exercise*: Move your guessing game HTML to be served by Flask. To do this, as per Flask documentation, you need to 1) create a directory named "templates" and move your HTML file there; 2) Set up a Flask "hello world" program such that there's a URL which serves your HTML file (using render_template()); and 3) Start Flask and try it out to ensure everything is set up correctly.

Asynchronous execution
======================

Asynchronous Javascript and XmlHttpRequest. Asynchronous means that the code is not run synchronously. So far all our code has run synchronously. For example here:

.. code-block:: js
    :linenos:

    var x = 3;
    x += 5;
    document.getElementById("foo").value = x;

Line 1 is executed before line 2, which is executed before line 3. Asynchronous execution changes this. Instead we have something like this (not real Javascript code):

.. code-block:: js
    :linenos:

    var request = new ServerRequest();
    request.callback = function () {
        document.getElementById("foo").value = request.response;
    }
    request.send();

What happens here is:

* Line 1: We create a new variable which is of type ServerRequest
* Lines 2-4: We define the *callback* - a function which will be called later, but not now
* Line 5: We send the request to the server (via HTTP)

After the request is sent to the server, the server (our Python code) will handle it and send something back (via HTTP). As this goes over the network it can take several milliseconds or even longer depending on what the server does. Once the server has sent the reply, the browser will receive it and its Javascript interpreter will call the function that was defined in lines 2-4 with the data from the server. In this case, the HTML element with ID "foo" will have its value changed, i.e. the result from the server is visible to the user.

What makes this asynchronous is that we define code which isn't run synchronously with the execution flow otherwise.

XmlHttpRequest
==============

XmlHttpRequest is the API that allows asynchronous communication with the server. It's now standardised across browsers such that all major browsers provide this API, allowing us as Javascript programmers to send and receive data from the server via HTTP. It's called XmlHttpRequest (probably) because it was originally mainly used to send and receive XML data, but in general it can be used to transfer any data. Specifically, we'll be using it to transfer JSON.

.. topic:: XML and JSON

  You might wonder, what is this XML that I keep hearing about. XML (Extensible Markup Language) is a markup language that shares many similarities with HTML. Here's an example XML document:

  .. code-block:: xml

    <?xml version="1.0" encoding="UTF-8"?>
    <start_tag>
        <second_tag attribute="value">
            second tag body
        </second_tag>
    </start_tag>

  In general, you can define all the values (tag names, attribute keys and values, the contents in the body of a tag) as you wish. In this sense XML can be used to transfer generic data between two programs or components. For this use case XML is very similar to JSON. We're focusing on JSON in this book instead of XML because it's generally simpler to work with, and seems to be at least as common as XML if not more.

Here's an example of XmlHttpRequest in practice - how to get some data from the server:

.. code-block:: js
    :linenos:

    var xhr = new XMLHttpRequest();
    xhr.open('GET', 'file.html', true);
    xhr.onreadystatechange = function () {
        if (xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
            console.log("Data received: " + xhr.responseText);
        }
    };
    xhr.send(null);
    
Let's go through this line by line:

* Line 1: We create a new object of type XMLHttpRequest which is the API for getting data from the server
* Line 2: We specify that we want to GET data (as opposed to POST), namely by getting file.html
* Line 3: We define the callback function which will be called when the server has its response. The syntax used here creates an *anonymous function* - a function that doesn't have a name but is defined. It's practical because we can define the function inline as opposed to defining it somewhere else and referencing it here, and it's also necessary as we can use variables in it that are in scope in this example (namely "xhr") that wouldn't be in scope if we defined it as a named function.
* Line 4: We check the state of the request. We want it to be DONE and have the status code 200 (which means "OK" in HTTP).
* Line 5: If this condition is satisfied then we write text in the console, including the response text from the server. This can be anything - plain text, JSON, XML, binary data - whatever the server is programmed to send.
* Line 8: We send the request to the server.

Let's try this out ourselves.

*Exercise*: Implement the above AJAX request. You'll need the following:

1) Create a new HTML file which has nothing but a button which calls a Javascript function (<input type="button" onclick="my_function()" value="Button to GET data">), and a Javascript function which does nothing more but the code from the block above.
2) Add a function in your Python code to serve the above HTML page using Flask (render_template()).
3) Add another function in your Python code to serve the URL that the AJAX request will request. In the example above, that URL is "file.html". Note that the URL doesn't need to have a file extension. That function should return a string, like "Hello world!"
4) Run your Python code using Flask. Navigate to the HTML page that has the button. Open the Javascript console in the browser developer menu. Click the button. You should see the text from the Python server code in the console.

.. topic:: GET vs. POST

  To summarise, GET and POST are both two "verbs" in HTTP - commands the client sends to the server. What are the differences?

  * GET typically has no data attached to it from the client, except for the URL - it's meant to say "I want to download a page or a file"
  * POST can have data - any kind of data - attached to it - it's meant to say "I want to upload data to the server"
  
  The rule of thumb is that if you're only reading information from the server - but not changing anything in the server - you should use GET. You should use POST if the action results in changing something on the server, for example adding data in the database.

The example above requests something from the server. We can also send data to the server by using the HTTP command POST. Here's an example of sending a block of JSON:

.. code-block:: js
    :linenos:

    var xhr = new XMLHttpRequest();
    xhr.open('POST', 'file.html', true);
    xhr.onreadystatechange = function () {
        if (xhr.readyState == XMLHttpRequest.DONE && xhr.status == 200) {
            console.log("Data received: " + xhr.responseText);
        }
    };
    xhr.setRequestHeader("Content-type", "application/json");
    xhr.send(JSON.stringify({'my_number': 42}));
 
This looks very similar to the GET request above. The differences are:

* Line 2: We use 'POST' as the first parameter as opposed to 'GET'
* Line 8: We have a new function call, namely setRequestHeader(). This sets the type of data we're sending to JSON. We need this so that the server can handle the incoming data properly.
* Line 9: We include the data we wish to send as a parameter to send(). We use JSON.stringify to convert JSON to a string. The server will need to parse the JSON when receiving the data.

*Exercise*: Add the above POST request in your HTML page. The server should return the same JSON data back but with the number multiplied by 2, e.g. if the client sends "{'my_number': 42}" to the server then the server should send back "{'my_number': 84}". Here are some hints to get you started:

* In your @app.route Python decorator, you need to explicitly tell Flask you're expecing POST requests. You can do this by defining the decorator e.g. like the following: @app.route("/post_test/", methods=['POST'])
* In your function handling the POST request, Flask allows you to access the incoming JSON data by the request.get_json() function: data = request.get_json(). This will automatically parse the JSON data, returning a Python dictionary or a list, depending on the JSON.
* You can use json.dumps() to serialise the JSON data in Python to string to be returned from your function, causing the data to be sent to the client.

After the above exercise we're starting to have some pieces we can put together. Let's do some more.

*Exercise*: Modify your guessing game such that when the number was guessed correctly, send the number of guesses to the server in JSON.

*Exercise*: Connect to the Redis server in your Python module. Note that the code in the Python module will be executed automatically when you start Flask. This means you can connect to the Redis server in your top level Python code, without having to define a function for this.

Now, let's see if we can store the number of guesses in the Redis database. Here's something to get you started:

.. code-block:: python
    :linenos:

    @app.route("/guess/finished", methods=['POST'])
    def finished():
        data = request.get_json()
        print data['guesses'] # prints the number of guesses
        curr_date = str(datetime.datetime.now())
        our_string_to_store = json.dumps({'guesses': 42})
        r.lpush("25", our_string_to_store)
        return 'abc'

This introduces a few new concepts:

* Line 1: We want the URL /guess/finished to handle POST requests. We need to tell this to Flask explicitly by using the "methods" optional parameter in the @app.route decorator.
* Line 3: As per Flask documentation, Flask provides the globally accessible object called "request" which includes all data associated with the request. More specifically, it allows us to access the POST data the user sent. If the user sent JSON, it's available to us using the get_json() member function.
* Line 4: As the JSON data has been parsed by the get_json() function, it's available to us in Python dictionary form.
* Line 5: We can query the current system time by calling the function "datetime.datetime.now()" (after importing datetime). Furthermore we can convert it (and almost any other data type including numbers) to a string by calling the str() function.
* Line 6: We can convert a Python dictionary to a JSON string by calling the "json.dumps()" function (after importing json).
* Line 7: Here, we insert data in the Redis database whereby the connection is represented by the "r" object. We use the member function "lpush" which ensures the value in the key-value pair is a list. More specifically, it:
  * Takes two parameters, the key and a value
  * Creates a new list if the key didn't yet exist, such that the value is a new list with a single element, namely the value
  * Appends the value to the end of the existing list if the key existed
* Line 8: We return a string which will be available in JSON in the xhr.responseText variable.

Here, we also implicitly defined the *database schema* for our data: the key is the maximum number that the computer could think of, as a string. For now it's hard coded to 25 but we'll make it configurable later. The value is a list whereby each element in a list describes a finished game. Each finished game should be represented in JSON, such that the JSON has a field describing the number of guesses required in the game as well as the timestamp when the game was finished. We'll expand on this later.

Note that there are different ways the schema could be defined for this use case - this is one of the simplest ways but in general, schemas can be defined in significantly different ways, depending on the use case, amount of data and any performance requirements.

*Exercise*: When your server receives the number of guesses from Javascript, store this in the Redis database.

Here are some hints:

* If you need to purge the database, call r.flushdb(). This will erase all data in the database.
* You may want to write a small script or a URL handler to check the contents of the database to ensure you're adding the correct data. You may alternatively want to use the Redis command line interface for this.

We should now be able to store something resembling a high score list in a database, but we're still lacking the possibility for the user to enter his or her name, configuring the maximum number the computer thinks of and understanding how exactly turn the contents of a key in a database to a correctly sorted high score list. This will be the scope for the next chapters.
