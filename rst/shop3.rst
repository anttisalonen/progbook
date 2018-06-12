Querying SQL databases
----------------------

In this section we'll learn how to query, or get data from our SQL database.

Querying an SQL database can be done using the select statement. We've already seen a couple of uses of this, e.g.:

.. code-block:: python

    cursor.execute('SELECT id FROM products')
    all_rows = cursor.fetchall()
    for row in all_rows:
        print row

This snippet will get all the product IDs from the database into a list of one-tuples and print the contents of those tuples.

Here's another query:

.. code-block:: python

    cursor.execute('SELECT count(id) FROM products')
    row = cursor.fetchone()
    print row # will print e.g. (400, )

* We tell SQLite we want to query for the *count* of IDs in the products table. The number of product IDs corresponds to the number of rows in the table so the result will indicate how many products we have stored in the database.
* Instead of using fetchall(), we can use fetchone() which will only give the first (and in this case, only) row of the query result.
* As before, the result will be a tuple, with one element in the tuple per column that was queried for. In this case, the result will be a one-tuple with an integer, namely the number of products.

Here's a bit more complicated query:

.. code-block:: python

    print 'distribution of return reasons:'
    cursor.execute('''SELECT   return_reason_id, count(products_returned.id)
                      FROM     products_returned
                      GROUP BY return_reason_id
                      LIMIT    5''')
    for row in cursor.fetchall():
        print row

There are a few points to make here:

* We select the return_reason_id and the count of ids. The column names listed between SELECT and FROM define which columns are included in the table that will be returned.
* We specify we want to receive the count of the column "products_returned.id". In this case, simply typing "id" would've been enough. Generally, the table can be written before the column name to be clearer about which column you mean. In some cases, as we shall see, you must type the table name in order to clear ambiguity. (SQLite will return an error when the table name was required but not provided.)
* We want to *group* the result by the column "return_reason_id". This means that SQLite will, on the table that is the query result, group the column "return_reason_id" such that each ID will only appear once. In other words, the resulting table (the list that is returned by fetchall()) won't have any more elements than the number of return reasons we have in the database.
* We furthermore *limit* the number of rows in the resulting table to 5. This means, we only query for and receive the data for the first five reason codes.

*Exercise*: What would you expect the output of the above code snippet to be? Try it out.

You may ask yourself, for business analytics to be more useful, if you wanted to limit the output of the above query at all, you wouldn't limit it based on the reason code. Instead you'd want to know e.g. the most common return reasons. This can be done as well:

.. code-block:: python

    cursor.execute('''SELECT   return_reason_id, count(products_returned.id)
                      FROM     products_returned
                      GROUP BY return_reason_id
                      ORDER BY count(products_returned.id) DESC
                      LIMIT    3''')
    for row in cursor.fetchall():
        print row

Here, we include the keywords *order by*. This means we want to order by the count of the products returned ID, i.e. the number of products that were returned. If you wonder which column to order by, it may help to imagine the resulting table: we define the columns in the result after the SELECT keyword, so our table will have the columns "return_reason_id" and "count(products_returned.id)". As we furthermore group the return_reason_id i.e. have at most only one row per return reason, the column we'd want to order by would result to be the number of products returned.

The keyword "DESC" means we want to order in descending order, i.e. from the most to the least. (Alternatively the keyword "ASC" could be used for the opposite direction.)

Joins
=====

Querying a single database table is fun enough, but things get more interesting when combining multiple tables.

Let's say we want to find out our top five ordered products. How would we imagine the table to look like? We'd want to know the product ID, name, size and how often it was ordered, sort by the number of orders, and group by the product ID as we'd want no more than one row per product. The first three columns we can retrieve from the products table, while the fourth one (how often it was ordered) we can, as may be apparent from our database schema, retrieve by counting the products_ordered.id column.

This query would then look like this:

.. code-block:: python

    cursor.execute('''SELECT     products.id, products.name, products.size, count(products_ordered.id)
                      FROM       products
                      INNER JOIN products_ordered ON products_ordered.product_id = products.id
                      GROUP BY   products.id
                      ORDER BY   count(products_ordered.id) DESC
                      LIMIT      5''')

Now, this is similar to the previous example but we have two new interesting characteristics:

* On the first line we describe not only columns from the table we're querying data from, but also *another* table, namely "products_ordered".
* Because we need a column from another table in our result, we need to *join* the other table. Hence we include the keywords "INNER JOIN" to join the table "products_ordered".
* We need to tell SQLite *how* to join the other table, so we tell it to join "ON products_ordered.product_id = products.id" - meaning, if a row from the "products" table has the same ID as the "product_id" column in the "products_ordered" table then the two rows will be joined into one in the query result.

There are different kinds of joins like outer join, left join etc. but in practice inner join, where two tables are joined at the intersection (i.e. when a foreign key matches the primary key of a row in another table) is the most common.

*Exercise*: Try out the above statement.

But there's more! You can also join *multiple* tables using multiple joins in one statement. For example, if you wanted to find out which products were returned the most, you'd need to have the product ID in your query result and order by the number of products returned, i.e. products_returned.id. However, there's no direct link from the "products" table to the "products_returned" table because of the many-to-many relationship, so we'd need to also join the "products_ordered" table. In practice this query would look like this:

.. code-block:: python

    cursor.execute('''SELECT     products.id, products.name, products.size, count(products_returned.id)
                      FROM       products_returned
                      INNER JOIN products_ordered ON products_ordered.id = products_returned.product_order_id
                      INNER JOIN products         ON products.id         = products_ordered.product_id
                      GROUP BY   products.id
                      ORDER BY   count(products_returned.id) DESC
                      LIMIT      5''')

This is very similar to the previous statement, but it has two joins: we query the table "products_returned", but join the table "products_ordered" to it such that the product order ID matches. We then join the "products" table such that the product ID between the "products" table and the "products_ordered" table matches.

*Exercise*: Try out the above statement.

*Exercise*: Find out which customers have put in the most orders. You'll need to select the customer ID as well as the customer name and the number of order IDs. Join the "orders" table with the "customers" table. Group by the customer ID. Order by the number of order IDs.

*Exercise*: Find out who has ordered most products. The query is similar to the previous exercise but you also need to join the "products_ordered" table.

*Exercise*: Find out the top five customers who have returned most products.

Another useful keyword is "WHERE" which allows you to filter which data you want in the result database. For example, we might want to see the details for the product with ID 123:

.. code-block:: python

    product_id = 123
    cursor.execute('''SELECT *
                      FROM   products
                      WHERE  products.id = ?''', (product_id, ))

Here, we supply the wildcard "*" to SELECT which tells SQLite that we want all the columns from the queried tables. This way we don't need to specify all the columns manually.

Besides only looking for columns with a specific value, WHERE also allows for different operators such as boolean (AND, OR), arithmetic operators (>, <, etc.) or keywords like BETWEEN (values within a range) etc. You can look up the details at an SQL reference if you're interested.

*Exercise*: List the products ordered by customer 123.

We now have some knowledge around querying useful data from the database. In the next section we can put together some code to generate a return form.
