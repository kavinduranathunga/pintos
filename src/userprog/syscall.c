#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "userprog/process.h"
#include "threads/init.h"
#include "threads/synch.h"
#include "filesys/filesys.h"
#include "filesys/file.h"
#include "devices/shutdown.h"
#include "devices/input.h"

struct lock filesys_lock;

static void syscall_handler (struct intr_frame *);

static int
get_user (const uint8_t *uaddr)
{
  int result;
  asm ("movl $1f, %0; movzbl %1, %0; 1:"
       : "=&a" (result) : "m" (*uaddr));
  return result;
}

/* Added UNUSED to prevent compiler warning when put_user isn't called directly */
static bool UNUSED
put_user (uint8_t *udst, uint8_t byte)
{
  int error_code;
  asm ("movl $1f, %0; movb %b2, %1; 1:"
       : "=&a" (error_code), "=m" (*udst) : "q" (byte));
  return error_code != -1;
}

static void
check_ptr (const void *ptr)
{
  if (ptr == NULL || !is_user_vaddr (ptr) || get_user ((const uint8_t *) ptr) == -1)
    {
      thread_current ()->exit_status = -1;
      thread_exit ();
    }
}

static void
check_buffer (const void *buffer, unsigned size)
{
  const char *ptr = (const char *) buffer;
  for (unsigned i = 0; i < size; i++)
    {
      check_ptr (ptr + i);
    }
}

static void
check_string (const char *str)
{
  check_ptr (str);
  while (get_user ((const uint8_t *) str) != '\0')
    {
      str++;
      check_ptr (str);
    }
}

static int
read_user_int (const int *uaddr)
{
  const uint8_t *byte_ptr = (const uint8_t *) uaddr;
  check_buffer (byte_ptr, 4);

  int value = 0;
  for (int i = 0; i < 4; i++)
    {
      int byte = get_user (byte_ptr + i);
      if (byte == -1) 
        {
          thread_current ()->exit_status = -1;
          thread_exit ();
        }
      value |= ((unsigned int) byte & 0xff) << (i * 8);
    }
  return value;
}

void
syscall_init (void) 
{
  lock_init (&filesys_lock);
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

/* System Call Implementations */

void
halt (void)
{
  shutdown_power_off ();
}

void
exit (int status)
{
  struct thread *cur = thread_current ();
  cur->exit_status = status;
  thread_exit ();
}

pid_t
exec (const char *cmd_line)
{
  check_string (cmd_line);
  return process_execute (cmd_line);
}

int
wait (pid_t pid)
{
  return process_wait (pid);
}

bool
create (const char *file, unsigned initial_size)
{
  check_string (file);

  lock_acquire (&filesys_lock);
  bool success = filesys_create (file, initial_size);
  lock_release (&filesys_lock);

  return success;
}

bool
remove (const char *file)
{
  check_string (file);

  lock_acquire (&filesys_lock);
  bool success = filesys_remove (file);
  lock_release (&filesys_lock);

  return success;
}

int
open (const char *file)
{
  check_string (file);
  return -1;
}

int
filesize (int fd UNUSED)
{
  return -1;
}

int
read (int fd, void *buffer, unsigned size)
{
  check_buffer (buffer, size);

  if (fd == STDIN_FILENO) /* STDIN_FILENO = 0 */
    {
      uint8_t *buf = (uint8_t *) buffer;
      for (unsigned i = 0; i < size; i++)
        {
          buf[i] = input_getc ();
        }
      return size;
    }

  return -1;
}

int
write (int fd, const void *buffer, unsigned size)
{
  check_buffer (buffer, size);

  if (fd == STDOUT_FILENO) /* STDOUT_FILENO = 1 */
    {
      putbuf (buffer, size);
      return size;
    }

  return -1;
}

void
seek (int fd UNUSED, unsigned position UNUSED)
{
}

unsigned
tell (int fd UNUSED)
{
  return 0;
}

void
close (int fd UNUSED)
{
}

static void
syscall_handler (struct intr_frame *f) 
{
  int syscall_num = read_user_int ((int *) f->esp);

  switch (syscall_num)
    {
    case SYS_HALT:
      halt ();
      break;

    case SYS_EXIT:
      {
        int status = read_user_int ((int *) f->esp + 1);
        exit (status);
        break;
      }

    case SYS_EXEC:
      {
        const char *cmd_line = (const char *) read_user_int ((int *) f->esp + 1);
        f->eax = exec (cmd_line);
        break;
      }

    case SYS_WAIT:
      {
        pid_t pid = read_user_int ((int *) f->esp + 1);
        f->eax = wait (pid);
        break;
      }

    case SYS_CREATE:
      {
        const char *file = (const char *) read_user_int ((int *) f->esp + 1);
        unsigned initial_size = (unsigned) read_user_int ((int *) f->esp + 2);
        f->eax = create (file, initial_size);
        break;
      }

    case SYS_REMOVE:
      {
        const char *file = (const char *) read_user_int ((int *) f->esp + 1);
        f->eax = remove (file);
        break;
      }

    case SYS_OPEN:
      {
        const char *file = (const char *) read_user_int ((int *) f->esp + 1);
        f->eax = open (file);
        break;
      }

    case SYS_FILESIZE:
      {
        int fd = read_user_int ((int *) f->esp + 1);
        f->eax = filesize (fd);
        break;
      }

    case SYS_READ:
      {
        int fd = read_user_int ((int *) f->esp + 1);
        void *buffer = (void *) read_user_int ((int *) f->esp + 2);
        unsigned size = (unsigned) read_user_int ((int *) f->esp + 3);
        f->eax = read (fd, buffer, size);
        break;
      }

    case SYS_WRITE:
      {
        int fd = read_user_int ((int *) f->esp + 1);
        const void *buffer = (const void *) read_user_int ((int *) f->esp + 2);
        unsigned size = (unsigned) read_user_int ((int *) f->esp + 3);
        f->eax = write (fd, buffer, size);
        break;
      }

    case SYS_SEEK:
      {
        int fd = read_user_int ((int *) f->esp + 1);
        unsigned position = (unsigned) read_user_int ((int *) f->esp + 2);
        seek (fd, position);
        break;
      }

    case SYS_TELL:
      {
        int fd = read_user_int ((int *) f->esp + 1);
        f->eax = tell (fd);
        break;
      }

    case SYS_CLOSE:
      {
        int fd = read_user_int ((int *) f->esp + 1);
        close (fd);
        break;
      }

    default:
      exit (-1);
      break;
    }
}