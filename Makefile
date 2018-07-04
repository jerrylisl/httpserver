httpserver: Server.o Socket.o EventLoopThreadPool.o EventLoopThread.o Thread.o EventLoop.o Parser.o Handler.o Epoll.o Buffer.o Optionloader.o
	g++ -std=c++11 -O3 -pthread -o httpserver Server.o Socket.o EventLoopThreadPool.o EventLoopThread.o Thread.o EventLoop.o Parser.o Handler.o Epoll.o Buffer.o Optionloader.o

Server.o: Server.cpp Socket.h EventLoopThreadPool.h EventLoopThread.h EventLoop.h

Socket.o: Socket.cpp Socket.h 

EventLoopThreadPool.o: EventLoopThreadPool.cpp EventLoopThread.h 	

EventLoopThread.o: EventLoopThread.cpp EventLoopThread.h EventLoop.h Thread.h 

Thread.o: Thread.cpp Thread.h 	

EventLoop.o: EventLoop.cpp EventLoop.h Epoll.h 	

Parser.o: Parser.cpp Parser.h 	

Handler.o: Handler.cpp Handler.h Parser.h 

Epoll.o: Handler.cpp Handler.h Parser.h Epoll.h 

Buffer.o: Buffer.cpp Buffer.h 

OptionLoader.o: Optionloader.h Optionloader.cpp



clean:
	rm *.o httpserver
