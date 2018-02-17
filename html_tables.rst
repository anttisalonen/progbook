HTML tables
-----------

HTML tables are a fairly straightforward concept. Here's one:

.. code-block:: html
   :linenos:

        <table id="hiscore" border="1">
            <tr>
                <td>Position</td>
                <td>Date</td>
                <td>User</td>
                <td>Guesses</td>
            </tr>
            <tr>
                <td>1</td>
                <td></td>
                <td></td>
                <td></td>
            </tr>
            <tr>
                <td>2</td>
                <td></td>
                <td></td>
                <td></td>
            </tr>
        </table>

* Line 1: We start the table with the <table> tag. We assign it an ID and request it to be rendered with a border.
* Line 2: We start defining a row for the table using the <tr> (table row) tag.
* Line 3: We define a cell using the <td> (table data) tag. The contents are simply text in this case but can in general be any HTML.

In order to modify tables in Javascript, we can do the following:

.. code-block:: js
    :linenos:

    table = document.getElementById("hiscore");
    for(var i = 1; i < 3; i++) {
        table.rows[i].cells.item(1).innerHTML = "2017-02-15";
    }

* Line 1: We obtain a reference to the table
* Line 2: We iterate over two table rows using a for loop, skipping the header (both rows and cells, and Javascript arrays in general, are 0-indexed, meaning the arrays start at 0)
* Line 3: We set the contents of the second cell of the i'th row to "2017-02-15".

*Exercise*: Add a HTML table in your guessing game web page as a placeholder for the high score list.
