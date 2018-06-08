#!/usr/bin/env python2

import re
import collections
import copy

# graph        ::= "digraph" characters '{' statements '}'
# statements   ::= '' | statement statements
# statement    ::= label | edge
# label        ::= characters labelname
# labelname    ::= '[' "label" '=' quotedstring  ']' ";"
# quotedstring ::= '"' labelstring '"'
# labelstring  ::= characters | ' ' labelstring | characters labelstring
# edge         ::= characters "->" characters ";"
# characters   ::= '' | 'a-zA-Z0-9_-' characters

class Token(object):
    EQUALS = 0
    DIGRAPH = 1
    IDENTIFIER = 2
    QUOTE = 3
    OPEN_CURLY = 4
    CLOSE_CURLY = 5
    OPEN_SQUARE = 6
    CLOSE_SQUARE = 7
    LABEL = 8
    SEMICOLON = 9
    ARROW = 10

tokens = [(re.compile('='), Token.EQUALS),
          (re.compile('[ \n\t]+'), None),
          (re.compile('digraph'), Token.DIGRAPH),
          (re.compile('\".*\"'), Token.QUOTE),
          (re.compile('{'), Token.OPEN_CURLY),
          (re.compile('}'), Token.CLOSE_CURLY),
          (re.compile('\['), Token.OPEN_SQUARE),
          (re.compile(']'), Token.CLOSE_SQUARE),
          (re.compile('label'), Token.LABEL),
          (re.compile(';'), Token.SEMICOLON),
          (re.compile('[a-zA-Z0-9_]+'), Token.IDENTIFIER),
          (re.compile('->'), Token.ARROW)]

lexer_output = list()

characters = open('/dev/stdin', 'r').read()

pos = 0
while pos < len(characters):
    match = None
    for reg, token in tokens:
        match = reg.match(characters[pos:])
        if match:
            if token is not None:
                lexer_output.append((token, match.group(0)))
            length = len(match.group(0))
            assert length > 0, characters[pos:(pos + 20)]
            pos += length
            break
    else:
        raise RuntimeError('Unable to lex: "%s"...' % (characters[pos:(pos + 20)]))

class Graph(object):
    def __init__(self, name):
        self.name = name
        self.statements = list()

    def add_statement(self, s):
        self.statements.append(s)

    def __str__(self):
        ret = 'digraph %s {\n' % self.name
        for s in self.statements:
            ret += str(s) + '\n'
        ret += '}\n'
        return ret

    def simplify(self):
        # return true if to_node is a child of from_node
        def is_reachable(from_node, to_node):
            for child in dep[from_node]:
                if child == to_node:
                    return True
                if is_reachable(child, to_node):
                    return True
            return False

        dep = collections.defaultdict(list)
        for s in self.statements:
            if isinstance(s, Edge):
                dep[s.n1].append(s.n2)

        # remove redundant edges
        # an edge from a to b is redundant if a connection from a child of a to b is found
        to_remove = list()
        for parent_node, my_edges in dep.items():
            for direct_child in copy.copy(my_edges):
                to_visit = copy.copy(dep[direct_child])
                while to_visit:
                    indirect_child = to_visit.pop()
                    to_visit.extend(dep[indirect_child])
                    if is_reachable(direct_child, indirect_child) and indirect_child in dep[parent_node]:
                        to_remove.append((parent_node, indirect_child))

        for dependency, this_node in to_remove:
            self.statements = [s for s in self.statements if not (isinstance(s, Edge) and s.n1 == dependency and s.n2 == this_node)]

class Label(object):
    def __init__(self, name, label):
        self.name = name
        self.label = label

    def __str__(self):
        return '    %s [label=%s];' % (self.name, self.label)

class Edge(object):
    def __init__(self, n1, n2):
        self.n1 = n1
        self.n2 = n2

    def __str__(self):
        return '    %s -> %s;' % (self.n1, self.n2)

class Parser(object):
    def __init__(self, lex):
        self.lex = lex
        self.pos = 0
        self.token = self.lex[self.pos]

    def next(self):
        self.pos += 1
        if self.pos < len(self.lex):
            self.token = self.lex[self.pos]

    def parse(self):
        self.match(Token.DIGRAPH)
        name = self.identifier()
        self.graph = Graph(name)
        self.match(Token.OPEN_CURLY)
        self.statements()
        self.match(Token.CLOSE_CURLY)

    def identifier(self):
        if self.token[0] != Token.IDENTIFIER:
            raise RuntimeError('Expected identifier, received "%s"' % self.token[0])
        ret = self.token[1]
        self.next()
        return ret

    def quote(self):
        if self.token[0] != Token.QUOTE:
            raise RuntimeError('Expected quote, received "%s"' % self.token[0])
        ret = self.token[1]
        self.next()
        return ret

    def match(self, char):
        if self.token[0] != char:
            raise RuntimeError('Expected "%s", received "%s"' % (char, self.token))
        self.next()

    def statements(self):
        while self.token[0] != Token.CLOSE_CURLY:
            name = self.identifier()
            if self.token[0] == Token.OPEN_SQUARE:
                self.match(Token.OPEN_SQUARE)
                self.match(Token.LABEL)
                self.match(Token.EQUALS)
                label = self.quote()
                self.match(Token.CLOSE_SQUARE)
                self.match(Token.SEMICOLON)
                self.graph.add_statement(Label(name, label))
            elif self.token[0] == Token.ARROW:
                self.match(Token.ARROW)
                name2 = self.identifier()
                self.match(Token.SEMICOLON)
                self.graph.add_statement(Edge(name, name2))
            else:
                raise RuntimeError('Expected statement, received "%s"' % self.token)

p = Parser(lexer_output)
p.parse()

p.graph.simplify()

print p.graph


