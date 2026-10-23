# Project 3a: Virtual Memory

## Preliminaries

>Fill in your name and email address.

FirstName LastName <email@domain.example>

>If you have any preliminary comments on your submission, notes for the TAs, please give them here.



>Please cite any offline or online sources you consulted while preparing your submission, other than the Pintos documentation, course text, lecture notes, and course staff.



## Page Table Management

#### DATA STRUCTURES

>A1: Copy here the declaration of each new or changed struct or struct member, global or static variable, typedef, or enumeration.  Identify the purpose of each in 25 words or less.



#### ALGORITHMS

>A2: In a few paragraphs, describe your code for accessing the data
>stored in the SPT about a given page.



>A3: How does your code coordinate accessed and dirty bits between
>kernel and user virtual addresses that alias a single frame, or
>alternatively how do you avoid the issue?



#### SYNCHRONIZATION

>A4: When two user processes both need a new frame at the same time,
>how are races avoided?



#### RATIONALE

>A5: Why did you choose the data structure(s) that you did for
>representing virtual-to-physical mappings?



## Paging To And From Disk

#### DATA STRUCTURES

>B1: Copy here the declaration of each new or changed struct or struct member, global or static variable, typedef, or enumeration.  Identify the purpose of each in 25 words or less.



#### ALGORITHMS

>B2: When a frame is required but none is free, some frame must be
>evicted.  Describe your code for choosing a frame to evict.



>B3: When a process P obtains a frame that was previously used by a
>process Q, how do you adjust the page table (and any other data
>structures) to reflect the frame Q no longer has?



#### SYNCHRONIZATION

>B5: Explain the basics of your VM synchronization design.  In
>particular, explain how it prevents deadlock.  (Refer to the
>textbook for an explanation of the necessary conditions for
>deadlock.)



>B6: A page fault in process P can cause another process Q's frame
>to be evicted.  How do you ensure that Q cannot access or modify
>the page during the eviction process?  How do you avoid a race
>between P evicting Q's frame and Q faulting the page back in?



>B7: Suppose a page fault in process P causes a page to be read from
>the file system or swap.  How do you ensure that a second process Q
>cannot interfere by e.g. attempting to evict the frame while it is
>still being read in?



>B8: Explain how you handle access to paged-out pages that occur
>during system calls.  Do you use page faults to bring in pages (as
>in user programs), or do you have a mechanism for "locking" frames
>into physical memory, or do you use some other design?  How do you
>gracefully handle attempted accesses to invalid virtual addresses?



#### RATIONALE

>B9: A single lock for the whole VM system would make
>synchronization easy, but limit parallelism.  On the other hand,
>using many locks complicates synchronization and raises the
>possibility for deadlock but allows for high parallelism.  Explain
>where your design falls along this continuum and why you chose to
>design it this way.





warning: test tests/userprog/sc-boundary-3 doesn't count for grading
warning: test tests/userprog/exec-bound-3 doesn't count for grading
warning: test tests/userprog/exec-bound-2 doesn't count for grading
warning: test tests/userprog/exec-bound doesn't count for grading
TOTAL TESTING SCORE: 100.0%
ALL TESTED PASSED -- PERFECT SCORE

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

SUMMARY BY TEST SET

Test Set                                      Pts Max  % Ttl  % Max
--------------------------------------------- --- --- ------ ------
tests/userprog/Rubric.functionality           108/108  35.0%/ 35.0%
tests/userprog/Rubric.robustness               88/ 88  25.0%/ 25.0%
tests/userprog/no-vm/Rubric                     1/  1  10.0%/ 10.0%
tests/filesys/base/Rubric                      30/ 30  30.0%/ 30.0%
--------------------------------------------- --- --- ------ ------
Total                                                 100.0%/100.0%

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

SUMMARY OF INDIVIDUAL TESTS

