/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_initializer_2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:55 by anzongan          #+#    #+#             */
/*   Updated: 2026/07/02 16:24:00 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_key(int index, t_shared_vars *vars, t_args *args)
{
	vars->keys[index].last_use = 0;
	vars->keys[index].cond_init = 1;
	vars->keys[index].mutex_init = 1;
	vars->keys[index].queue = pq_create(args->number_of_coders, &min_cmp);
	if (vars->keys[index].queue == NULL)
		return (1);
	if (pthread_mutex_init(&(vars->keys[index].mutex), NULL) != 0)
		return (1);
	vars->keys[index].mutex_init = 0;
	if (pthread_cond_init(&(vars->keys[index].cond), NULL) != 0)
		return (1);
	vars->keys[index].cond_init = 0;
	return (0);
}

int	init_keys(t_shared_vars *vars, t_args *args)
{
	int	i;

	vars->keys = \
	(t_key_context *)malloc(sizeof(t_key_context) * args->number_of_coders);
	if (vars->keys == NULL)
		return (1);
	i = 0;
	while (i < args->number_of_coders)
		vars->keys[i++].queue = NULL;
	i = 0;
	while (i < args->number_of_coders)
	{
		if (init_key(i, vars, args) == 1)
			return (1);
		i++;
	}
	return (0);
}

int	init_thread_ids(t_shared_vars *vars, t_args *args)
{
	vars->thread_ids = \
	(pthread_t *)malloc(sizeof(pthread_t) * args->number_of_coders);
	if (vars->thread_ids == NULL)
		return (1);
	return (0);
}

int	init_coders(t_shared_vars *vars, t_args *args)
{
	int	i;

	vars->coders = (t_coder *)malloc(sizeof(t_coder) * args->number_of_coders);
	if (vars->coders == NULL)
		return (1);
	i = 0;
	while (i < args->number_of_coders)
	{
		vars->coders[i].id = i + 1;
		vars->coders[i].last_compile_start = vars->start_time;
		vars->coders[i].number_of_compiles_remaining = \
args->number_of_compiles_required;
		vars->coders[i].vars = vars;
		vars->coders[i].args = args;
		vars->coders[i].l_key = i;
		vars->coders[i].r_key = (i + 1) % args->number_of_coders;
		vars->coders[i].last_compile_start = vars->start_time;
		vars->coders[i].last_compile_end = vars->start_time;
		i++;
	}
	return (0);
}

void	set_defaults(t_shared_vars *vars)
{
	vars->start_time = timestamp_us();
	vars->output_opened = 1;
	vars->finished_count = 0;
	vars->burnout_sig = -1;
	vars->burnout_cond_init = 1;
	vars->burnout_mutex_init = 1;
	vars->output_mutex_init = 1;
	vars->coders = NULL;
	vars->thread_ids = NULL;
	vars->keys = NULL;
	vars->get_priority = &get_priority;
}
