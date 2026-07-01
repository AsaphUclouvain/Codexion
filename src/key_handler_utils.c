/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handler_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:27 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/30 17:58:28 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_node	*request_key(int id, int64_t priority, t_key_context *context)
{
	t_node	*err;

	pthread_mutex_lock(&(context->mutex));
	err = enqueue(priority, id, context->queue);
	pthread_mutex_unlock(&(context->mutex));
	return (err);
}

int	sleep_cond(int *rc, t_coder *coder, t_key_context *ctx)
{
	return (
		1 - (
			coder->vars->burnout_sig == -1
			&& *rc == 0
			&& ctx->queue->size > 0
			&& peek(ctx->queue)->id != coder->id
		)
	);
}

void	drop_key(t_key_context *ctx)
{
	free(dequeue(ctx->queue));
	pthread_cond_broadcast(&(ctx->cond));
	pthread_mutex_unlock(&(ctx->mutex));
}

void	burnout_alert(t_coder *coder)
{
	if (coder->vars->burnout_sig == -1)
	{
		coder->vars->burnout_sig = coder->id;
		pthread_cond_broadcast(&(coder->vars->burnout_cond));
		exec_action(BURNED_OUT, coder);
	}
	pthread_mutex_unlock(&(coder->vars->burnout_mutex));
}
