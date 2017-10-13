
nm命令可以查看可执行文件的标识符。
模型:
####简单对象模型:

![这里写图片描述](http://img.blog.csdn.net/20171011215048621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这个模型里面,members本身并不放在object中，只有指向menber的指针放在object中。这么做可以避免members有不同的类型，避免需要不同的存储空间，因此这时候class object大小如下计算:
指针大小*class中所声明的members个数。这个日后被引用到了指向成员的指针。

###表格驱动对象模型：

![这里写图片描述](http://img.blog.csdn.net/20171011215803855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

member function table的观念成为支持virtual function的有效方案。
###更改：
vptr指向的第一个也就是偏移量为0的是type_info。第二个是析构函数,然后是其他函数。

###c++对象模型

![这里写图片描述](http://img.blog.csdn.net/20171012135916308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

nonstatic member:对象内
static member:个别的class object之外
nonstatic func:同上
static func():同上
constructor:同上
destructor:类中的虚函数表中的指针指向析构函数
vptr指向虚函数表。

有如下例子：

```
/*************************************************************************
	> File Name: 2.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月12日 星期四 13时22分50秒
 ************************************************************************/

#include<iostream>
#include<string>
using namespace std;
class Base
{
public:
    Base(int){};
    virtual ~Base(void){};
 
    int getIBase() const{};
    static int instanceCount(){};
    virtual void print() const{};
 
protected:
 
    int iBase;
    static int count;
}; 

int main()
{
    Base b1(1000);
    cout << "对象b1的起始内存地址：" << &b1 << endl;
    cout << "type_info信息：" << ((int*)*(int*)(&b1) - 1) << endl;
    cout << "虚函数表地址：\t\t\t" << (int*)(&b1) << endl;
    cout << "虚函数表 — 第1个函数地址：\t" << (int*)*(int*)(&b1) << "\t即析构函数地址：" << (int*)*((int*)*(int*)(&b1)) << endl;
    cout << "虚函数表 — 第2个函数地址：\t" << ((int*)*(int*)(&b1) + 1) << "\n";
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*(((int*)*(int*)(&b1)) + 1);
    pFun();
    b1.print();
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&b1) +1) << "\t通过地址取值iBase的值：" << *((int*)(&b1) +1) << endl;
    cout << "Base::getIBase(): " << b1.getIBase() << endl;

    b1.instanceCount();
    cout << "静态函数instanceCount地址： " << b1.instanceCount << endl;
    return 0;
}

```

运行结果:

对象b1的起始内存地址：0x7ffd273775a0
type_info信息：0x401084
虚函数表地址：			0x7ffd273775a0
虚函数表 — 第1个函数地址：	0x401088	即析构函数地址：0x400e2a
虚函数表 — 第2个函数地址：	0x40108c


更改后的例子:

```
/*************************************************************************
	> File Name: 2.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月12日 星期四 13时22分50秒
 ************************************************************************/

#include<iostream>
#include<string>
using namespace std;
class Base
{
public:
    Base(int){};
    ~Base(void){};
 
    int getIBase() const{};
    static int instanceCount(){};
    virtual void print() const{};
    virtual void print2() const{};
protected:
 
    int iBase;
    static int count;
}; 

int main()
{
    Base b1(1000);
    cout << "对象b1的起始内存地址：" << &b1 << endl;
    cout << "type_info信息：" << ((int*)*(int*)(&b1) - 1) << endl;
    cout << "虚函数表地址：\t\t\t" << (int*)(&b1) << endl;
    cout << "虚函数表 — 第1个函数地址：\t" << (int*)*(int*)(&b1) << "\t即析构函数地址：" << (int*)*((int*)*(int*)(&b1)) << endl;
    cout << "虚函数表 — 第2个函数地址：\t" << ((int*)*(int*)(&b1) + 1) << "\n";
    typedef void(*Fun)(void);
    Fun pFun = (Fun)*(((int*)*(int*)(&b1)) + 1);
    pFun();
    b1.print();
    cout << endl;
    cout << "推测数据成员iBase地址：\t\t" << ((int*)(&b1) +1) << "\t通过地址取值iBase的值：" << *((int*)(&b1) +1) << endl;
    cout << "Base::getIBase(): " << b1.getIBase() << endl;

    b1.instanceCount();
    cout << "静态函数instanceCount地址： " << b1.instanceCount << endl;
    return 0;
}

```
运行结果:
对象b1的起始内存地址：0x7ffcffff5940
type_info信息：0x401014
虚函数表地址：			0x7ffcffff5940
虚函数表 — 第1个函数地址：	0x401018	即析构函数地址：0x400e14
虚函数表 — 第2个函数地址：	0x40101c



不管是单继承、多继承，还是虚继承，如果基于“简单对象模型”，每一个基类都可以被派生类中的一个slot指出，该slot内包含基类对象的地址。这个机制的主要缺点是，因为间接性而导致空间和存取时间上的额外负担；优点则是派生类对象的大小不会因其基类的改变而受影响。
如果基于“表格驱动模型”，派生类中有一个slot指向基类表，表格中的每一个slot含一个相关的基类地址（这个很像虚函数表，内含每一个虚函数的地址）。这样每个派生类对象都有一个bptr，它会被初始化，指向其基类表。这种策略的主要缺点是由于间接性而导致的空间和存取时间上的额外负担；优点则是在每一个派生类对象中对继承都有一致的表现方式，每一个派生类对象都应该在某个固定位置上放置一个基类表指针，与基类的大小或数量无关。第二个优点是，不需要改变派生类对象本身，就可以放大，缩小、或更改基类表。

###无重写的单继承

![这里写图片描述](http://img.blog.csdn.net/20171012145100635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


###重写的单继承

```
class Derived_Overrite :
    public Base
{
public:
    Derived_Overrite(int);
    virtual ~Derived_Overrite(void);
    virtual void print(void) const;
 
protected:
    int iDerived;
};
```

![这里写图片描述](http://img.blog.csdn.net/20171012145653092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

这时候子类的虚函数表中有的只有子类自己的print,而没有父类的print。没有被替换的父类虚函数在前，子类的虚函数在后。



###可重写的多继承
每个基类都有自己的虚表。
子类的成员函数被放到了第一个基类的表中。
内存布局中，其父类布局依次按声明顺序排列。
每个基类的虚表中的print()函数都被overwrite成了子类的print ()。这样做就是为了解决不同的基类类型的指针指向同一个子类实例，而能够调用到实际的函数。

###
