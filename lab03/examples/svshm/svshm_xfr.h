#include <sys/types.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include "binary sems.h"
#include "tlpi_hdr.h"
#define SHM_KEY 0x1234
#define SEM_KEY 0x5678

#define OBJ_PERMS (S| IRUSRS|IWUSRS|IRGRPS|IWGRP)
#define WRITE SEM O
#define READ SEM 1
#ifndef BUF SIZE
#define BUF SIZE 1024
#endif
struct shmseg {int cnt;char buf[BUF SIZE];};