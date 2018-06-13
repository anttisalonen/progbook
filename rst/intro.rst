Why this book?
--------------

My wife was asking if she could become a software engineer like I did.

I ended up checking the offerings around Computer Science at the local university. They basically have two kinds of degrees: the "traditional" Computer Science degree which is pretty good. It has a lot of useful things but it's also heavy on maths and a bunch of other stuff that I now think isn't very relevant for most software engineering jobs. As my wife has some (to some degree healthy) aversion to maths she wasn't too eager about it.

The other kind of degree is more engineering and job market focused. Having interviewed some candidates with this degree I wasn't sure I could really recommend it; it seemed to be very limited in technical scope, and emphasise superficial learning of memorising facts instead of skills needed in software development. Plus, for whatever reason they have even more advanced maths than the traditional Computer Science degree.

As I have some years of experience in software engineering, and also am now hiring and hence interviewing software engineers, it seemed to me like there's a bit of a gap between what industry needs and what the schools are producing, at least in my local area. To some extent this is expected: universities live in the academic world, often without an explicit goal of ensuring the graduates are employed by the industry. To mitigate, universities may offer degrees directed more towards real world, but, like the degree referenced above, these run into the risk of not being relevant or effective enough for the industry.

So I ended up teaching my wife what I think is the right stuff. Now, since we also have a little daughter, finding time for both of us at the same time was sometimes difficult, so I'd write down some notes or exercises for her that she could dive into when she had time. I ended up writing more and more and structuring things better and all of it ended up being this book.

So, I'm writing this book to teach my wife all the necessary basics around software development so that she'll be able to land a job. However, the book could be useful to others as well.

No, really, why are you writing this book?
==========================================

If I'm being completely selfish, it's because I'm hiring software engineers and am finding it difficult to find any. Without going into my specific requirements, I want to include enough in the book that if the reader were to do all of the following:

* Read and understand everything in the book
* Finish all the exercises
* Write at least some personal mini project in some language (shouldn't be a problem after the above two points)
* Interact with the rest of the software development community, may it be through Reddit or other social media, chat, Stack Overflow or Github

...then I think that that person should have the *technical* skills to at least seriously be considered for a software engineering position.

(Apart from technical skills, you'll also need the interpersonal skills and common sense, or analytical and critical thinking skills, among others, before I'd consider you for a position. I'm expecting the reader to obtain these some other way than reading this book.)

Do you mean, if I do the points above, I too can become a professional software engineer?
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

I obviously can't guarantee anything. Actually getting a job depends on the economy and the job market, where you live, what you're willing to do and work for etc. But I do think that after doing all the above points, the reader should have enough or almost enough *technical* skills for many software engineering jobs.

Apart from raising false hopes about getting a job as a software engineer, another reason for this book is that I'm not happy with how the gap between "techies" and "non-techies" is growing. I think more people should be familiar with software. I want to write a book that has very little prerequisites and teaches enough about software and technology so that a "non-techie" is able to get a better understanding about how software affects the world we live in. So even if you don't get a job, at least feel proud about having a better understanding of software and its role in the world.

What's in this book?
====================

The book covers a lot of topics but is very shallow.

The book doesn't include *everything* I know, but it includes enough that the reader should be able to follow a software development discussion, and more importantly, learn more about a specific software development subject without a hugely steep learning curve.

Different programming languages such as C, Python and Javascript are touched upon. Some traditional Computer Science topics such as algorithms and data structures are touched upon. Some topics that are typical in modern software development work environments such as databases, version control and web development are touched upon. Almost nothing is treated in depth.

Although many subjects are treated almost comically lightly, overall the material should be enough for the reader to get a general understanding around software development.

This book is not a replacement for a Computer Science degree, though it does (shallowly) include some parts of one, and complements one in other cases.

You say "engineering", "development" and "computer science" - what are the differences?
=======================================================================================

This book teaches software development. This book doesn't teach engineering, but the term "software engineer" is used to describe a person who develops software because that's what we seem to have arrived at as an industry.

More specifically, the terminology is defined as follows:

* Engineering is something that's very structured, and something you want to learn in a very structured way. In other words, this book definitely doesn't teach engineering.
* Software development is about solving a problem using software: it's not very clean, it's usually not really engineering nor very scientific, but it's useful in practice and there's a demand for it.

Programming, or coding, is one part of software development, among design, testing, writing specifications etc. This book is not only about programming but about software development.

There's also Computer Science which arguably is separate from both software development and engineering. (It is a science after all. "Computer Science is no more about computers than astronomy is about telescopes.")

You possibly can't have all the technical stuff that one needs in one book!
===========================================================================

No, but I can try.

More seriously, whatever code you'll be writing after reading this book, you'll need to study the technology for that particular topic in more detail. The goal of this book is not to include everything; the goal is to put together a foundation so that the reader will be able to learn to write almost any software without signifant hurdles, as necessary.

What are the prerequisites for this book?
=========================================

The book is aimed at people with no experience or skills within software development.

Some parts of the book use high school level maths so you should be somewhat familiar with that. You need to have a computer and be able to use it (install software etc.).

The book requires your computer to be able to run a Unix shell and standard Unix tools. Mac, Linux or Windows 10 should all suffice.

You should have some skills around analytical thinking and problem solving. I don't know how to teach those. I believe this book does teach them to some extent though, as a side effect.

I do believe that almost anyone who is able to finish high school without significant struggles can learn how to develop software. It's not magic. It does require persistence though; you need to be able to put effort to the book, and it'll take time. I believe someone with the capability to work on the book full time, enough perseverance and help where needed, should be able to finish the book including all the exercises in a matter of months.

It's probably very helpful to have a tutor or someone who's familiar with software development to answer your questions. Use the Internet to your advantage; there are literally thousands of techies online waiting to share their knowledge and answer newbies' questions. Try Stack Overflow, the learnprogramming reddit, Github, or various IRC channels, e.g. those dedicated to specific programming languages.

You must be able to find information online. Notice how I just dropped "IRC channels" into a sentence? If you ever do feel the need to ask a question and decide to investigate the IRC bit further, you need to use the Internet to a) find out what IRC is, b) find out what IRC channels there are e.g. for a programming language you're having trouble with, c) how to connect to such a channel and ask your question. This book won't have that information.

