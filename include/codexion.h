/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:44:28 by anzongan          #+#    #+#             */
/*   Updated: 2026/07/06 18:33:26 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <time.h>
# include <sys/time.h>
# include <stdio.h>
# include <limits.h>
# include <string.h>
# include <inttypes.h>
# include <errno.h>
# include "priority_queue.h"
# include "input_parser.h"

typedef struct s_shared_vars	t_shared_vars;

typedef enum e_action
{
	DONGLE_TAKEN,
	COMPILE,
	DEBUG,
	REFACTOR,
	BURNED_OUT
}	t_action;

typedef struct s_coder
{
	t_shared_vars	*vars;
	t_args			*args;
	int64_t			last_compile_start;
	int64_t			last_compile_end;
	int				l_key;
	int				r_key;
	int				arrival_time;
	int				id;
	int				number_of_compiles_remaining;
}	t_coder;

typedef struct s_key_context
{
	t_priority_queue	*queue;
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	int64_t				last_use;
	int					cond_init;
	int					mutex_init;
}	t_key_context;

struct s_shared_vars
{
	pthread_mutex_t	output_mutex;
	pthread_mutex_t	burnout_mutex;
	pthread_cond_t	burnout_cond;
	t_key_context	*keys;
	pthread_t		*thread_ids;
	int64_t			start_time;
	int64_t			(*get_priority)(struct s_coder *);
	t_coder			*coders;
	int				burnout_sig;
	int				finished_count;
	int				burnout_mutex_init;
	int				burnout_cond_init;
	int				output_mutex_init;
	int				output_opened;
};

struct timespec	get_timeout(int64_t deadline_us);
int64_t			timestamp_us(void);
int64_t			timestamp_relative(int64_t start_time);
int64_t			max(int64_t a, int64_t b);
int64_t			get_priority(t_coder *coder);
t_node			*request_key(int id, int64_t priority, t_key_context *context);
void			exec_action(t_action action, t_coder *coder);
void			burnout_alert(t_coder *coder);
void			drop_key(t_key_context *ctx);
void			set_defaults(t_shared_vars *vars);
void			sleep_and_burnout(t_coder *coder);
int				sleep_cond(int *rc, t_coder *coder, t_key_context *ctx);
int				get_keys_and_compile_step_1(struct timespec ts, \
t_key_context *ctx_1, t_coder *coder);
int				get_keys_and_compile_step_2(struct timespec ts, \
t_key_context *ctx_1, t_key_context *ctx_2, t_coder *coder);
int				launcher(t_shared_vars *vars, t_args *args);
int				joiner(t_shared_vars *vars, t_args *args);
int				min_cmp(t_node *n1, t_node *n2);
int				init_keys(t_shared_vars *vars, t_args *args);
int				init_thread_ids(t_shared_vars *vars, t_args *args);
int				init_coders(t_shared_vars *vars, t_args *args);
int				init_shared_variables(t_shared_vars **vars, t_args *args);
int				work_many(t_coder *coder);
int				work_one(t_coder *coder);
int				cleanup(t_shared_vars *vars, t_args *args);

#endif