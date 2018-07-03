#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "list.h"
#include "process.h"
#include <threads/synch.h>
static void syscall_handler (struct intr_frame *);
void* check_addr(const void*);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int *stack = f->esp;

	check_addr(stack);



  int system_call = *stack;
	

		if(system_call== SYS_HALT)
		shutdown_power_off();
		
		else if(system_call == SYS_EXIT)
		{
			check_addr(stack+1);
			exit(*(stack+1));
		}
		else if(system_call==SYS_EXEC)
		{
			check_addr(stack+1);
			check_addr(*(stack+1));
			// f->eax = exec_proc(*(stack+1));
			char *file_name=*(stack+1);
			lock_acquire_filesystem();
			char * fn_copy=malloc (strlen(file_name)+1);
			strlcpy(fn_copy, file_name, strlen(file_name)+1);
		
			char * save_ptr;
			fn_copy = strtok_r(fn_copy," ",&save_ptr);

			struct file* file = filesys_open (fn_copy);

			if(file==NULL)
			{
				lock_release_filesystem();
				f->eax=-1;
			}
			else
			{
				file_close(file);
				lock_release_filesystem();
				f->eax=process_execute(file_name);
			}
		}
		
		
		else if(system_call==SYS_WAIT)
		{
			check_addr(stack+1);
			f->eax = process_wait(*(stack+1));
		}
		

		else if(system_call==SYS_CREATE)
		{
			check_addr(stack+5);
			check_addr(*(stack+4));
			lock_acquire_filesystem();
			f->eax = filesys_create(*(stack+4),*(stack+5));
			lock_release_filesystem();
		}
		
		else if(system_call==SYS_REMOVE)
		{
			check_addr(stack+1);
			check_addr(*(stack+1));
			lock_acquire_filesystem();
			if(filesys_remove(*(stack+1))==NULL)
				f->eax = false;
			else
				f->eax = true;
			lock_release_filesystem();
		}
		
		else if(system_call== SYS_OPEN)
		{
			check_addr(stack+1);	
			check_addr(*(stack+1));
			lock_acquire_filesystem();
			struct file* fptr = filesys_open (*(stack+1));
			lock_release_filesystem();
			if(fptr==NULL)
				f->eax = -1;
			else
			{
				struct thread_open_file *pfile=malloc(sizeof(*pfile));
				pfile->file = fptr;
				pfile->fd = thread_current()->fd_count;
				thread_current()->fd_count++;
				list_push_back (&thread_current()->file_list, &pfile->elem);
				f->eax = pfile->fd;

			}
		}
		

		else if(system_call==SYS_FILESIZE)
		{
			check_addr(stack+1);
			lock_acquire_filesystem();
			f->eax = file_length (list_search(&thread_current()->file_list, *(stack+1))->file);
			lock_release_filesystem();
		}
		
		else if(system_call== SYS_READ)
		{
			check_addr(stack+7);
			check_addr(*(stack+6));
			if(*(stack+5)==0)
			{
				int i;
				uint8_t* buffer = *(stack+6);
				for(i=0;i<*(stack+7);i++)
					buffer[i] = input_getc();
				f->eax = *(stack+7);
			}
			else
			{
				struct thread_open_file* fptr = list_search(&thread_current()->file_list, *(stack+5));
				if(fptr==NULL)
					f->eax=-1;
				else
				{
					lock_acquire_filesystem();
					f->eax = file_read (fptr->file, *(stack+6), *(stack+7));
					lock_release_filesystem();
				}
			}
		}
		
		else if(system_call== SYS_WRITE)
		{
			check_addr(stack+7);
			check_addr(*(stack+6));
			if(*(stack+5)==1)
			{
				putbuf(*(stack+6),*(stack+7));
				f->eax = *(stack+7);
			}
			else
			{
				struct thread_open_file* fptr = list_search(&thread_current()->file_list, *(stack+5));
				if(fptr==NULL)
					f->eax=-1;
				else
				{
					lock_acquire_filesystem();
					f->eax = file_write (fptr->file, *(stack+6), *(stack+7));
					lock_release_filesystem();
				}
			}
		}
	
		else if(system_call== SYS_SEEK)
		{
			check_addr(stack+5);
			lock_acquire_filesystem();
			file_seek(list_search(&thread_current()->file_list, *(stack+4))->file,*(stack+5));
			lock_release_filesystem();
		}
		
		else if(system_call== SYS_TELL)
		{
			check_addr(stack+1);
			lock_acquire_filesystem();
			f->eax = file_tell(list_search(&thread_current()->file_list, *(stack+1))->file);
			lock_release_filesystem();
		}
		
		else if(system_call== SYS_CLOSE)
		{

			struct list_elem *e;
			struct thread_open_file *f;
			check_addr(stack+1);
			lock_acquire_filesystem();
			for (e = list_begin (&thread_current()->file_list); e != list_end (&thread_current()->file_list);
				e = list_next (e))
				{
				f = list_entry (e, struct thread_open_file, elem);
				if(f->fd == *(stack+1))
				{
					file_close(f->file);
					list_remove(e);
				}
				}

			lock_release_filesystem();
		}	
		else 
		{
			printf("Default %d\n",*stack);
		}
}



void exit(int status)
{
	struct list_elem *e;

      for (e = list_begin (&thread_current()->parent->child_thread); e != list_end (&thread_current()->parent->child_thread);
           e = list_next (e))
        {
          struct child *f = list_entry (e, struct child, elem);
          if(f->tid == thread_current()->tid)
          {
          	f->died = true;
          	f->exit_status = status;
          }
        }


	thread_current()->exit_status = status;

	if(thread_current()->parent->child_to_die == thread_current()->tid)
		sema_up(&thread_current()->parent->sema_value);

	thread_exit();
}

void* check_addr(const void *vaddr)
{
	if (!is_user_vaddr(vaddr))
	{
		exit(-1);
		return 0;
	}
	void *ptr = pagedir_get_page(thread_current()->pagedir, vaddr);
	if (!ptr)
	{
		exit(-1);
		return 0;
	}
	return ptr;
}

struct thread_open_file* list_search(struct list* files, int fd)
{

	struct list_elem *e;

      for (e = list_begin (files); e != list_end (files);
           e = list_next (e))
        {
          struct thread_open_file *f = list_entry (e, struct thread_open_file, elem);
          if(f->fd == fd)
          	return f;
        }
   return NULL;
}

