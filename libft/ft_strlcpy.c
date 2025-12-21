/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:58:14 by mhaizan           #+#    #+#             */
/*   Updated: 2025/09/04 15:45:49 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!dst || !src)
		return (0);
	if (dstsize == 0)
		return (ft_strlen(src));
	while (src[i] && i < dstsize - 1)
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
#include <stdio.h>
#include <string.h>
#include <bsd/string.h>
int main()
{
    char dest[6];
	char dest2[6];
    const char *src = "HelloWorld";
    size_t ret;
	size_t ret2;

    ret = ft_strlcpy(dest, src, sizeof(dest));
	ret2 = strlcpy(dest2, src, sizeof(dest2));

    printf("Copied string: %s\n", dest);
    printf("Returned length of src: %zu\n", ret);
	printf("Copied string: %s\n", dest2);
    printf("Returned length of src: %zu\n", ret2);

    return 0;
}