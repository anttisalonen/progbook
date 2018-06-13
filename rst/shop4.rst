Generating a return form
------------------------

We now get to the meat of the chapter: generating the return form. We should have a program which has the following characteristics:

* As input, it should take the return ID
* As output, it should store a PDF in a file
* It should be possible to later easily refactor it to a function that could be called from a web app. This function should take the return ID as input and output a binary stream that represents the PDF that can be sent to the client over HTTP.

We'll do this in three steps:

* Fetch all the data we need for the form from our SQL database
* Generate the bar code
* Create an empty PDF, add all the data to it and save it

Fetching the data
=================

We'll be needing the following columns:

* Customer name and full address
* The ID, name, size and return reason for all products that will be returned
* The order ID
* The return ID (provided as input parameter)

*Exercise*: Given the return ID, fetch the above data. You can either combine everything in one query, where e.g. the customer data is duplicated for each row, or use multiple queries where you first fetch e.g. the customer data and then the data for the products to be returned.

Furthermore we'll be needing the contents of the table holding the return reasons.

*Exercise*: Fetch the return reason codes and descriptions.

Generating the bar code
=======================

Let's generate the bar code next. Let's first recap the motivation for the bar code: the worst case would be the worker having to pick each returned item, walk to the correct place in our huge warehouse and put the item in the correct box. The ideal workflow would be that the worker who unpacks the return package can put each returned item into a box on a conveyor belt, such that a robot would then come and pick up each box and store it in the correct place in our warehouse. How would we achieve this? The worker should be able to easily let the robot system know that they will send a number of boxes to the robots, each box holding one specific item, and the robots then pick up the boxes and store the items correctly. In order to transfer the information, the act of scanning a bar code should inform the system of what products are incoming.

What should the bar code encode? It seems the easiest way to communicate the information about the products returned is the return ID. How can we do this? A quick search online for a Python barcode library reveals the existence of pyBarcode which can be installed using "pip install pybarcode". We'll furthermore need to decide what kind of bar code we use. The library seems to support a few, e.g. EAN-13, ISBN and Code 39. Now, from software development point of view it doesn't really matter which one we use but Code 39 seems to be general purpose enough that it's suitable for our purposes.

Looking at the pyBarcode documentation and adapting it a bit, we can then end up with this snippet:

.. code-block:: python

    import barcode
    from barcode.writer import ImageWriter

    bclass = barcode.get_barcode_class('code39')
    code = bclass('123abc', writer=ImageWriter(), add_checksum=False)
    fullname = code.save('barcode')

This will create a file "barcode.png" which will include a Code 39 barcode of the string "123abc".

*Exercise*: Create a barcode image with the return ID. You'll need to convert the ID from integer to string using str(number).

Creating the PDF
================

Now that we have our data and our barcode, the last bit is creating the PDF. Again searching online it seems there are several Python libraries for generating PDF files. For the purposes of this book I picked pyfpdf. It can be installed using "pip install fpdf".

*Exercise*: Install pyfpdf. Look up its reference online.

Now, the interface that pyfpdf provides for creating PDF files is quite a bit wider than the one from pyBarcode. We can create a very simple PDF file with the following code:

.. code-block:: python
    :linenos:

    from fpdf import FPDF

    pdf = FPDF()
    pdf.add_page()
    pdf.set_font('Arial', 'B', 16)
    pdf.cell(40, 10, 'Hello world', align='l')
    pdf.output('output.pdf', 'F')

Going through this line by line:

* Line 1: import our library.
* Line 3: create a PDF object.
* Line 4: add a page (duh).
* Line 5: Set font: Arial, bold, size 16.
* Line 6: Create a *cell* which will hold text. As per pyfpdf reference, its width is 40 units (in this case 40 mm - by default the unit is mm and the page format is A4) and its height is 10 mm. The text in the cell is "Hello world" and it's left aligned within the cell. (For reference, an A4 page is 210 mm wide and 297 mm long.)

* Line 7: We output our PDF to output.pdf.

*Exercise*: Create a test PDF. Look up the reference for the cell() member function.

You may ask yourself, where on the page does the cell with the text land up in. The answer is, the default location. Pyfpdf is a *state machine* in that it maintains the current location of new elements internally.

Here's a snippet that demonstrates the following concepts:

* Including images
* Designating the position of a text label
* Having multiple text labels, one after another (like customer name and address)

.. code-block:: python
    :linenos:

    from fpdf import FPDF

    pdf = FPDF()
    pdf.add_page()
    pdf.set_font('Arial', 'B', 16)
    pdf.image('logo.png', 10, 10, 33)

    pdf.set_xy(10, 45)
    pdf.cell(40, 10, 'Hello world', align='l')

    pdf.set_xy(10, 80)
    pdf.cell(40, 10, "Text 1",  ln=1, align='l')
    pdf.cell(40, 10, "Text 2",  ln=1, align='l')
    pdf.output('output.pdf', 'F')

Let's look at this closer again:

* Line 6: We add the image "logo.png" in our PDF. The second and third parameters describe the position of the image. The fourth parameter designates the width of the image, here 33 millimeters. Pyfpdf calculates the height of the image automatically based on the width but if you wanted to stretch the image, as per pyfpdf reference, you could provide the height as the fifth parameter.
* Line 8: We set the X and Y coordinates of the next item to (10, 45).
* Line 11: We create our second text label. It will be created at position (10, 80) and have the width and height of 40 and 10 respectively. We pass the cell() member function the named parameter "ln" (line, or line break) with the value 1 which indicates that the next cell should appear below this cell (as opposed to to the right of it which is the default).
* Line 12: Because we passed ln=1 to the previous function call, the current X and Y positions are now shifted to right below the previous cell. This means that the text "Text 2" should appear right below the text "Text 1".

We now have some tools available for starting to create our return form.

*Exercise*: Create the beginning of the return form: if you have a picture available that could be used as a logo, include it. You can also pick up an image file from here: :download:`logo <../material/retail/logo.png>`. Add a label that designates the name of your company. Add labels including the name and address of the customer. Include the barcode around top right, and a label for the order number below that. See the :download:`example PDF <../material/retail/example.pdf>` for inspiration if necessary.

Now, how about the table that describes the products that are being returned? Here are the clues that you need:

* By passing the named parameter "border=1" to pdf.cell(), the cell will have borders. You can construct a table by including borders for each cell in the table.
* If you don't include "ln=0", the next cell will be to the right of that cell. If you do include "ln=1", the next cell will be below the leftmost cell on the previous line.
* In order to have the table fit on the page, you may need to reduce the font size, e.g. to 12.

In other words, if you were to run e.g. this:

.. code-block:: python

    pdf.cell(40, 10, "Text 1", border=1, ln=0, align='l')
    pdf.cell(80, 10, "Text 2", border=1, ln=0, align='l')
    pdf.cell(30, 10, "Text 3", border=1, ln=1, align='l')
    pdf.cell(40, 10, "Text 4", border=1, ln=0, align='l')
    pdf.cell(80, 10, "Text 5", border=1, ln=0, align='l')
    pdf.cell(30, 10, "Text 6", border=1, ln=1, align='l')

...then you'd end up with a 3x2 table, i.e. three columns and two rows. The widths of the columns would be 40, 80 and 30 mm. The heights of the rows would be 10 mm. The "ln" parameter controls the dimensions of the table.

*Exercise*: Put together the table showing all products that will be returned, including their product IDs, descriptions, sizes and reason code for return.

Now, the final bit missing in our PDF is the table describing the return reason codes.

*Exercise*: Add this table.

If you made it here, congratulations.
