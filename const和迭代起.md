###
区别：
vector<int> iv;
vector<int>::iterator it0 = iv.begin();
vector<int>::const_iterator it1 = iv.begin();
const vector<int>::iterator it2 = iv.begin();

*it0 = 1;//对
*it1 = 1;//错，表示const_iterator指向的迭代起值不能改变
*it2 = 1;//对，const迭代起只能指向这种迭代器，值可以改变

++it0;//对
++it1;//对，可以改迭代起，值不能改
++it2;//错，只能指向那个迭代器，能改值

常量成员函数声明：
如：int get() const;
规则：
1.常量成员函数不修改对象。
2.常量成员函数在定义和声明中都应加const限定
3.非常量成员函数不能被常量成员函数调用，但构造函数和析构函数除外。
4.常量（const对象）对象只能调用常量成员函数。（const对象的数据成员在对象寿命周期内不能改变，因此其只能调用常量成员函数）。
意义：
1.使成员函数的意义更加清楚，将成员函数分修改对象和不修改对象两类。
2.增加程序的健壮性，常量成员函数企图修改数据成员或调用非常量成员函数，编译器会指出错误。
原因：
     对于X类型的非常量成员函数而言，其this指针的类型是 X * const，该指针自身是常量；但是对于X类型的常量成员函数而言，其this指针的类型是const X * const，是一个常量指针。

###mutable
mutable关键字可以突破const。对于const成员函数同样适用。
例子：
#include <iostream>
using namespace std;
class Test
{
    public:
        Test();
        int value() const;

    private:
        mutable int v;

};
Test::Test()
{
        v = 1;

}

int Test::value() const
{
        v++;
        return v;

}


int main()
{
        Test A;
        cout <<  A.value() << endl;
        return 0;

}




