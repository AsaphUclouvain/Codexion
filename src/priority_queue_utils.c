/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:44:53 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:44:54 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "priority_queue.h"

void	swap(t_node **a, t_node **b)
{
	t_node	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	heapify_up(int index, t_priority_queue *pq)
{
	int	parent;

	if (index == 0)
		return ;
	parent = (index - 1) / 2;
	if (pq->cmp(
			pq->items[index],
			pq->items[parent]))
	{
		swap(pq->items + parent, pq->items + index);
		heapify_up(parent, pq);
	}
}

void	heapify_down(int index, t_priority_queue *pq)
{
	int	prioritised;
	int	left_child;
	int	right_child;

	prioritised = index;
	left_child = 2 * index + 1;
	right_child = 2 * index + 2;
	if (left_child < pq->size
		&& pq->cmp(
			pq->items[left_child],
			pq->items[prioritised])
	)
		prioritised = left_child;
	if (right_child < pq->size
		&& pq->cmp(
			pq->items[right_child],
			pq->items[prioritised])
	)
		prioritised = right_child;
	if (prioritised != index)
	{
		swap(pq->items + prioritised, pq->items + index);
		heapify_down(prioritised, pq);
	}
}
