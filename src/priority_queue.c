/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:37 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:43:38 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "priority_queue.h"

t_priority_queue	*pq_create(int size, int (*cmp)(t_node *, t_node *))
{
	t_priority_queue	*pq;

	pq = (t_priority_queue *)malloc(sizeof(t_priority_queue));
	if (!pq)
		return (NULL);
	pq->cmp = cmp;
	pq->max_size = size;
	pq->size = 0;
	pq->items = (t_node **)malloc(sizeof(t_node *) * size);
	if (!pq->items)
		return (free(pq), NULL);
	return (pq);
}

t_node	*enqueue(int64_t priority, int id, t_priority_queue *pq)
{
	t_node	*node;

	if (pq->size == pq->max_size)
		return (NULL);
	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->priority = priority;
	node->id = id;
	pq->items[pq->size++] = node;
	heapify_up(pq->size - 1, pq);
	return (node);
}

t_node	*dequeue(t_priority_queue *pq)
{
	t_node	*node;

	if (pq->size == 0)
		return (NULL);
	node = pq->items[0];
	pq->items[0] = pq->items[--pq->size];
	heapify_down(0, pq);
	return (node);
}

t_node	*peek(t_priority_queue *pq)
{
	if (pq->size == 0)
		return (NULL);
	return (pq->items[0]);
}
