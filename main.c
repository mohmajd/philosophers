/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:36:15 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/16 22:37:21 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_info	*args;
	t_prog	prog;

	args = ft_check_args(ac, av);
	if (!args)
		return (1);
	if (!ft_innit(&prog, args))
		return (printf("error while initializing variables"), 1);
	if (prog.args.philo_num == 1)
	{
		pthread_mutex_lock(&prog.forks[0]);
		printf("0 1 has taken a fork\n");
		usleep(prog.args.t_die * 1000);
		printf("%ld 1 died\n", prog.args.t_die);
		pthread_mutex_unlock(&prog.forks[0]);
		return (ft_cleanup(&prog), 0);
	}
	if (!ft_creat_philo(&prog))
	{
		ft_cleanup(&prog);
		return (printf("errors while creating philosophers"), 1);
	}
	ft_cleanup(&prog);
	return (0);
}
