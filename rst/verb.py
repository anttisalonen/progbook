import sys

from docutils import nodes
from docutils.parsers.rst import directives
from docutils.parsers.rst.directives.body import ParsedLiteral

from sphinx.util import logging

logger = logging.getLogger(__name__)

# This thing exists to replace sphinxVerbatim with
# Verbatim[samepage=true] for some literal blocks
# (namely those that are in the style of ASCII art).
# This is necessary because in the PDF, the ASCII art
# style diagrams would otherwise be hit by page breaks
# which isn't pretty, and Verbatim allows you to
# prevent it.

# This is a Sphinx extension and works as follows:
# We create and register a new directive called "verb".
# This directive, for literal nodes, when the builder
# is latex, replaces those literal nodes with its own
# node "verb". This node is otherwise the same as
# nodes.literal_block from docutils but has a different
# type.

# Having a different type causes Sphinx to call our
# visitor for it instead of the literal_block
# visitor. Our visitor then uses Verbatim instead of
# sphinxVerbatim. It also doesn't call the original
# visitor because that one uses pygments for code
# highlighting which doesn't work with Verbatim.

# Implementation notes:
# Sphinx caches the builder so e.g. running plain
# "make latexpdf html" would break the HTML build.
# Passing "-E" to Sphinx prevents this.
# We raise nodes.SkipNode in our visitor because
# otherwise our verbatim text would be pasted into
# TeX again, but this time with missing line breaks
# and other formatting issues.
# As a result of this, our depart function won't be
# called (I think). I've still defined it for
# completeness.
# This extension shouldn't do anything for non-tex
# builders, and simply treat verb as a parsed
# literal (:: or .. parsed-literal::).
# The hack to change the class of the node isn't
# very pretty but seems like the most reliable way
# to get this done, and semantically it should be
# correct as verb doesn't contribute anything to
# literal_block except a different type.

class verb(nodes.literal_block): pass

def visit_verb_node(self, node):
    self.body.append('\\begin{Verbatim}[samepage=true]\n')
    self.body.append(node.astext())
    self.body.append('\n')
    self.body.append('\\end{Verbatim}\n')
    raise nodes.SkipNode

def depart_verb_node(self, node):
    pass

class Verb(ParsedLiteral):
    def run(self):
        env = self.state.document.settings.env
        builder_name = env.app.builder.name

        orig_out = super(Verb, self).run()
        if builder_name != 'latex':
            return orig_out

        final_out = list()
        for o in orig_out:
            if isinstance(o, nodes.literal_block):
                o.__class__ = verb
                final_out.append(o)
            else:
                final_out.append(o)
        return final_out

def setup(app):
    app.add_directive('verb', Verb)
    app.add_node(verb,
                 latex=(visit_verb_node, depart_verb_node))
