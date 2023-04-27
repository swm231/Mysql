#include "foo.h"
extern Foo& globalFoo();
class Bar{
public:
    explicit Bar(const Foo& foo){
        printf("Bar(const Foo&) foo.a = %d\n", foo.a_);
    }
};
Bar bar(globalFoo());