#httpServer


一个httpserver
 可直接make，用了一些新特性，请按如下make：
 ```
 make CXXFLAGS=-std=c++11
 ```
 基本情况：线程池+epoll+loop
 
 已完成：相关head的解析和get请求的处理，文件根目录在Handle.h中设置
 
 已完成：添加setting页用于参数设置，添加post功能，将post数据统一作为参数提交给对应cgi脚本（.py或.php或二进制程序），通过system进行执行，并回传结果。
 
 post文件的应对：增加了tmp文件夹，位于root路径下（setting可设置root目录），将文件复制到该文件夹下，并将文件名作为参数传递给cgi。这个方案参考了php等脚本的设计。
 
 待完成：一些实例页面以及后台cgi的完善与修正，一些额外的封装
