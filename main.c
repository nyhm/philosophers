/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:37:42 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/04 10:46:20 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_init(t_philo *philo)
{
	philo->time_die = 0;
	philo->time_eat = 0;
	philo->time_sleep = 0;
	philo->philo_num = 0;
	philo->must_eat = 0;
	return (1);
}

int	philo_check(t_philo *philo)
{
	if (philo->philo_num <= 0 || philo->time_die <= 0 || philo->time_eat <= 0
		|| philo->time_sleep <= 0 || philo->must_eat <= -1)
	{
		printf("No valid input. Please insert positive integers.\n");
		philo_destroy(philo);
		return (1);
	}
	return (0);
}

int	philo_set(t_philo *philo, int ac, char **av)
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
	philo->philo_num = ft_atoi(av[1]);
	philo->time_die = ft_atoi(av[2]);
	philo->time_eat = ft_atoi(av[3]);
	philo->time_sleep = ft_atoi(av[4]);
	if (ac == 5)
		philo->must_eat = 0;
	else
		philo->must_eat = ft_atoi(av[5]);
	if (philo_check(philo))
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_philo	*philo;

	philo = NULL;
	philo = malloc(sizeof(t_philo));
	if (!philo)
	{
		printf("Memory allocation failed\n");
		return (1);
	}
	philo_init(philo);
	if (philo_set(philo, ac, av))
		return (1);
	start(philo);
	philo_destroy(philo);
	return (0);
}
