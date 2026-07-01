/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:42:55 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/30 18:28:25 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	sleep_and_die(t_coder *coder)
{
	usleep(
		max(
			coder->last_compile_start + coder->args->time_to_burnout
			- timestamp_us(),
			0)
		);
}

static void	sleep_dwait(int64_t sleep_time, t_coder *coder)
{
	int				rc;
	struct timespec	ts;

	pthread_mutex_lock(&(coder->vars->burnout_mutex));
	if (coder->last_compile_start + coder->args->time_to_burnout
		< timestamp_us() + sleep_time)
		return (sleep_and_die(coder), burnout_alert(coder));
	ts = get_timeout(timestamp_us() + sleep_time);
	rc = 0;
	while (coder->vars->burnout_sig == -1
		&& rc == 0)
	{
		rc = pthread_cond_timedwait(
				&(coder->vars->burnout_cond),
				&(coder->vars->burnout_mutex), &ts);
	}
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
}

static void	coder_sleep(t_action action, t_coder *coder)
{
	int64_t	key_last_use;

	if (action == COMPILE)
		sleep_dwait(coder->args->time_to_compile, coder);
	else if (action == DEBUG)
		sleep_dwait(coder->args->time_to_debug, coder);
	else if (action == REFACTOR)
		sleep_dwait(coder->args->time_to_refactor, coder);
	if (action == COMPILE)
	{
		key_last_use = timestamp_us();
		coder->vars->keys[coder->l_key].last_use = key_last_use;
		coder->vars->keys[coder->r_key].last_use = key_last_use;
	}
}

void	exec_action(t_action action, t_coder *coder)
{
	int64_t	tms;

	pthread_mutex_lock(&(coder->vars->output_mutex));
	if (coder->vars->output_opened == 0)
	{
		pthread_mutex_unlock(&(coder->vars->output_mutex));
		return ;
	}
	if (action == COMPILE)
		coder->last_compile_start = timestamp_us();
	if (action == BURNED_OUT)
		coder->vars->output_opened = 0;
	tms = timestamp_relative(coder->vars->start_time);
	if (action == DONGLE_TAKEN)
		printf("%" PRId64 " %d has taken a dongle\n", tms, coder->id);
	else if (action == COMPILE)
		printf("%" PRId64 " %d is compiling\n", tms, coder->id);
	else if (action == DEBUG)
		printf("%" PRId64 " %d is debugging\n", tms, coder->id);
	else if (action == REFACTOR)
		printf("%" PRId64 " %d is refactoring\n", tms, coder->id);
	else if (action == BURNED_OUT)
		printf("%" PRId64 " %d burned out\n", tms, coder->id);
	pthread_mutex_unlock(&(coder->vars->output_mutex));
	coder_sleep(action, coder);
}
