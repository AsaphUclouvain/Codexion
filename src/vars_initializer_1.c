/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars_initializer_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:50 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:43:51 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_mutex_cond_vars(t_shared_vars *vars)
{
	if (pthread_mutex_init(&(vars->output_mutex), NULL) != 0)
		return (1);
	vars->output_mutex_init = 0;
	if (pthread_mutex_init(&(vars->burnout_mutex), NULL) != 0)
		return (1);
	vars->burnout_mutex_init = 0;
	if (pthread_cond_init(&(vars->burnout_cond), NULL) != 0)
		return (1);
	vars->burnout_cond_init = 0;
	return (0);
}

int	init_shared_variables(t_shared_vars **vars, t_args *args)
{
	*vars = (t_shared_vars *)malloc(sizeof(t_shared_vars));
	if (*vars == NULL)
		return (1);
	set_defaults(*vars);
	if (init_mutex_cond_vars(*vars) == 1)
		return (1);
	if (init_keys(*vars, args) == 1)
		return (1);
	if (init_coders(*vars, args) == 1)
		return (1);
	if (init_thread_ids(*vars, args) == 1)
		return (1);
	return (0);
}
