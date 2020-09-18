#include "mlx.h"

#define WIDTH 500
#define HEIGHT 500



int main()
{
	void	 *mlx;
	void	 *window;
	void	*mlx_image;
	void	*img;
	void 	***object_map
	int		garbage;
	int		line_size;
	int		loop;

	mlx = mlx_init();
	window = mlx_new_window(mlx, WIDTH, HEIGHT, "A basic RTS");
	mlx_image = mlx_new_image(mlx, WIDTH, HEIGHT);
	img = mlx_get_data_addr(mlx_image, &garbage, &line_size, &garbage);
	object_map = malloc(sizeof(object_map) * (HEIGHT + 1));
	object_map[HEIGHT] = 0;
	loop = 0;
	while (loop != HEIGHT)
	{
		object_map[loop] = malloc(sizeof(*object_map) * (WIDTH + 1));
		loop++;
	} 
}
