/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:47 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 18:02:14 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int64_t	get_priority(t_coder *coder)
{
	if (coder->args->scheduler == FIFO)
		return (coder->arrival_time);
	return (coder->last_compile_start + coder->args->time_to_burnout);
}

int	min_cmp(t_node *n1, t_node *n2)
{
	if (n1->priority == n2->priority)
		return (n1->id > n2->id);
	return (n1->priority < n2->priority);
}

int64_t	max(int64_t a, int64_t b)
{
	if (a > b)
		return (a);
	return (b);
}
