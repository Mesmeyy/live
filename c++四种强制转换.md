const_cast:字面上理解就是去掉const属性
static_cast:明明上的理解是静态类型转换，int转换成char
dynamic_case:动态类型转换，子类和父类之间的多态类型转换
reinterpreter_cast:重新解释类型，没有二进制转换

const_cast:去掉const属性和volatile属性

struct SA {
　　int i;
};
const SA ra;
//ra.i = 10; //直接修改const类型，编译错误
SA &rb = const_castSA&>(ra);
rb.i = 10;

static_cast:静态类型转换
使用方式:
    1.子类转成父类的指针
    2.空指针转成目标类型
    3.基本的数据类型之间的转换
    4.表达式转成void型
    5.有点遗憾，不能去掉const volitale属性

int n = 6;
double d = static_castdouble>(n); // 基本类型转换
int *pn = &n;
double *d = static_castdouble *>(&n) //无关类型指针转换，编译错误
void *p = static_castvoid *>(pn); //任意类型转换成void类型


dynamic_case:
    1.动态类型转换
    2.基类和子类之间的转换
    3.必须要有虚函数
    4.相同基类的不同子类之间的交叉转换NULL


class BaseClass {
public:
　int m_iNum;
　virtual void foo(){};
//基类必须有虚函数。保持多台特性才能使用dynamic_cast
};
class DerivedClass: public BaseClass {
public:
　　char *m_szName[100];
　　void bar(){};
};
BaseClass* pb = new DerivedClass();
DerivedClass *pd1 = static_castDerivedClass *>(pb);
//子类->父类，静态类型转换，正确但不推荐
DerivedClass *pd2 = dynamic_castDerivedClass *>(pb);
//子类->父类，动态类型转换，正确
BaseClass* pb2 = new BaseClass();
DerivedClass *pd21 = static_castDerivedClass *>(pb2);
//父类->子类，静态类型转换，危险！访问子类m_szName成员越界
DerivedClass *pd22 = dynamic_castDerivedClass *>(pb2);
//父类->子类，动态类型转换，安全的。结果是NULL

reinterpreter_cast:
    1.没有进行二进制转换的重新类型解释
    2.函数指针类型之间的转换
    3.把指针转换成一个整数,把整数转换成指针


总之:
去const属性用const_cast。
基本类型转换用static_cast。
多态类之间的类型转换用daynamic_cast。
不同类型的指针类型转换用reinterpreter_cast。

