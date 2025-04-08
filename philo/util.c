/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:41:31 by hiroki            #+#    #+#             */
/*   Updated: 2025/04/08 16:37:43 by hiroki           ###   ########.fr       */
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

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		// ① 考える
		print_action(philo, "is thinking");
		// ② フォークを取る
		if (philo->id % 2 == 0)
		{
			pthread_mutex_lock(philo->left_fork);
			print_action(philo, "has taken a fork");
			pthread_mutex_lock(philo->right_fork);
			print_action(philo, "has taken a fork");
		}
		else
		{
			pthread_mutex_lock(philo->right_fork);
			print_action(philo, "has taken a fork");
			pthread_mutex_lock(philo->left_fork);
			print_action(philo, "has taken a fork");
		}
		// ③ 食べる
		print_action(philo, "is eating");
		pthread_mutex_lock(philo->meal_mutex);
		philo->last_eat = get_time_in_ms();
		pthread_mutex_unlock(philo->meal_mutex);
		philo->eat_count++;
		if (philo->eat_count == philo->table->must_eat)
		{
			pthread_mutex_unlock(philo->right_fork);
			pthread_mutex_unlock(philo->left_fork);
			return (NULL);
		}
		precise_sleep(philo->table->time_eat);
		// ④ フォークを置く
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
		// ⑤ 寝る
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
				return (NULL); // 死亡を検知して終了
			}
			pthread_mutex_unlock(philos[i].meal_mutex);
			i++;
		}
		usleep(100); // 負荷軽減のため少し待つ
	}
	return (NULL);
}
