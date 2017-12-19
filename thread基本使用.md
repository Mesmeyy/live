###创建线程:

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年12月19日 星期二 13时32分14秒
 ************************************************************************/

#include<iostream>
#include <thread>
using namespace std;
void fun()
{
    cout << "This is thread of " << this_thread::get_id()<<  endl;

}
void fun2(int a)
{
    cout << "This is a thread of " << this_thread::get_id() <<"with refenerce " << a << endl;
}
int main()
{
    thread t1(fun);
    t1.join();//子线程执行完主线程才会执行
    
    thread t2(fun2,5);
    t2.join();

    cout << "father end" <<endl;

}
```
运行结果:
![这里写图片描述](http://img.blog.csdn.net/20171219135243946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

###分离子线程
这样原来的线程不用等它了

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年12月19日 星期二 13时32分14秒
 ************************************************************************/

#include<iostream>
#include <thread>
using namespace std;
void fun()
{
    cout << "This is thread of " << this_thread::get_id()<<  endl;

}
void fun2(int a)
{
    cout << "This is a thread of " << this_thread::get_id() <<"with refenerce " << a << endl;
}
int main()
{
    thread t1(fun);
    t1.detah();
    
    thread t2(fun2,5);
    t2.detah();

    cout << "father end" <<endl;

}
```
运行结果:
![这里写图片描述](http://img.blog.csdn.net/20171219135429976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

因为此时的线程没有确定的顺序,因此显示结果比较乱

###加上互斥量
互斥锁对于公共访问区有巨大作用。线程共用一份代码，而没有确定执行顺序的情况下，必须对公共访问区加锁以保证数据不会乱。
先看没有加锁的情况

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年12月19日 星期二 13时32分14秒
 ************************************************************************/

#include<iostream>
#include <thread>
#include<mutex>

using namespace std;
mutex lock;
static int vary = 50;

void fun()
{
    cout << this_thread::get_id() << "  vary =  " << ++vary <<  endl;

}
void fun2()
{
    cout << this_thread::get_id() << "  vary =  " << ++vary << endl;
}
int main()
{
    thread t1(fun);
    thread t2(fun2);
    t1.detach();
    t2.detach();
    cout << "father end" <<endl;

}
```
运行结果:
![这里写图片描述](http://img.blog.csdn.net/20171219140249626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

再看加锁后的使用：

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年12月19日 星期二 13时32分14秒
 ************************************************************************/

#include<iostream>
#include <thread>
#include<mutex>
#include<algorithm>
using namespace std;
mutex m;
static int vary = 50;

void fun()
{
    m.lock();
    ++vary;
    cout << this_thread::get_id() << "  vary =  " << ++vary <<  endl;
     m.unlock();
}
void fun2()
{
    m.lock();
    ++vary;
    cout << this_thread::get_id() << "  vary =  " << ++vary << endl;
     m.unlock();
}
int main()
{
    thread t1(fun);
    thread t2(fun2);
    t1.join();
    t2.join();
    cout << "father end" <<endl;

}
```
![这里写图片描述](http://img.blog.csdn.net/20171219141328910?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

###锁中lock_guard的使用
lock_guard:更加灵活的锁管理类模板，构造时是否加锁是可选的，在对象析构时如果持有锁会自动释放锁，所有权可以转移。对象生命期内允许手动加锁和释放锁。

```
/*************************************************************************
	> File Name: 1.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年12月19日 星期二 13时32分14秒
 ************************************************************************/

#include<iostream>
#include <thread>
#include<mutex>
#include<algorithm>
using namespace std;
mutex m;
static int vary = 50;

void fun()
{
    std::lock_guard<std::mutex> lck(m);
    ++vary;
    cout << this_thread::get_id() << "  vary =  " << ++vary <<  endl;
}
void fun2()
{
    std::lock_guard<std::mutex> lck(m);
    ++vary;
    cout << this_thread::get_id() << "  vary =  " << ++vary << endl;

}
int main()
{
    thread t1(fun);
    thread t2(fun2);
    t1.join();
    t2.join();
    cout << "father end" <<endl;

}
```
因为有封装mutex的意思，即销毁lock_guard变量执行析构函数自动解锁,因此更方便管理。
结果:
![这里写图片描述](http://img.blog.csdn.net/20171219141726633?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
