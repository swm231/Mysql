# Effective C++

## 视C++为一个语言联邦

- C
- Object-Oriented C++
- Template C++
- The STL

## 尽量不使用#define

### Simple constants, prefer const objects or enums to #defines

```cpp
#define constant 1.653
```

使用 *define*  定义一个常量 *constant* ，编译时进行预处理，凡是出现 *constant* 的地方都会被替换为*1.653*， *constant* 并不会加入到符号表中，因此报错的时候就不会报 *constant* 相关的提示，而是 *1.653* 相关的错误。

使用下面的定义方式可以解决这个问题：

```cpp
const double constant = 1.653;
```

对于*char* *可以用下面的定义方式：

  ```cpp
const char* const const_name = "swm_231";
  ```

对于string使用下面的定义方式：

```cpp
const std::string const_name = "swm_231";
```



```cpp
class GamePlayer{
private:
	static const int NumTurns = 5;   // constant declaration
	int scores[NumTurns];			 // use of constant
    // ...
};
```

*Numturns* 是一个常量的声明，第二行是对其的使用。但是在下面的代码中使用 *NumTurns* 时（如取地址）编译器会报错，因为这是声明，并没有为其分配地址空间。需要在另一个cpp代码中写上一下语句：

```cpp
const int GamePlayer::NumTurns; // definition of NumTurns
							    // see below why no value is given
```

除了使用上面的方法，还可以使用 *enum hack* 技术：

``` cpp
class GamePlayer{
private:
	enum { NumTurns = 5; }   		// "the enum hack" --makes
									// NumTurns a symbolic name for 5 fine
	int scores[NumTurns];
    // ...
}; 
```

其中 *NumTurns* 就是一个常量。

### Function-like macros, prefer inline functions to #defines

```cpp
// call f with the maximum of a and b
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))
```

在上面的宏定义中，把 *a* 和 *b* 用括号括起来，防止其不必要的展开。

```cpp
int a = 5, b = 0;
CALL_WITH_MAX(++a, b);			// a is incremented twice
CALL_WITH_MAX(++a, b + 10); 	// a is incremented once
```

当 *a* 大于 *b* 的时候会调用函数：

```cpp
f((++a) > (b) ? (++a) : b);    	// a is incremented twice
```

最好使用模板函数来替代：

```cpp
template<typename T>
inline void callWithMax(const T& a, const T& b)
{
    f(a > b ? a : b);
}
```

## 尽可能使用const

### Declaring something const helps compilers detect usage erroes

```cpp
char greeting[] = "Hello";
char* p = greeting; 			// non-const pointer, non-const data
const char* p = greeting; 		// non-const pointer, const data
char* const p = greeting; 		// const pointer, non-const data
const char* const p = greeting; // const pointer, const data
```

```cpp
void f1(const Widget* pw); 		// f1 takes a poniter to a constant Widget object
void f2(Widget const* pw); 		// so does f2
```

对于迭代器，它的使用和指针类似：

```cpp
std::vector<int> vec;
// ...
const auto iter = vec.begin(); 	// iter acts like a T* const
*iter = 10; 					// OK, changes what iter points to
++iter;							// error! iter is const

auto clter = vec.cbegin();		// clter acts like a const T*, std::vecotr<int>::const_iterator
*clter = 10;					// error! *clter is const
++clter;						// fine, chanegs clter
```



定义一个有理数的类，并重定义*：

```cpp
class Rational{ ... };
const Rational operator* (const Rational& lhs, const Rational& rhs);
```

如果重定义的返回值不用 *const* 修饰，那么下面的代码将不会报错：

```cpp
Rational a, b, c;
// ...
(a * b) = c; 			// invoke operator = on the result of a * b!
if (a * b = c) ...  	// meant do to a comparison!
```

### const Member Functons

定义一个文本类：

```cpp
class TextBlock{
public:
   	const char& operator[] (std::size_t position) const // operator[] for const objects
    {
        return text[position];
    }
    char& operator[] (std::size_t position)				// operator[] for non-const objects
    {
        return text[position];
    }
private:
    std::string text;
};
```

