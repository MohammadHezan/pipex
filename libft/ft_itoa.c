/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:50:09 by mhaizan           #+#    #+#             */
/*   Updated: 2025/08/26 14:50:11 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	init_vars(int n, int *sign, int *len, int *temp)
{
	if (n < 0)
		*sign = -1;
	else
		*sign = 1;
	if (n <= 0)
		*len = 1;
	else
		*len = 0;
	*temp = n;
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	int		sign;
	int		temp;

	init_vars(n, &sign, &len, &temp);
	while (temp)
	{
		temp /= 10;
		len++;
	}
	str = (char *)malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n == 0)
		str[0] = '0';
	while (n != 0)
	{
		str[--len] = '0' + sign * (n % 10);
		n /= 10;
	}
	if (sign < 0)
		str[0] = '-';
	return (str);
}
