## Note
cpp-httplib 作为服务端，需要改动两个地方
- #include "httplib.h" 之前 define 下 CPPHTTPLIB_USE_POLL
- CPPHTTPLIB_LISTEN_BACKLOG 宏的值改为 1024（默认是 5）


## 压测对比
用 go 和 httplib 分别写一个 unix domain socket 的服务端，用 go 作为客户端进行压测。

go 客户端用同一个 client 对象，在不同的协程中并发发起 post 请求
```txt
并发数      go qps       httplib qps
 1k          16k            12k
 5k          16k            12k
 10k         16k            12k
```

go 客户端在并发的协程中，创建各自的 client 对象，并发起 post 请求
```txt
并发数      go qps       httplib qps
 1k          14k            < 1k
 5k          14k            < 1k
 10k         14k            < 1k
```
