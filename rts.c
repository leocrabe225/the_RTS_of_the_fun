#include "mlx.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
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
	clock_t time;
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
		while (y_from <= y_to)
		{
			x = x_from;
			while (x <= x_to)
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

	while (list)
	{
		x_from = list->pos_x - list->width;
		x_to = list->pos_x + list->width;
		y_from = list->pos_y - list->height;
		y_to = list->pos_y + list->height;
		while (y_from <= y_to)
		{
			x = x_from;
			while (x <= x_to)
			{
				data->img[y_from * WIDTH + x] = list->color;
				x++;
			}
			y_from++;
		}
		list = list->next;
	}
}

void erase_rendered_selection(t_main *data, t_selection selection)
{
	int temp;

	if (selection.x_end < selection.x_begin)
	{
		temp = selection.x_end;
		selection.x_end = selection.x_begin;
		selection.x_begin = temp;
	}
	if (selection.y_end < selection.y_begin)
	{
		temp = selection.y_end;
		selection.y_end = selection.y_begin;
		selection.y_begin = temp;
	}
	while (selection.y_begin <= selection.y_end)
	{
		selection.on = selection.x_begin;
		while (selection.on <= selection.x_end)
		{
			data->img[selection.y_begin * WIDTH + selection.on] = 0;
			selection.on++;;
		}
		selection.y_begin++;
	}
}

void render_selection(t_main *data, t_selection selection)
{
	int temp;
	clock_t secs;

	if (selection.x_end < selection.x_begin)
	{
		temp = selection.x_end;
		selection.x_end = selection.x_begin;
		selection.x_begin = temp;
	}
	if (selection.y_end < selection.y_begin)
	{
		temp = selection.y_end;
		selection.y_end = selection.y_begin;
		selection.y_begin = temp;
	}
	secs = clock();
	while (selection.y_begin <= selection.y_end)
	{
		selection.on = selection.x_begin;
		while (selection.on <= selection.x_end)
		{
			((unsigned char *)(&(data->img[selection.y_begin * WIDTH + selection.on])))[0] /= 2;
			((unsigned char *)(&(data->img[selection.y_begin * WIDTH + selection.on])))[1] = (((unsigned char *)(&(data->img[selection.y_begin * WIDTH + selection.on])))[1] + 80) / 2;
			((unsigned char *)(&(data->img[selection.y_begin * WIDTH + selection.on])))[2] /= 2;
			//data->img[selection.y_begin * WIDTH + selection.on] =(data->img[selection.y_begin * WIDTH + selection.on] / 2) + 10240;
			selection.on++;
		}
		selection.y_begin++;
	}
	printf("Time to draw selection : %lu\n", clock() - secs);
}

int loop_hook(t_main *data)
{
	clock_t secs;
	secs = clock();
	data->time = secs;
	erase_rendered_objects(data, data->object_list);
	printf("Time erase objects : %lu\n", clock() - secs);
	secs = clock();
	if (data->selection_state.on)
		erase_rendered_selection(data, data->selection_state);
	printf("Time erase selection : %lu\n", clock() - secs);
	if (data->mouse_state && !data->selection_state.on)
	{
		data->selection_state.x_begin = data->mouse_x;
		data->selection_state.x_end = data->mouse_x;
		data->selection_state.y_begin = data->mouse_y;
		data->selection_state.y_end = data->mouse_y;
		data->selection_state.on = 1;
	}
	else if (data->mouse_state)
	{
		data->selection_state.x_end = data->mouse_x;
		data->selection_state.y_end = data->mouse_y;
	}
	else
		data->selection_state.on = 0;
	data->object_list->pos_x++;
	render_objects(data, data->object_list);
	if (data->selection_state.on)
		render_selection(data, data->selection_state);
	mlx_put_image_to_window(data->mlx, data->window, data->mlx_image, 0, 0);
	secs = clock();
	printf("Time for loop : %lu\n", secs - data->time);
	return (0);
}

int button_press(int button, int x, int y, t_main *data)
{
	data->mouse_state = 1;
	return (0);
}

int button_release(int button, int x, int y, t_main *data)
{
	data->mouse_state = 0;
	return (0);
}

int button_motion(int x, int y, t_main *data)
{
	if (x < 0)
		data->mouse_x = 0;
	else if (x >= WIDTH)
		data->mouse_x = WIDTH - 1;
	else
		data->mouse_x = x;
	if (y < 0)
		data->mouse_y = 0;
	else if (y >= HEIGHT)
		data->mouse_y = HEIGHT - 1;
	else
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
	data.selection_state.on = 0;
	mlx_loop_hook(data.mlx, *loop_hook, &data);
	mlx_hook(data.window, 4, 1L << 2, *button_press, &data);
	mlx_hook(data.window, 5, 1L << 3, *button_release, &data);
	mlx_hook(data.window, 6, 1L << 13, *button_motion, &data);
	data.object_list = get_new_object(40,40, 7,10, get_color(255, 0, 0));
	data.object_list->next = get_new_object(200,350, 20,3, get_color(120, 0, 120));
	mlx_loop(data.mlx);
}
