/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagashi <hnagashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:41:31 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/10 16:56:44 by hnagashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	all_destroy(t_philo *philos, t_table *table, pthread_mutex_t *forks,
		pthread_mutex_t *meal_mutexes)
{
	if (philos)
	{
		free(philos);
		philos = NULL;
	}
	if (forks)
	{
		free(forks);
		forks = NULL;
	}
	if (meal_mutexes)
	{
		free(meal_mutexes);
		meal_mutexes = NULL;
	}
	if (table)
		free(table);
}

void	left_start(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a left fork");
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a right fork");
}

void	right_start(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a right fork");
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a left fork");
}

void	print_action(t_philo *philo, const char *action)
{
	long	timestamp;

	pthread_mutex_lock(philo->print_mutex);
	timestamp = get_time_in_ms() - philo->table->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(philo->print_mutex);
}

int	eat_action(t_philo *philo)
{
	print_action(philo, "is eating");
	pthread_mutex_lock(philo->meal_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(philo->meal_mutex);
	philo->eat_count++;
	precise_sleep(philo->table->time_eat);
	if (philo->eat_count == philo->table->must_eat)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		philo->finished = 1;
		return (0);
	}
	return (1);
}
