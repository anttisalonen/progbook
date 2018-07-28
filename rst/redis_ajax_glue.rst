Gluing AJAX and Redis together
------------------------------

After the exercises in the previous section we're starting to have some pieces we can put together. Let's start with a couple more exercises.

*Exercise*: Modify your guessing game such that when the number was guessed correctly, send the number of guesses to the server in JSON. You don't yet need to handle this data on the server side.

*Exercise*: Connect to the Redis server in your Python module. Note that the code in the Python module will be executed automatically when you start Flask. This means you can connect to the Redis server in your top level Python code (e.g. with "r = redis.StrictRedis(host='localhost', port=6379, db=0)"), without having to define a function for this.

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

* Line 5: We can query the current system time by calling the function "datetime.datetime.now()" (after importing datetime). Furthermore we can convert it (and almost any other data type including numbers) to a string by calling the str() function.
* Line 7: Here, we insert data in the Redis database whereby the connection is represented by the "r" object. We use the member function "lpush" which ensures the value in the key-value pair is a list. More specifically, it:

  * Takes two parameters, the key and a value
  * Creates a new list if the key didn't yet exist, such that the value is a new list with a single element, namely the value
  * Appends the value to the end of the existing list if the key existed

* Line 8: We return a string which will be available in JSON in the xhr.responseText variable.

Here, we also implicitly defined the *database schema* for our data: the key is the maximum number that the computer could think of, as a string. For now it's hard coded to 25 but we'll make it configurable later. The value is a list whereby each element in a list describes a finished game. Each finished game should be represented in JSON, such that the JSON has a field describing the number of guesses required in the game as well as the timestamp when the game was finished. We'll expand on this later.

Note that there are different ways the schema could be defined for this use case - this is one of the simplest ways but in general, schemas can be defined in significantly different ways, depending on the use case, amount of data and any performance requirements.

*Exercise*: When your server receives the number of guesses from JavaScript, store this in the Redis database.

Here are some hints:

* If you need to purge the database, call r.flushdb(). This will erase all data in the database.
* You may want to write a small script or a URL handler to check the contents of the database to ensure you're adding the correct data. You may alternatively want to use the Redis command line interface for this.

This should result in two relevant URLs that we serve in our Python code:

* The guessing game itself: an HTML page with the relevant JavaScript that allows the user to play the game
* The URL that receives the number of guesses and is called by our AJAX request

We should now be able to store something resembling a high score list in a database, but we're still lacking the possibility for the user to enter his or her name, configuring the maximum number the computer thinks of and understanding how exactly turn the contents of a key in a database to a correctly sorted high score list. This will be the scope for the next sections.
