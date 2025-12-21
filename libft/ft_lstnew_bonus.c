/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/26 14:52:52 by mhaizan           #+#    #+#             */
/*   Updated: 2025/08/26 14:52:53 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*elmement;

	if (!content)
		content = NULL;
	elmement = malloc(sizeof(t_list));
	if (!elmement)
		return (NULL);
	elmement->content = content;
	elmement->next = NULL;
	return (elmement);
}
