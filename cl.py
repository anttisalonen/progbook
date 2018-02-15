class A:
    def call_me(self):
        print 'A called'

class B:
    def __init__(self, foo):
        self.foo = foo

    def call_me(self):
        print 'B called - my foo is: ' + str(self.foo)

my_objects = [A(), B(42)]

for o in my_objects:
    o.call_me()

