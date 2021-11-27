/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hmap_collision.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 03:12:20 by cempassi          #+#    #+#             */
/*   Updated: 2020/07/23 03:12:20 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "hashmap.h"

size_t		ft_hmap_collision_rate(t_hash *hashmap)
{
	return ((ft_hmap_collision(hashmap) * 100)
				>> ft_get_two_pw(hashmap->map_size));
}

size_t		ft_hmap_collision(t_hash *hashmap)
{
	size_t	i;
	size_t	collisions;
	t_list	*ptr;

	i = 0;
	collisions = 0;
	while (i < hashmap->map_size)
	{
		ptr = hashmap->map[i];
		if (ptr && ft_lstlen(ptr) > 1)
		{
			collisions += ft_lstlen(ptr);
		}
		i++;
	}
	return (collisions);
}
