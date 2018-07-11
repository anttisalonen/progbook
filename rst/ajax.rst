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
6. The server fetches the high score list from the database, and sends it to the client (Javascript)
7. The client displays the high score list

Seems simple, doesn't it?

Apart from entering the name, we've already covered steps 1-3 and to some extent step 5. In this chapter we'll cover steps 4 and 6. They're basically what AJAX is about.

Before we get to AJAX in detail, you might have noticed that we talk about server and client here, however if we simply load our HTML guessing game in the browser there is no server, only client. This means that we will have to pull up Flask, our web microframework again, as it has the capability of running a web server which we need going forwards.

*Exercise*: Move your guessing game HTML to be served by Flask. To do this, as per Flask documentation, you need to 1) create a directory named "templates" and move your HTML file there; 2) Set up a Flask "hello world" program such that there's a URL which serves your HTML file (using render_template()); and 3) Start Flask and try it out to ensure everything is set up correctly.

A note on security: As we've seen, we're not really in control of the Javascript code - a user can replace the Javascript code with whatever code they prefer. In this sense, were we to use this architecture in a real program, a user could easily replace the data sent to the server and fake their way to the top of the high score list. For educational purposes we assume we can trust the user in this case, but were one serious about such a game and offering it for the public, the game logic would have to be done on the server side, such that the correct number would be generated on the server and each guess would need to be sent to the server.

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

XmlHttpRequest is an API that allows asynchronous communication with the server. It's now standardised across browsers such that all major browsers provide this API, allowing us as Javascript programmers to send and receive data from the server via HTTP. It's called XmlHttpRequest (probably) because it was originally mainly used to send and receive XML data, but in general it can be used to transfer any data. Specifically, we'll be using it to transfer JSON.

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

* Line 1: We create a new object of type XMLHttpRequest which is an API for getting data from the server
* Line 2: We specify that we want to send a GET request (as opposed to POST), namely the URL "file.html"
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