The book is free and licensed under the Creative Commons Attribution-ShareAlike 4.0 International License. This means that you're free to share the book in any medium and modify the book for any purpose, even commercially, as long as you give appropriate credit and your distribution uses the same license. See more details at http://creativecommons.org/licenses/by-sa/4.0/.

How should I work with this book?
=================================

The information in this book is very dense. Most sentences are important for the topic at hand, and information is rarely duplicated. I expect the reader to go through the book several times. You might miss a lot of details the first time, or two times, but should eventually be able to understand all of it.

The book uses the pedagogical approach of assimilation, or constructionism; information is provided to the reader in bulk, and learning is facilitated by exercises which ask the student to think for themselves in an effort to learn the material. If you don't understand something, that may be fine. Let it be and come back to it later. There's a chapter dependency diagram at the end of the book which can give indication as to which chapters you need to understand before proceeding. Different topics are interleaved to some degree, allowing the reader to digest some areas while working on others and to better understand the relationships and connections between topics.

I should mention at this point, if it wasn't already clear, that the author has no real pedagogical experience.

If you think there's something very unclear about some part of the book, let me know. My email is ajsalonen@gmail.com. You may also create an issue or a pull request in Github. I'm also not a native English speaker so any corrections on that front are welcome as well.

Who are you?
============

My name is Antti. I've worked as a software engineer for a few years and am currently an engineering manager at a tech company. I started writing code when I was about six years old, or 28 years ago. I've written non-trivial code in about 14 different programming languages, or about one new language every two years. I've professionally written medical device software, company internal tools, software for controlling a telescope, and a few other things. I've been a tech lead for a web app, a network admin, an engineering team lead, and a small scale software architect. I've written some humble, ugly, open source games as a hobby, and made some minor contributions to some open source projects.

Even though I started programming at a young age, it doesn't mean you're already a lost cause if you didn't. In fact, I know several great software engineers who didn't start programming until much later.