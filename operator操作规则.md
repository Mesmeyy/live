##当重载的运算符是=号的时候，要遵循的规则:
   
 1:要返回*this
    这一条是显然的。
```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月02日 星期四 09时00分19秒
 ************************************************************************/

#include<iostream>
using namespace std;
class AAA
{
    public:
    AAA &operator=(const AAA &a){
        AAA temp(a);
        this -> name  = temp.name;
        return *this;
    }
    string name;
};

int main()
{
    AAA a,b;
    a.name = "zhuziyu";
    b.name = "dulun";
    a = b;
    cout << "a.name = " << a.name << endl;
    return 0;
}
```
运行结果:
![这里写图片描述](http://img.blog.csdn.net/20171102091526891?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 2:返回值引用和非引用对结果没有影响。
 

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月02日 星期四 09时00分19秒
 ************************************************************************/

#include<iostream>
using namespace std;
class AAA
{
    public:
    AAA operator=(const AAA &a){
        AAA temp(a);
        this -> name  = temp.name;
        return *this;
    }
    string name;
};

int main()
{
    AAA a,b;
    a.name = "zhuziyu";
    b.name = "dulun";
    a = b;
    cout << "a.name = " << a.name << endl;
    return 0;
}
```
![这里写图片描述](http://img.blog.csdn.net/20171102091718656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

 
3:参数是const 和非const 都可以。
  参数是&和非&都可以。
  

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年11月02日 星期四 09时00分19秒
 ************************************************************************/

#include<iostream>
using namespace std;
class AAA
{
    public:
    AAA operator=(const AAA a){
    //AAA operator=(AAA &a)
        AAA temp(a);
        this -> name  = temp.name;
        return *this;
    }
    string name;
};

int main()
{
    AAA a,b;
    a.name = "zhuziyu";
    b.name = "dulun";
    a = b;
    cout << "a.name = " << a.name << endl;
    return 0;
}
```
结果已经验证。

当重载的运算符是=号的时候，要遵循的规则:
    1:要返回*this
    这一条是显然的。
    2:
    3:

