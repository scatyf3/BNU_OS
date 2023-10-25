# fork
fork一个新进程
* 在父进程中，fork()返回新创建子进程的进程ID
* 在子进程中，fork()返回0

# message queue
POSIX.1‐2017 defination:
3.226 Message Queue 3.226 消息队列
In the context of programmatic message passing, an object to which messages can be added and removed. Messages may be removed in the order in which they were added or in priority order.
在编程消息传递的上下文中，可以向其添加和删除消息的对象。邮件可以按添加顺序或优先级顺序删除。

## 本部分参考资料
- [某手册网站](https://linux.die.net/man/2/msgsnd)
- [man7](https://man7.org/index.html)

总而言之还是建议看英文原版一手资料手册，中文教程和ai回答的很多细节模糊，会导致debug非常难办

# msg_queue 里传递的消息格式
在msg_queue里传递的格式有特定的要求:
* 结构体里的第一个元素是一个数据类型为long的message type
* 实际接收消息的缓冲区，建议为一个数组或者结构体较为便利，具体细节参考[这个回答](https://stackoverflow.com/questions/66843903/structure-of-msg-in-ipc-message-queues)

```c
struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[1];    /* message data */
};
```


# msgget
原型：`msgget(key_t key,int flag)`
参数含义：
* key：消息队列的键值。如果该键值对应的消息队列不存在，则根据 flag 参数的值创建一个新的消息队列。
* flag：控制函数行为的标志，取值可以是 0 或 IPC_CREAT。
返回值：成功后，msgget（） 返回消息队列标识符（非负整数）
注意事项：server端flag=`0777|IPC_CREAT`，0777代表文件权限，含义是表示创建的消息队列的访问权限为所有者、所属组和其他用户都具有读、写和执行权限，IPC_CREAT代表创建队列。而client端flag=`0777`，无需重复创建新的队列

# msgrcv
原型：`ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);`
参数含义：
* msgqid：消息队列的标识符。
* msgp：指向接收消息的缓冲区。
* msgsz:需要接收消息的大小，注意，不包括mtype
* msgtyp：指定要接收的消息类型。
    * 如果为 0，则表示接收队列中的第一条消息。
    * 如果不为0，接受mtype==msgtype的消息
* msgflg：控制函数行为的标志，取值可以是 0 或 IPC_NOWAIT。
* 函数返回值为成功接收到的消息字节数

# msgsnd
原型：`int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);`
参数含义：
* msgid：消息队列的标识符。
* msgp：指向要发送的消息的缓冲区。
* msgsz：发送消息的长度
* msgflg：控制函数行为的标志，取值可以是 0 或 IPC_NOWAIT。
* 函数返回值为 0 表示成功，如果出错则返回 -1


# 命令行工具
## ipcs
在命令行里返回全部的消息队列和状态，示例如下：
```shell
juanitahowe@JuanitadeMacBook-Air 消息队列实验 % ipcs
IPC status from <running system> as of Tue Oct 17 11:05:13 CST 2023
T     ID     KEY        MODE       OWNER    GROUP
Message Queues:
q  65536 0x0000004b --rw-rw-rw- juanitahowe    staff

T     ID     KEY        MODE       OWNER    GROUP
Shared Memory:

T     ID     KEY        MODE       OWNER    GROUP
Semaphores:

```

## ipcrm -q <queue_id>
删除某个指定id的消息队列

