/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conversion.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cedricmpassi <cempassi@student.42.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/17 20:46:19 by cedricmpa         #+#    #+#             */
/*   Updated: 2020/07/23 03:31:23 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONVERSION_H
# define CONVERSION_H

int					ft_atoi(const char *str);
char				*ft_itoa(int n);
char				*ft_llitoa(long long n);
char				*ft_ullitoa(unsigned long long int n);
long long			ft_atoll_base(char *str, char *base);
unsigned long long	ft_atoull_base(char *str, char *base);
char				*ft_convert_base(char *nbr, char *base_from, char *base_to);
int					ft_abs(int c);

#endif
