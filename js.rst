Guessing game in JS
-------------------

Let's write a guessing game. This game is fairly simple: the computer thinks of a number between 1 and 25, and you need to guess what it is. The computer will give hints such as "my number is smaller" or "my number is bigger" on wrong guesses.

Moreover, let's write this such that it's run in the browser. It should look something like this:

.. image:: guess.png

Now, in order to do this we're going to need Javascript and HTML.

In order to get started, let's search online for "html js". We'll find lots of tutorials and reference material for web development, including Javascript tutorials. You can go through those as much as you want.

The development environment will work like this: create a file with an extension .html and open it in your text editor. After you've made changes to your file, you can open it in your browser. For example, if your file is called hello.html, you can open it in Firefox by running "firefox hello.html" in your terminal. When you make changes to the file, refresh the tab in your browser.

What happens here is that the browser will read in the contents of your file and interpret it. The browser has an engine for reading and rendering HTML, such that if you for example mark a part of text as red in your HTML code, the browser will know to render that text red. Similarly the browser has a Javascript interpreter built in, and will execute the Javascript code embedded in the HTML file.

In order to break down this task, we need to identify the main components of our software. We can accomplish this breakdown by looking at the tutorials online.

After browsing some tutorials we're able to construct one of the most basic HTML files:

.. literalinclude:: js1.html
   :language: html
   :linenos:

You can copy-paste this code into a file and open it in your browser. You should see a plain web page with a text "some text".

If we go through this in detail, we see a bunch of tags such as "<html>" which mostly have a start and an end (e.g. "</html>", and the text "some text" in the middle. Most of the tags aren't very important now though I should note the tag "<p>" denotes a paragraph.

After browsing some tutorials we can write one of the most basic "programs" that utilise JS and HTML:

.. literalinclude:: js2.html
   :language: html
   :linenos:

If you run this in your browser, you should see a text "old text" and a button. If you press the button, the text is replaced with "new text".

What happens here is that the paragraph has an ID, and the button has a Javascript function associated with it. When pressing the button, the browser executes the Javascript code which changes the text.

In general, HTML describes the content, i.e. what is shown to the user, while Javascript describes the logic, i.e. what happens.

Let's put together one more example to capture the other bits and pieces we need for our game:

.. literalinclude:: js3.html
   :language: html
   :linenos:

This example introduces a few concepts:

* A random floating point number between 0 and 1 can be obtained by calling Math.random()
* Numbers can be rounded down by using Math.floor()
* HTML tag <input> with type="text" introduces a text box, the value of which can be read using Javascript.

Now we have everything we need to put our game together. What we'll need is:

1) Some JS logic to generate the number and check the guess against the number the computer is thinking of
2) A field where to input what the guess is
3) A button indicating we'd like to make a guess
4) Text indicating whether our guess is correct or not

You may need to debug Javascript, for example if you make a typo, causing your program to not run correctly. How to debug Javascript depends on your browser. For example with Chrome, you can hit F12 to bring up the developer information panel. This shows any Javascript errors, for example. You'll also need to store the value to be guessed as a global variable, that is, outside any function. This ensures the value will stay persistent.

*Exercise*: Implement the guessing game. Good luck! Once you're done, or if you get stuck, you can take a look at the appendix to see an example implementation.

*Exercise*: Have the program count the number of guesses, and display the total number at the end.
