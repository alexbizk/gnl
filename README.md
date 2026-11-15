*This project has been created as part of the 42 curriculum by weitwang.*

# get_next_line

## Description
`get_next_line` is a C function that reads a file one line at a time.
Each call returns the next line, including \n if present.
It returns NULL when the file ends or an error occurs.
It works with files and standard input.

## Instructions
Compile together with your own `main.c`:

    cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c

`BUFFER_SIZE` sets how many bytes `read()` reads at a time.
If it is not given, it defaults to 42 (defined in `get_next_line.h`).

Each line returned by  `get_next_line() ` uses  `malloc() `. Remember to  `free() ` it after use.

## Algorithm
get_next_line uses a static variable called stash to save unread text between calls.

Each call follows three steps:

1. Read: `read()` data into `buf`, add `'\0'`, and join it to `stash` until a `'\n'` is found or the file ends.
2. Extract: Get the first line from `stash`, including `'\n'` if there is one.
3. Update: Remove the returned line from `stash` and keep the remaining text for the next call.

The function returns `NULL` when there is no more data or an error occurs.

Why this approach:
* Reading: Read data until a full line is found, instead of reading the whole file.
* BUFFER_SIZE: Works with different buffer sizes. Extra text is saved in stash for the next call.
* Functions: Split the code into small functions to make it easier to read and follow Norminette.
* Memory: Use malloc() to allocate memory and free() to avoid memory leaks.

## Resources
- `man 2 read`, `man 3 malloc`
- https://www.youtube.com/watch?v=-Mt2FdJjVno

**AI usage:** 
AI was used as a learning assistant to explain C concepts, help understand compiler errors, and review the implementation.
I wrote the code myself.