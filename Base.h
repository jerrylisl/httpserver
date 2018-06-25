#ifndef BASE
#define BASE
#include<stdint.h>

//C++11没有找到noncopyable，实现了一个简单的，除了私有化，也可以delete相关的函数
class Noncopyable {
protected:
    Noncopyable(){}
    ~Noncopyable(){}
private:
    Noncopyable(const Noncopyable&);
    Noncopyable& operator=(const Noncopyable&);
    Noncopyable(const Noncopyable&&);
    Noncopyable& operator=(const Noncopyable&&);
};

#endif // BASE_H_INCLUDED
