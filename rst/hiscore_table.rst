Generating the high score table
-------------------------------

We should now have the following:

* The user can play the game and data over previous games is stored in the database, and we have an empty table as a placeholder for the high score table

Let's connect the dots. Here's our high level architecture:

* On page load, the Javascript code requests the high score data from the server.
* In Python, we have a handler that replies to this request by fetching data from the database and use this data to generate and send back a suitable JSON that can be used for the high score table.
* In Javascript, we receive this data, and write and call a function that takes this data as the input parameter and inserts it in the HTML high score table.
* Once the player has guessed the correct number, the server shall provide updated high score data to the client which the client shall use to update values in the high score table again.

You might wonder, why have the Javascript code request data from the server at page load time instead of generating the high score table as part of the template on the server? While this is certainly a feasible approach, the good thing about having the Javascript generate the high score table at page load is that as we want to have Javascript code to update the high score table at the end of the game anyway, we can reuse that code at page load and hence have only one piece of code update the high score table instead of two.

Actually updating the high score table
======================================

You may remember from the chapter how to call a Javascript function at page load. If not, the hint is that you assign the member function "onload" in the global object "window" as your callback function.

Let's install a function called update_hiscore() as the function that will be called at page load time, and implement it.

First of all, we need to send an XMLHttpRequest to the server. We should use GET as we're only asking the server for some data. However we'll need to supply a parameter to the server, namely *which* high score table we want - as the high score table should be different based on the maximum number the computer can think of. As we've learnt, with GET we can only pass parameters as part of the URL, so let's do that. The URL should look something like "hiscore?max_value=25" - in this example, we'd fetch the page "hiscore" from the server, with "max_value" as the GET parameter, with 25 as its value.

*Exercise*: Implement the update_hiscore() function. It should do nothing else but send an XMLHttpRequest to the server, asking for the high score data, for the maximum number that the user is currently using in his or her game. You can concatenate a number to a Javascript string with the '+' operator, e.g. "hiscore?max_value=" + my_variable. The handler for the data that is received from the server should do nothing else but call a function "display_hiscore()" (which we haven't defined yet), passing the response text to this function. For testing purposes you'll want to add a Flask handler for this URL, but for now it can return only a test string.

Providing the necessary data server side
========================================

We now have the client asking for high score data, but the server not providing any. Let's fix that next.

We might already have some Flask handler that will be called when the client requests for high score data. (If not, then add that now.) What we'll need to do in that function is:

* Fetch the relevant data from the database
* Parse the JSON we've stored
* Sort the data and pick the top five entries
* Serialise this data in JSON
* Send it to the client

You may recall we decided that in our database schema, the key is the maximum number and the value is a list, with each entry in the list representing one finished game. In order to fetch data from Redis where the value is a list, according to Redis Python client documentation, we can use the "lrange" function:

.. code-block:: python

    stored_data = r.lrange(key, 0, -1)

The "lrange" function takes three parameters: the key, start index and end index, which, if negatively indexed, counts from the end. In other words, pssing 0 and -1 as the indices we fetch the full list. The variable "stored_data" now holds a list of strings, each string being JSON data.

*Exercise*: In your handler function, read all the data for the relevant key in a variable. You need to use "request.get.args()" to get the GET parameter.

We can parse a string which should be well formatted JSON data by using the "json.loads()" function:

.. code-block:: python

    parsed_data = json.loads(element)

*Exercise*: Parse all the JSON strings in the list such that you have the data available in Python dictionaries. You should be able to e.g. access the number of guesses field. Ideally you'd parse the strings by using a list comprehension.

Now that we have the data available, we need to sort it by the number of guesses and send the data to the client (by converting it to JSON using (json.dumps() and "return"ing it in our Flask handler function).

*Exercise*: Sort the data based on the number of guesses, from lowest to highest. Pass the optional argument "key" to the list.sort() or sorted() function. As the key, pass a function (anonymous or named) which return the field in the dictionary that is the number of guesses. Serialise the first five elements of that list into a JSON string and return the string to the client.

Handling the high score data on the client
==========================================

On the client side, we have a function that requests the high score data from the server, and calls a callback function when this data is available, passing the data to the function.

The data that this function receives should be the JSON string that we programmed the server to send in the previous section. (You can use console.log() to verify.)

Now, what we should do is take that data and put it in our HTML table.

We already saw some Javascript code before to change a cell in an HTML table:

.. code-block:: js
    :linenos:

    table = document.getElementById("hiscore");
    for(var i = 1; i < 3; i++) {
        table.rows[i].cells.item(1).innerHTML = "2017-02-15";
    }

We've also seen how to parse a JSON string into a Javascript dictionary:

.. code-block:: js

    var parsed = JSON.parse(data);

Then, it seems what we need to do is take the data from our dictionary, iterate over it in a loop and add the data in the relevant cells in our HTML table, cell by cell.

*Exercise*: Fill in the high score table. Make sure you don't overwrite the header row in the table - add one to your index where necessary. If you're sending more entries in your server code than you have space for in your table, you can use the function "Math.min(a, b)" to limit the number of iterations in your for loop.

Updating the high score table at game end
=========================================

It would be nice to update the high score table once the game has finished. It seems all we need to do is call the update_hiscore() function at the right time. This would work but seems a bit ugly:

* At game end, the client POSTs the game result to the server
* The server replies with... nothing in particular
* The client immediately after GETs the updated high score data

It seems more elegant if we, when the client POSTs the game result, reply with the new high score data, and have the client use this to update the high score list - no additional GET necessary. So let's do this instead.

*Exercise*: In your server code, in your POST handler, have the POST handler return the JSON string the same way your GET handler does when the high score data is being requested. Instead of copying code, move any common code to a function and call that function from different places instead.

*Exercise*: In your client code, when POSTing the game result, have the handler call display_hiscore() the same way you do when GETting high score data at page load.

Congratulations! Our odyssey of turning our guessing game to a simple web app is now done.


