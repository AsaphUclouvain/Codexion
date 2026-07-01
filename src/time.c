/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:45 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:43:46 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

struct timespec	get_timeout(int64_t deadline_us)
{
	struct timespec	ts;

	ts.tv_sec = deadline_us / 1000000;
	ts.tv_nsec = (deadline_us % 1000000) * 1000;
	return (ts);
}

int64_t	timestamp_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec);
}

int64_t	timestamp_relative(int64_t start_time)
{
	return ((timestamp_us() - start_time) / 1000);
}
