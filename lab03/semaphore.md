# Linux信号量集 

Linux信号量作为IPC机制的一种，与其他通信方式类似，Linux也是通过kern_ipc_perm结构中的key来唯一标志一个信号量集，并通过该结构设置并检查访问权限。针对信号量集，系统维护一个由信号量集组成的数组，数组中的每个单元指向一个信号量集。


## 创建新的信号量集或获取已存在的信号量集

`int semget(key_t key ,int nsems ,int flag)``

参数说明：

* key：用户指定一个非0整数型值，对信号量集的打开或存取依赖于semflg参数的取值。
* nsems：指定打开或者新创建的信号量集将包含的信号量的数目。
* flag：
  * 若只设置semflg的IPC_CREAT位，则创建一个信号量集，如果该信号量集已经存在，则返回其标识符。
  * semflg的IPC_CREAT\|IPC_EXCL位，则创建一个新的信号量集，如果该key值的信号量已经存在，则返回错误信息。
  *  只设置IPC_EXCL而不设置IPC_CREAT位没有任何意义
* 返回值：正确返回信号量集的标识符，错误时返回-1。

例如，创建一个只包含一个信号量的信号量集：

```c
semid = semget(IPC_PRIVATE,1,IPC_CREAT\|0666);
```

## 对信号量的PV操作

`int semop(int semid ,struct sembuf *sops ,unsigned nsops);``

参数说明：
* semid：是信号量集的标识符，由semget()得到
* sops：指向一个sembuf结构数组，该数组的每一个元素对以一次信号量操作。
  * struct sembuf解释见下
* nsops：进行操作信号量的个数，即sops结构变量的个数，需大于或等于1。最常见设置此值等于1，只完成对一个信号量的操作


```c
struct sembuf
{
unsigned short sem_num; /*semaphore index in array*/
short sem_op; /*semaphore operation*/
short sem_flg; /*operation flags*/
};

```
* sem_num标明它是信号量集的第几个元素，从0开始
* sem_op指定信号量采取的操作
  * `<0`相当于P操作，占有资源
  * `>0`相当于V操作，释放资源
  * `=0`进程睡眠直到信号量的值为0
* sem_flg指明操作的执行模式，两个标志位。一个是IPC_NOWAIT，指明以非阻塞方式操作信号量。一个是SEM_UNDO，指明内核为信号量操作保留恢复值。

当操作信号量(semop)时，sem_flg可以设置SEM_UNDO标识；SEM_UNDO用于将修改的信号量值在进程正常退出（调用exit退出或main执行完）或异常退出（如段异常、除0异常、收到KILL信号等）时归还给信号量。

如信号量初始值是20，进程以SEM_UNDO方式操作信号量减2，减5，加1；在进程未退出时，信号量变成20-2-5+1=14；在进程退出时，将修改的值归还给信号量，信号量变成14+2+5-1=20。

返回值：正确返回0，错误时返回-1

##  信号量集的控制函数

```c
union semun
{
int val;                    /*value for setval*/
struct semid_ds *buf;        /*buffer for IPC_STAT & IPC_SET*/
unsigned short *array;      /*array for GETALL & SETALL*/
struct seminfo *_buf;       /*buffer for IPC_INFO*/
};
```


`int semctl(int semid ,int semnum ,int cmd ,union semun arg);`

参数说明：
* semid：是信号量集的标识符，由semget()得到
* semnum：指定semid信号量集的第几个信号量，在撤销信号量集时，此参数可缺省。
* cmd：指定操作类型。
* arg：数据类型是共同体类型semun，该类型在include/linux/sem.h中定义

  ----------------------------------------------------------------------------------------------------------------------------------
  **取值**    **含义**
  ----------- ----------------------------------------------------------------------------------------------------------------------
  GETVAL      返回semnum指定的信号量的semval域值

  SETVAL      指定semval域值为arg.val

  GETPID      返回semnum指定信号量sempid

  GETNCNT     返回semncnt

  GETZCNT     返回semzcnt

  GETALL      返回所有信号量的值，结果保存到arg.array中

  SETALL      通过arg.array更新所有信号量的值

  IPC_STAT    获取信号量集的arg.array，存入arg.buf

  IPC_SET     将arg.buf数据结构的sem_perm.uid,sem_perm.gid,sem_perm.mode赋给sem_array，此操作仅限root、sem_perm.cuid或sem_perm.uid

  IPC_RMID    删除指定信号量集。此操作仅限root、sem_perm.cuid或sem_perm.uid

  IPC_INFO    获取信号量集的相关信息存放于arg.buf中
  ----------------------------------------------------------------------------------------------------------------------------------

返回值：正确时根据cmd的的不同返回值或0，错误时返回-1。

例如：撤销信号量集 semctl**(semid ,IPC_RMID ,0)
