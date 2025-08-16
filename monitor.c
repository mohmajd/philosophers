/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 22:37:04 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/16 22:37:50 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_check_is_died(t_prog *prog, long time_since_last_meal)
{
	if (time_since_last_meal > prog->args.t_die)
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

static void	*check_death_and_meals(t_prog *prog)
{
	int		i;
	long	time_since_meal;
	long	time;

	i = 0;
	while (i < prog->args.philo_num)
	{
		pthread_mutex_lock(prog->philosophers[i].meals_mutex);
		time_since_meal = get_time_ms() - prog->philosophers[i].last_time_eat;
		pthread_mutex_unlock(prog->philosophers[i].meals_mutex);
		if (ft_check_is_died(prog, time_since_meal))
		{
			time = get_time_ms() - prog->philosophers[i].shared->start_time;
			return (printf("%ld %d died\n", time,
					prog->philosophers[i].id), NULL);
		}
		i++;
	}
	if (prog->args.num_meals > 0 && check_meals_eaten(prog))
		return (NULL);
	return ((void *)1);
}

void	*mounitor_routine(void *args)
{
	t_prog	*prog;
	void	*result;

	prog = (t_prog *)args;
	pthread_mutex_lock(&prog->shared.start_mutex);
	pthread_mutex_unlock(&prog->shared.start_mutex);
	while (1)
	{
		pthread_mutex_lock(&prog->shared.stop_mutex);
		if (prog->shared.stop_simulation)
		{
			pthread_mutex_unlock(&prog->shared.stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&prog->shared.stop_mutex);
		result = check_death_and_meals(prog);
		if (result != (void *)1)
			return (result);
		usleep(2000);
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
