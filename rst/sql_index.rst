SQL and its relationship with online shops
==========================================

If you've ever shopped for clothes online, you may be familiar with the return form for returning products you don't want to have: you order something, decide for whatever reason you don't want to keep some products, and, along with some piece of paper, send the products you don't want back. The piece of paper may either be a form you fill out yourself or something you receive pre-filled from the online shop and print yourself.

In this section we'll assume the role of an online clothing shop and write some code to generate that return form in PDF format. Our business works like this:

* Our products are all stored in a warehouse which is automated as much as possible to save money and give a competitive advantage, with robots moving products from the massive warehouse to conveyor belts which further transport the products to workers which actually package and send the packages.
* Upon receiving a return package, a worker will look for the return form in the package which will include a *bar code* which the worker will scan. The bar code will contain information about which products were returned such that the worker can then hand the products directly over to a robot.

From customers' point of view, it looks like this:

* When a customer orders something, we send the products to them, and note the order in the database
* When a customer decides to return something, they enter a web page where they can enter which products they want to return, and why. We'll store this information in the database. The customer will then receive a PDF for printing which they'll send to us alongside the products.

This all furthermore implies that we have a database with a bunch of relevant information, like who our customers are, what our products are, who has ordered or returned what etc.

From technical point of view, in this section we'll learn about the following:

* SQL - schemas, adding, modifying and querying data
* Generating a bunch of random test data
* Generating PDFs and barcodes using Python
* Primitive business analytics
* Displaying database contents, filling forms and sending PDFs using Flask (again - but this time without Javascript)

The whole thing will end up at about 300-400 lines of Python, with a good chunk of it being SQL statements.

.. toctree::
   :maxdepth: 1

   webshop
   shop2
   shop3
   shop4
   shop5


