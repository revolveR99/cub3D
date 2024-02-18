/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: edupless <edupless@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 09:05:35 by zabdulza          #+#    #+#             */
/*   Updated: 2024/02/07 15:41:49 by edupless         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub.h"

static uint32_t	switch_color_bytes(uint32_t bad)
{
	uint32_t	good;

	good = ((bad & 0xff) << 24) | (((bad >> 8) & 0xff) << 16)
		| (((bad >> 16) & 0xff) << 8) | bad >> 24;
	return (good);
}

static void	fill_array(uint32_t pixel_arr[TEXTURE_HEIGHT][TEXTURE_WIDTH],
						uint32_t *pixels_raw, uint32_t width, uint32_t height)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	color;

	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			color = switch_color_bytes(pixels_raw[i * width + j]);
			pixel_arr[i][j] = color;
			j++;
		}
		i++;
	}
}

static int	load_file(uint32_t pixl_arr[TEXTURE_HEIGHT][TEXTURE_WIDTH],
				char *txt_file)
{
	mlx_texture_t	*texture;

	if (!txt_file)
		return (1);
	texture = mlx_load_png(txt_file);
	if (!texture)
	{
		dprintf(2, "%s: %s\n", txt_file, mlx_strerror(mlx_errno));
		return (1);
	}
	if (texture->height != TEXTURE_HEIGHT
		|| texture->width != TEXTURE_WIDTH)
	{
		dprintf(2, "The `%s' dimensions do not match the ones set in the"
			" header file\n", txt_file);
		mlx_delete_texture(texture);
		return (1);
	}
	fill_array(pixl_arr, (uint32_t *)texture->pixels,
		texture->width, texture->height);
	mlx_delete_texture(texture);
	return (0);
}

int	load_textures(uint32_t textures[4][TEXTURE_HEIGHT][TEXTURE_WIDTH],
		t_map_data *data)
{
	if (load_file(textures[TXT_NORTH], data->north_txt))
	{
		free_map_data(data);
		exit(1);
	}
	if (load_file(textures[TXT_SOUTH], data->south_txt))
	{
		free_map_data(data);
		exit(1);
	}
	if (load_file(textures[TXT_EAST], data->east_txt))
	{
		free_map_data(data);
		exit(1);
	}
	if (load_file(textures[TXT_WEST], data->west_txt))
	{
		free_map_data(data);
		exit(1);
	}
	return (0);
}
