/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weitwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 15:45:20 by weitwang          #+#    #+#             */
/*   Updated: 2026/11/07 20:28:15 by weitwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*join_stash(char *stash, char *buf)
{
	char	*tmp;

	if (stash == NULL)
		return (ft_substr(buf, 0, ft_strlen(buf)));
	tmp = stash;
	stash = ft_strjoin(stash, buf);
	free(tmp);
	return (stash);
}

static char	*read_to_stash(int fd, char *stash)
{
	char	*buf;
	ssize_t	bytes;

	if (BUFFER_SIZE <= 0)
		return (free(stash), NULL);
	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (free(stash), NULL);
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
	return (free(buf), stash);
}

static char	*extract_line(char *stash)
{
	char	*nl;

	nl = ft_strchr(stash, '\n');
	if (nl == NULL)
		return (ft_substr(stash, 0, ft_strlen(stash)));
	return (ft_substr(stash, 0, ((size_t)(nl - stash)) + 1));
}

static char	*update_stash(char *stash)
{
	char	*nl;
	char	*new_stash;

	nl = ft_strchr(stash, '\n');
	if (nl == NULL || nl[1] == '\0')
	{
		free(stash);
		return (NULL);
	}
	new_stash = ft_substr(stash, nl - stash + 1, ft_strlen(nl + 1));
	free(stash);
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	stash = read_to_stash(fd, stash);
	if (stash == NULL)
		return (NULL);
	line = extract_line(stash);
	if (line == NULL)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	stash = update_stash(stash);
	return (line);
}
