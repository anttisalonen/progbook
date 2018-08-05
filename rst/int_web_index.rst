Web development with Python and JavaScript
==========================================

In this chapter we'll turn our guessing game from a single HTML page to a simple web app. Our goal is to have a *high score table* for the guessing game: the page should store and track the number of guesses for different players, and show which players have finished the game in the smallest number of guesses.

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
| 1        | 2018-02-14 22:00:47.418645 | ai   | 5       |
+----------+----------------------------+------+---------+
| 2        | 2018-02-14 22:01:14.371682 | User | 6       |
+----------+----------------------------+------+---------+
| 3        | 2018-02-14 22:00:35.543943 | ai   | 6       |
+----------+----------------------------+------+---------+
| 4        | 2018-02-14 22:00:45.238895 | ai   | 7       |
+----------+----------------------------+------+---------+
| 5        | 2018-02-14 22:00:51.128031 | ai   | 8       |
+----------+----------------------------+------+---------+

It doesn't sound like much but includes some software development fundamentals.

Again, this book does *not* cover modern web development practices. However, the following sections will help the reader understand some concepts related to web development such as the difference around frontend and backend and using a database in the backend.

.. toctree::
   :maxdepth: 1

   html_tables
   redis
   web_arch
   ajax
   redis_ajax_glue
   guess_start
   hiscore_table
   web_tips


