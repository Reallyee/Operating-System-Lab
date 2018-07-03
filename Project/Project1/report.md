#  Project1 Report



##  Task 1

#### Main idea:

- Block the thread by calling `timer_sleep()`.

- Add a new member variable `ticks_blocked` to struct thread, which records the number of ticks the thread needs to sleep. Decrease the `ticks_blocked` in every tick until when it becomes 0, unblock the thread.

#### Adding variables:

in struct thread (thread.h):

- `int64_t ticks_blocked`: Number of ticks a blocked thread still has to sleep.

#### Adding function:

in thread.h:

	void check_for_unblock (struct thread *t, void *aux UNUSED);

in thread.c:

	void 
	check_for_unblock (struct thread *t, void *aux UNUSED){
		if (t->status == THREAD_BLOCKED && t->ticks_blocked > 0){
			--(t->ticks_blocked);
			if(!t->ticks_blocked) thread_unblock(t);
		}
	}

#### Modified functions:

in timer.c:

	void
	timer_sleep (int64_t ticks) 
	{
		if(ticks <= 0) return;
		ASSERT(intr_get_level() == INTR_ON);
		enum intr_level pre_level = intr_disable();
		struct thread *current_thread = thread_current();
		current_thread -> ticks_blocked = ticks;
		thread_block();
		intr_set_level(pre_level);   
	}
	
	static void
	timer_interrupt (struct intr_frame *args UNUSED){
		ticks++;
		thread_tick ();
		thread_foreach (check_for_unblock, NULL);
	
	}

​	
​	
in thread.c:

	static void
	init_thread (struct thread *t, const char *name, int priority){
		...
		t->ticks_blocked = 0;
		...
	}

#### Result:

Result for test point alarm-single:

![larm-singl](C:\Users\THINKPAD\Desktop\OS_project1_毛玉莲_11510086_周宸宇_11510374\report\alarm-single.png)

Result for test point alarm-multiple:

![larm-multipl](C:\Users\THINKPAD\Desktop\OS_project1_毛玉莲_11510086_周宸宇_11510374\report\alarm-multiple.png)

## Task 2

#### Main idea:

- Change two global variables in thread.c to member variables of struct thread, `time_slice` and `thread_ticks`, that respectively record total number of ticks a thread can run every time, and number of ticks it has run since last yield.

- `time_slice` won't be changed once a thread has been initialized.

- Set `thread_ticks` 0 and let it auto-increment by 1 at each tick. Reduce thread's priority by 3 when `thread_ticks` reaches to `time_slice`.

#### Adding variables:

in struct thread (thread.h):

- `int64_t time_slice`: Total number of ticks a thread can run every time.

- `int64_t thread_ticks`: Number of ticks a thread has run since last yield.

p.s. Also need to dispose all references to `TIME_SLICE` and `static unsigned thread_ticks` defined in thread.c of the source code.

#### Modified functions:

	static void
	init_thread (struct thread *t, const char *name, int priority){
		...
		t->time_slice = priority % 7 + 2;
		t->thread_ticks = 0;
		...
	}
	
	void
	thread_tick (void) {
		...
		if (++t->thread_ticks >= t->time_slice){
			if(t->priority >= 3)
				thread_set_priority(t->priority - 3);
			else
				thread_set_priority(0);
			intr_yield_on_return ();
		}
	}
	
	void
	thread_schedule_tail (struct thread *prev) {
		...
		cur->thread_ticks = 0;
		...
	}

## Task 3

#### Main idea:

- Implement two priority queues: semaphore wait list and condition waiters list.

- Maintain the ready list as a priority queue when adding new thread into it.

- Rearrange orders of all threads once any thread's priority changes.

- When a thread aquire a lock, improve priority of the thread holding the lock if its priority is lower. Recursive donation will be necessary if the lock is locked by some other locks. After the lock being released, the priority should be restored to previous value.

- If more than one threads donate priority to one thread, keep the maximum one.

