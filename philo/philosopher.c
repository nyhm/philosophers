/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:37:52 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/08 00:20:10 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

pthread_mutex_t	print_mutex = PTHREAD_MUTEX_INITIALIZER;

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
		usleep(100); // 100マイクロ秒ごとにチェック
}

void	start(t_table *table)
{
	t_philo			*philos;
	pthread_mutex_t	*forks;
	int				i;
	pthread_mutex_t	*meal_mutexes;
	pthread_t		monitor_thread;

	philos = malloc(sizeof(t_philo) * table->philo_num);
	if (!philos)
		return ;
	forks = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	if (!forks)
		return ;
	meal_mutexes = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	if (!meal_mutexes)
		return ;
	table->start_time = get_time_in_ms();
	i = 0;
	while (i < table->philo_num)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos[i].id = i + 1;
		philos[i].eat_count = 0;
		philos[i].table = table;
		philos[i].left_fork = &forks[i];
		philos[i].right_fork = &forks[(i + 1) % table->philo_num];
		philos[i].print_mutex = &print_mutex;
		pthread_mutex_init(&meal_mutexes[i], NULL);
		philos[i].meal_mutex = &meal_mutexes[i];
		philos[i].last_eat = get_time_in_ms();
		pthread_create(&philos[i].thread, NULL, philosopher_routine,
			(void *)&philos[i]);
		i++;
	}
	pthread_create(&monitor_thread, NULL, monitor_philosopher, philos);
	pthread_join(monitor_thread, NULL);
}
