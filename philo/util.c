/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagashi <hnagashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:41:31 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/15 21:07:48 by hnagashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	zero_time_die(t_philo *philo)
{
	if (philo->table->time_die == 0)
	{
		pthread_mutex_lock(&philo->table->finish_mutex);
		philo->table->finish_flag = 1;
		pthread_mutex_unlock(&philo->table->finish_mutex);
		return (1);
	}
	return (0);
}

void	all_destroy(t_philo *philos, t_table *table)
{
	int	i;

	if (philos)
	{
		pthread_mutex_destroy(&table->print_mutex);
		pthread_mutex_destroy(&table->finish_mutex);
		i = 0;
		while (i < table->philo_num)
		{
			pthread_mutex_destroy(&philos[i].meal_mutex);
			pthread_mutex_destroy(&table->forks[i]);
			i++;
		}
		free(table->forks);
		free(philos);
		philos = NULL;
	}
	if (table)
		free(table);
}

void	left_start(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->forks[philo->left_fork]);
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->table->forks[philo->right_fork]);
	print_action(philo, "has taken a fork");
}

void	print_action(t_philo *philo, const char *action)
{
	long	timestamp;

	pthread_mutex_lock(&philo->table->finish_mutex);
	if (philo->table->finish_flag == 1)
	{
		pthread_mutex_unlock(&philo->table->finish_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->table->finish_mutex);
	pthread_mutex_lock(&philo->table->print_mutex);
	timestamp = get_time_in_ms() - philo->table->start_time;
	printf("%ld %d %s\n", timestamp, philo->id, action);
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	eat_action(t_philo *philo)
{
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_eat = get_time_in_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	precise_sleep(philo->table->time_eat);
	if (philo->eat_count == philo->table->must_eat)
	{
		pthread_mutex_lock(&philo->table->finish_mutex);
		philo->finished = 1;
		pthread_mutex_unlock(&philo->table->finish_mutex);
	}
	pthread_mutex_lock(&philo->meal_mutex);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->meal_mutex);
	return (1);
}
