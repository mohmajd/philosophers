/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:36:15 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/23 08:07:10 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	check_num_args(char **av)
{
	int	i;
	int	j;

	i = 1;
	while (av[i])
	{
		j = 0;
		while (av[i][j] == ' ')
			j++;
		if (av[i][j] == '\0')
			return (false);
		if (av[i][j] == '-' || av[i][j] == '+')
			j++;
		while (av[i][j])
		{
			if (av[i][j] < '0' || av[i][j] > '9')
				return (false);
			j++;
		}
		i++;
	}
	return (true);
}

t_args	*check_args(int ac, char **av)
{
	t_args	*args;

	if (ac < 5 || ac > 6)
		return (printf("Wrong number of arguments\n"), NULL);
	args = malloc(sizeof(t_args));
	if (!args)
		return (NULL);
	args->num_philo = atoi(av[1]);
	args->time_die = atoi(av[2]);
	args->time_eat = atoi(av[3]);
	args->time_sleep = atoi(av[4]);
	if (args->num_philo < 1 || args->time_die < 0 || args->time_eat < 0 || \
		args->time_sleep < 0)
		return (printf("Wrong arguments\n"), free(args), NULL);
	if (ac == 6)
	{
		args->num_eat = atoi(av[5]);
		if (args->num_eat < 1)
			return (printf("Wrong arguments\n"), free(args), NULL);
	}
	else
		args->num_eat = -1;
	// args->died = 0;
	return (args);
}

int	main(int ac, char **av)
{
	t_args			*args;
	pthread_mutex_t	*forks;
	pthread_t		*philo_id;
	t_philo			*philosophers;

	if (check_num_args(av) == false)
		return (printf("args error\n"), 1);
	args = check_args(ac, av);
	if (!args)
		return (1);
	forks = create_forks(args->num_philo);
	if (!forks)
		return (free(args), 1);
	philosophers = init_variables(args, &philo_id);
	if (!philosophers)
		return (free(args), free(forks), 1);
	philo(philosophers, forks, args, philo_id);
	return (0);
}
