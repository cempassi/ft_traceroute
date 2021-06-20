/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 11:41:07 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void display_help(char *name)
{
	ft_dprintf(STDERR_FILENO, "usage: %s [-hoqv] [-c count] ", name);
	ft_dprintf(STDERR_FILENO, "[-i wait] [-m ttl] [-s packetsize] host\n");
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_C);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_I);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_M);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_S);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_H);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_O);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_Q);
	// ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_V);
}
