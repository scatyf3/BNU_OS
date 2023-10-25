- 怎么debug多进程程序？
- 怎么做到client发送消息后...然后等待接受SERVER端返回给自己的消息后...再发送下一条消息？ ->自定义msgform，以及通过msgtype控制读取的数据类型
- server函数卡在了`struct msqid_ds buf={0};`，证据是在这一行之前的printf语句都可以打印出来，其后则不能->奇怪的阻塞

- 为什么不能正确break？我认为我的逻辑是对的