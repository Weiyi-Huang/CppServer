#include <iostream>
#include <string>
#include "source/ThreadPool.h"

void Print(int a, double b, const char *c, std::string d)
{
    std::cout << a << b << c << d << std::endl;
}

void Test()
{
    std::cout << "help" << std::endl;
}

int main()
{
    ThreadPool *poll = new ThreadPool();
    std::function<void()> func = std::bind(Print, 1, 3.14, "hello", std::string("world"));
    poll->Add(func);
    func = Test;
    poll->Add(func);
    delete poll;
    return 0;
}