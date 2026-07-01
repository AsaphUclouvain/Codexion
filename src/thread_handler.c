/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:41 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/30 17:57:25 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	*coder_routine(void *args)
{
	t_coder	*coder;
	int		err;

	coder = (t_coder *)args;
	while (1)
	{
		if (coder->number_of_compiles_remaining == 0)
			break ;
		if (coder->args->number_of_coders == 1)
			err = work_one(coder);
		else
			err = work_many(coder);
		if (err == 1)
			break ;
		coder->number_of_compiles_remaining -= 1;
	}
	pthread_mutex_lock(&(coder->vars->burnout_mutex));
	coder->vars->finished_count += 1;
	pthread_cond_broadcast(&(coder->vars->burnout_cond));
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
	return (NULL);
}

int	launcher(t_shared_vars *vars, t_args *args)
{
	int		i;

	i = 0;
	while (i < args->number_of_coders)
	{
		if (pthread_create(&(vars->thread_ids[i]), NULL,
				&coder_routine, &(vars->coders[i])) != 0)
			return (1);
		i++;
	}
	pthread_mutex_lock(&(vars->burnout_mutex));
	while (vars->burnout_sig == -1
		&& vars->finished_count < args->number_of_coders)
		pthread_cond_wait(&(vars->burnout_cond), &(vars->burnout_mutex));
	pthread_mutex_unlock(&(vars->burnout_mutex));
	return (0);
}

int	joiner(t_shared_vars *vars, t_args *args)
{
	int	i;

	i = 0;
	while (i < args->number_of_coders)
	{
		if (pthread_join(vars->thread_ids[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}
