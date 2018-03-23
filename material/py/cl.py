class A(object):
    def __init__(self, foo):
        self.foo = foo

    def add_one(self):
        self.foo += 1

    def call_me(self):
        print 'A called - my foo is: ' + str(self.foo)

class B(object):
    def call_me(self):
        print 'B called'

my_objects = [A(42), B()]

for o in my_objects:
    o.call_me()

