/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:41:18 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:41:19 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	destroy_mutex_cond_vars(t_shared_vars *vars)
{
	int	err;

	err = 0;
	if (vars->burnout_cond_init == 0
		&& pthread_cond_destroy(&(vars->burnout_cond)) != 0)
		err = 1;
	if (vars->burnout_mutex_init == 0
		&& pthread_mutex_destroy(&(vars->burnout_mutex)) != 0)
		err = 1;
	if (vars->output_mutex_init == 0
		&& pthread_mutex_destroy(&(vars->output_mutex)) != 0)
		err = 1;
	return (err);
}

static int	destroyer(t_shared_vars *vars, t_args *args)
{
	int	i;
	int	err;

	err = 0;
	if (vars == NULL)
		return (1);
	err = destroy_mutex_cond_vars(vars);
	if (args == NULL || vars->keys == NULL)
		return (1);
	i = 0;
	while (i < args->number_of_coders)
	{
		if (vars->keys[i].cond_init == 0
			&& pthread_cond_destroy(&(vars->keys[i].cond)) != 0)
			err = 1;
		if (vars->keys[i].mutex_init == 0
			&& pthread_mutex_destroy(&(vars->keys[i].mutex)) != 0)
			err = 1;
		i++;
	}
	return (err);
}

static void	free_all(t_shared_vars *vars, t_args *args)
{
	int					i;
	int					j;
	t_priority_queue	*pq;

	if (vars->keys != NULL)
	{
		i = 0;
		while (i < args->number_of_coders)
		{
			pq = vars->keys[i].queue;
			if (pq == NULL)
				break ;
			j = 0;
			while (j < pq->size)
				free(pq->items[j++]);
			free(pq->items);
			free(pq);
			i++;
		}
		free(vars->keys);
	}
	free(vars->thread_ids);
	free(vars->coders);
}

int	cleanup(t_shared_vars *vars, t_args *args)
{
	int	err;

	err = 0;
	if (args != NULL && vars != NULL)
	{
		err = destroyer(vars, args);
		free_all(vars, args);
	}
	free(vars);
	free(args);
	return (err);
}
