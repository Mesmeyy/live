我对nginx中server_name的理解是:权限+ip复用。
在wwwroot下有一个域名叫butter,那么假如dns承认这个域名对应ip:123.123.123.123，那么wwwroot下就有一个目录butter，下有index.html，当你地址栏输入这个域名,会自动返回index.html。
加入.conf中又配置了一个server_name叫chichi，那么加入dns同样承认这个域名对应ip:123.123.123.123，那么地址栏输入chichi，会有wwwroot下chichi下的index.html来返回给浏览,会自动返回index.html。

因此多个域名对应ip复用，同时也对实际上真正的文件访问增加了权限。
