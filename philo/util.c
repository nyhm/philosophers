/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:41:31 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/09 08:56:18 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_destroy(t_table *table)
{
	if (table)
	{
		free(table);
		table = NULL;
	}
	return (1);
}

void	print_action(t_philo *philo, const char *action)
{
	long	timestamp;

	timestamp = get_time_in_ms() - philo->table->start_time;
	pthread_mutex_lock(philo->print_mutex);
	printf("%ld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(philo->print_mutex);
}

void	left_start(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");
}

void	right_start(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, "has taken a fork");
}

int	eat_action(t_philo *philo)
{
	print_action(philo, "is eating");
	pthread_mutex_lock(philo->meal_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(philo->meal_mutex);
	philo->eat_count++;
	if (philo->eat_count == philo->table->must_eat)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	precise_sleep(philo->table->time_eat);
	return (1);
}
