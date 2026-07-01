/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anzongan <anzongan@student.42belgium.be    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/27 16:43:34 by anzongan          #+#    #+#             */
/*   Updated: 2026/06/27 16:43:35 by anzongan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_shared_vars	*vars;
	t_args			*args;

	vars = NULL;
	args = NULL;
	if (init_args(ac, av, &args) == 1)
		return (cleanup(vars, args), 1);
	if (init_shared_variables(&vars, args) == 1)
		return (cleanup(vars, args), 1);
	if (launcher(vars, args) == 1)
		return (cleanup(vars, args), 1);
	if (joiner(vars, args) == 1)
		return (cleanup(vars, args), 1);
	return (cleanup(vars, args));
}
