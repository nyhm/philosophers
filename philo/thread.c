/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:55:36 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/09 08:55:56 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		print_action(philo, "is thinking");
		if (philo->id % 2 == 0)
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

void	*monitor_philosopher(void *arg)
{
	t_philo		*philos;
	int			i;
	long long	now;

	philos = (t_philo *)arg;
	while (1)
	{
		i = 0;
		now = get_time_in_ms();
		while (i < philos->table->philo_num)
		{
			pthread_mutex_lock(philos[i].meal_mutex);
			if (now - philos[i].last_eat > philos[i].table->time_die)
			{
				pthread_mutex_lock(philos[i].print_mutex);
				printf("%lld %d died\n", now - philos[i].table->start_time,
					philos[i].id);
				pthread_mutex_unlock(philos[i].meal_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(philos[i].meal_mutex);
			i++;
		}
		usleep(100);
	}
}
