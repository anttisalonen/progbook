Adding data to an SQL database
------------------------------

Now, we don't really have an online shop with products and customers so we have to generate some mock data. Python with its random number generator come into rescue here.

For example, for our products we have two columns we need to generate data for: the product name and size. Here's an example code snippet to generate 400 different products:

.. code-block:: python
    :linenos:

    import random

    # ... get hold of the database and the cursor here

    for i in xrange(100):
        color = random.choice(['Red', 'Green', 'Blue', 'Yellow', 'Black', 'White', 'Orange'])
        gender = random.choice(["Men's", "Women's"])
        clothing = random.choice(['Jeans', 'Shirt', 'T-Shirt', 'Cardigan', 'Skirt', 'Dress', 'Belt'])
        name = '%s %s %s' % (color, gender, clothing)
        sizes = random.choice([['S', 'M', 'L', 'XL'], ['36', '38', '40', '42']])
        for size in sizes:
            cursor.execute('INSERT INTO products(name, size) VALUES(?, ?)',
                    (name, size))

Let's go through this line by line:

* Line 1: We import random, the Python random number generator library
* Line 5: We have a loop we loop through a 100 times
* Line 6: We select a random string, each describing a colour. The Python function random.choice() will select one item from a given list at random.
* Lines 7-8: We repeat the above for more descriptions of a clothing.
* Line 9: We combine our three words to one possibly not very sensical product name.
* Line 10: We select a list by random: either a list with elements 'S', 'M', 'L', 'XL' or with elements '36', '38', '40', '42'. Either way, a list of strings.
* Lines 11-13: We execute our SQL insert statement which inserts a new row into our "products" table, with our name and size as values.

Note that you'll need to run db.commit() before the changes are actually made persistent.

The insert statement deserves a bit more attention. There are the following things to take note of:

* One insert statement adds one row in the table. You need to supply the table you want the new data to be added for, and exactly what data you want to add.
* The columns for which data is added are provided in parentheses after the table name. In our case, these are "name" and "size". For the first column, "id", SQLite generates a number automatically (by picking a number not used before, typically one larger than the largest one currently in the database).
* We then provide the values for the columns in parentheses after the "VALUES" keyword. Because we want to read the values from our variables, we use the question marks as *placeholders* instead, and provide the variables as another parameter, in a tuple. SQLite will then replace the placeholders with the contents of the variables.

Hence, the insert statement will add random product data to our database.

The variable contents could also have been included in the insert statement using Python string syntax:

.. code-block:: python

    # Do NOT do this ever
    cursor.execute('INSERT INTO products(name, size) VALUES(%s, %s)' % (name, size))

However, the difference is that the Python string syntax (%s) inserts the variable contents in the string blindly while the SQLite built in syntax (?) *sanitises* the input first. This means that using the SQLite syntax is not a security hole unlike when the Python string syntax is used: if the variable name contents were dependent on user input, an attacker could craft an input string such as "foo'); DROP TABLE users; --" which, if inserted into the SQL statement blindly, would delete the table "users" in the database. Depending on the database the attacker could e.g. change or read passwords or other sensitive data. This kind of an attack is known as an *SQL injection*. To avoid this, use the SQLite built in syntax (?) when substituting variables in SQL statements.

Now that we have some mock products in our database, let's add some mock customers.

*Exercise*: Include the above snippet in a script that generates data in your database. Add another snippet that generates random customers: you'll need to generate and insert names, addresses, zip codes and cities for them. You can also use the function random.randint(a, b) which generates a random integer between a and b (both numbers inclusive), or random.shuffle() which randomly shuffles a list.

*Note*: You can set the *seed* of the Python random generator. E.g. running "random.seed(21)" before will set the seed to 21 for all future random numbers. This means that the random numbers will be *deterministic*, i.e. the output of the random functions will be the same every time you run your code. This can be useful for debugging and development purposes.

We now have some products and customers but there are no references between data. Generating some orders will help here. An order has a date column as well as a foreign key to the customer ID. Once we have generated a random year, month and day, we can turn them into a string using the following:

.. code-block:: python

    date = "%04d-%02d-%02d" % (year, month, day)

And once we have this and the customer ID, we can insert a new row in the "orders" table like this:

.. code-block:: python

    cursor.execute('INSERT INTO orders(date, customer_id) VALUES(?, ?)',
            (date, customer_id))

Now, how do we get the customer ID? We could query for one, but there's another way. After an insert statement, SQLite provides the means to retrieve the primary key of the row that was just inserted like this:

.. code-block:: python

    cursor.execute('INSERT INTO customers(name, address, zipcode, city) VALUES(?, ?, ?, ?)',
            (name, address, zipcode, city))
    customer_id = cursor.lastrowid

That is, the attribute "lastrowid" of the cursor object will provide the primary key.

*Exercise*: Generate some orders for your customers.

We now have some orders but the orders don't include any products so let's fix that. We can do this by generating some rows for the "products_ordered" table. What we'll need for this are the order ID as well as the product ID. How would we know what product IDs there are? One way is to store the returned primary key for each product table insert statement in a list. Alternatively, you can also *query* the product IDs after you've inserted them. The following code queries all product IDs in the database:

.. code-block:: python

    cursor.execute('SELECT id FROM products')
    all_product_ids = cursor.fetchall()
    all_product_ids = [x for (x,) in all_product_ids]

* The first line asks SQLite to execute a query to the "products" table such that the result of the query includes only the "id" column
* The second line fetches all the results of the query and stores them in a list
* When storing the results of a query, SQLite returns a list of tuples where each element in the tuple is one column of the row. When we query for only one column, the result is a list of tuples where each tuple has only one element (yes, such tuples exist, at least in Python). The third line converts this list of tuples to a list of integers using list comprehension.

*Exercise*: Generate a few rows for the "products_ordered" table. Use the order ID from the orders you've generated. You can either store the product ID in a list after inserting a product and reuse those, or query for the product IDs.

We now have customers, products, and customers ordering products. The final bit is returns.

*Exercise*: Generate some rows as return reasons for the "return_reasons" table. Note that SQLite requires the placeholder parameter for the insert statement to be a tuple so you may need a tuple with only one element.

*Exercise*: Generate some rows for the "returns" and the "products_returned" tables. You will need to keep hold of the order ID and the ID for the "products_ordered" table.

We now have some nice mock data. In the next section we'll learn how to query it in detail.
