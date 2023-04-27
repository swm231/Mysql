#include <iostream>

class Foo{
public:
    Foo(int a = 0, std::string name = "unkown") :
        a_(a), name_(name) {
            printf("Foo(int, string) %p a:%d name:%s\n",
            this, a_, name_.c_str());
        }
    Foo(const Foo& other){
        a_ = other.a_;
        name_ = other.name_;
        printf("Foo(const Foo&) SCR:%p DST:%p a:%d name:%s\n",
            &other, this, a_, name_.c_str());
    }
    Foo (Foo&& other){
        a_ = other.a_;
        name_ = std::move(other.name_);
        printf("Foo(Foo&&) SCR:%p DST:%p a:%d name:%s\n",
            &other, this, a_, name_.c_str());
    }
    Foo& operator= (const Foo& other){
        a_ = other.a_;
        name_ = other.name_;
        printf("operator= Foo(const Foo&) SCR:%p DST:%p a:%d name:%s\n",
            &other, this, a_, name_.c_str());
        return *this;
    }
    Foo& operator= (Foo&& other){
        a_ = other.a_;
        name_ = std::move(other.name_);
        printf("operator= Foo(Foo&&) SCR:%p DST:%p a:%d name:%s\n",
            &other, this, a_, name_.c_str());
        return *this;
    }
    ~Foo(){
        printf("~Foo() %p a:%d name:%s\n", this, a_, name_.c_str());
    }
public:
    int a_;
    std::string name_;
};
