/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:55:36 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/10 10:34:38 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_until_start(long start_time)
{
	while (get_time_in_ms() < start_time)
		usleep(100);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_until_start(philo->table->start_time);
	if (philo->id % 2 == 0)
		usleep(200);
	while (1)
	{
		print_action(philo, "is thinking");
		if (philo->id % 2 == 1)
			left_start(philo);
		else
			right_start(philo);
		if (!eat_action(philo))
			return (NULL);
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		print_action(philo, "is sleeping");
		precise_sleep(philo->table->time_sleep);
	}
	return (NULL);
}

int	philo_check(t_philo *philos, long long now, int i, int *finish_count)
{
	pthread_mutex_lock(philos[i].meal_mutex);
	if (now - philos[i].last_eat > philos[i].table->time_die)
	{
		pthread_mutex_lock(philos[i].print_mutex);
		printf("%lld %d died\n", now - philos[i].table->start_time,
			philos[i].id);
		pthread_mutex_unlock(philos[i].meal_mutex);
		return (1);
	}
	if (philos[i].finished == 1)
		(*finish_count)++;
	pthread_mutex_unlock(philos[i].meal_mutex);
	return (0);
}

void	*monitor_philosopher(void *arg)
{
	t_philo		*philos;
	int			i;
	int			finish_count;
	long long	now;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		finish_count = 0;
		now = get_time_in_ms();
		while (i < philos->table->philo_num)
		{
			if (philo_check(philos, now, i, &finish_count) == 1)
				return (NULL);
			i++;
		}
		if (finish_count == philos->table->philo_num)
		{
			pthread_mutex_lock(philos->print_mutex);
			printf("%lld All philosophers have finished eating\n", now
				- philos->table->start_time);
			pthread_mutex_unlock(philos->print_mutex);
			return (NULL);
		}
		usleep(100);
	}
	return (NULL);
}
