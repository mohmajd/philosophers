/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:36:15 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/05/21 15:39:41 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// bool	ft_check_is_died(t_prog *prog, long time_since_last_meal)
// {
// 	if (time_since_last_meal >= prog->args.t_die)
// 	{
// 		pthread_mutex_lock(&prog->shared.stop_mutex);
// 		prog->shared.stop_simulation = true;
// 		pthread_mutex_unlock(&prog->shared.stop_mutex);
// 		return (true);
// 	}
// 	return (false);
// }
bool    ft_check_is_died(t_prog *prog, long time_since_last_meal)
{
    if (time_since_last_meal > prog->args.t_die)  // Changed >= to >
    {
        pthread_mutex_lock(&prog->shared.stop_mutex);
        prog->shared.stop_simulation = true;
        pthread_mutex_unlock(&prog->shared.stop_mutex);
        return (true);
    }
    return (false);
}


bool	check_meals_eaten(t_prog *prog)
{
	int	i;
	int	full_philo;

	i = 0;
	full_philo = 0;
	while (i < prog->args.philo_num)
	{
		pthread_mutex_lock(prog->philosophers[i].meals_mutex);
		if (prog->philosophers[i].meals_eaten >= prog->args.num_meals)
			full_philo++;
		pthread_mutex_unlock(prog->philosophers[i].meals_mutex);
		i++;
	}
	if (full_philo == prog->args.philo_num)
	{
		pthread_mutex_lock(&prog->shared.stop_mutex);
		prog->shared.stop_simulation = true;
		pthread_mutex_unlock(&prog->shared.stop_mutex);
		return (true);
	}
	return (false);
}
void	*mounitor_routine(void *args)
{
	t_prog	*prog;
	int		i;
	long	time_since_meal;
	long	time;

	prog = (t_prog *)args;
	pthread_mutex_lock(&prog->shared.start_mutex);
	pthread_mutex_unlock(&prog->shared.start_mutex);
	while (1)
	{
		pthread_mutex_lock(&prog->shared.stop_mutex);
        if (prog->shared.stop_simulation)
        {
            pthread_mutex_unlock(&prog->shared.stop_mutex);
            break;
        }
        pthread_mutex_unlock(&prog->shared.stop_mutex);
		i = 0;
		while (i < prog->args.philo_num)
		{
			pthread_mutex_lock(prog->philosophers[i].meals_mutex);
			time_since_meal = get_time_ms() - prog->philosophers[i].last_time_eat;
			pthread_mutex_unlock(prog->philosophers[i].meals_mutex);
			if (ft_check_is_died(prog, time_since_meal))
			{
				time = get_time_ms() - prog->philosophers[i].shared->start_time;
				return (printf("%ld %d died\n", time, prog->philosophers[i].id), NULL);
			}
			i++;
		}
		if (prog->args.num_meals > 0 && check_meals_eaten(prog))
			return (print_state(&(prog->philosophers[0]), "all philo are full") ,NULL);
		usleep(3000);
	}
	return (NULL);
}

void	ft_cleanup(t_prog *prog)
{
	int	i;

	i = -1;
	while (++i > prog->args.philo_num)
		pthread_mutex_destroy(&prog->forks[i]);
	free (prog->forks);
	i = -1;
	while (++i < prog->args.philo_num)
	{
		pthread_mutex_destroy(prog->philosophers[i].meals_mutex);
		free(prog->philosophers[i].meals_mutex);
	}
	free(prog->philosophers);
	pthread_mutex_destroy(&prog->shared.print_mutex);
	pthread_mutex_destroy(&prog->shared.start_mutex);
	pthread_mutex_destroy(&prog->shared.stop_mutex);  
}

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
		return(printf("errors while creating philosophers"), 1);
	}
	ft_cleanup(&prog);
	return (0);
}
