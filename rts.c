#include "mlx.h"
#include <stdlib.h>

#define WIDTH 500
#define HEIGHT 500

typedef struct	s_main
{
	void *mlx;
	void *window;
	void *mlx_image;
	void *img;
	void ***object_map;
	int line_size;
}				t_main;

int loop_hook(t_main *data)
{
	mlx_put_image_to_window(data->mlx, data->window, data->mlx_image, 0, 0);
	return (0);
}

int main()
{
	t_main data;
	int		garbage;
	int		loop;

	data.mlx = mlx_init();
	data.window = mlx_new_window(data.mlx, WIDTH, HEIGHT, "A basic RTS");
	data.mlx_image = mlx_new_image(data.mlx, WIDTH, HEIGHT);
	data.img = mlx_get_data_addr(data.mlx_image, &garbage, &data.line_size, &garbage);
	data.object_map = malloc(sizeof(data.object_map) * (HEIGHT + 1));
	data.object_map[HEIGHT] = 0;
	loop = 0;
	while (loop != HEIGHT)
	{
		data.object_map[loop] = malloc(sizeof(*data.object_map) * (WIDTH + 1));
		loop++;
	}
	mlx_loop_hook(data.mlx, *mlx_loop, &data);
	mlx_loop(data.mlx);
}
