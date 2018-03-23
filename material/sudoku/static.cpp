#include <iostream>

class Foo {
    public:
        static void foo();
};

void Foo::foo()
{
    std::cout << "in foo\n";
}

int main(void)
{
    Foo::foo();
}
