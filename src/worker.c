/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   worker.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:57 by anzongan          #+#    #+#             */
/*   Updated: 2026/07/06 19:34:47 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	work_many(t_coder *coder)
{
	t_key_context	*ctx_1;
	t_key_context	*ctx_2;
	struct timespec	ts;

	coder->arrival_time = timestamp_us();
	ts = get_timeout(coder->last_compile_end + coder->args->time_to_burnout);
	if (coder->r_key < coder->l_key)
	{
		ctx_1 = &(coder->vars->keys[coder->r_key]);
		ctx_2 = &(coder->vars->keys[coder->l_key]);
	}
	else
	{
		ctx_1 = &(coder->vars->keys[coder->l_key]);
		ctx_2 = &(coder->vars->keys[coder->r_key]);
	}
	if (get_keys_and_compile_step_1(ts, ctx_1, coder) == 1)
		return (1);
	if (get_keys_and_compile_step_2(ts, ctx_1, ctx_2, coder) == 1)
		return (1);
	exec_action(DEBUG, coder);
	exec_action(REFACTOR, coder);
	return (0);
}

int	work_one(t_coder *coder)
{
	t_key_context	*ctx;
	struct timespec	ts;

	exec_action(DONGLE_TAKEN, coder);
	sleep_and_burnout(coder);
	exec_action(BURNED_OUT, coder);
	return (0);
}
