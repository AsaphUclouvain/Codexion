/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:31 by anzongan          #+#    #+#             */
/*   Updated: 2026/07/06 18:33:37 by anzongan         ###   ########.fr       */
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

static struct timespec	get_wait_ts(t_coder *coder, t_key_context *ctx,
		struct timespec burnout_ts)
{
	int64_t	now;
	int64_t	cooldown_end;
	int64_t	burnout_deadline;

	now = timestamp_us();
	burnout_deadline = coder->last_compile_end + coder->args->time_to_burnout;
	if (ctx->queue->size > 0 && peek(ctx->queue)->id == coder->id)
	{
		cooldown_end = ctx->last_use + coder->args->dongle_cooldown;
		if (cooldown_end > now && cooldown_end < burnout_deadline)
			return (get_timeout(cooldown_end));
	}
	return (burnout_ts);
}

static int	handle_timedwait_rc(int rc, t_coder *coder, t_key_context *ctx)
{
	int64_t	burnout_deadline;

	if (rc == 0)
		return (0);
	burnout_deadline = coder->last_compile_end + coder->args->time_to_burnout;
	if (timestamp_us() >= burnout_deadline)
		return (rc);
	if (ctx->queue->size > 0 && peek(ctx->queue)->id == coder->id
		&& timestamp_us() - ctx->last_use >= coder->args->dongle_cooldown)
		return (0);
	return (rc);
}

int	get_keys_and_compile_step_1(struct timespec ts, \
	t_key_context *ctx_1, t_coder *coder)
{
	int				rc;
	int64_t			priority;
	struct timespec	wait_ts;

	priority = (coder->vars->get_priority)(coder);
	if (request_key(coder->id, priority, ctx_1) == NULL)
		return (1);
	pthread_mutex_lock(&(ctx_1->mutex));
	pthread_mutex_lock(&(coder->vars->burnout_mutex));
	rc = 0;
	while (sleep_cond(&rc, coder, ctx_1) == 0)
	{
		wait_ts = get_wait_ts(coder, ctx_1, ts);
		pthread_mutex_unlock(&(coder->vars->burnout_mutex));
		rc = pthread_cond_timedwait(&(ctx_1->cond), &(ctx_1->mutex), &wait_ts);
		rc = handle_timedwait_rc(rc, coder, ctx_1);
		pthread_mutex_lock(&(coder->vars->burnout_mutex));
	}
	if (coder->vars->burnout_sig != -1)
		return (burnout_alert(coder), drop_key(ctx_1), 1);
	if (rc != 0)
		return (burnout_alert(coder), drop_key(ctx_1), 1);
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
	exec_action(DONGLE_TAKEN, coder);
	return (0);
}

int	get_keys_and_compile_step_2(struct timespec ts, t_key_context *ctx_1, \
	t_key_context *ctx_2, t_coder *coder)
{
	int				rc;
	int64_t			priority;
	struct timespec	wait_ts;

	priority = (coder->vars->get_priority)(coder);
	if (request_key(coder->id, priority, ctx_2) == NULL)
		return (drop_key(ctx_1), 1);
	pthread_mutex_lock(&(ctx_2->mutex));
	pthread_mutex_lock(&(coder->vars->burnout_mutex));
	rc = 0;
	while (sleep_cond(&rc, coder, ctx_2) == 0)
	{
		wait_ts = get_wait_ts(coder, ctx_2, ts);
		pthread_mutex_unlock(&(coder->vars->burnout_mutex));
		rc = pthread_cond_timedwait(&(ctx_2->cond), &(ctx_2->mutex), &wait_ts);
		rc = handle_timedwait_rc(rc, coder, ctx_2);
		pthread_mutex_lock(&(coder->vars->burnout_mutex));
	}
	if (coder->vars->burnout_sig != -1)
		return (burnout_alert(coder), drop_key(ctx_1), drop_key(ctx_2), 1);
	if (rc != 0)
		return (burnout_alert(coder), drop_key(ctx_1), drop_key(ctx_2), 1);
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
	exec_action(DONGLE_TAKEN, coder);
	return (exec_action(COMPILE, coder), drop_key(ctx_1), drop_key(ctx_2), 0);
}
