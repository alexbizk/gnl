/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weitwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 15:45:20 by weitwang          #+#    #+#             */
/*   Updated: 2026/11/18 21:51:24 by weitwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join_stash(char *stash, char *buf)
{
	char	*tmp;

	if (stash == NULL)
		return (ft_substr(buf, 0, ft_strlen(buf)));
	tmp = stash;//两个指针指向同一块内存,用于释放旧的stash
	stash = ft_strjoin(stash, buf);
	free(tmp);
	return (stash);
}

static int	trim_stash(char **stash, char *cur)
{
	char	*rest;

	if (*stash == NULL || cur == *stash)
		return (1);
	rest = ft_substr(cur, 0, ft_strlen(cur));
	free(*stash);//free 只能拿 malloc 返回的开头地址。
	*stash = rest;//把调用者手里的那个 stash 指针，改成指向新口袋。//rest 是新口袋，接下来还要继续用；*stash 是旧口袋，已经没用了。free 只能用在不再需要的内存上。
	return (rest != NULL);
/*
	if (rest != NULL)
		return (1);
	return (0);
*/
}

static char	*read_to_stash(int fd, char *stash, char **cur)
{
	char	*buf;
	ssize_t	bytes;

	if (stash && ft_strchr(*cur, '\n'))
		return (stash);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf || !trim_stash(&stash, *cur))
		return (free(buf), free(stash), NULL);
	bytes = 1;
	while (bytes > 0 && (stash == NULL || !ft_strchr(stash, '\n')))
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes == -1)
			return (free(buf), free(stash), NULL);
		if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		stash = join_stash(stash, buf);
		if (stash == NULL)
			return (free(buf), NULL);
	}
	*cur = stash;
	return (free(buf), stash);
}

static char	*extract_line(char *cur)
{
	char	*nl;
	char	*line;
	size_t	len;

	nl = ft_strchr(cur, '\n');
	if (nl == NULL)
		len = ft_strlen(cur);
	else
		len = (size_t)(nl - cur) + 1;
	line = malloc(len + 1);
	if (line == NULL)
		return (NULL);
	ft_memcpy(line, cur, len);
	line[len] = '\0';
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*start_tmp_2_free;
	static char	*cur;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)//read(fd, NULL, 0) 读 0 个字节，只用来检查 fd 还能不能用。fd 有效时它返回 0，什么都不会读走；fd 无效或者出错时返回 -1，代码就清空 stash 并返回 NULL
	{
		free(start_tmp_2_free);
		start_tmp_2_free = NULL;
		return (cur = NULL, NULL);
	}
	start_tmp_2_free = read_to_stash(fd, start_tmp_2_free, &cur);
	if (start_tmp_2_free == NULL)
		return (cur = NULL, NULL);
	line = extract_line(cur);
	if (line != NULL)
		cur += ft_strlen(line);
	if (line == NULL || *cur == '\0')
	{
		free(start_tmp_2_free);
		start_tmp_2_free = NULL;
		cur = NULL;
	}
	return (line);
}


// static char	*join_stash(char *stash, char *buf)
// {
// 	char	*tmp;

// 	if (stash == NULL)
// 		return (ft_substr(buf, 0, ft_strlen(buf)));
// 	tmp = stash;//两个指针指向同一块内存,用于释放旧的stash
// 	stash = ft_strjoin(stash, buf);
// 	free(tmp);
// 	return (stash);
// }
// /*如果 ft_strjoin() 失败：
// 1. tmp 仍然指向旧字符串 "Hello"。
// 2. stash 变成 NULL。
// 3. free(tmp) 释放旧字符串。
// 这样不会泄漏旧的内存。*/

// static char	*read_to_stash(int fd, char *stash)
// {
// 	char	*buf;
// 	ssize_t	bytes;

// 	if (BUFFER_SIZE <= 0)
// 		return (free(stash), NULL);
// 	buf = malloc(BUFFER_SIZE + 1);
// 	if (!buf)
// 		return (free(stash), NULL);
// 	bytes = 1;//先把变量 bytes 设置为 1，让下面的 while 循环能够开始执行。
// 	while (bytes > 0 && (stash == NULL || !ft_strchr(stash, '\n')))
// 	{
// 		bytes = read(fd, buf, BUFFER_SIZE);
// 		if (bytes == -1)
// 			return (free(buf), free(stash), NULL);
// 		if (bytes == 0)
// 			break ;
// 		buf[bytes] = '\0';
// 		stash = join_stash(stash, buf);
// 		if (stash == NULL)
// 			return (free(buf), NULL);
// 	}
// 	return (free(buf), stash);
// }

// static char	*extract_line(char *stash)
// {
// 	char	*nl;

// 	nl = ft_strchr(stash, '\n');
// 	if (nl == NULL)
// 		return (ft_substr(stash, 0, ft_strlen(stash)));
// 	return (ft_substr(stash, 0, ((size_t)(nl - stash)) + 1));
// }

// static char	*update_stash(char *stash)
// {
// 	char	*nl;
// 	char	*new_stash;

// 	nl = ft_strchr(stash, '\n');
// 	if (nl == NULL || nl[1] == '\0')
// 	{
// 		free(stash);
// 		return (NULL);
// 	}
// 	new_stash = ft_substr(stash, nl - stash + 1, ft_strlen(nl + 1));
// 	free(stash);
// 	return (new_stash);
// }

// char	*get_next_line(int fd)
// {
// 	static char	*stash;//static 可以让 stash 在多次调用 get_next_line() 之间保留它的值。
// 	char		*line;

// 	if (fd < 0 || BUFFER_SIZE <= 0)
// 		return (NULL);
// 	stash = read_to_stash(fd, stash);
// 	if (stash == NULL)
// 		return (NULL);
// 	line = extract_line(stash);
// 	if (line == NULL)
// 	{
// 		free(stash);
// 		stash = NULL;
// 		return (NULL);
// 	}
// 	stash = update_stash(stash);
// 	return (line);
// }
