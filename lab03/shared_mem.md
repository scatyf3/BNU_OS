# 共享内存
## shmget
原型和头文件:
```c
#include <sys/ipc.h>
#include <sys/shm.h>

int shmget(key_t key, size_t size, int shmflg);
```
参数分别是：
- 进程间通信键值
- 共享内存大小
- 权限等级

shmget()函数返回的是一个整数，即共享内存段的标识符。这个标识符可以用于后续的共享内存操作，例如将共享内存连接到当前进程的地址空间。

## shmat
`shmat` 函数是 Linux 系统编程中用于将共享内存连接到当前进程的地址空间中的函数
```c

void *shmat(int shmid, const void *shmaddr, int shmflg);
```
该函数的参数包括共享内存标识符、连接地址和标志等。调用该函数会返回一个指向共享内存第一个字节的指针。

- 共享内存标识符：由 `shmget` 函数返回的共享内存标识符。
- 连接地址：指定共享内存连接到当前进程的地址，通常设置为 `NULL`。
- 标志：指定连接方式，包括只读和读写等。

## shmdt
`shmdt` 函数是 Linux 系统编程中用于将共享内存从当前进程的地址空间中分离的函数之一。它的原型如下：
```c

int shmdt(const void *shmaddr);
```
该函数的参数是由 `shmat` 函数返回的地址指针。调用该函数会将共享内存从当前进程的地址空间中分离，但并不会删除共享内存。调用成功时返回 `0`，失败时返回 `-1`。

## shmctl
`shmctl` 函数是 Linux 系统编程中用于控制共享内存的函数之一。它的原型如下：

```c
#include <sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```

该函数的参数包括共享内存标识符、命令和 `shmid_ds` 结构体等。调用该函数可以对共享内存进行多种操作，包括获取共享内存信息、设置共享内存信息、删除共享内存等。

- `shmid`：由 `shmget` 函数返回的共享内存标识符。
- `cmd`：指定需要执行的操作，包括获取共享内存信息、设置共享内存信息、删除共享内存等。
- `shmid_ds` 结构体：用于传递共享内存信息，包括共享内存的大小、访问权限、最后一次连接时间等。
