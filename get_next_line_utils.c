/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: weitwang <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/07 15:45:44 by weitwang          #+#    #+#             */
/*   Updated: 2026/11/07 15:45:48 by weitwang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	const char	*p;

	p = s;
	while (*p)
	{
		p++;
	}
	return ((size_t)(p - s));
}

char	*ft_strchr(const char *s, int c)
{
	while ((unsigned char)*s != (unsigned char)c)
	{
		if (!*s)
		{
			return (NULL);
		}
		s++;
	}
	return ((char *)s);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	i = 0;
	d = dest;
	s = src;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*p;
	size_t	len1;
	size_t	len2;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	p = malloc(len1 + len2 + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, s1, len1);
	ft_memcpy(&p[len1], s2, len2);
	p[len1 + len2] = '\0';
	return (p);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	i;

	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		start = ft_strlen(s);
	i = 0;
	while ((i < len) && s[start + i])
		i++;
	p = malloc(i + 1);
	if (!p)
		return (NULL);
	ft_memcpy(p, s + start, i);
	p[i] = '\0';
	return (p);
}
