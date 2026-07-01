/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:14 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 17:26:51 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_parser.h"

void	populate_args(t_args *args, char **strs)
{
	args->number_of_coders = atoi(strs[0]);
	args->time_to_burnout = atoi(strs[1]) * 1000;
	args->time_to_compile = atoi(strs[2]) * 1000;
	args->time_to_debug = atoi(strs[3]) * 1000;
	args->time_to_refactor = atoi(strs[4]) * 1000;
	args->number_of_compiles_required = atoi(strs[5]);
	args->dongle_cooldown = atoi(strs[6]) * 1000;
	if (strcmp(strs[7], "edf") == 0)
		args->scheduler = EDF;
	else
		args->scheduler = FIFO;
}

int	validate_args(char **strs)
{
	int	i;
	int	j;
	int	num;

	i = 0;
	while (i < NUM_PARAMS - 1)
	{
		j = 0;
		num = 0;
		while (strs[i][j] >= '0' && strs[i][j] <= '9')
		{
			if (num > (INT_MAX - (strs[i][j] - '0')) / 10)
				return (1);
			num = num * 10 + (strs[i][j] - '0');
			j++;
		}
		if (strs[i][j])
			return (1);
		i++;
	}
	if (atoi(strs[0]) > MAX_NUM_CODER)
		return (1);
	if (strcmp(strs[7], "edf") && strcmp(strs[7], "fifo"))
		return (1);
	return (0);
}

void	print_args(t_args *args)
{
	printf("number_of_coders: %d\n", args->number_of_coders);
	printf("time_to_burnout: %" PRId64 "\n", args->time_to_burnout);
	printf("time_to_compile: %" PRId64 "\n", args->time_to_compile);
	printf("time_to_debug: %" PRId64 "\n", args->time_to_debug);
	printf("time_to_refactor: %" PRId64 "\n", args->time_to_refactor);
	printf("number_of_compiles_required: %d\n",
		args->number_of_compiles_required);
	printf("dongle_cooldown: %" PRId64 "\n", args->dongle_cooldown);
	printf("scheduler: ");
	if (args->scheduler == EDF)
		printf("EDF\n");
	else
		printf("FIFO\n");
}

void	input_error(void)
{
	write(2, "Invalid Arguments\n", 18);
}
