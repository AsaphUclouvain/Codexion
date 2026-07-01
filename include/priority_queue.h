/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:44:38 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:44:39 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIORITY_QUEUE_H
# define PRIORITY_QUEUE_H

# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <inttypes.h>

typedef struct s_node
{
	int64_t	priority;
	int		id;
}	t_node;

typedef struct s_priority_queue
{
	t_node	**items;
	int		max_size;
	int		size;
	int		type;
	int		(*cmp)(t_node *, t_node *);
}	t_priority_queue;

t_priority_queue	*pq_create(int size, int (*cmp)(t_node *, t_node *));
t_node				*enqueue(int64_t priority, int id, t_priority_queue *pq);
t_node				*dequeue(t_priority_queue *pq);
t_node				*peek(t_priority_queue *pq);
void				heapify_up(int index, t_priority_queue *pq);
void				swap(t_node **a, t_node **b);
void				heapify_down(int index, t_priority_queue *pq);

#endif