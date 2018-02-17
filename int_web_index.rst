Intermediate web development
============================

.. toctree::
   :maxdepth: 1

   js2
   html_tables
   redis
   ajax
   guess_start
   hiscore_table

In this section we'll turn our guessing game from a single HTML page to a simple web app.

We'll start by adding some logic to gain some more experience in JS, and then learn about the following topics:

* HTML table generation
* JSON; which we'll need to transfer data between backend and frontend
* Setting up a simple NoSQL database
* AJAX; data transfer between backend and frontend in practice
* Putting everything together

What we'll end up is a two-page guessing game as well as a simple algorithm to play it for us, with a high score list displayed on the page and stored in a database. The high score list could look e.g. like this:

+----------+----------------------------+------+---------+
| Position | Date                       | User | Guesses |
+==========+============================+======+=========+
| 1        | 2018=02=14 22:00:47.418645 | ai   | 5       |
+----------+----------------------------+------+---------+
| 2        | 2018-02-14 22:01:14.371682 | User | 6       |
+----------+----------------------------+------+---------+
| 3        | 2018-02-14 22:00:35.543943 | ai   | 6       |
+----------+----------------------------+------+---------+
| 4        | 2018-02-14 22:00:45.238895 | ai   | 7       |
+----------+----------------------------+------+---------+
| 5        | 2018-02-14 22:00:51.128031 | ai   | 8       |
+----------+----------------------------+------+---------+

It doesn't sound like much but includes lots of software development fundamentals.
