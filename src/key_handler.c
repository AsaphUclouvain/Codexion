/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:31 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/30 17:23:01 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	cool_down(t_coder *coder, t_key_context *ctx)
{
	usleep(
		max(
			coder->args->dongle_cooldown
			- (timestamp_us() - ctx->last_use),
			0
			)
		);
}

int	get_first_key(struct timespec ts, t_key_context *ctx_1, t_coder *coder)
{
	int		rc;
	int64_t	priority;

	priority = (coder->vars->get_priority)(coder);
	if (request_key(coder->id, priority, ctx_1) == NULL)
		return (1);
	pthread_mutex_lock(&(ctx_1->mutex));
	pthread_mutex_lock(&(coder->vars->burnout_mutex));
	rc = 0;
	while (sleep_cond(&rc, coder, ctx_1) == 0)
	{
		pthread_mutex_unlock(&(coder->vars->burnout_mutex));
		rc = pthread_cond_timedwait(&(ctx_1->cond), &(ctx_1->mutex), &ts);
		pthread_mutex_lock(&(coder->vars->burnout_mutex));
	}
	if (coder->vars->burnout_sig != -1)
		return (burnout_alert(coder), drop_key(ctx_1), 1);
	if (rc != 0)
		return (burnout_alert(coder), drop_key(ctx_1), 1);
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
	if (timestamp_us() - ctx_1->last_use < coder->args->dongle_cooldown)
		cool_down(coder, ctx_1);
	exec_action(DONGLE_TAKEN, coder);
	return (0);
}

int	get_second_key(struct timespec ts, t_key_context *ctx_1, \
	t_key_context *ctx_2, t_coder *coder)
{
	int		rc;
	int64_t	priority;

	priority = (coder->vars->get_priority)(coder);
	if (request_key(coder->id, priority, ctx_2) == NULL)
		return (drop_key(ctx_1), 1);
	pthread_mutex_lock(&(ctx_2->mutex));
	pthread_mutex_lock(&(coder->vars->burnout_mutex));
	rc = 0;
	while (sleep_cond(&rc, coder, ctx_2) == 0)
	{
		pthread_mutex_unlock(&(coder->vars->burnout_mutex));
		rc = pthread_cond_timedwait(&(ctx_2->cond), &(ctx_2->mutex), &ts);
		pthread_mutex_lock(&(coder->vars->burnout_mutex));
	}
	if (coder->vars->burnout_sig != -1)
		return (burnout_alert(coder), drop_key(ctx_1), drop_key(ctx_2), 1);
	if (rc != 0)
		return (burnout_alert(coder), drop_key(ctx_1), drop_key(ctx_2), 1);
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
	if (timestamp_us() - ctx_2->last_use < coder->args->dongle_cooldown)
		cool_down(coder, ctx_2);
	exec_action(DONGLE_TAKEN, coder);
	exec_action(COMPILE, coder);
	return (drop_key(ctx_1), drop_key(ctx_2), 0);
}
