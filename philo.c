/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:44:12 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/05/21 15:43:16 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	ft_innit_forks(t_prog *prog)
{
	int	i;

	i = 0;
	prog->forks = malloc(sizeof(pthread_mutex_t) * prog->args.philo_num);
	if (!prog->forks)
		return (false);
	while (i < prog->args.philo_num)
	{
		if (pthread_mutex_init(&prog->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&prog->forks[i]);
			return (free(prog->forks), false);
		}
		i++;
	}
	return (true);
}

bool	ft_innit_philo(t_prog *prog)
{
	int	i;

	i = 0;
	if (!prog->philosophers)
		return (false);
	while (i < prog->args.philo_num)
	{
		prog->philosophers[i].id = i + 1;
		prog->philosophers[i].left_fork = &prog->forks[i];
		prog->philosophers[i].right_fork = &prog->forks[(i + 1) % prog->args.philo_num];
		prog->philosophers[i].meals_eaten = 0;
		prog->philosophers[i].meals_mutex = malloc(sizeof(pthread_mutex_t));
		if (!prog->philosophers[i].meals_mutex)
			return (free_meals_mutex(prog, i), false);
		if (pthread_mutex_init(prog->philosophers[i].meals_mutex, NULL) != 0)
		{
			free (prog->philosophers[i].meals_mutex);
			return (free_meals_mutex(prog, i), false);
		}
		prog->philosophers[i].shared = &prog->shared;
		prog->philosophers[i].args = &prog->args;
		i++;
	}
	return (true);
}

bool	ft_innit(t_prog *prog, t_info *args)
{
	prog->args = *args;
	free (args);
	if (pthread_mutex_init(&prog->shared.print_mutex, NULL)!= 0)
		return (false);
	if (pthread_mutex_init(&prog->shared.start_mutex, NULL) != 0)
		return (pthread_mutex_destroy(&prog->shared.print_mutex), false);
	if (pthread_mutex_init(&prog->shared.stop_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&prog->shared.print_mutex);
		return (pthread_mutex_destroy(&prog->shared.start_mutex), false);
	}
	prog->shared.stop_simulation = false;
	if (!ft_innit_forks(prog))
	{
		pthread_mutex_destroy(&prog->shared.print_mutex);
		pthread_mutex_destroy(&prog->shared.stop_mutex);
		return (pthread_mutex_destroy(&prog->shared.start_mutex), false);
	}
	prog->philosophers = malloc(sizeof(t_philo) * prog->args.philo_num);
	if (!prog->philosophers)
		return (free_philo(prog), false);
	if (!ft_innit_philo(prog))
		return (free_philo(prog), free(prog->philosophers), false);
	return (true);
}

void	innit_philo_start_time(t_prog *prog)
{
	int	i;

	i = 0;
	while (i < prog->args.philo_num)
	{
		pthread_mutex_lock(prog->philosophers[i].meals_mutex);
		prog->philosophers[i].last_time_eat = prog->shared.start_time;
		pthread_mutex_unlock(prog->philosophers[i].meals_mutex);
		i++;
	}
}
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->shared->start_mutex);
	pthread_mutex_unlock(&philo->shared->start_mutex);
	if (philo->id % 2 == 1)
		usleep (1000 * (philo->args->t_eat / 2));
	while (1)
	{
		if (ft_check_simulation(philo))
			break;
		if (philo->args->num_meals > 0)
        {
            pthread_mutex_lock(philo->meals_mutex);
            if (philo->meals_eaten >= philo->args->num_meals)
            {
                pthread_mutex_unlock(philo->meals_mutex);
                break;
            }
            pthread_mutex_unlock(philo->meals_mutex);
        }
		print_state(philo, "is thinking");
		take_fork(philo);
		last_meal_eaten(philo);
		print_state(philo, "is eating");
		last_meal_eaten_2(philo);
		ft_sleep(philo, philo->args->t_eat);
		put_down_fork(philo);
		print_state(philo, "is sleeping");
		ft_sleep(philo, philo->args->t_sleep);
	}
	return (NULL);
}

bool	ft_creat_philo(t_prog *prog)
{
	pthread_t	monitor;
	int	i;

	i = 0;
	pthread_mutex_lock(&prog->shared.start_mutex);
	while (i < prog->args.philo_num)
	{
		if (pthread_create(&prog->philosophers[i].thread, NULL, routine, &prog->philosophers[i]) != 0)
		{
			prog->shared.stop_simulation = true;
			return (pthread_mutex_unlock(&prog->shared.start_mutex), false);
		}
		i++;
	}
	if (pthread_create(&monitor, NULL, mounitor_routine, prog) != 0)
	{
		prog->shared.stop_simulation = true;
		return (pthread_mutex_unlock(&prog->shared.start_mutex), false);
	}
	prog->shared.start_time = get_time_ms();
	innit_philo_start_time(prog);
	pthread_mutex_unlock(&prog->shared.start_mutex);
	i = -1;
	while (++i <  prog->args.philo_num)
		pthread_join(prog->philosophers[i].thread, NULL);
	return (pthread_join(monitor, NULL), true);
}
