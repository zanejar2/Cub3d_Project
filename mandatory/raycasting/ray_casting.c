/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zanejar <zanejar@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/22 04:17:26 by wiessaiy          #+#    #+#             */
/*   Updated: 2023/06/05 03:06:49 by zanejar          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ray_caster(t_data *data)
{
	int		i;
	float	fov;

	fov = 60 * (M_PI / 180);
	data->wall.projection = (WINDOW_WIDTH / 2) / tan(fov / 2);
	i = 0;
	data->ray[i].ray_angle = data->player.rotation_angle - (fov / 2);
	while (i < NBR_RAYS)
	{
		cast_ray(data, i);
		if (i + 1 < NBR_RAYS)
		{	
			data->ray[i + 1].ray_angle = data->player.rotation_angle + \
			(atan((i - (NBR_RAYS / 2)) / data->wall.projection));
		}
		i++;
	}
}

int	update(t_data *data)
{
	direction(data);
	ray_caster(data);
	render_3d(data);
	mlx_put_image_to_window(data->mlx_ptr, data->win_ptr, data->img.img_ptr, 0,
		0);
	return (0);
}

void	render_loop(t_data *data, float x, float y, int i)
{
	int	a;

	a = data->wall.strip_top;
	while (a < data->wall.strip_bottom)
	{
		if (data->wall_side == NORTH)
			my_mlx_pixel_put(&data->img, i, a, get_color(&data->texture[0], x,
					y));
		else if (data->wall_side == SOUTH)
			my_mlx_pixel_put(&data->img, i, a, get_color(&data->texture[1], x,
					y));
		else if (data->wall_side == EAST)
			my_mlx_pixel_put(&data->img, i, a, get_color(&data->texture[2], x,
					y));
		else if (data->wall_side == WEST)
			my_mlx_pixel_put(&data->img, i, a, get_color(&data->texture[3], x,
					y));
		y += TEX_SIZE / data->wall.strip_height;
		a++;
	}
}

void	render_walls(t_data *data, int i)
{
	float	x;
	float	y;
	int		a;
	float	tex_ratio;
	int		pos_y;

	if (data->ray[i].vert)
		x = (int)data->ray[i].hit_y % (int)data->tile_size;
	else
		x = (int)data->ray[i].hit_x % (int)data->tile_size;
	tex_ratio = TEX_SIZE / data->tile_size;
	x *= tex_ratio;
	if (data->wall.strip_top > 0)
		y = 0;
	else
	{
		pos_y = (data->wall.strip_height - WINDOW_HEIGHT) / 2;
		y = pos_y * (TEX_SIZE / data->wall.strip_height);
	}
	a = data->wall.strip_top;
	render_loop(data, x, y, i);
}
