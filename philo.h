/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 12:39:06 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/04 10:49:14 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <errno.h>
# include <limits.h>
# include <pthread.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	time_t	time_die;
	time_t	time_eat;
	time_t	time_sleep;
	int		philo_num;
	int		must_eat;
}			t_philo;

int			ft_atoi(const char *str);
void		start(t_philo *philo);
int			philo_destroy(t_philo *philo);

#endif