我们在定义这个类的时候，选择是否使用 *const* 修饰来决定其调用的函数：

```cpp
TextBlock tb("Hello");
std::cout << tb[0]; 		// calls non-const TextBlock::operator[]
const TextBlock ctb("Hello");
std::cout << ctb[0];		// calls const TextBlock::operator[]
```

```cpp
void print(const TextBlock& ctb)	// in this function, ctb is const
{
    std::cout << ctb[0]; 			// calls const TextBlock::operator[]
    // ...
}
```



### bitwise constness and logical const ness

```cpp
classs CTextBlock{
public:
    //inappropriate (but bitwise const) declaration of operator[]
    char& operator[] (std::size_t position) const{
        return pText[position];
    }
private:
    char* pText;
};

const CTextBlock cctb("Hello");		// declare constant object
char* pc = &cctb[0]; 				// call the const operator[] to get a pointer to cctb's data
*pc = 'J';							// cctb now has the value 'Jello'
```



对于 *const* 修饰的成员函数，我们无法在其中改变变量的值：

```cpp
class CTextBlock{
public:
    std::size_t length() const;
private:
    char* pText;
    std::size_t textLength; 		// last calculated length of textblock
    bool LengthIsVaild;				// whether length is currently valid
};
std::size_t CTextBlock::length() const
{
    if(!LengthIsVaild){
        // error! can't assign to textLength and LengthIsVaild in a const member function
        textLength = std::strlen(pText);
        LengthIsVaild = true;
    }
    return textLength;
}
```

但我们对需要修改的值使用 *mutable* 修饰就可以在 *const* 修饰的成员函数中改变值：

```cpp
class CTextBlock{
public:
    std::size_t length() const;
private:
    char* pText;
    // these data members may always be modified, even in const member functions
    mutable std::size_t textLength; 
    mutable bool LengthIsVaild;
};
std::size_t CTextBlock::length() const
{
    if(!LengthIsVaild){
        textLength = std::strlen(pText);	// now fine
        LengthIsVaild = true;				// also fine
    }
    return textLength;
}
```

### Avoiding Duplication in const and Non-const Merber Functions

在下面代码中两个函数返回前，可能需要做一些准备工作，比如边界检查、记录日志、检验数据完整性，这就造成了代码重复，不利于后期维护：

```cpp
class TextBlock{
public:
   	const char& operator[] (std::size_t position) const
    {
        // ...      do bounds checking
        // ... 		log access data
        // ... 		verify data integrity
        return text[position];
    }
    char& operator[] (std::size_t position)
    {
        // ...      do bounds checking
        // ... 		log access data
        // ... 		verify data integrity
        return text[position];
    }
private:
    std::string text;
};
```

我们可以在非 *const* 修饰的成员函数中调用 *const* 修饰的代码来减少代码重复：

```cpp
class TextBlock{
public:
    // ...
    const char& operator[] (std::size_t position) const} { // same as before
    	// ...
    	return text[position];
	}
	char& operator[] (std::size_t position){
        return const_cast<char&>(		        		// cast away const on op[]'s return type
        		static_cast<const TextBlock&>(*this) 	// add const to *this's type
            	[position]);							// call const version of op[]
    }
};
```

## 对象使用前应该被初始化

定义三个类A、B、C，C中包含类A和B，然后初始化C，查看输出结果：

```cpp
class A{
public:
    explicit A(std::string name, int x = 0) :
   		name_(name), x_(x) {
            printf("A(string):%s x:%d\n", name_.c_str(), x_);
        }
    ~A(){
        printf("~A():%s x:%d\n", name_.c_str(), x_);
    }
private:
    std::string name_;
    int x_;
};
class B{
public:
    explicit B(int v) : v_(v){
        printf("B(int):%d\n", v);
    }
    ~B(){
        printf("~B():%d\b\n", v_);
    }
public:
    int v_;
};
class C{
public:
    C(std::string aName, int bValue) : b_(bValue), a_(aName, b_.v_){}
    
private:
    A a_;
    B b_;
};

int main(){
    C c("member", 4);
    return 0;
}
```