- When setting the priority of a donated thread, set its `priority` if the new value is larger than the current value. Restore `original_priority` when the donation terminates.

- If releasing a lock will influence its priority, other donated priorities and current priority should be considered.

#### Adding variables:

in struct thread (thread.h):

- `int original_priority`: The base priority of a thread, only can be set in `init_thread()` and `thread_set_priority()`.

- `struct list thread_hold_locks`: Record all the locks that the thread holds.

- `struct lock *thread_waiting_lock`: Record the lock which the thread is waiting for.

in struct lock (synch.h):

- `struct list_elem thread_to_donate`: Record the threads waiting for the lock and has to donate.

- `int max_priority`: Max priority among the threads aquiring the lock.

#### Adding functions:

**1. Implement a thread priority comparation function used for the priority queue.**

in thread.h:

	bool thread_compare(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED);

in thread.c:

	bool
	thread_compare(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED){
		return list_entry(a, struct thread, elem)->priority > list_entry(b,struct thread,elem)->priority;
	}

**2. Implement condition priority comparation functions used for the priority queues.**

in synch.h:

	bool cond_compare_priority(const struct list_elem *, const struct list_elem *, void *aux);
	bool lock_compare(const struct list_elem *, const struct list_elem *, void *aux );

in synch.c:

	bool cond_compare_priority(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED)
	{
		struct semaphore_elem *sema_a = list_entry(a, struct semaphore_elem, elem);
		struct semaphore_elem *sema_b = list_entry(b, struct semaphore_elem, elem);
		return list_entry(list_pop_front(&sema_a->semaphore.waiters), struct thread, elem)->priority>list_entry(list_pop_front(&sema_b->semaphore.waiters), struct thread, elem)->priority;
	}
	
	bool lock_compare(const struct list_elem *a, const struct list_elem *b, void *aux UNUSED)
	{
		return list_entry(a, struct lock, thread_to_donate)->max_priority>list_entry(b, struct lock, thread_to_donate)->max_priority;
	}

#### Modified functions:

**1.  Change all references to function list_push_back() to list_insert_ordered(), to ensure that the ready list is ordered as a priority queue after adding a new thread.**

in thread.c:

	void
	thread_unblock (struct thread *t) {
		...
		// list_push_back (&ready_list, &t->elem);
		list_insert_ordered(&ready_list, &t->elem, (list_less_func *) &thread_compare, NULL);
		...
	}
	
	static void
	init_thread (struct thread *t, const char *name, int priority){
		...
		// list_push_back (&all_list, &t->allelem);
		list_insert_ordered(&all_list, &t->allelem, (list_less_func*) &thread_compare, NULL);
		...
	}
	
	void
	thread_yield (void) {
		...
		// list_push_back (&ready_list, &cur->elem);
		list_insert_ordered(&ready_list, &cur->elem, (list_less_func *) &thread_compare, NULL);
		...
	}

**2. Add calling to thread_yield() to rearrange the list after every time any of threads change priority.**

in thread.c:

	void
	thread_set_priority (int new_priority) 
	{
		thread_current ()->priority = new_priority;
		thread_yield();
	}

​	
	tid_t
	thread_create (const char *name, int priority, thread_func *function, void *aux) {
		...
		thread_unblock (t);
		if(thread_current()->priority < priority)
			thread_yield();
		...
	}

**3. Implement two priority queues: condition queue and wait list of semaphore.**

in synch.c:

	void cond_signal (struct condition *cond, struct lock *lock UNUSED) {
		...
		if (!list_empty (&cond->waiters))
	    {
	
		list_sort(&cond->waiters, cond_compare_priority, NULL);
		sema_up (&list_entry (list_pop_front (&cond->waiters), 					struct semaphore_elem, elem)->semaphore);
			}
		}

​	
	

​	

```
void

	sema_up (struct semaphore *sema) 

	{

		...

		if (!list_empty (&sema->waiters)) 

		{

			list_sort(&sema->waiters, thread_compare, NULL);

			thread_unblock (list_entry (list_pop_front (&sema->waiters), struct thread, elem));

		}

		sema->value++;

		thread_yield();

		intr_set_level (old_level);

	}

```

