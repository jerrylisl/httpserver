# httpServer


一个httpserver
 可直接make，用了一些新特性，请按如下make：
 ```
 make CXXFLAGS=-std=c++11
 ```
 基本情况：线程池+epoll+loop
 
 对GET POST的处理

增加setting文件

添加了makefile

重写了部分Parser内容，第二次优化（string复制加入std::move，效果不佳）

增加了keep-alive的处理，去掉了调试时的一些输出语句

一个main函数的修改，加入SIGINT的处理，第三次优化（sendfile替换，有一定效果）封装了服务器类，现在可直接在main中调用，简化了启动过程，加入了对ctrl+C(SIGINT)的信号处理，会调用相关清理函数再退出程序

加入了一些测试页面

请在make时加入-O3
