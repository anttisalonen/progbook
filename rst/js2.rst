Javascript meets algorithms
---------------------------

Try to play the guessing game a few times.

What is the best strategy to win the game in the least number of guesses? Have a think before reading further.

You may have noticed that the best strategy is to guess at the middle point of the available range, that is, 12 or 13 when the maximum number is 25, and then on the middle point of the available range again depending on the answer, so either 6 if the number is smaller or 19 if the number is larger. This method will in general arrive at the right answer in the minimum number of guesses.

In terms of big O notation, the performance of such an algorithm would be O(log n), i.e. doubling the problem space will cause one more iteration. Think about it: if the maximum number was 50, the first guess should be 25, after which we know the number is either below 25 or between 26 and 50, such that the range is the same as if the original maximum had been 25.

This algorithm is called "binary search" because it searches for an answer, and splits the search space in half with each lookup. It's one of the most important concepts in computer science.

What we'll write next is a binary search algorithm to solve the game for us. Let's add a button that whenever it's pressed, it'll make an ideal guess (without looking up the correct answer!), such that we can skip playing the game and hit the button enough times to have the function find the correct number for us.

In more detail, what should happen is that pressing the new button will call a function which will need to decide what to guess, enter that number in the field, press the button to make a guess, and read and understand the text saying whether the number was too small, too big or correct.

Here are the building blocks for this to get you started:

.. code-block:: javascript
    :linenos:

    var my_guess = /* TODO */;
    document.getElementById("guess").value = my_guess;
    document.getElementById("guess_button").click();
    var answer = document.getElementById("paragraph").innerHTML;
    if(answer.search("smaller") != -1) {
        /* TODO */
    }

The above snippet demonstrates the following:

* On line 2, the label is set to be whatever the value of "my_guess" is
* On line 3, a button with the ID "guess_button" is clicked
* On line 4, the text indicating the answer is stored in a variable
* On line 5, the search string is searched for the text "smaller". -1 will indicate the string was not found.

*Exercise*: What does the search() function as used on line 5 actually return, and what exactly can you pass to it as a parameter? Find the reference for the function online. What is the type of the variable "answer" in the snippet above?

The above should be enough to put the general infrastructure in place for our button. What's still required is the actual logic. I can share some hints:

* You will need to track the upper and lower limit of the range where the correct number might reside
* As these will need to be stored between button clicks (one click will only guess once), they need to be stored outside the function
* The upper and lower limit are needed to define the value to guess, and are influenced by the answer string

*Exercise*: Implement this button that plays the number guessing game.

*Exercise*: Once you've implemented this button, you might want to experiment with increasing the maximum number from 25 to something larger, for example 5000. See if your algorithm still finds the correct number with a small number of guesses. Make sure you define the maximum number only once in the source code such that there's no worry of your different functions going out of sync.

Calling a Javascript function at page load
==========================================

A quick search online reveals a way to do this:

.. code-block:: js

    function init() {
        /* my code goes here */
    }
    window.onload = init;

That is, we define a function which we want to be called at page load time, and install it as a *callback* function in the member function "onload" in the global object "window". (We'll get to the concepts of callback functions, member functions and objects in more detail later.)

*Exercise*: The original image of the game included a text "I'm thinking of a number between 1 and 25," but after the previous exercise, this might not be true anymore. Modify this string based on the variable you have that holds the maximum value. For this you'll need to run Javascript at the page loading phase.

*Exercise*: Assuming the maximum number is 5000, what is the maximum number of guesses the algorithm will need in order to find the correct number? Hint: the algorithm has O(log n) complexity.

Note that the Javascript code is executed in the browser. This means that if you were to load a public webpage with, for example, such a game, you can modify the HTML your browser renders to add a new button, and the Javascript your browser executes to solve the game. It may be difficult to identify and understand the correct Javascript in order to do this, depending on the web page, but as the Javascript execution and HTML are in the end controlled by the browser, in general there's nothing stopping any webpage visitors from making their own additions to the code they execute.
