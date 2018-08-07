# -*- coding: utf-8 -*-
#
# book build configuration file, created by
# sphinx-quickstart on Mon Feb  5 23:45:12 2018.
#
# This file is execfile()d with the current directory set to its
# containing dir.
#
# Note that not all possible configuration values are present in this
# autogenerated file.
#
# All configuration values have a default; values that are commented out
# serve to show the default.

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
# import os
# import sys
# sys.path.insert(0, os.path.abspath('.'))


# -- General configuration ------------------------------------------------

# If your documentation needs a minimal Sphinx version, state it here.
#
# needs_sphinx = '1.0'

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = ['sphinx.ext.githubpages']

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# The suffix(es) of source filenames.
# You can specify multiple suffix as a list of string:
#
# source_suffix = ['.rst', '.md']
source_suffix = '.rst'

# The master toctree document.
master_doc = 'index'

# General information about the project.
project = u'Learn Programming'
copyright = u'2018, Antti Salonen'
author = u'Antti Salonen'

# The version info for the project you're documenting, acts as replacement for
# |version| and |release|, also used in various other places throughout the
# built documents.
#
# The short X.Y version.
version = u'1.0'
# The full version, including alpha/beta/rc tags.
release = u'1.0'

# The language for content autogenerated by Sphinx. Refer to documentation
# for a list of supported languages.
#
# This is also used if you do content translation via gettext catalogs.
# Usually you set "language" from the command line for these cases.
language = None

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This patterns also effect to html_static_path and html_extra_path
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']

# The name of the Pygments (syntax highlighting) style to use.
pygments_style = 'sphinx'

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False


# -- Options for HTML output ----------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'bizstyle'

# Theme options are theme-specific and customize the look and feel of a theme
# further.  For a list of options available for each theme, see the
# documentation.
#

# sphinx 1.7.5 defines body_min_width as 450 by default.
# This seems to break render for iPhone, at least with bizstyle.
html_theme_options = {
    'body_min_width': 0,
    'body_max_width': None
}

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']

# remove "documentation" from title
html_title = ('%s %s') % (project, release)


# -- Options for HTMLHelp output ------------------------------------------

# Output file base name for HTML help builder.
htmlhelp_basename = 'guidedoc'


# -- Options for LaTeX output ---------------------------------------------

latex_elements = {
    # The paper size ('letterpaper' or 'a4paper').
    #
    # 'papersize': 'letterpaper',

    # The font size ('10pt', '11pt' or '12pt').
    #
    # 'pointsize': '10pt',

    # Additional stuff for the LaTeX preamble.
    #
    # 'preamble': '',

    # Latex figure (float) alignment
    #
    # 'figure_align': 'htbp',
}

# Grouping the document tree into LaTeX files. List of tuples
# (source start file, target name, title,
#  author, documentclass [howto, manual, or own class]).
latex_documents = [
    (master_doc, 'book.tex', project,
     u'Antti Salonen', 'scrbook'),
]

latex_engine = 'xelatex'


latex_elements = {
    'fontpkg': r'''
\setmainfont{Alegreya}
\setsansfont{Linux Biolinum O}
\setmonofont{Inconsolata}
''',
    'preamble': r'''
\let\oldsection\section
\renewcommand\section{\clearpage\oldsection}
\renewcommand\sphinxurl{\sphinxnolinkurl}
\pagestyle{headings} % include page numbers
\usepackage{xltxtra} % font support
\usepackage[all]{nowidow}
\let\oldquote\quote
\renewcommand\quote{\mbox{}\oldquote} % keep quote and quote name together

% replace sphinxshadowbox with mdframed due to issues with page breaks
\usepackage[framemethod=TikZ]{mdframed}
\newenvironment{aside}
  {\begin{mdframed}[leftline=true,%
      rightline=true,%
      leftmargin=0em,%
      rightmargin=0em,%
      innerleftmargin=1em,%
      innerrightmargin=1em,%
      linewidth=1pt,%
      needspace=6em,%
      roundcorner=8pt,%
      skipabove=7pt,%
      skipbelow=7pt]}
  {\end{mdframed}}
\let\sphinxShadowBox\aside
\let\endsphinxShadowBox\endaside

% no page break between section and first subsection
% https://tex.stackexchange.com/questions/153444/insert-newpage-before-section-but-prevent-pagebreak-after-part
\usepackage{etoolbox}
\newtoggle{aftersection}
\pretocmd{\subsection}
  {\iftoggle{aftersection}
    {\global\togglefalse{aftersection}}% we're after a section
    {\clearpage}% we're not immediately after a section
  }{}{}
\pretocmd{\section}
  {\clearpage % do a page break
   \global\toggletrue{aftersection}% tell \subsection we're just after a section
  }
  {}{}

''',
    'fncychap': r'\usepackage[Bjarne]{fncychap}',
    'printindex': r'\footnotesize\raggedright\printindex',
    'papersize': r'paper=7in:10in,pagesize=pdftex,headinclude=on,footinclude=on',
    'pointsize': r'12pt',
    'tableofcontents': '''\\begin{center}Learn Programming, first edition
    
    Antti Salonen
    
    This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License. To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to Creative Commons, PO Box 1866, Mountain View, CA 94042, USA.

    The code within this book is licensed under the MIT license.
    
    Copyright 2018, Antti Salonen.\\end{center}\\clearpage\\tableofcontents'''
}

latex_show_urls = 'footnote'
latex_toplevel_sectioning = 'chapter'

# -- Options for manual page output ---------------------------------------

# One entry per manual page. List of tuples
# (source start file, name, description, authors, manual section).
man_pages = [
    (master_doc, 'book', project,
     [author], 1)
]


# -- Options for Texinfo output -------------------------------------------

# Grouping the document tree into Texinfo files. List of tuples
# (source start file, target name, title, author,
#  dir menu entry, description, category)
texinfo_documents = [
    (master_doc, 'book', project,
     author, 'book', project,
     'Miscellaneous'),
]



