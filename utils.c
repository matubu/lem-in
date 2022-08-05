/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsamain <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:24:22 by dsamain           #+#    #+#             */
/*   Updated: 2021/10/27 13:13:53 by dsamain          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lem_in.h"

void ft_memcpy(void *dst, void *src, size_t n) {
	for (int i = 0; i < n; i++)
		*(unsigned char *)(dst + i) = *(unsigned char *)(src + i);
}
