/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:37:10 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/05/19 17:43:55 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <limits.h>

typedef struct s_info
{
	int		philo_num;
	long	t_die;
	long	t_eat;
	long	t_sleep;
	int		num_meals;
} t_info ;

typedef struct s_shared {
    pthread_mutex_t     print_mutex;
    pthread_mutex_t     start_mutex;
    pthread_mutex_t     stop_mutex;
    bool               stop_simulation;
    long	start_time;
} t_shared;

typedef struct s_philo
{
	int	id;
	pthread_t	thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*meals_mutex;
	long	last_time_eat;
	int		meals_eaten;
	t_shared	*shared;
	t_info	*args;
} t_philo;

typedef struct s_prog
{
	t_philo	*philosophers;
	pthread_mutex_t	*forks;
	t_shared	shared;
	t_info	args;
}	t_prog;


void	print_state(t_philo *philo, char *str);
long	get_time_ms(void);
t_info	*ft_check_args(int ac, char **av);
bool	ft_innit_forks(t_prog *prog);
bool	ft_innit_philo(t_prog *prog);
bool	ft_innit(t_prog *prog, t_info *args);
void	*routine(void *arg);
void	*mounitor_routine(void *args);
bool	ft_creat_philo(t_prog *prog);
bool	ft_check_simulation(t_philo *philo);
void	take_fork(t_philo *philo);
void	put_down_fork(t_philo *philo);
void	last_meal_eaten(t_philo *philo);
void	ft_sleep(t_philo *philo, long num);
void	free_meals_mutex(t_prog *prog, int i);
void	free_philo(t_prog *prog);
int	ft_atoi(char *str);
void	innit_philo_start_time(t_prog *prog);
void	last_meal_eaten_2(t_philo *philo);

#endif