Functionality of system calls (tests/userprog/Rubric.functionality):
	- Test argument passing on Pintos command line.
	     3/ 3 tests/userprog/args-none
	     3/ 3 tests/userprog/args-single
	     3/ 3 tests/userprog/args-multiple
	     3/ 3 tests/userprog/args-many
	     3/ 3 tests/userprog/args-dbl-space

	- Test "create" system call.
	     3/ 3 tests/userprog/create-empty
	     3/ 3 tests/userprog/create-long
	     3/ 3 tests/userprog/create-normal
	     3/ 3 tests/userprog/create-exists

	- Test "open" system call.
	     3/ 3 tests/userprog/open-missing
	     3/ 3 tests/userprog/open-normal
	     3/ 3 tests/userprog/open-twice

	- Test "read" system call.
	     3/ 3 tests/userprog/read-normal
	     3/ 3 tests/userprog/read-zero

	- Test "write" system call.
	     3/ 3 tests/userprog/write-normal
	     3/ 3 tests/userprog/write-zero

	- Test "close" system call.
	     3/ 3 tests/userprog/close-normal

	- Test "exec" system call.
	     5/ 5 tests/userprog/exec-once
	     5/ 5 tests/userprog/exec-multiple
	     5/ 5 tests/userprog/exec-arg

	- Test "wait" system call.
	     5/ 5 tests/userprog/wait-simple
	     5/ 5 tests/userprog/wait-twice

	- Test "exit" system call.
	     5/ 5 tests/userprog/exit

	- Test "halt" system call.
	     3/ 3 tests/userprog/halt

	- Test recursive execution of user programs.
	    15/15 tests/userprog/multi-recurse

	- Test read-only executable feature.
	     3/ 3 tests/userprog/rox-simple
	     3/ 3 tests/userprog/rox-child
	     3/ 3 tests/userprog/rox-multichild

	- Section summary.
	     28/ 28 tests passed
	    108/108 points subtotal

Robustness of system calls (tests/userprog/Rubric.robustness):
	- Test robustness of file descriptor handling.
	     2/ 2 tests/userprog/close-stdin
	     2/ 2 tests/userprog/close-stdout
	     2/ 2 tests/userprog/close-bad-fd
	     2/ 2 tests/userprog/close-twice
	     2/ 2 tests/userprog/read-bad-fd
	     2/ 2 tests/userprog/read-stdout
	     2/ 2 tests/userprog/write-bad-fd
	     2/ 2 tests/userprog/write-stdin
	     2/ 2 tests/userprog/multi-child-fd

	- Test robustness of pointer handling.
	     3/ 3 tests/userprog/create-bad-ptr
	     3/ 3 tests/userprog/exec-bad-ptr
	     3/ 3 tests/userprog/open-bad-ptr
	     3/ 3 tests/userprog/read-bad-ptr
	     3/ 3 tests/userprog/write-bad-ptr

	- Test robustness of buffer copying across page boundaries.
	     3/ 3 tests/userprog/create-bound
	     3/ 3 tests/userprog/open-boundary
	     3/ 3 tests/userprog/read-boundary
	     3/ 3 tests/userprog/write-boundary

	- Test handling of null pointer and empty strings.
	     2/ 2 tests/userprog/create-null
	     2/ 2 tests/userprog/open-null
	     2/ 2 tests/userprog/open-empty

	- Test robustness of system call implementation.
	     3/ 3 tests/userprog/sc-bad-arg
	     3/ 3 tests/userprog/sc-bad-sp
	     5/ 5 tests/userprog/sc-boundary
	     5/ 5 tests/userprog/sc-boundary-2

	- Test robustness of "exec" and "wait" system calls.
	     5/ 5 tests/userprog/exec-missing
	     5/ 5 tests/userprog/wait-bad-pid
	     5/ 5 tests/userprog/wait-killed

	- Test robustness of exception handling.
	     1/ 1 tests/userprog/bad-read
	     1/ 1 tests/userprog/bad-write
	     1/ 1 tests/userprog/bad-jump
	     1/ 1 tests/userprog/bad-read2
	     1/ 1 tests/userprog/bad-write2
	     1/ 1 tests/userprog/bad-jump2

	- Section summary.
	     34/ 34 tests passed
	     88/ 88 points subtotal

Functionality of features that VM might break (tests/userprog/no-vm/Rubric):

	     1/ 1 tests/userprog/no-vm/multi-oom

	- Section summary.
	      1/  1 tests passed
	      1/  1 points subtotal

Functionality of base file system (tests/filesys/base/Rubric):
	- Test basic support for small files.
	     1/ 1 tests/filesys/base/sm-create
	     2/ 2 tests/filesys/base/sm-full
	     2/ 2 tests/filesys/base/sm-random
	     2/ 2 tests/filesys/base/sm-seq-block
	     3/ 3 tests/filesys/base/sm-seq-random

	- Test basic support for large files.
	     1/ 1 tests/filesys/base/lg-create
	     2/ 2 tests/filesys/base/lg-full
	     2/ 2 tests/filesys/base/lg-random
	     2/ 2 tests/filesys/base/lg-seq-block
	     3/ 3 tests/filesys/base/lg-seq-random

	- Test synchronized multiprogram access to files.
	     4/ 4 tests/filesys/base/syn-read
	     4/ 4 tests/filesys/base/syn-write
	     2/ 2 tests/filesys/base/syn-remove

	- Section summary.
	     13/ 13 tests passed
	     30/ 30 points subtotal

