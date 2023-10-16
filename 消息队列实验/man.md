# msgget
原型：`msgget(key_t key,int flag)`
参数含义：
* key：消息队列的键值。如果该键值对应的消息队列不存在，则根据 flag 参数的值创建一个新的消息队列。
* flag：控制函数行为的标志，取值可以是 0 或 IPC_CREAT。
返回值：成功后，msgget（） 返回消息队列标识符（非负整数）
注意事项：server端flag=`0777|IPC_CREAT`，0777代表文件权限，含义是表示创建的消息队列的访问权限为所有者、所属组和其他用户都具有读、写和执行权限，IPC_CREAT代表创建队列。而client端flag=`0777`，无需重复创建新的队列

# msgrcv
原型：`size_t msgrcv(int msgqid, void *msgp, size_t maxcnt, long msgtype, int flag)`
参数含义：
* msgqid：消息队列的标识符。
* msgp：指向接收消息的缓冲区。
* maxcnt：接收消息缓冲区的大小。
* msgtype：指定要接收的消息类型。如果为 0，则表示接收队列中的第一条消息。
* flag：控制函数行为的标志，取值可以是 0 或 IPC_NOWAIT。
函数返回值为成功接收到的消息字节数

# msgsnd
原型：`int msgsnd(int msgid, const void *msgp, size_t count, int flag)`
参数含义：
* msgid：消息队列的标识符。
* msgp：指向要发送的消息的缓冲区。
* count：要发送的消息的长度。
* flag：控制函数行为的标志，取值可以是 0 或 IPC_NOWAIT。
函数返回值为 0 表示成功，如果出错则返回 -1

# msgctl
原型：`int msgctl(int msgid, int cmd, struct msqid_ds *buf)`
* msgid：消息队列的标识符。
* cmd：指定要对消息队列进行的操作。常见的操作包括：
    * IPC_STAT：获取消息队列的状态信息，将其存储在 buf 指向的结构体中。
    * IPC_SET：设置消息队列的状态信息，使用 buf 指向的结构体中的值。
    * IPC_RMID：从系统中删除消息队列。
* buf：指向存储消息队列状态信息的结构体。