Drawing the schedule screen using SDL2
--------------------------------------

We're now able to draw some text on a screen using SDL. Let's expand this so we can draw the bus schedule screen.

As per the requirements and our engineering spec, we need to draw 23 different text labels. We'll want one piece of C++ code to be shared for both implementations but let's start with the simpler one - the Unix way. We have this written in our engineering spec:

* "display" shall take one parameter: a filename for the output of "merge". It shall output, to a window in a test environment, the bus information as specified in the requirements. It shall read the contents of the input file every ten seconds to update the output view.
* "merge" shall [...] output, to stdout, 23 lines, each one containing the text for one label, from top to bottom, from left to right, i.e. the bus stop name, current time, then route number, destination and arrival time for the next seven buses.

So let's adapt our SDL code to read in a text file which has 23 lines, and then display these lines on the final image.

If your existing SDL code is anything close to mine, it's difficult to extend. The exercise solution has all the code in one function, main(), which has about 50 lines of code including several variables declared along the way which we don't really care about but which are necessary for the rendering. Surely we can do better.

Refactoring
===========

Taking existing code and modifying it such that the functionality stays the same but the code is cleaner or easier to maintain is called *refactoring*. Let's refactor our code to start with. First, let's consider how we'd want our code to look for the purpose of the specification of showing 23 different labels based on file contents. Our input is the file name, our output is shown on the screen, and we need a bunch of SDL related variables in between to take care of the displaying.

Generally, a function is a nice way to abstract *stateless* logic - where you have some certain input, causing some certain output, not specifically depending on anything external. A class is a way to capture *state* - such that the member functions of an object also produce some output for a certain input but the output also depends on the state, i.e. the member variables, of the object. Because we need a few variables which are relatively fixed after the initialisation, such as the font handle, let's implement a class that takes care of our task in a clean way.

A good way to start designing such a class is to write *the interface* - the public functions that the class exposes. We want the object creation to only take the filename as input, and we want to update the screen every ten seconds. It seems like one way to design this, then, is to have a constructor and a render member function. We'll also need a *destructor* - a function that's automatically called when the memory the object resides in is freed.

We can also already think about the private member variables for this class. Since we already have a starting point for our work in the previous exercise's main function, we can think of all the variables used in the main function to be private member variables of our class. We'll then end up with the following class declaration:

.. code-block:: cpp
    :linenos:

    class SDL_Schedule {
        public:
            SDL_Schedule(const std::string& filename);
            ~SDL_Schedule();
            void render();

        private:
            TTF_Font *font;
            SDL_Window *screen;
            SDL_Renderer *renderer;
            SDL_Color col_white;
            SDL_Rect dest;
    };

In other words, let's refactor our code such that it does what it did before, but such that the old code now resides in a class, whereby the initialisation and the cleanup is captured in the constructor and destructor functions and the actual rendering of the text happens in another member function. This way, when making further changes to actually use the data from the file to decide what to render, future code changes will be easier to make.

Once we have the functions defined then we can have the main function instantiate an object of this class, call the render() function, wait for three seconds, and exit. The main function could have e.g. the following contents:

.. code-block:: cpp
    :linenos:

    int main(int argc, char** argv)
    {
        if(argc != 2) {
            fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
            return 1;
        }
        SDL_Schedule sched(argv[1]);
        sched.render();
        SDL_Delay(3000);

        return 0;
    }

Here we also parse the command line options. Although we don't yet have any code to read in the file, as per the class interface we'll need to pass the function some string as the filename, so we can just as well go ahead and pass on the actual command line parameter. If we were to compile such a main function to a binary and run the binary with command line options such as e.g.:

.. code-block:: bash

    $ ./main file.txt

...then the variable "argc" (argument count) would be 2, and the variable "argv" (argument vector) would be a pointer to an array of two pointers, first one (argv[0]) pointing to a character buffer containing "./main" (null delimited) and the second one (argv[1]) pointing to "file.txt". This way we can access the command line options in our C or C++ code.

Here, then, we check that there was one command line parameter passed to our function, and pass that on to our constructor. We then call the "render" member function, wait for three seconds, and exit the program, which also calls the destructor of our SDL_Schedule class as a side effect.

*Exercise*: Refactor your existing code to live in a class instead of a function. You need to define the functions mentioned above, i.e. the constructor, the destructor and the render function, and ensure they include the code of your old main function which displayed "Hello world" on the screen. You don't need to read in the file contents at this stage.

This pattern of having a class constructor do all the necessary initialisation to use the object, and the destructor free all acquired resources is called *RAII* (resource acquisition is initialisation), and is a C++ specific pattern for ensuring objects have a valid state. In order to have good error handling in the constructor, it's typically necessary to *throw an exception* in case of an error. This can be done e.g. by stating 'throw std::runtime_error("error")' in your code. You may need to #include <stdexcept> to get std::runtime_error in scope.

Now that we've done some refactoring, we can go ahead and do the rest of the work.

Reading in the labels
=====================

A quick online search will reveal a way to parse lines from a file to a std::vector<std::string> in C++:

.. code-block:: cpp

    #include <fstream>
    #include <sstream>

    ...

    std::ifstream ifs(file);
    std::string contents((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));
    std::stringstream ss(contents);
    std::string to;
    std::vector<std::string> res;
    while(std::getline(ss, to, '\n')) {
        res.push_back(to);
    }

Here, "file" is our input file name. The C++ way includes defining several intermediate variables but the final output we actually care about is stored in the "res" variable.

*Exercise*: Integrate the above to your program. Store the parameter to your constructor in a member variable. In the render function, read the file contents using the snippet above. For now you don't need to do much with the result though you may want to write it out to stdout. In order to test it, create an input file.

.. only:: html

  You may also download a sample input file here:

  * :download:`label_info.txt <../material/bus/label_info.txt>`

.. only:: not html

  You may also download a sample input file from the book web site.

Now, instead of drawing a single "Hello world" label, let's draw 23 of them.

You'll need to write a loop in your render() function that loops through your std::vector<std::string>.

Instead of passing a static string to TTF_RenderUTF8_Blended, pass in a string that you read from the file. You can convert an std::string to a character buffer by calling the "c_str()" member function, e.g:

.. code-block:: cpp

    std::string s("abc");
    const char* my_pointer = s.c_str(); // my_pointer now points to "abc"

Define another colour, in addition to white; this other colour should be yellow and used for the first row of labels. In your for loop, make the colour depend on the loop variable.

Before, for our "Hello world", we used an SDL_Rect variable to determine the location of the label. We need to do the same thing here. This is the formula to get the correct layout (whereby (0, 0) is the top left corner of the screen):

* The first column should be 10 pixels from the left border of the screen
* The second column should be 100 pixels from the left border
* The third column should be right aligned, such that the text ends 10 pixels from the right border of the screen
* The first row should be 10 pixels from the top of the screen
* The second row should be 80 pixels from the top of the screen
* The following rows should be 55 pixels further down; e.g. the third row should be at 80 + 55 = 135 pixels from the top of the screen
* The font size should be 36

You'll need to define the SDL_Rect variable correctly and pass it to the SDL_RenderCopy() function so that the layout of the labels is correct. You can do this using loops. The right alignment of the text can be done by taking the width of the screen (640 pixels) and subtracting the width of the label which is available after calling the SDL_QueryTexture() function.

*Exercise*: Add the necessary code in your program as described above to display the 23 labels on the screen. If you made it here, congratulations.
