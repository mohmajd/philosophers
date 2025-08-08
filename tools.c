/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:40:01 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/05/19 18:30:17 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_info	*ft_check_args(int ac, char **av)
{
	t_info	*args;

	if (ac < 5 || ac > 6)
		return (printf("too many args\n"), NULL);
	args =  malloc(sizeof(t_info));
	if (!args)
		return (NULL);
	args->philo_num = ft_atoi(av[1]);
	args->t_die = ft_atoi(av[2]);  //atol
	args->t_eat = ft_atoi(av[3]);
	args->t_sleep = ft_atoi(av[4]);
	if (ac == 6)
	{
		args->num_meals = ft_atoi(av[5]);
		if (args->num_meals < 0)
			return (NULL);
	}
	else
		args->num_meals = -1;
	if (args->philo_num < 1 || args->t_die < 1 || args->t_eat < 0
			|| args->t_sleep < 0)
			return (NULL);
	return (args);
}

void	print_state(t_philo *philo, char *str)
{
	bool	stop;
	long	time;
	
	pthread_mutex_lock(&philo->shared->stop_mutex);
	stop = philo->shared->stop_simulation;
	pthread_mutex_unlock(&philo->shared->stop_mutex);
	if (!stop)
	{
		time = get_time_ms() - philo->shared->start_time;
		pthread_mutex_lock(&philo->shared->print_mutex);
		printf("%ld %d %s\n", time, philo->id, str);
		pthread_mutex_unlock(&philo->shared->print_mutex);
	}
}


void    take_fork(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->left_fork);
        print_state(philo, "has taken a fork");
        pthread_mutex_lock(philo->right_fork);  // Remove trylock, use blocking
        print_state(philo, "has taken a fork");
    }
    else
    {
        usleep(100);
        pthread_mutex_lock(philo->right_fork);
        print_state(philo, "has taken a fork");
        pthread_mutex_lock(philo->left_fork);   // Remove trylock, use blocking
        print_state(philo, "has taken a fork");
    }
    last_meal_eaten(philo);
}

void	put_down_fork(t_philo *philo)
{
	if (philo->left_fork > philo->right_fork)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}

void	last_meal_eaten(t_philo *philo)
{
	pthread_mutex_lock(philo->meals_mutex);
	philo->last_time_eat = get_time_ms();
	// philo->meals_eaten++;
	pthread_mutex_unlock(philo->meals_mutex);
}

void	last_meal_eaten_2(t_philo *philo)
{
	pthread_mutex_lock(philo->meals_mutex);
	// philo->last_time_eat = get_time_ms();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meals_mutex);
}
