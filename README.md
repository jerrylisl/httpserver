#httpServer


一个httpserver
 可直接make，用了一些新特性，请按如下make：
 ```
 make CXXFLAGS=-std=c++11
 ```
 基本情况：线程池+epoll+loop
 已完成：相关head的解析和get请求的处理，文件根目录在Handle.h中设置
 待完成：添加setting页用于参数设置，添加post功能，其他对动态页面的支持等
