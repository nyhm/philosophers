/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hiroki <hiroki@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 07:25:07 by hnagashi          #+#    #+#             */
/*   Updated: 2025/04/04 10:15:53 by hiroki           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

static int	overflow_check(int sign, int num, char next)
{
	if (sign == 1 && (num > INT_MAX / 10 || (num == INT_MAX / 10 && next
				- '0' > INT_MAX % 10)))
		return (-1);
	if (sign == -1 && (num > -((INT_MIN + (next - '0')) / 10)))
		return (-1);
	return (1);
}

int	ft_atoi(const char *str)
{
	int	ret;
	int	sign;
	int	overflow;

	ret = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			sign = -1;
	while (*str >= '0' && *str <= '9')
	{
		overflow = overflow_check(sign, ret, *str);
		if (overflow != 1)
			return (-1);
		ret = ret * 10 + (*str - '0');
		str++;
	}
	return ((int)(ret * sign));
}
