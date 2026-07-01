/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:22 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 17:16:37 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_parser.h"

void	free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

int	init_from_str(char *str_arg, char ***strs)
{
	if (count_words(str_arg) != NUM_PARAMS)
		return (1);
	if (split(str_arg, strs) == 1)
		return (1);
	return (0);
}

char	*ft_strdup(char *str)
{
	int		i;
	char	*new_str;

	new_str = (char *)malloc(strlen(str) + 1);
	if (!new_str)
		return (NULL);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	init_from_stream(char **argv, char ***strs)
{
	int	i;

	*strs = (char **)malloc(sizeof(char *) * (NUM_PARAMS + 1));
	if (!*strs)
		return (1);
	i = 1;
	while (i < NUM_PARAMS + 1)
	{
		(*strs)[i - 1] = ft_strdup(argv[i]);
		if (!(*strs)[i - 1])
			return (1);
		i++;
	}
	(*strs)[i - 1] = NULL;
	return (0);
}

int	init_args(int argc, char **argv, t_args **args)
{
	char	**strs;

	strs = NULL;
	if (argc != 2 && argc != 9)
		return (input_error(), 1);
	if (argc == 2 && init_from_str(argv[1], &strs) == 1)
		return (free_str_arr(strs), input_error(), 1);
	if (argc == 9 && init_from_stream(argv, &strs) == 1)
		return (free_str_arr(strs), input_error(), 1);
	*args = (t_args *)malloc(sizeof(t_args));
	if (*args == NULL)
		return (free_str_arr(strs), 1);
	if (validate_args(strs) == 1)
		return (free_str_arr(strs), input_error(), 1);
	populate_args(*args, strs);
	free_str_arr(strs);
	return (0);
}
