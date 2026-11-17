*This project has been created as part of the 42 curriculum by weitwang.*

# get_next_line

## Description
`get_next_line` is a C function that reads a file one line at a time. Each call returns the next line, including `\n` if present.
It returns NULL when the file ends or an error occurs.
It works with files and standard input.

Prototype:

```c
char	*get_next_line(int fd);
```

Files:
- `get_next_line.c` – the main function and its helpers
- `get_next_line_utils.c` – string helpers (`ft_strlen`, `ft_strchr`, `ft_memcpy`, `ft_strjoin`, `ft_substr`)
- `get_next_line.h` – prototypes and the default `BUFFER_SIZE`

## Instructions
Compile together with your own `main.c`:

```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```

`BUFFER_SIZE` sets how many bytes `read()` reads at a time.
If it is not given, it defaults to 42 (defined in `get_next_line.h`).
If it is 0 or negative, `get_next_line` returns `NULL`.
Each line returned by  `get_next_line()` uses  `malloc()`. 
Remember to  `free() ` it after use.

## Testing
Example `main.c`. 
It reads the file given as argument, or standard input if no argument is given, and prints every line between `[` and `]` so the `\n` is easy to see:

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc > 1)
		fd = open(argv[1], O_RDONLY);
	else
		fd = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		printf("[%s]", line);
		free(line);
		line = get_next_line(fd);
	}
	if (fd > 0)
		close(fd);
	return (0);
}
```

testing files:
```sh
printf ""                > empty.txt
printf "hi\nworld"       > no_nl_end.txt
printf "hi\nworld\n"     > nl_end.txt
printf "a\n\nb\n"        > empty_line.txt
printf "\n"              > only_nl.txt
```

Run with a test file:

```bash
./a.out test.txt
```

Check for memory leaks (macOS):

```sh
leaks -atExit -- ./a.out nl_end.txt     # last line: 0 leaks
```

On Linux:

```sh
valgrind --leak-check=full ./a.out nl_end.txt
```

## Algorithm

`read()` is declared in `<unistd.h>`:
```c
ssize_t	read(int fd, void *buf, size_t count);
```

It reads up to `count` bytes from `fd` into `buf` and returns:
- a positive number: how many bytes were read (can be less than `count`)
- `0`: end of file
- `-1`: an error

`read()` does not add a `'\0'` at the end, so `get_next_line` needs to add it (`buf[bytes] = '\0'`) itself.
The return value is stored in a `ssize_t` to hold `-1`.

`read()` does not know about lines. 
So it often reads over the end of the current line, and bytes that were already read cannot be read again.

Thus the extra text must be kept between calls.
It is kept in 2 static variables starting as `NULL`.
They keep their values after the function returns, but only `get_next_line` can use them. 
- `start_tmp_2_free` – the start of the saved text (the stash). It always
  points to the beginning of the `malloc`ed memory, so it can be freed.
- `cur` – where the next line starts inside the stash.

Each call follows three steps:

1. **Read** 
  (`read_to_stash`): if there is already a `'\n'` after `cur`, nothing needs to be read. 
  Otherwise, the unused text after `cur` is copied into a new stash (`trim_stash`), then `read()` data into `buf`, add `'\0'`, and join it to the stash (`join_stash`) until a `'\n'` is found or the file ends.
2. **Extract** 
  (`extract_line`): copy the line starting at `cur`, including the `'\n'` if there is one.
3. **Move**:
  move `cur` forward by the length of that line. When `cur` reaches the end of the stash, the stash is freed and both pointers become `NULL`.

The function returns `NULL` when there is no more data or an error occurs.

Example with a large `BUFFER_SIZE` and the file `a\nb\nc\n`
(when whole read at once):

| Call | `cur` points to | Returns | `cur` moves to |
|---|---|---|---|
| 1 | `a` | `"a\n"` | `b` |
| 2 | `b` | `"b\n"` | `c` |
| 3 | `c` | `"c\n"` | end → stash freed |
| 4 | – | `NULL` | – |

Why this approach:
* **Reading:** 
  it only reads until a full line is found, instead of reading the whole file at once.
* **Speed:** 
  the saved text is not copied again for every line. 
  Only `cur` moves forward, so each line is copied once. 
  The stash is trimmed only before reading new data, and then only an unfinished line is left.
  This keeps it fast even with a very large `BUFFER_SIZE`, where one `read()` can load the whole file.
* **BUFFER_SIZE:**
  it works with any size.
  `buf` is allocated with `malloc()` on the heap, so a very large size does not overflow the stack.
* **Functions:**
  the work is split into small functions, which makes the code easier to read and keeps it within the Norm limits.
* **Memory:**
  the old stash is freed whenever a new one is made. 
  On end of file or on any `read()`/`malloc()` error, the stash is freed and both pointers are set to `NULL`, so nothing leaks.

## Resources
- `man 2 read`, `man 3 malloc`
- Help and discussions with peers
- https://www.youtube.com/watch?v=-Mt2FdJjVno

**AI usage:** 
AI was used as a learning assistant to explain C concepts, help understand compiler errors, review the implementation, and suggest the performance improvement (keeping a `cur` pointer instead of copying the stash for every line). I wrote the code myself.
