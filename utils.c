/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 19:10:13 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/23 08:50:23 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_philo(t_philo *philo)
{
	int	i;

	i = 0;
	if (!philo)
		return ;
	pthread_mutex_destroy(philo->print_lock);
	pthread_mutex_destroy(philo->meals_lock);
	free(philo->print_lock);
	free(philo->meals_lock);
	while (i < philo->args->num_philo)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		pthread_mutex_destroy(philo[i].last_eat);
		free(philo[i].last_eat);
		i++;
	}
	free(philo->forks);
	free(philo->args);
	free(philo);
}

void	free_philo_fails(t_philo *philo, int i)
{
	int	j;

	j = 0;
	if (!philo)
		return ;
	pthread_mutex_destroy(philo->print_lock);
	pthread_mutex_destroy(philo->meals_lock);
	while (--i > 0)
	{
		pthread_mutex_destroy(philo[i].last_eat);
		free(philo[i].last_eat);
	}
	i = 0;
	while (i < philo->args->num_philo)
	{
		pthread_mutex_destroy(&philo->forks[i]);
		i++;
	}
	free (philo->forks);
	free(philo->meals_lock);
	free(philo->print_lock);
	free(philo->args);
	free(philo);
}
