/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:10:13 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/05/21 15:32:05 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_check_simulation(t_philo *philo)
{
	bool	stop;

	pthread_mutex_lock(&philo->shared->stop_mutex);
	stop = philo->shared->stop_simulation;
	pthread_mutex_unlock(&philo->shared->stop_mutex);
	if (!stop)
		return (false);
	return (true);
}

void	ft_sleep(t_philo *philo, long num)
{
	long	start;

	start = get_time_ms();
	while (get_time_ms() - start < num)
	{
		if (ft_check_simulation(philo))
			break;
		usleep(150);
	}
}
long	get_time_ms(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}
void	free_meals_mutex(t_prog *prog, int i)
{
	while (--i >= 0)
	{
		pthread_mutex_destroy(prog->philosophers[i].meals_mutex);
		free(prog->philosophers[i].meals_mutex);
	}
}

void	free_philo(t_prog *prog)
{
	int	i;
	
	i = 0;
	while (i < prog->args.philo_num)
	{
		pthread_mutex_destroy(&prog->forks[i]);
		i++;
	}
	free (prog->forks);
	pthread_mutex_destroy(&prog->shared.print_mutex);
	pthread_mutex_destroy(&prog->shared.start_mutex);
	pthread_mutex_destroy(&prog->shared.stop_mutex);
}