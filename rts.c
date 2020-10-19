#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>

#define WIDTH 500
#define HEIGHT 500

typedef struct	s_object
{
	int pos_x;
	int pos_y;
	int width;
	int height;
	int color;
	void *next;
}				t_object;

typedef struct	s_selection
{
	int x_begin;
	int x_end;
	int y_begin;
	int y_end;
	int on;
}				t_selection;

typedef struct	s_main
{
	void *mlx;
	void *window;
	void *mlx_image;
	int *img;
	void ***object_map;
	int line_size;
	int mouse_state;
	int mouse_x;
	int mouse_y;
	t_selection selection_state;
	t_object *object_list;
}				t_main;

int get_color(int r, int g, int b)
{
	return (r * 256 * 256 + g * 256 + b);
}

t_object *get_new_object(int pos_x, int pos_y, int width, int height, int color)
{
	t_object *link;
	
	link = malloc(sizeof(t_object));
	link->next = 0;
	link->pos_x = pos_x;
	link->pos_y = pos_y;
	link->width = width;
	link->height = height;
	link->color = color;
	return (link);
}

void erase_rendered_objects(t_main *data, t_object *list)
{
	int x_from;
	int x_to;
	int x;
	int y_from;
	int y_to;

	while (list)
	{
		x_from = list->pos_x - list->width;
		x_to = list->pos_x + list->width;
		y_from = list->pos_y - list->height;
		y_to = list->pos_y + list->height;
		while (y_from != y_to)
		{
			x = x_from;
			while (x != x_to)
			{
				data->img[y_from * WIDTH + x] = 0;
				x++;
			}
			y_from++;
		}
		list = list->next;
	}
}

void render_objects(t_main *data, t_object *list)
{
 	int x_from;
	int x_to;
	int x;
	int y_from;
	int y_to;

	//printf("Launched\n");
	while (list)
	{
		//printf("WOW\n");
		x_from = list->pos_x - list->width;
		x_to = list->pos_x + list->width;
		y_from = list->pos_y - list->height;
		y_to = list->pos_y + list->height;
		while (y_from != y_to)
		{
			x = x_from;
			while (x != x_to)
			{
				data->img[y_from * WIDTH + x] = list->color;
				x++;
			}
			y_from++;
		}
		list = list->next;
	}

}

int loop_hook(t_main *data)
{
	erase_rendered_objects(data, data->object_list);
	data->object_list->pos_x++;
	render_objects(data, data->object_list);
	mlx_put_image_to_window(data->mlx, data->window, data->mlx_image, 0, 0);
	return (0);
}

int button_press(int button, int x, int y, t_main *data)
{
	printf("Are you there ?\n");
	data->mouse_state = 1;
	data->mouse_x = x;
	data->mouse_y = y;
	return (0);
}

int button_release(int button, int x, int y, t_main *data)
{
	data->mouse_state = 0;
	data->mouse_x = x;
	data->mouse_y = y;
	return (0);
}

int button_motion(int x, int y, t_main *data)
{
	data->mouse_x = x;
	data->mouse_y = y;
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
	data.img = (int*)mlx_get_data_addr(data.mlx_image, &garbage, &data.line_size, &garbage);
	data.object_map = malloc(sizeof(data.object_map) * (HEIGHT + 1));
	data.object_map[HEIGHT] = 0;
	loop = 0;
	while (loop != HEIGHT)
	{
		data.object_map[loop] = malloc(sizeof(*data.object_map) * (WIDTH + 1));
		loop++;
	}
	data.mouse_state = 0;
	mlx_loop_hook(data.mlx, *loop_hook, &data);
	mlx_hook(data.window, 4, 1L << 2, *button_press, &data);
	mlx_hook(data.window, 5, 1L << 3, *button_release, &data);
	mlx_hook(data.window, 6, 1L << 13, *button_motion, &data);
	data.object_list = get_new_object(40,40, 7,10, get_color(255, 0, 0));
	data.object_list->next = get_new_object(200,350, 20,3, get_color(120, 0, 120));
	mlx_loop(data.mlx);
}
