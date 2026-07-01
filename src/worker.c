/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:57 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/30 17:45:27 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	work_many(t_coder *coder)
{
	t_key_context	*r_ctx;
	t_key_context	*l_ctx;
	struct timespec	ts;

	coder->arrival_time = timestamp_us();
	ts = get_timeout(coder->last_compile_start + coder->args->time_to_burnout);
	r_ctx = &(coder->vars->keys[coder->r_key]);
	l_ctx = &(coder->vars->keys[coder->l_key]);
	if (coder->r_key < coder->l_key)
	{
		if (get_first_key(ts, r_ctx, coder) == 1)
			return (1);
		if (get_second_key(ts, r_ctx, l_ctx, coder) == 1)
			return (1);
		return (exec_action(DEBUG, coder), exec_action(REFACTOR, coder), 0);
	}
	if (get_first_key(ts, l_ctx, coder) == 1)
		return (1);
	if (get_second_key(ts, l_ctx, r_ctx, coder) == 1)
		return (1);
	return (exec_action(DEBUG, coder), exec_action(REFACTOR, coder), 0);
}

int	work_one(t_coder *coder)
{
	t_key_context	*ctx;
	struct timespec	ts;

	coder->arrival_time = timestamp_us();
	ts = get_timeout(coder->last_compile_start + coder->args->time_to_burnout);
	ctx = &(coder->vars->keys[coder->l_key]);
	if (get_first_key(ts, ctx, coder) == 1)
		return (1);
	exec_action(COMPILE, coder);
	drop_key(ctx);
	exec_action(DEBUG, coder);
	exec_action(REFACTOR, coder);
	return (0);
}
