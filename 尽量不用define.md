
define 没有并不重视作用域，因此define 不仅不能用来定义class专属常量，也不能提供任何封装。


###用const
1.h中
class A
{
private:
    static const int num = 5;//这是一个常量声明
}

1.c中可以有：
const int A::num;//这是num定义

只要不取这常量的地址，可以仅声明无需定义便可以使用。
如果编译器不允许一个静态常量在声明时候被赋值，那可以用枚举类型的数值权充int。

###用enum
如下：
class A
{
private:
    enum {num = 5};

}


enum比较像define，不像const,取const地址合法，但是enum不行。define也不能提供任何封装行。有了这个限制,enum就相当于可以做为约束。

###define作为可以作为宏函数声定义

无论你什么时候使用宏函数，这个函数都不会带来作为函数的额外开销，且所有实参都必须加上括号。
inline更安全，有真正函数开销。

###总结:
    对于单纯常量，最好以const或者enums替换define;
    对于形似函数的宏，最好使用inline函数替换define;
