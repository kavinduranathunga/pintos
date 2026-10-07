#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
//
#include "threads/vaddr.h"
#include "userprog/process.h"

static void syscall_handler (struct intr_frame *);

static int
get_user (const uint8_t *uaddr)
{
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}

static bool
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

static int
read_user_int (const int *uaddr)
{
  if (!is_user_vaddr (uaddr) || !is_user_vaddr ((const uint8_t *) uaddr + 3))
    {
      thread_current ()->exit_status = -1;
      thread_exit ();
    }

  int value = 0;
  for (int i = 0; i < 4; i++)
    {
      int byte = get_user ((const uint8_t *) uaddr + i);
      if (byte == -1) 
        {
          thread_current ()->exit_status = -1;
          thread_exit ();
        }
      value |= (byte & 0xff) << (i * 8);
    }
  return value;
}

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  
  int syscall_num = read_user_int ((int *) f->esp);

  switch (syscall_num)
    {
    case SYS_HALT:
    
      break;

    case SYS_EXIT:
      {
        int status = read_user_int ((int *) f->esp + 1);
        thread_current ()->exit_status = status;
        thread_exit ();
        break;
      }

    default:
      thread_current ()->exit_status = -1;
      thread_exit ();
      break;
    }
}
