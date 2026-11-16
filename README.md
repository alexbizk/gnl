*This project has been created as part of the 42 curriculum by weitwang.*

# get_next_line

## Description
`get_next_line` is a C function that reads a file one line at a time. Each call returns the next line, including \n if present.
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

Each line returned by  `get_next_line() ` uses  `malloc() `. 
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

`read()` does not know about lines. It often reads past the end of the current line, and bytes that were already read cannot be read again.

So the extra text must be kept between calls. 

It is stored in a static variable called `stash`: it keeps its value after the function returns, but only `get_next_line` can use it 
(lit. get_next_line uses a static variable called stash to save unread text between calls).

The static variable starts as `NULL`, which means "nothing saved yet".

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
- Help and discussions with peers
- https://www.youtube.com/watch?v=-Mt2FdJjVno

**AI usage:** 
AI was used as a learning assistant to explain C concepts, help understand compiler errors, and review the implementation.
I wrote the code myself.