输出结果：

```cpp
A(string):member x:1574327808
B(int):4
~B():4
~A():member x:1574327808
```

发现类A在初始化时是一个不正确的值，但是在C的初始化列表中似乎是一个正确的代码...

这里涉及到初始化列表初始化顺序的问题，无论初始化列表是怎么样书写的，它都是按照变量定义的顺序（类C私有成员定义的顺序）来初始化的。

### 构造函数成员初始化列表优于函数体内赋值

先定义一个类，将其默认构造、拷贝构造、移动构造、拷贝赋值、移动赋值函数：

```cpp
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
```

然后定义一个测试类，不使用初始化列表初始化它：

```cpp
class Test{
public:
    explicit Test(const Foo& foo){
        foo_ = foo;					//don't use initializer list
    }
private:
    Foo foo_;
};
int main(){
    Foo foo;
    Test test(foo);
    return 0;
}
```

输出：

```cpp
Foo(int, string) 0x7fff30ae2e60 a:1 name:foo1
Foo(int, string) 0x7fff30ae2e90 a:0 name:unkown
operator= Foo(const Foo&) SCR:0x7fff30ae2e60 DST:0x7fff30ae2e90 a:1 name:foo1
~Foo() 0x7fff30ae2e90 a:1 name:foo1
~Foo() 0x7fff30ae2e60 a:1 name:foo1
```

发现在初始化的过程中其构造了一个匿名类并调用了了拷贝赋值函数。

但是我们如果使用初始化列表：

```cpp
Foo(int, string) 0x7ffd6b66f440 a:1 name:foo1
Foo(const Foo&) SCR:0x7ffd6b66f440 DST:0x7ffd6b66f470 a:1 name:foo1
~Foo() 0x7ffd6b66f470 a:1 name:foo1
~Foo() 0x7ffd6b66f440 a:1 name:foo1
```

发现只调用一次拷贝构造，从而节省资源。

### 非局部变量的初始化顺序问题

在一个文件中调用另一个文件的全部变量时，其有可能还没被初始化：

```cpp
// init_before_use.cpp
#include "foo.h"
Foo globalFoo(123, "globalFoo");

int main(){
    printf("======main======\n");
}
```

```cpp
// init_before_use1.cpp
#include "foo.h"
extern Foo globalFoo;
class Bar{
public:
    explicit Bar(const Foo& foo){
        printf("Bar(const Foo&) foo.a = %d\n", foo.a_);
    }
};
Bar bar(globalFoo);
```

上面代码的输出有可能是一下形式：

```cpp
Bar(const Foo&) foo.a = 0
Foo(int, string) 0x5601dd6ae040 a:123 name:globalFoo
======main======
~Foo() 0x5601dd6ae040 a:123 name:globalFoo
```

即在 *Bar* 调用 *globalFoo* 初始化自己时， *globalFoo* 还未被初始化。

可以更改上面代码的书写：

```cpp
// init_before_use.cpp
#include "foo.h"

Foo& globalFoo(){
    static Foo globalFoo(123, "globalFoo");
    return globalFoo;
}
int main(){
    printf("======main======\n");
}
```

```cpp
// init_before_use1.cpp
#include "foo.h"

extern Foo& globalFoo();
class Bar{
public:
    explicit Bar(const Foo& foo){
        printf("Bar(const Foo&) foo.a = %d\n", foo.a_);
    }
};
Bar bar(globalFoo());
```

把全局变量更改成一个函数，函数里用 *static* 定义一个静态局部变量，返回其引用。在使用全局变量的时候，使用这个函数即可。

## 了解C++默认生成的函数

### 成员对象对C++默认生成函数的影响

