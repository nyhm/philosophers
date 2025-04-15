/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagashi <hnagashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:37:52 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/15 21:10:23 by hnagashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	finish_check(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->finish_mutex);
	if (philo->table->finish_flag == 1)
	{
		pthread_mutex_unlock(&philo->table->finish_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->table->finish_mutex);
	return (0);
}

long	get_time_in_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	if (tv.tv_sec < 0 || tv.tv_usec < 0)
	{
		printf("Error: gettimeofday failed\n");
		return (-1);
	}
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	precise_sleep(long ms)
{
	long	start;

	start = get_time_in_ms();
	while (get_time_in_ms() - start < ms)
		usleep(100);
}

void	create_philo(t_philo *philos, int i, t_table *table)
{
	pthread_mutex_init(&table->forks[i], NULL);
	philos[i].table = table;
	philos[i].id = i + 1;
	philos[i].eat_count = 0;
	philos[i].left_fork = i;
	philos[i].right_fork = (i + 1) % philos[i].table->philo_num;
	pthread_mutex_init(&philos[i].meal_mutex, NULL);
	philos[i].last_eat = philos[i].table->start_time;
	philos[i].finished = 0;
	philos[i].table->finish_flag = 0;
	philos[i].table->finish_count = 0;
	pthread_create(&philos[i].thread, NULL, philosopher_routine,
		(void *)&philos[i]);
}

void	start(t_table *table)
{
	t_philo		*philos;
	int			i;
	pthread_t	monitor_thread;

	philos = malloc(sizeof(t_philo) * table->philo_num);
	if (!philos)
	{
		all_destroy(philos, table);
		return ;
	}
	table->start_time = get_time_in_ms() + 1000;
	pthread_mutex_init(&table->print_mutex, NULL);
	pthread_mutex_init(&table->finish_mutex, NULL);
	i = 0;
	while (i < table->philo_num)
		create_philo(philos, i++, table);
	pthread_create(&monitor_thread, NULL, monitor_philosopher, philos);
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < table->philo_num)
		pthread_join(philos[i++].thread, NULL);
	all_destroy(philos, table);
}
