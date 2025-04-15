/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagashi <hnagashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 08:55:36 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/15 21:08:36 by hnagashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	alone_philo(t_philo *philo)
{
	if (philo->table->philo_num == 1)
	{
		pthread_mutex_lock(&philo->table->forks[philo->left_fork]);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(&philo->table->finish_mutex);
		philo->table->finish_flag = 1;
		pthread_mutex_unlock(&philo->table->finish_mutex);
		pthread_mutex_unlock(&philo->table->forks[philo->left_fork]);
		return (1);
	}
	return (0);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_until_start(philo->table->start_time);
	if (philo->id % 2 == 0)
		usleep(500);
	if (zero_time_die(philo) == 1)
		return (NULL);
	if (alone_philo(philo) == 1)
		return (NULL);
	while (1)
	{
		if (finish_check(philo) == 1)
			break ;
		print_action(philo, "is thinking");
		left_start(philo);
		eat_action(philo);
		pthread_mutex_unlock(&philo->table->forks[philo->right_fork]);
		pthread_mutex_unlock(&philo->table->forks[philo->left_fork]);
		if (finish_check(philo) == 1)
			break ;
		print_action(philo, "is sleeping");
		precise_sleep(philo->table->time_sleep);
	}
	return (NULL);
}

int	philo_check(t_philo *philos, long long now, int i)
{
	pthread_mutex_lock(&philos[i].meal_mutex);
	if (now - philos[i].last_eat > philos[i].table->time_die)
	{
		pthread_mutex_lock(&philos[i].table->finish_mutex);
		philos[i].table->finish_flag = 1;
		pthread_mutex_unlock(&philos[i].table->finish_mutex);
		usleep(300);
		pthread_mutex_lock(&philos[i].table->print_mutex);
		printf("%lld %d died\n", now - philos[i].table->start_time,
			philos[i].id);
		pthread_mutex_unlock(&philos[i].table->print_mutex);
		pthread_mutex_unlock(&philos[i].meal_mutex);
		return (1);
	}
	pthread_mutex_lock(&philos[i].table->finish_mutex);
	if (philos[i].finished == 1)
	{
		philos->table->finish_count++;
		philos[i].finished = 0;
	}
	pthread_mutex_unlock(&philos[i].table->finish_mutex);
	pthread_mutex_unlock(&philos[i].meal_mutex);
	return (0);
}

int	philo_finish(t_philo *philos, long long now)
{
	if (philos->table->finish_count == philos->table->philo_num
		&& philos->table->must_eat > 0)
	{
		pthread_mutex_lock(&philos->table->finish_mutex);
		philos->table->finish_flag = 1;
		pthread_mutex_unlock(&philos->table->finish_mutex);
		usleep(300);
		pthread_mutex_lock(&philos->table->print_mutex);
		printf("%lld All philosophers have finished eating\n", now
			- philos->table->start_time);
		pthread_mutex_unlock(&philos->table->print_mutex);
		return (1);
	}
	return (0);
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
			if (philo_check(philos, now, i) == 1)
				return (NULL);
			i++;
		}
		if (philo_finish(philos, now) == 1)
			return (NULL);
		usleep(50);
	}
	return (NULL);
}
