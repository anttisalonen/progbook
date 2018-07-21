#include <iostream>

class Foo {
    public:
        static void foo();
};

void Foo::foo()
{
    std::cout << "static member function Foo::foo called\n";
}

int main(void)
{
    Foo::foo();
}