```cpp
class Bar{
public:
    explicit Bar(std::string name, int intValue = 10) :
        _name(name), _intValue(intValue){}
public:
    std::string & _name;
    const int _intValue;
    std::mutex _mtx; 			// don't copy/move
};

int main()
{
    std::string name1 = "bar1";
    std::string name2 = "bar1";
    Bar bar1(name1), bar2(name2);
    Bar bar3(bar1);
    Bar bar4(std::move(bar1));
    bar3 = bar2;				// wrong
    bar4 = std::move(bar2);		// wrong
}
```

如果成员函数中存在 *const* 修饰的变量或引用，那么就不能使用默认赋值函数。

如果成员函数中存在 *mutex* （不能拷贝，也不能移动），那么其不能使用赋值函数，也不能使用拷贝构造和移动构造函数。

## 不想要的特种成员函数，明确禁止编译器自动生成

### Modern C++做法

```cpp
class HomeForSale{
public:
	// ...
    HomeForSale(const HomeForSale&) = delete;					// ban
    HomeForSale& operator= (const HomeForSale&) = delete;		// ban
};
```

### C++ 98做法

```cpp
class HomeForSale{
public:
	// ...
private:
    // ...
    HomeForSale(const HomeForSale&);							// declarations only
    HomeForSale& operator= (const HomeForSale&);
};
```

如果想在编译阶段就发现错误：

```cpp
class Uncopyable{
protected:
    // 允许构造和销毁派生对象
	Uncopyable(){}
    ~Uncopyable(){}
private:
    Uncopyable(const Uncopyable&) 	// 但是阻止拷贝
    Uncopyable& operator=(const Uncopyable&);
};
class HomeForSale: private Uncopyable{
    // 类不再声明复制操作符或复制赋值操作符
    // ...
};
```

在上面的操作中，定义一个Uncopyable类，把需要禁止的函数私有定义，对于不想进行拷贝构造或拷贝复制的类，就继承Uncopyable，那么即使在HomeForSale成员函数里也不能使用自己的拷贝构造或拷贝复制函数。

## 为多态基类声明virtual析构函数

### 不是为了多态特性，不要随意声明virtual析构函数

```cpp
class Point{
public:
	int x, y;
};
Point ponit;
printf("&point:&p &point.x:%p\n", &point, &point.x);
printf("sizeof(Point):%lu\n", sizoef Point);
```

``` cpp
// out:
&point:0x7ffcf5c2ece0 &point.x:0x7ffcf5c2ece0
sizeof(Point):8
```

发现 x 的地址和对象的地址是一样的，如果增加了virtual析构函数：

```cpp
// out:
&point:0x7ffd5b9fe230 &point.x:0x7ffd5b9fe238
sizeof(Point):16
```

发现不仅地址不一样，内存也扩大了一倍，是因为前八个字节是一个指向虚表的指针。

### 类继承时小心父类析构函数不具有多态特性

```cpp
class SpecialString: public std::string{	// std::string没有virtual析构函数
    // ...
};
```

```cpp
SpecialString *pss = new SpecialString("I am String");
std::string *ps;
// ...
ps = pss; 		// SpecialString* => std::string*
// ...
delete ps; 		// 其只会释放std::string中的内存，而SpecialString中则会产生内存泄漏
```

```cpp
class AWOV{
public:
    virtual ~AWOV() = 0;	// 声明纯虚析构函数
};
AWOV::~AWOV(){}				// 需要被实现
```

## 析构函数不要抛出异常

```cpp
class Widget{
public:
    // ...
   	~Widget(){}		// 假设它可能抛出异常
};
void DoSomething(){
    std::vector<Widget> v;
}
```

上述代码在DoSomething函数结束时会调用vector的析构函数，再依次调用其内部每个Widget的析构函数，在这个过程中如果有一个析构函数抛出了异常，那么后面的析构函数将不会被调用！

### 避免异常从析构函数抛出的方法

