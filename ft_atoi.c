/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:43:27 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/16 22:52:03 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_sign(char **str)
{
	int	signe;

	signe = 1;
	while (**str == ' ' || **str == '\n' || **str == '\t'
		|| **str == '\v' || **str == '\f' || **str == '\r')
		(*str)++;
	if (**str == '-' || **str == '+')
	{
		if (**str == '-')
			signe *= (-1);
		(*str)++;
	}
	return (signe);
}

int	ft_atoi(char *str)
{
	unsigned long	r;
	int				signe;

	r = 0;
	signe = ft_sign(&str);
	while (*str >= '0' && *str <= '9')
	{
		r = r * 10 + (*str - 48);
		str++;
	}
	if (r > LONG_MAX && signe == 1)
		return (-1);
	if (r > (unsigned long)LONG_MAX + 1 && signe == -1)
		return (0);
	return ((int)r * signe);
}

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
