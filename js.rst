
Task: Guessing game
-------------------

Let's write a guessing game. This game is fairly simple: the computer thinks of a number between 1 and 25, and you need to guess what it is. The computer will give hints such as "my number is smaller" or "my number is bigger" on wrong guesses.

Moreover, let's write this such that it's run in the browser. It should look something like this:

.. image:: guess.png

Now, in order to do this we're going to need Javascript and HTML.

In order to get started, let's google for "html js". We'll end up at w3schools.com, a website with lots of tutorials and reference material for web development, including Javascript tutorials. You can go through those as much as you want.

The development environment will work like this: create a file with an extension .html and open it in your text editor. After you've made changes to your file, you can open it in your browser. For example, if your file is called hello.html, you can open it in Firefox by running "firefox hello.html" in your terminal. When you make changes to the file, refresh the tab in your browser.

What happens here is that the browser will read in the contents of your file and interpret it. The browser has an engine for reading and rendering HTML, such that if you for example mark a part of text as red, the browser will know to render that text red. Similarly the browser has a Javascript interpreter built in, and will execute the Javascript code embedded in the HTML file.

In order to break down this task, we need to identify the main components of our software. We can accomplish this breakdown by looking at the tutorials at w3schools.com.

For example, we can take the example about adding numbers in Javascript, i.e. https://www.w3schools.com/js/tryit.asp?filename=tryjs_variables_add_numbers.

This example shows how to run Javascript and show the results on a webpage, but it's missing the interactivity that we need in our game, namely a button and a field for entering the guess.

Browsing the tutorials some more, we'll also find examples for generating random numbers, adding a button and a field (HTML form). Namely these:

https://www.w3schools.com/js/tryit.asp?filename=tryjs_random

https://www.w3schools.com/js/tryit.asp?filename=tryjs_functionarg2

https://www.w3schools.com/html/tryit.asp?filename=tryhtml_form_submit

Now we have everything we need to put our game together. What we'll need is:

1) Some JS logic to generate the number and check the guess against the number the computer is thinking of
2) A field where to input what the guess is
3) A button indicating we'd like to make a guess
4) Text indicating whether our guess is correct or not

One more tricky bit is that the random function returns a floating point number, while the guessing should be based on integers. Searching for this online you can find a question around this at Stack Overflow (https://stackoverflow.com/questions/596467/how-do-i-convert-a-float-number-to-a-whole-number-in-javascript): by using Math.floor(value) we can ensure the number the computer is thinking of is integer.

You should now have the pieces you need to implement the guessing game. Good luck! Once you're done, or if you get stuck, you can take a look at the appendix to see an example implementation.

Extra work: have the program count the number of guesses, and display the total number at the end.
