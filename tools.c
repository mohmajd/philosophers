/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:40:01 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/22 21:45:28 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get(t_philo *philosophers, int i)
{
	long	time;

	pthread_mutex_lock(philosophers[i].last_eat);
	time = philosophers[i].last_time_eat;
	pthread_mutex_unlock(philosophers[i].last_eat);
	return (time);
}

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_sleep(long time_ms)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time_ms)
		usleep(500);
}

int	meals_number(t_philo philo)
{
	int	num_eat;

	pthread_mutex_lock(philo.meals_lock);
	num_eat = *philo.n_eat;
	pthread_mutex_unlock(philo.meals_lock);
	return (num_eat);
}

void	print_state(char *str, int id, long start_time,
			pthread_mutex_t *print_lock)
{
	pthread_mutex_lock(print_lock);
	printf("%ld %d %s\n", get_time() - start_time, id, str);
	if (str[0] != 'd')
		pthread_mutex_unlock(print_lock);
}
