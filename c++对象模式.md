
###简单对象模型:
![这里写图片描述](http://img.blog.csdn.net/20171011215048621?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

在这个模型里面,members本身并不放在object中，只有指向menber的指针放在object中。这么做可以避免members有不同的类型，避免需要不同的存储空间，因此这时候class object大小如下计算:
指针大小*class中所声明的members个数。这个日后被引用到了指向成员的指针。

###表格驱动对象模型：
![这里写图片描述](http://img.blog.csdn.net/20171011215803855?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1eml5dTExNTc4MTc1NDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

member function table的观念成为支持virtual function的有效方案。
