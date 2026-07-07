/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:44:34 by anzongan          #+#    #+#             */
/*   Updated: 2026/07/06 19:26:33 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INPUT_PARSER_H
# define INPUT_PARSER_H

# define NUM_PARAMS 8
# define MAX_NUM_CODER 999
# include <string.h>
# include <inttypes.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# include <unistd.h>

typedef enum e_heap_type
{
	FIFO,
	EDF
}	t_heap_type;

typedef struct s_args
{
	t_heap_type	scheduler;
	int64_t		time_to_burnout;
	int64_t		time_to_compile;
	int64_t		time_to_debug;
	int64_t		time_to_refactor;
	int64_t		dongle_cooldown;
	int			number_of_coders;
	int			number_of_compiles_required;
}	t_args;

void	copy_substr(int start_i, int len_dest, char *src, char *dest);
void	free_str_arr(char **arr);
void	populate_args(t_args *args, char **strs);
void	print_args(t_args *args);
void	input_error(void);
char	*ft_strdup(char *str);
int		count_words(char *str);
int		init_args(int argc, char **argv, t_args **args);
int		get_word_len(int cur_i, char *str_arg);
int		split(char *str_arg, char ***strs);
int		validate_args(char **strs);
int		init_from_str(char *str_arg, char ***strs);
int		init_from_stream(char **argv, char ***strs);

#endif