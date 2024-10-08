/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maheleni <maheleni@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 15:43:28 by eberkowi          #+#    #+#             */
/*   Updated: 2024/09/19 15:20:06 by maheleni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
* Locates the first occurrence of c (converted to a char) in the string 
*	pointed to by s.
* The terminating null character is considered to be part of the 
*	string; therefore if c is `\0', the functions locate the terminating `\0'.
* Parameters:
*	*s - pointer to the string to find character c from
*	c - int representation of the character to find
* Returns:
*	Pointer to the first occurance of the character found.
*	NULL if c is not found anywhere in the string
*/
char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if ((char)c == '\0')
		return ((char *)s);
	else
		return (NULL);
}
