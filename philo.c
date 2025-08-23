/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:44:12 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/23 08:55:18 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	*create_forks(int num_philos)
{
	int				i;
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * num_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < num_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			while (--i > 0)
				pthread_mutex_destroy(&forks[i]);
			free (forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

t_philo	*init_variables(t_args *args, pthread_t **philo_id)
{
	t_philo	*philosophers;

	philosophers = malloc(sizeof(t_philo) * args->num_philo);
	if (!philosophers)
		return (NULL);
	*philo_id = malloc(args->num_philo * sizeof(pthread_t));
	if (!*philo_id)
		return (free(philosophers), NULL);
	philosophers->print_lock = malloc(sizeof(pthread_mutex_t));
	if (!philosophers->print_lock)
		return (free(*philo_id), free(philosophers), NULL);
	if (pthread_mutex_init(philosophers->print_lock, NULL) != 0)
		return (free(philosophers->print_lock), free(*philo_id),
			free(philosophers), NULL);
	philosophers->meals_lock = malloc(sizeof(pthread_mutex_t));
	if (!philosophers->meals_lock)
	{
		pthread_mutex_destroy(philosophers->print_lock);
		free(philosophers->print_lock);
		return (free(*philo_id), free(philosophers), NULL);
	}
	if (pthread_mutex_init(philosophers->meals_lock, NULL) != 0)
		return (pthread_mutex_destroy(philosophers->print_lock), free(philosophers->meals_lock), free(philosophers->print_lock),
			free(*philo_id), free(philosophers), NULL);
	return (philosophers);
}

void	first_innit_philo(t_philo *philo, int i,
			t_args *args, pthread_mutex_t *forks)
{
	long	time;

	time = get_time();
	philo[i].id = i;
	philo[i].forks = forks;
	philo[i].args = args;
	philo[i].start_time = time;
	philo[i].last_time_eat = time;
}

bool	philo(t_philo *philosophers, pthread_mutex_t *forks,
				t_args *args, pthread_t *philo_id)
{
	int				i;
	int				var;

	var = 0;
	i = -1;
	while (++i < args->num_philo)
	{
		first_innit_philo(philosophers, i, args, forks);
		philosophers[i].n_eat = &var;
		philosophers[i].last_eat = malloc(sizeof(pthread_mutex_t));
		if (!philosophers[i].last_eat)
			return (free_philo_fails(philosophers, i), free(philo_id), false);
		return (free_philo_fails(philosophers, i), free(philo_id), false);
		if (pthread_mutex_init(philosophers[i].last_eat, NULL) != 0)
			return (free_philo_fails(philosophers, i + 1), free(philo_id), false);
		philosophers[i].print_lock = philosophers->print_lock;
		philosophers[i].meals_lock = philosophers->meals_lock;
	}
	create_threads_and_check_death(philosophers, args, philo_id);
	return (true);
}

void	create_threads_and_check_death(t_philo *philosophers, t_args *args,
			pthread_t *philo_id)
{
	int	i;

	i = 0;
	while (i < args->num_philo)
	{
		pthread_create(&philo_id[i], NULL, routine, &philosophers[i]);
		pthread_detach(philo_id[i]);
		i++;
	}
	check_is_died(philosophers, philo_id, 0, 0);
}
