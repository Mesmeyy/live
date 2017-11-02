###自我赋值的两种办法:
1.`/*************************************************************************
	> File Name: 2.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月02日 星期四 09时25分56秒
 ************************************************************************/

#include<iostream>
using namespace std;
class AAA{
    public:
    AAA& operator=(const AAA&rhs){
        if(this == &rhs) return *this;
        delete p;
        p = new int(*rhs.p);
        return *this;
    }
    AAA (const AAA &rhs){
        *(this -> p) = *(rhs.p);
    }
    AAA(int a){
        this -> p = new int(a); 
    }
    public:
    int *p;
};
int main()
{
    AAA a(4),b(5);
    a = b;
    cout << "a : "<<*(a.p) << endl;
}`

测试结果:
![这里写图片描述](http://img.blog.csdn.net/20171102095129000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2.用第三方的swap函数:

```
/*************************************************************************
	> File Name: 2.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月02日 星期四 09时25分56秒
 ************************************************************************/

#include<iostream>
using namespace std;
class AAA{
    public:
    AAA& operator=( AAA&rhs){
        if(this == &rhs) return *this;
        /*delete p;
        p = new int(*rhs.p);
        return *this;
        */
        this -> swap(rhs);
        return *this;
    }
    AAA (const AAA &rhs){
        this -> p = new int(*rhs.p); 
    }
    AAA(int a){
        this -> p = new int(a); 
    }
    private:
    void swap(AAA &rhs){
        AAA temp(rhs);
        *( rhs.p ) = *( this -> p);
        *(this -> p) =*(temp.p);
        cout << *(this -> p )<< endl;
    }
    public:
    int *p;
};
int main()
{
    AAA a(4),b(5);
    a = b;
    cout << "a : "<<*(a.p) << endl;
}
```
结果:
![这里写图片描述](http://img.blog.csdn.net/20171102101559175?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

###总结:
1 我的水平实在态差了，编写这两个cpp的时候，一直会有段错误，不用gdb调试不出来。int *p一定要开空间再赋值。

2 而且:赋值拷贝构造函数不要调用重载=号的函数，重载=运算符的函数也不能调用赋值拷贝函数，否则会一直无穷无尽～你懂得。

 一旦自己定义了opetator运算符，那么在类中一定要很注意使用这个运算符。尤其是小心规则2，很容易不经意间就入坑了



prioritycustomer::prioritycustomer(const prioritycustomer& rhs):Customer(rhs),priority(rhs.priority)//调用baseclass的构造函数
{
    logcall("");
}

prioritycustomer& prioritycustomer::operator=(const prioritycustomer& rhs)
{
    logcall("");
    Customer::operator=(rhs);//对baseclass进行赋值
    priority = rhs.priority;
    return *this;
}