```cpp
class DBConnection{
public:
    // ...
    static DBConnection create();
    
    void close();
};
class DBConn{
public:
    ~DBConn(){
        // 在这个析构函数中可能会抛出异常
        db.close();
    }
private:
  	DBConnection db;
};
```

可采用下面两种方法避免异常在析构函数中抛出

```cpp
DBConn::~DBConn(){
    try{ db.close(); }
	catch(...){
        // 直接终止程序
        std::abort();
    }
}
```

```cpp
DBConn::~DBConn(){
    try{ db.close(); }
	catch(...){
        // 记录日志
    }
}
```

### 抽取单独方法，将异常处理留给调用者

对于上面的情况，可以让用户自己调用 close() 关闭连接，这时可设置标志表示连接是否关闭...

## 不要在构造函数和析构函数中调用virtual函数

```cpp
class Base{
public:
    Base(){
        sayHello();
    }
    ~Base(){
        sayBye();
    }
    virtual void sayHello(){
        std::cout << "Hello, Base!" << std::endl;
    }
    virtual void sayBye(){
        std::cout << "Bye, Base!" << std::endl;
    }
};
class Derived : public Base{
public:
    Derived() {}
    ~Derived() {}
    void sayHello(){
        std::cout << "Hello, Derived!" << std::endl;
    }
    void sayBye(){
        std::cout << "Bye, Derived!" << std::endl;
    }
};
Derived derived;
```

输出：

```cpp
Hello, Base!
Bye, Base!
```

在构造子类时调用重写的函数，但是并没有呈现出多态的原因是，子类还没有创建完成或者已经被销毁了。

但是可以调用成员函数，虚函数、静态成员函数这些依赖构造函数完成的函数不能被调用。

## operator=返回自身引用

```cpp
class Widget{
public:
    // ...
    void operator= (const Widget& rhs){
        // ...
    }
}
```

当拷贝复制没有返回值时，一般的赋值函数可以通过，但是对于连等的情况则不能编译通过

```cpp
Wigdet w1(1), w2(2), w3(3);
w2 = w1;
w3 = w2 = w1;		// 错误，把void赋值给Widget
```

改成下面的写法就可以：

```cpp
class Widget{
public:
    // ...
    Widget& operator= (const Widget& rhs){
        // ...
        return *this;
    }
    Widget& operator+= (const Widget& rhs){		// 这种情况还可以应用于 -=,*= ...
        // ...
        return *this;
    }
    Widget& operator= (int rhs){
        // ...
        return *this;
    }
}
```

## 安全实现赋值运算符

```cpp
class Bitmap {
    // ...
};
class Widget{
    // ...
private:
    Bitmap *pb;
};
```

对于 Widget 的拷贝赋值函数，一般会写成下面的形式：

```cpp
Widget& Widget::operator= (const Widget& rhs){
    delete pb;
    pb = new Widget(*rhs.pb);
    return *this;
}
```

但是对 Widget 自赋值的情况，就会出现错误。因此可以先判断来排除这种情况：

```cpp
Widget& Widget::operator= (const Widget& rhs){
    if(this = &rhs) return *this;
    delete pb;
    pb = new Widget(*rhs.pb);
    return *this;
}
```

但是上面在 new 的过程中可能会抛出异常，一旦抛出异常，那么 pb 所指的内容就会被释放掉。

因此可以先用一个指针保存一下原始内容，再进行构造即可：

```cpp
Widget& Widget::operator= (const Widget& rhs){
	Bitmap *pOrig = pb;
    pb = new widget(*rhs.pb);
    delete pOrig;
    return *this;
}
```

而且上面这种写法，没有分支语句，因此也减少了一定的开销。

 还有一种写法：

```cpp
class Widget{
    // ...
    void swap(Widget& rhs);
    // ...
};
Widget& Widget::operator=(Widget rhs){
    swap(rhs);
    return *this;
}
```

这种写法，是直接利用拷贝构造函数构建新对象，然后通过swap函数来完成赋值。

## 



















































