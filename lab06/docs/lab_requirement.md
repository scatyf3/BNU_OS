**实验目的**：

通过编写和调试请求页式存储管理的模拟程序以加深对请求页式存储管理方案的理解。

**实验内容：（**FIFO置换算法**示例:[[for
windows]{.underline}](pageFIFO.exe)）**

页面淘汰算法采用FIFO置换算法（或者其他置换算法），在淘汰一页时，判断它是否被改写过，如果被修改过，将它写回到外存上。

1．数据结构

(1)虚地址结构

16 10 9 0

  -----------------------------------------------------------------------
  页号                                页内地址
  ----------------------------------- -----------------------------------

  -----------------------------------------------------------------------

(2)页表结构

define N 64

struct

{

int lNumber;//页号

int pNumber;//物理块号

int dNumber;//在磁盘上的位置

int write;//修改位标志

int flag;//存在位标志

}page\[N\];

(3)存放页号的队列p\[m\]，m为分配给进程的内存块数。当装入一个新的页面时，将其页号存入数组。在模拟试验中，我们采用预调页的方法。

请求页式存储管理算法的流程图如下：

![](media/image1.jpg){width="1.9846161417322834in"
height="2.7524332895888013in"}

图1：请求页式存储管理模拟程序的完整流程图

![](media/image2.jpg){width="2.8607589676290464in"
height="4.989520997375328in"}

图2：一条指令执行的模拟流程图

**上交要求：**

-   实验报告包括：源程序代码、运行结果、实验总结。实验报告命名方式：学号_姓名_page.docx；

-   源程序命名为：学号_姓名_page.c或.cpp；

-   将源程序和实验报告打包为：学号_姓名_page.rar，将该打包文件发送到邮箱：<os_experiment@126.com>；

-   上交截止时间：11月28日24：00。