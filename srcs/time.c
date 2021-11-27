/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 22:21:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/26 22:25:16 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

double deltaT(t_time *time)
{
	double dt;

	dt = (double)(time->sent.tv_sec - time->sent.tv_sec) * 1000.0 +
	     (double)(time->recv.tv_usec - time->recv.tv_usec) / 1000.0;
	return (dt);
}
