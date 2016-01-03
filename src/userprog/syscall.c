#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler(struct intr_frame *);

void syscall_init(void) {
	intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void syscall_handler(struct intr_frame *f) {
	int syscallNr = ((int*) f->esp)[0];

	switch (syscallNr) {
	case SYS_HALT:
		shutdown_power_off();
		break;

	case SYS_EXIT:
		thread_exit();
		break;

	case SYS_EXEC:
		char * file_name = (char *) *(f->esp)++;
		process_execute(file_name);
		break;

	case SYS_WAIT:
		process_exit();
		break;

	case SYS_CREATE:
		f->esp++;
		char * file_name = (char *) *(f->esp)++;
		filesys_create(file_name, 0);
		break;
	}
	thread_exit();
}
