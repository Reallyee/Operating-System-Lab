
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "list.h"
#include "process.h"
#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);
struct thread_open_file* list_search(struct list* files, int fd);
struct thread_open_file {
	struct file* file;
	int fd;
	struct list_elem elem;
};

#endif /* userprog/syscall.h */
