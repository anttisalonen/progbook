:orphan:

How would you teach programming to a beginner?
----------------------------------------------

September 3, 2018

While I've never taught programming professionally, I've had various discussions with beginning and experienced programmers about the subject, and mentored some software engineers. This post contains some of my reflections on this, and especially on what is required to teach an adult with none or very little technology background enough skills so they can perform well as a professional software engineer.

Software development is based on abstractions. It seems that the mainstream method nowadays, when teaching programming to beginners, is to begin with a high level of abstraction, i.e. the most accessible concepts:

.. image:: abs.png

Here, the first box refers to programming languages and environments explicitly made accessible, especially to beginners, and includes programming languages such as Scratch but also environments such as programming applications using Windows Forms Designer and Visual Basic .NET (even if the latter typically isn't considered visual programming as such).

The main upside of this strategy seems to be that on the face of it, the general programming concepts are easy to explain. One can use several metaphors for explaining basic concepts such as branches or loops, and skip long-winded explanations of the inner workings of a computer, a software library or a framework.

While I think this approach is a good choice for teaching programming basics to children, I think it can cause confusion for adults or high school students beginning to learn programming. I've seen different kinds of confusion arise: "Yes, I understand how I can use the framework to do things for me, but..."

* "what does the framework *do*? How does it all hang together?"
* "this isn't *real programming*, is it? What's the difference?"

Some courses or educators begin with a bit lower level than what I call "visual" programming, e.g. by focussing on teaching Python, or JavaScript. This is great for focus but I've seen somewhat similar confusion arise: "I now know this language, but..."

* "how does *the language* work?"
* "what are the relationships between programming languages, and how does it all hang together?"
* "what language should I learn next? When have I learned enough to understand *the big picture*?"

Apart from these issues, a symptom of using a specific environment or a framework has been that it's not clear to the student what is environment or framework specific and what is generic. Furthermore, generic concepts might not be clear; for example, the student might not understand the relationships between variables, types, and functions, especially if object-orientation is brought into the mix early.

While e.g. teaching JavaScript may be enough for a student wanting to fully focus on front-end development, if the student wishes to gain a deeper understanding of programming then I advocate teaching the concepts in *reverse* order instead:

* First, teach how digital logic works - this will not only help understand the functionality and limitations of computer hardware but also give an introduction to logic in general
* Then, at a high level, explain how computers work - including the CPU and I/O
* Introduce basic programming concepts such as variables, branches, loops and functions. I've opted for teaching these first in Python, and immediately after in C. This has the downside of introducing two different programming languages and hence causing extra mental burden, but on the other hand the main concepts are transferable across programming languages. Furthermore, it becomes clearer how relevant, or irrelevant, the syntax of a programming language is. While I generally don't think introducing a high level language like Python at an early stage is a good idea, sticking to the very basics of the language maintains a relatively low level of abstraction.
* Introduce more difficult programming exercises utilising functions, with the focus on learning how to solve problems. Again, I've opted for teaching these both in Python and C.
* After these foundations have been laid, the concept of libraries, and re-use of software by others can be introduced. I think an important concept at this stage is reading and understanding the documentation of APIs and libraries, including the standard libraries. This step is necessary for understanding how a high level framework or a programming environment could be designed and implemented. As the number of APIs and libraries as of today is very high, I think it's good to have lots of examples of different libraries in different languages. This is also an opportunity to teach some other concepts such as databases.
* As part of going through various libraries, I think, it's also a good idea to introduce the basics of computer science. This includes the default data structures such as array, stack and queue as well as binary search trees and hash tables. I think it's important to teach binary search trees and hash tables before actually using a set or a dictionary in e.g. Python or other high level programming language, so that the student can understand the concepts behind the implementation; but I also think it's not necessary, for typical software engineering work, for the student to implement binary search trees or hash tables themselves. If necessary then the student should be able to read up on how to do this themselves at a later stage.
* Apart from basics of computer science, I think there are lots of various tools that a software engineer must at least be aware of; these include version control, the concepts of the call stack and memory allocation in C, basics of network programming, parsing, virtual machines, software design, shell scripting, various type systems etc. I think it's also a good idea to introduce concepts such as multithreading and NP-hard problems, though I'm not sure if there are many entry level software engineering positions that require an understanding of these, and the student may at this point be able to read up about them on their own if necessary.

After these steps, the student should in theory be able to write "any" run-of-the-mill software: when faced with a problem, possibly constrainted on a programming language or framework previously unknown to the student, the student should be able to both find out how to use the given language or framework and write the necessary code to solve the problem, possibly with the use of external libraries. I think that if the student has built up the understanding of the concepts bottom-up, then it is easy to move up and down on the abstraction hierarchy as needed, and also be better prepared for leaky abstractions.

This doesn't replace a full, proper Computer Science program, but on the other hand, it is a lot more material than some programming courses I've seen, and in my view it's enough knowledge for an entry level software engineering position. I followed this philosophy as I wrote my book, and am curious to learn from my readers how their experience has been.

`Front page <https://progbook.org/>`_
