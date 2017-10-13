/*************************************************************************
	> File Name: 虚拟c++代码小例子.cpp
	> Author: 朱紫钰
	> Mail: zhuziyu1157817544@gmail.com
	> Created Time: 2017年10月13日 星期五 10时09分40秒
 ************************************************************************/

#include<iostream>
using namespace std;
//constructor:commen
//destructor:virtual
//foo:virtual
X foobar()
{
    X xx;
    X *p = new X;
    //foo()是一个virtual functiona
    xx.foo();
    p -> foo();
    delete p;
    return xx;
};

//内部转换大致情况
void foobar(X &_result)
{
    //构造_result
    //_result取代本地xx
    _result.X::X();

    //扩展X *p = new X;
    p = _new(sizeof(X));
    if(px != 0) px-> X::X();

    //扩展xx.foo()但是不用virtual机制
    //以_result取代xx
    foo(& _result);

    //使用virtual机制扩展p -> foo()
    (*px -> vtbl[2])(px);

    //扩展delete p
    if(p != 0){
        (*p -> vtbl[1])(p);
        _delete(p);
    }
    
    //不用销毁xx
    return ;
}
