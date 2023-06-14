## module
1. epoll server [Main Epoll + Logic Epoll]
2. [Ring | Vec] buffer [Read | Write] 
3. TcpConnection [Client]

## src
1. buffer
    每个tcp连接的用户层缓冲buffer,会从socket buffer里面拷贝数据到用户层，后面TcpConnection类会来取数据
    如何取解析buffer里面的流数据，也是TcpConnection需要做的事情
2. common
    错误码定义、一些全局变量
3. epoll_server
    epoll总的实现,包括对TcpConnection类的管理
4. interface
5. logger
    日志收集，开额外线程写日志文件
6. queue
    multi-producer + multi-consumer lock-free queue,用来存用户的请求，工作线程消费请求
7. tcp
    tcp连接的实现,需要实现解析tcp流的函数
