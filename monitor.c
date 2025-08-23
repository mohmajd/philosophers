/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 22:37:04 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/23 08:14:24 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	check_is_died(t_philo *philo, pthread_t *thread_id, int i, long time)
{
	long	start_time;

	while (1)
	{
		i = 0;
		while (i < philo->args->num_philo)
		{
			start_time = philo[i].start_time;
			time = get_time();
			if (time - get(philo, i) >= philo[i].args->time_die || \
				meals_number(philo[i]) == philo[i].args->num_philo)
			{
				if (time - get(philo, i) >= philo[i].args->time_die)
					print_state("died", i + 1, start_time, philo[i].print_lock);
				else
					pthread_mutex_lock(philo[i].print_lock);
				ft_sleep(1000);
				free(thread_id);
				free_philo(philo);
				return ;
			}
			i++;
			usleep(1000);
		}
	}
}

void	eat_update(t_philo *philo, int *meals)
{
	pthread_mutex_lock(philo->last_eat);
	philo->last_time_eat = get_time();
	pthread_mutex_unlock(philo->last_eat);
	ft_sleep(philo->args->time_eat);
	++(*meals);
	if (*meals == philo->args->num_eat)
	{
		pthread_mutex_lock(philo->meals_lock);
		++(*philo->n_eat);
		pthread_mutex_unlock(philo->meals_lock);
	}
}

void	take_forks(t_philo *philo, pthread_mutex_t *forks)
{
	pthread_mutex_lock(&forks[philo->id]);
	print_state("has taken a fork", philo->id + 1,
		(philo)->start_time, philo->print_lock);
	pthread_mutex_lock(&forks[(philo->id + 1) % philo->args->num_philo]);
	print_state("has taken a fork", philo->id + 1,
		(philo)->start_time, philo->print_lock);
}

void	put_fork_down(t_philo *philo, pthread_mutex_t *forks)
{
	pthread_mutex_unlock(&forks[philo->id]);
	pthread_mutex_unlock(&forks[(philo->id + 1) % philo->args->num_philo]);
}

void	*routine(void *arg)
{
	t_philo			*philo;
	pthread_mutex_t	*forks;
	int				meals;

	meals = 0;
	philo = arg;
	forks = philo->forks;
	print_state("is thinking", philo->id + 1,
		philo->start_time, philo->print_lock);
	if (philo->id % 2)
		ft_sleep(philo->args->time_eat / 2);
	while (1)
	{
		take_forks(philo, forks);
		print_state("is eating", philo->id + 1,
			(philo)->start_time, philo->print_lock);
		eat_update(philo, &meals);
		put_fork_down(philo, forks);
		print_state("is sleeping",
			philo->id + 1, (philo)->start_time, philo->print_lock);
		ft_sleep(philo->args->time_sleep);
		print_state("is thinking", philo->id + 1,
			(philo)->start_time, philo->print_lock);
	}
	return (NULL);
}