​	

	void
	sema_down (struct semaphore *sema) {
		...
		while (sema->value == 0) 
		{
			list_insert_ordered (&sema->waiters, &thread_current ()->elem, thread_compare, NULL);
			thread_block ();
		}
		...
	}

**4. Modify the lock acquire function. If the lock already has a holder and the acquirer's priority is larger, donate the larger priority to the holder. And if the holder is waiting for another lock, recursively donation should be done to other holder(s). Every lock should record the maximum priority of its acquirers.**

in synch.c:

	void
	lock_acquire (struct lock *lock)
	{
		ASSERT (lock != NULL);
		ASSERT (!intr_context ());
		ASSERT (!lock_held_by_current_thread (lock));
		
		struct thread *t = thread_current ();
		if (lock->holder != NULL)
		{
			t->thread_waiting_lock = lock;
			struct lock *pointer= lock;
			/*donate the priority recursively*/
			while (pointer!=NULL && t->priority > pointer->max_priority)
			{
				pointer->max_priority = t->priority;
				if (!list_empty (&pointer->holder->thread_hold_locks))
				{
					list_sort (&pointer->holder->thread_hold_locks, lock_compare, NULL);
					int lock_priority=list_entry (list_front (&pointer->holder->thread_hold_locks), struct lock, thread_to_donate)->max_priority;
					if ( lock_priority> pointer->holder->original_priority)
						pointer->holder->priority = lock_priority;
				}
				/*donate the priority may change the ready list*/
				thread_yield();
				pointer = pointer->holder->thread_waiting_lock;
				}
		}
	
		sema_down (&lock->semaphore);
	
		t= thread_current ();
		lock->holder = t;
		t->thread_waiting_lock = NULL;
		lock->max_priority = t->priority;
		/*build a priority queue*/
		list_insert_ordered (&thread_current ()->thread_hold_locks, &lock->thread_to_donate, lock_compare, NULL);
	}

**5. Modify the lock release function. If the holder has been donated, recursively recover all the priority.**

in synch.c:
	
	void
	lock_release (struct lock *lock) 
	{
		enum intr_level old_level= intr_disable ();
	
		ASSERT (lock != NULL);
		ASSERT (lock_held_by_current_thread (lock));
		struct thread *t=thread_current();
		/*remove the lock from thread_hold_locks*/
		list_remove (&lock->thread_to_donate);
		list_sort (&t->thread_hold_locks, lock_compare, NULL);
	
		if (!list_empty (&t->thread_hold_locks))
		{
			int lock_priority=list_entry (list_front (&t->thread_hold_locks), struct lock, thread_to_donate)->max_priority;
			if ( lock_priority> t->original_priority)
				t->priority = lock_priority;
		}
		else
		{
			t->priority=t->original_priority;
		}
	  
		lock->holder = NULL;
		
		sema_up (&lock->semaphore);
	
		intr_set_level (old_level);
	}

**6. When thread priority is changed, change its original_priority. If the new priority is larger than its current priority, also change its current priority.**

in thread.c:

	void
	thread_set_priority (int new_priority) 
	{
		enum intr_level old_level=intr_disable();
		struct thread *t=thread_current();
		t->original_priority=new_priority;
		if(list_empty(&t->thread_hold_locks) || new_priority>t->priority)
			t->priority=new_priority;
		thread_yield();
		intr_set_level(old_level);
	}

#### Result:

Result for test make check:

![018-05-02 18-45-24屏幕截](C:\Users\THINKPAD\Desktop\OS_project1_毛玉莲_11510086_周宸宇_11510374\report\2018-05-02 18-45-24屏幕截图.png)

Result for test point alarm-priority:

![](C:\Users\THINKPAD\Desktop\OS_project1_毛玉莲_11510086_周宸宇_11510374\report\tu.png)



