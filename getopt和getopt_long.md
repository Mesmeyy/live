##getopt和getopt_long函数

1.介绍getopt

```
int getopt(int argc, char * const argv[],  
           const char *optstring);  
```
           
getopt函数的前两个参数，就是main函数的argc和argv，这两者直接传入即可，要考虑的就只剩下第三个参数。
optstring的格式举例说明比较方便，例如：
    char *optstring = "abcd:";
上面这个optstring在传入之后，getopt函数将依次检查命令行是否指定了 -a， -b， -c及 -d（这需要多次调用getopt函数，直到其返回-1），c才能检查完所有你输入的参数。因为每次检查一个参数,当检查到上面某一个参数被指定时，函数会返回被指定的参数名称（即该字母）
最后一个参数d后面带有冒号，代表可以加指定的参数，如 -d 10086
optind表示的是下一个将被处理到的参数在argv中的下标值。
如果指定opterr = 0的话，在getopt、getopt_long遇到错误将不会输出错误信息到标准输出流。

代码和运行结果:
![这里写图片描述](http://img.blog.csdn.net/20171216212639826?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](http://img.blog.csdn.net/20171216212655672?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

2介绍getopt_long函数

```
int getopt_long(int argc, char * const argv[],  
           const char *optstring,  
           const struct option *longopts, int *longindex);  
```
2.1
在这里第一个多余的参数:longopts指向的是一个由option结构体组成的数组，这是已经定义好的，你动态调用就行。
结构如下:

```
struct option {
               const char *name;
               //是参数的名称
               int         has_arg;
               //无参0有参1都行2
               int        *flag;
               //flag = 0函数返回val
               int         val;
               //指定函数找到选项时候的番薯返回值或者flag指向的值
           };
```
2.2
如果longindex不为,0,,即是找到参数在longopts中的下标值。
代码:
![这里写图片描述](http://img.blog.csdn.net/20171216213441114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结果:
![这里写图片描述](http://img.blog.csdn.net/20171216213509092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


