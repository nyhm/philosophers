/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:39:06 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/13 12:41:15 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_table
{
	time_t			time_die;
	time_t			time_eat;
	time_t			time_sleep;
	int				philo_num;
	int				must_eat;
	long			start_time;
	pthread_mutex_t	print_mutex;
	int				finish_count;
	int				finish_flag;
}					t_table;

typedef struct s_philo
{
	int				id;
	int				eat_count;
	int				finished;
	time_t			last_eat;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*meal_mutex;
	pthread_mutex_t	*finish_mutex;
	pthread_t		thread;
	t_table			*table;
}					t_philo;

// ft_atoi.c
int					ft_atoi(const char *str);

// philosopher.c
void				start(t_table *table);
void				precise_sleep(long ms);
long				get_time_in_ms(void);

// util.c
void				print_action(t_philo *philo, const char *action);
void				right_start(t_philo *philo);
void				left_start(t_philo *philo);
int					eat_action(t_philo *philo);
void				all_destroy(t_philo *philos, t_table *table,
						pthread_mutex_t *forks, pthread_mutex_t *meal_mutexes);

// thread.c
void				*monitor_philosopher(void *arg);
void				*philosopher_routine(void *arg);
int					philo_check(t_philo *philos, long long now, int i);

#endif