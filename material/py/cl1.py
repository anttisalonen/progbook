class A(object):
    def __init__(self, foo):
        self.foo = foo

    def add_one(self):
        self.foo += 1

    def call_me(self):
        print 'A called - my foo is: ' + str(self.foo)

obj = A(42)

obj.call_me()
obj.add_one()
obj.call_me()

