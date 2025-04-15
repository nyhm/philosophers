/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hnagashi <hnagashi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:37:42 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/15 21:14:44 by hnagashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	table_init(t_table *table)
{
	table->time_die = 0;
	table->time_eat = 0;
	table->time_sleep = 0;
	table->philo_num = 0;
	table->must_eat = 0;
	table->finish_count = 0;
	table->start_time = 0;
	return (1);
}

int	table_check(t_table *table)
{
	if (table->philo_num <= 0 || table->time_die < 0 || table->time_eat <= 0
		|| table->time_sleep <= 0 || table->must_eat <= -1)
	{
		printf("No valid input. \nPlease insert positive integers.\n");
		return (1);
	}
	return (0);
}

int	set_fork(t_table *table)
{
	pthread_mutex_t	*forks;
	int				i;

	forks = malloc(sizeof(pthread_mutex_t) * table->philo_num);
	if (!forks)
	{
		printf("Memory allocation for forks failed\n");
		return (1);
	}
	i = 0;
	while (i < table->philo_num)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			printf("Mutex initialization failed\n");
			free(forks);
			return (1);
		}
		i++;
	}
	table->forks = forks;
	return (0);
}

int	philo_set(t_table *table, int ac, char **av)
{
	if (ac < 5)
	{
		printf("No valid input.\nNot enough arguments!\n");
		return (1);
	}
	else if (ac > 6)
	{
		printf("No valid input.\nToo many arguments!\n");
		return (1);
	}
	table->philo_num = ft_atoi(av[1]);
	table->time_die = ft_atoi(av[2]);
	table->time_eat = ft_atoi(av[3]);
	table->time_sleep = ft_atoi(av[4]);
	if (ac == 5)
		table->must_eat = 0;
	else
		table->must_eat = ft_atoi(av[5]);
	if (set_fork(table))
		return (1);
	if (table_check(table))
	{
		return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_table	*table;

	table = NULL;
	table = malloc(sizeof(t_table));
	if (!table)
	{
		printf("Memory allocation failed\n");
		return (1);
	}
	table_init(table);
	if (philo_set(table, ac, av))
	{
		free(table);
		return (1);
	}
	start(table);
	return (0);
}
