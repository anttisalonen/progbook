Vim
---

In general, in order to install vim, I'll refer you to your OS. If on Linux, you should be able to install vim using your package manager, if it's not already installed. On Mac and Windows you should be able to find a way to install it after searching online.

After you've installed vim, you can start it in Unix shell by running e.g.:

.. code-block:: bash

    $ vim file.txt

This opens the file called "file.txt" in vim. If the file doesn't exist, vim opens an empty buffer which, if saved, will be saved as file.txt.

Vim is a modal editor. This means it has *modes*. When starting vim, it is in *normal mode*. You can always get to this mode by hitting the Escape key (ESC) three times. Go ahead, try it out.

The second mode is the *command line mode*. You can enter this mode from the normal mode by pressing colon (:) (without hitting enter afterwards). You should then see a colon at the bottom of the screen.

In command line mode, you can enter several commands. For exit, you can quit vim by typing ":q" followed by enter (or ":q<Enter>"). Try it out. Enter vim again.

Another useful command is ":w", for writing, i.e. saving a file. Try running ":w<Enter>". This should save file.txt. As we didn't enter any contents to the file, it'll be empty.

You can also combine commands. For example, the command ":wq" will save and quit vim.

The third mode is the *insert mode*. This allows you to actually type text in the file. One way to enter this mode from the normal mode is by pressing "i". Now, anything you type will be added in the buffer. You can exit the insert mode by pressing the Escape key.

For example, if you were to type "iHello.<ESC>", you'd enter the insert mode, add the text "Hello." in the file, then go back to the normal mode. Try this out. Save your file with ":w". If you want to be sure, quit vim and run "cat file.txt" to see the file contents. Enter vim again.

Now you have the basics of how to use vim for general file editing. So far, there isn't much that makes vim much better than any other editor.

Some more commands
==================

There are some more commands that are helpful when editing text. These commands are all run from the normal mode. For example, the command "Y" will copy the current line. That is, if you are in the normal mode (which you can reach by pressing ESC), then press the keys for capital Y (shift+y), you've copied the current line. You can then paste this line after the current line by pressing "p". Try this out. You should then have the line "Hello." twice.

You can repeat commands multiple times by entering a number before the command. Try, in the normal mode, after you've copied the line using "Y", to type "10p". This should add ten lines with the text "Hello." in them.

You can undo with the command "u". You can re-do (undo the undo) by pressing ctrl+r. Try this out a few times. You can undo until the point where you started vim, and re-do until the last command.

Let's modify one of our "Hello." lines. You can use the arrow keys to move around the file (or also the h, j, k and l keys). The command "x" will remove the character under the cursor. Try it out. You can also type e.g. "3x" to remove the three next characters.

You can remove a whole line using the command "dd". (This cuts the line such that you can paste it again.) You can remove the next e.g. three lines with "3dd". Try it out. Paste them again with "P". (Capital "P" pastes before the cursor, small "p" pastes after the cursor.) The text from the cursor to the end of the line can be removed with "D".

You can navigate in the file with arrow keys, but there are shortcuts. Typing "gg" moves the cursor to the top of the file. Typing "G" moves the cursor to the end of the file. "$" moves the cursor to the end of the current line. "^" moves the cursor to the beginning of the current line.

Move the cursor to the top of the file with "gg". You can delete e.g. the next 50 lines of the file by running "50dd". Run this to empty the file.

