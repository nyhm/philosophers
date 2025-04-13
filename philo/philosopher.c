/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:37:52 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/13 12:43:10 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(long ms)
{
	long	start;

	start = get_time_in_ms();
	while (get_time_in_ms() - start < ms)
		usleep(100);
}

void	create_philo(t_philo *philos, pthread_mutex_t *meal_mutexes,
		pthread_mutex_t *forks, int i)
{
	pthread_mutex_init(&forks[i], NULL);
	philos[i].id = i + 1;
	philos[i].eat_count = 0;
	philos[i].left_fork = &forks[i];
	philos[i].right_fork = &forks[(i + 1) % philos[i].table->philo_num];
	pthread_mutex_init(&philos[i].table->print_mutex, NULL);
	pthread_mutex_init(&meal_mutexes[i], NULL);
	philos[i].meal_mutex = &meal_mutexes[i];
	philos[i].last_eat = philos[i].table->start_time;
	philos[i].finished = 0;
	pthread_create(&philos[i].thread, NULL, philosopher_routine,
		(void *)&philos[i]);
}

void	start(t_table *table)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				i;
	pthread_mutex_t	*meal_mutexes;
	pthread_mutex_t	*finish_mutexes;
	pthread_t		monitor_thread;

	philos = malloc(sizeof(t_philo) * table->philo_num);
	forks = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	meal_mutexes = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	finish_mutexes = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	if (philos == NULL || forks == NULL || meal_mutexes == NULL)
	{
		all_destroy(philos, table, forks, meal_mutexes);
		return ;
	}
	table->start_time = get_time_in_ms() + 1000;
	i = 0;
	while (i < table->philo_num)
	{
		philos[i].table = table;
		pthread_mutex_init(&finish_mutexes[i], NULL);
		philos[i].finish_mutex = &finish_mutexes[i];
		create_philo(philos, meal_mutexes, forks, i);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor_philosopher, philos);
	pthread_join(monitor_thread, NULL);
	all_destroy(philos, table, forks, meal_mutexes);
}
