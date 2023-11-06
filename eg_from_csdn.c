
int main() {
    int key, i;                  // key 为共享内存的键值
    int shmexist = 0;            // 表示共享内存是否已存在
    void *shm = NULL;            // 连接共享内存的标记
    struct sharedMemory *share;  // 连接到共享内存的指针
    int shmid;                   // 共享内存标识符
    int semid;                   // 信号量集标识符
    int choice;                  // 选择标识
    int ifbreak = 0;             // 跳出循环的标识
    char message[BUFFERSIZE];    // 临时消息存储
    // 获取共享内存的键值
    key = ftok("./consumer.c", 0);
    // 判断是否获取成功
    if (key == -1) {
        printf("ftok() failed\n");
        exit(EXIT_FAILURE);
    }

    // 请求创建新共享内存
    shmid =
        shmget(key, sizeof(struct sharedMemory), 0666 | IPC_CREAT | IPC_EXCL);
    // 如果创建新的共享内存失败，则试图获取已有的共享内存
    if (shmid == -1) {
        shmexist = 1;  // 共享内存已存在
        shmid = shmget(key, sizeof(struct sharedMemory), 0666 | IPC_CREAT);
        if (shmid == -1) {
            printf("shmget() failed\n");
            exit(EXIT_FAILURE);
        }
    }
    // 进程请求连接共享内存，若失败则退出
    if ((shm = shmat(shmid, NULL, 0)) == (void *)(-1)) {
        printf("shmat() failed\n");
        exit(EXIT_FAILURE);
    }

    // share 访问共享内存
    share = (struct sharedMemory *)shm;
    // 创建含有 3 个信号量的信号量集
    // 0 : mutex
    // 1 : full
    // 2 : empty
    semid = semget(key, SEMNUM, IPC_CREAT | 0666);
    // 如果共享内存是第一次创建，则初始化
    if (shmexist == 0) {
        for (i = 0; i < N; ++i) strcpy(share->buffer[i], "");
        share->put = share->take = 0;
        setSemVal(semid, 0, 1);  // 互斥信号量 mutex
        setSemVal(semid, 1, 0);  // 信号量 full
        setSemVal(semid, 2, N);  // 信号量 empty
    }
    // 获得信号量的值
    full = getSemVal(semid, 1);
    empty = getSemVal(semid, 2);

    // 一系列输出
    printf("This is %d pid producer，\n\n", getpid());
    printf("full = %d, empty = %d, put = %d, take = %d.\n\n", full, empty,
           share->put, share->take);
    for (i = 0; i < N; ++i)
        if (i == share->put)
            printf("%d --- %s \t <- put\n", i, share->buffer[i]);
        else
            printf("%d --- %s\n", i, share->buffer[i]);
    printf("\n");
    printf("1 : produce a product.\n");
    printf("2 : exit.\n");
    printf("3 : delete semaphores and sharedMemory.\n");
    printf("Enter your choice:\n");
    // 重新获得信号量的值
    full = getSemVal(semid, 1);
    empty = getSemVal(semid, 2);
    // 检测共享内存和信号量集是否存在，若不存在，程序退出
    if (semid == -1 || full == -1 && empty == -1) {
        printf("semaphores and sharedMemory don't exist.\n");
        printf("%d pid consumer ended.\n", getpid());
        exit(EXIT_FAILURE);
    }
    scanf("%d", &choice);
    switch (choice) {
        // 选择 1：生产产品，produce a product
        case 1:
            // 输出，尝试生产中
            printf("Input your message:\n");
            scanf("%s", message);
            printf("processing...\n");

            P(semid, 2);  // wait (empty);
            P(semid, 0);  // wait (mutex);

            // 生产成功
            strcpy(share->buffer[share->put], message);
            share->put = (share->put + 1) % N;  // 移动指针
            printf("produce successfully\n\n");

            V(semid, 0);  // signal (mutex);
            V(semid, 1);  // signal (full);
            break;
        // 选择 2：退出，exit
        case 2:
            printf("exit...\n");
            ifbreak = 1;  // 可以退出循环
            shmdt(shm);
            break;
        // 选择 3：删除信号量集和共享内存并退出，delete semaphores and
        // sharedMemory
        case 3:
            // 断开共享内存的连接
            shmdt(shm);

            // 删除共享内存和信号量集
            semctl(semid, IPC_RMID, 0);
            shmctl(shmid, IPC_RMID, 0);

            ifbreak = 1;  // 可以退出循环
            printf("delete semaphores and sharedMemory done!\n");
            break;
            // 错误选择
        default:
            printf("input invalid, try again!\n");
            break;
    }
    printf("%d pid producer ended.\n", getpid());

    return 0;
}
