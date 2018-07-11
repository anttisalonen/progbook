Programming with vim
--------------------

In order to use vim for programming, it's usually a good idea to configure vim accordingly. Vim can be configured by entering commands in the file "~/.vimrc", i.e. the hidden file called .vimrc in your $HOME directory. Let's open this file in vim:

.. code-block:: bash

    $ vim ~/.vimrc

Depending on how vim is installed, it might already have some commands. What happens is vim automatically runs all the commands listed in the ~/.vimrc file in the command line mode when vim is started. For example, this line in ~/.vimrc:

.. code-block:: vim

    set hlsearch        " Highlight search results

...means that when vim is started, it automatically runs ":set hlsearch", which enables highlighting search results. (The quote character " in .vimrc denotes a comment until the end of the line.)

I recommend you add at least the following lines to your .vimrc file:

.. code-block:: vim

    syntax on                 " Syntax highlighting
    set background=dark       " Vim colours for dark background
    set showcmd               " Show (partial) command in status line.
    set showmatch             " Show matching brackets.
    set incsearch             " Incremental search
    set history=1000          " History of ":" commands remembered
    set hlsearch              " Highlight search results
    set autoread              " Automatically read file after it's been modified elsewhere
    set hidden                " Hide buffers when they are abandoned
    filetype on               " detect filetypes
    filetype indent on        " indent depending on file type

    autocmd FileType *.c,*.cpp set cindent   " c indent
    autocmd FileType *.py      set nocindent " no c indent for python

    " python specific
    autocmd BufNewFile,BufRead *.py set tabstop=4     " tabstop 4
    autocmd BufNewFile,BufRead *.py set softtabstop=4 " spaces in tab
    autocmd BufNewFile,BufRead *.py set shiftwidth=4
    autocmd BufNewFile,BufRead *.py set smarttab      " tab inserts spaces
    autocmd BufNewFile,BufRead *.py set expandtab     " tabs expanded to spaces
    autocmd BufNewFile,BufRead *.py set smartindent   " autoindent

The first block sets a bunch of useful vim settings. The second one enables C indenting for .c and .cpp files (you may add move file extensions here depending on languages you use), and disables C indenting for python. The third block ensures only spaces are ever added in Python programs, not tabs, as mixing the two will break Python programs.

Note that if you copy text outside vim and want to paste it in vim, enter the insert mode in vim first by pressing "i", then paste. Otherwise you'll be entering the text in normal mode, causing odd things to happen. Save your changes in .vimrc.

Windows in vim
==============

You can get a description of what each setting does using the ":help" command. For example, try running ":help hlsearch". This opens another *window* in vim with the help on hlsearch. You can browse the window as you normally would. If you want move between windows, first press "ctrl+w", then either arrow down or up to move to either the lower or upper window respectively. Move back and forth a couple of times.

To close the window you're currently in, run ":q".

Let's run the command ":help hlsearch" again. The command line mode (":") supports history. This means you can type ":", then press arrow up a couple of times, and you should see your previous ":help hlsearch" command. Press enter to run this again.

If you have multiple windows open and want to quit all of them, i.e. quit vim, you can run ":qa" (quit all). If you had changes in one of the files you hadn't yet saved, vim will warn you. You can save all unsaved changes by running ":wa". You can quit all without saving your changes by running ":qa!".

Multiple files in vim
=====================

You can open multiple files in vim. For example, if you run "vim file.txt file2.txt", you'll open two buffers, one for each file. You can list all open buffers using the ":ls" command. Try it out.

You can switch between the buffers with the ":b" command. For example, ":b1" opens the first buffer. ":b2" opens the second buffer, and so on. Make some changes in the second buffer ("file2.txt"), then change back to the first buffer. If you have "set hidden" in your .vimrc as per above, you should be able to change between buffers without having to save the file in between.

You can always change to the previous buffer, whichever it was, by running ":b#". Try this out. If you run ":ls", you can see which buffer is the previous one (it's marked with a #). Files with unsaved changes are marked with a +. You can save all changes with ":wa".

Typing code in vim
==================

Let's quit vim and open a file "hello.c". Let's go to insert mode using "i" and then type (not copy paste) the following:

.. literalinclude:: ../material/vim/hello.c
   :language: c

Exit the insert mode by pressing ESC and save your file using ":w". Vim should have taken care of indentation for you.

Typically, as part of the edit-compile-run cycle, you'd have vim running on one terminal window or tab, and have another window or tab for compiling and running the program. After you've saved the file, you could try compiling and running it e.g. by running "gcc -Wall -o hello hello.c && ./hello".

Alternatively, vim allows you to run shell commands from within vim. E.g. ":!ls" will run "ls" in the shell. You could also type e.g. ":!gcc hello.c && ./a.out" to compile and run your program. Vim will then display the output of your command.

In C, it's optional for the main function to return a value. Let's add a line in our program for this. You could enter the insert mode by pressing "i", then navigating to the end of the printf line, then pressing enter and typing the new line, but vim provides a shortcut for this. The command "o" will automatically create a new line under the current one and enter the insert mode. Try this: move the cursor to the line with the printf statement, then, while in normal mode, press "o", type "return 0;", go back to normal mode by pressing ESC and save with ":w".

Search and replace
==================

Let's say we want to replace word "hello" in our program with "hullo". The following command achieves this: ":%s/hello/hullo/g". Let's go through this one by one:

* ":": We enter the command line mode
* "%": We want the search-and-replace to run for the whole file, not only the current line
* "s": search and replace
* "/hello": search for "hello"
* "/hullo": replace with "hullo"
* "g": replace all occurrences on each line, not only the first one

You can also simply search. If you want to e.g. search for printf, type, while in normal mode, "/printf". This will move the cursor to printf and also highlight the word. To get to the next search match, use the command "n". To get to the previous match, use "N".

Another very useful command is "*". This searches for the word currently under the cursor. If you e.g. have a variable, you can use this to highlight and go to all places where the variable is used.

Code modifications
==================

Let's now duplicate our printf statement by typing "Y" and "P" in the normal mode. We should now have two statements. They both print "hullo world". Let's change one of them back to "hello" manually. Originally, I assume your cursor is at the "p" of the word "printf". To change "hullo" to "hello", we could simply go to insert mode with "i", move to the correct position with the arrow keys, use backspace to remove the "u", and add "e", but vim provides a few shortcuts here.

First, we can move forwards one word at a time by pressing "w" in normal mode. Move forwards a few times. You can move backwards one word at a time with "b". Normally, special symbols like parentheses ("(" and ")") break words, such that you'll need to press "w" a few times (or run e.g. "7w") to go through one printf statement. You can also use capital letters ("W" and "B") which only understand spaces to break words. Use "w" to get to the "h" in "hullo".

You can then move the cursor one character to the right using the arrow keys and press "s" which does both "x" and "i", i.e. removes the current character and goes to insert mode. You can then press "e" to insert "e" and ESC to go back to normal mode.

Another very useful command is ".". Pressing the dot character re-does whichever command you ran. For example, the previous command replaced the character currently under the cursor with "e". Move the cursor around, and press ".". This will replace the character under the cursor with "e".

Indentation
===========

You can shift a line to the left or right by "<<" or ">>" respectively. Try it out.

The command "==" will indent the line correctly based on the line or lines above it.

Now, for a test, let's mess up the indentation and fix it. First, using "<<" and ">>", have the braces ("{" and "}") stick out to the right. Have the printf statements be indented differently. 

Then, type "gg=G" to correctly indent the whole file. You may recall that "gg" goes to the top of the file, "=" indents correctly, and "G" denotes the end of the file.

Working with braces
===================

If you have the cursor on top of a start or end of any kind of parenthesis ("(", "[", "{", either opening or closing), pressing "%" will jump to either the closing or opening of the parenthesis. You can use e.g. the command "di(" to delete all text within the "(" parenthesis the cursor is at. (Similarly you can delete a word with "dw", or e.g. delete the next three words with "d3w".) Another useful command is "c", for changing text. Running "cc" will remove the current line and go to insert mode. "cw" will remove the current word and go to insert mode. Running "ci(" will remove the text within parenthesis and go to insert mode. Similarly, running "ci"" (that is, ci followed by quote) will remove the text within quotes (") and go to insert mode.

Combining the above, if you e.g. move the cursor to an opening brace, running "=%" will indent everything within the braces.

Visual mode
===========

Vim also has a visual mode which allows you to select parts of the file and apply operations to it. You can enter the visual mode by pressing "v". Try entering the visual mode, then moving the cursor a few lines up or down, and then pressing "=". This will indent the selected lines correctly.

This concludes our introduction to vim, and should cover a good portion of the functionality to improve productivity.

*Exercise*: Use vim in your future software development.
