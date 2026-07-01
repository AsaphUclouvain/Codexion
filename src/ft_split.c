/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:04 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:43:05 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "input_parser.h"

int	count_words(char *str)
{
	int	count;
	int	chr;
	int	i;

	count = 0;
	i = 0;
	chr = 0;
	while (1)
	{
		if (str[i] == ' ' || str[i] == '\0')
		{
			count += (chr > 0);
			chr = 0;
		}
		else
			chr++;
		if (str[i] == '\0')
			return (count);
		i++;
	}
	return (count);
}

int	get_word_len(int cur_i, char *str_arg)
{
	int	count;

	count = 0;
	while (str_arg[cur_i + count] && str_arg[cur_i + count] != ' ')
		count++;
	return (count);
}

// This function copy len_dest characters from src to dest
void	copy_substr(int start_i, int len_dest, char *src, char *dest)
{
	int	i;

	i = 0;
	while (i < len_dest)
	{
		dest[i] = src[start_i + i];
		i++;
	}
	dest[i] = '\0';
}

int	split(char *str_arg, char ***strs)
{
	int	i;
	int	word_i;
	int	word_count;
	int	word_len;

	word_count = count_words(str_arg);
	*strs = (char **)malloc(sizeof(char *) * (word_count + 1));
	if (!(*strs))
		return (1);
	i = 0;
	word_i = 0;
	while (word_i < word_count)
	{
		while (str_arg[i] == ' ')
			i++;
		word_len = get_word_len(i, str_arg);
		(*strs)[word_i] = (char *)malloc(word_len + 1);
		if (!(*strs)[word_i])
			return (1);
		copy_substr(i, word_len, str_arg, (*strs)[word_i]);
		i += word_len;
		word_i++;
	}
	(*strs)[word_i] = NULL;
	return (0);
}
