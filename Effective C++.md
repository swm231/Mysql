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

















































