/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohmajdo <mohmajdo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 01:37:10 by mohmajdo          #+#    #+#             */
/*   Updated: 2025/08/23 08:54:18 by mohmajdo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <pthread.h>
# include <sys/time.h>
# include <stdlib.h>
# include <limits.h>
# include <unistd.h>
# include <stdbool.h>

typedef struct s_args
{
	int		num_philo;
	int		time_die;
	int		time_eat;
	int		time_sleep;
	int		num_eat;
	int		died;
}	t_args;

typedef struct s_philo
{
	t_args			*args;
	pthread_mutex_t	*print_lock;
	pthread_mutex_t	*meals_lock;
	pthread_mutex_t	*last_eat;
	pthread_mutex_t	*forks;
	int				id;
	long			last_time_eat;
	long			start_time;
	int				*n_eat;
	int				stop;
}	t_philo;

void			*routine(void *arg);
pthread_mutex_t	*create_forks(int num_philos);
t_philo			*init_variables(t_args *args, pthread_t **thread_id);
bool			philo(t_philo *philosophers, pthread_mutex_t *forks		\
					, t_args *args, pthread_t *id);
void			first_innit_philo(t_philo *philo, int i, t_args *args		\
					, pthread_mutex_t *forks);
void			create_threads_and_check_death(t_philo *philosophers		\
					, t_args *args, pthread_t *id);
void			eat_update(t_philo *philo, int *meals);
void			check_is_died(t_philo *philo, pthread_t *thread_id		\
					, int i, long time);
long			get(t_philo *philosophers, int i);
long			get_time(void);
void			ft_sleep(long time_ms);
void			free_philo(t_philo *philo);
t_args			*check_args(int ac, char **av);
void			print_state(char *str, int id, long start_time	\
					, pthread_mutex_t *print_lock);
int				meals_number(t_philo philo);
bool			check_num_args(char **av);
int				ft_atoi(char *str);
void			free_philo_fails(t_philo *philo, int i);
#endif