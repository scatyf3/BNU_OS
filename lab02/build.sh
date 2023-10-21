ipcs -q | awk '{print $2}' | xargs ipcrm -q
gcc msg_queue.c
./a.out