//
//  tactile_layer.c
//  Tactile
//
//  Created by Sidney Just
//  Copyright (c) 2014 by Sidney Just
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "tactile_layer.h"

static void tactile_update_proc(Layer *layer, GContext *context)
{
	TactileLayer *tactile = layer_get_data(layer);
	GRect rect = layer_get_bounds(layer);

	int lowset = rect.size.h > rect.size.w ? rect.size.w : rect.size.h;
	const int padding = 5;

	GPoint center = GPoint(rect.size.w / 2, rect.size.h / 2);

	int width  = (lowset - padding * 3) / 2;
	int radius = width / 2;

	GPoint point;
	point.x = center.x - (radius + padding);
	point.y = center.y - (radius + padding);

	graphics_context_set_fill_color(context, tactile->color);
	graphics_context_set_stroke_color(context, tactile->color);

	for(int i = 0; i < 4; i ++)
	{
		if(tactile->state & (1 << i))
			graphics_fill_circle(context, point, radius);
		else
			graphics_draw_circle(context, point, radius);

		point.x += width + padding;
		
		if((i % 2) == 1)
		{
			point.x = center.x - (radius + padding);
			point.y += width + padding;
		}
	}

	if(tactile->extraDot)
	{
		point.x = center.x - (radius + padding);
		point.y = center.y - (radius + padding);

		point.x += width * 2 + padding;
		point.y += width + radius / 2 + padding;

		radius /= 2;

		if(tactile->state & (1 << 4))
			graphics_fill_circle(context, point, radius);
		else
			graphics_draw_circle(context, point, radius);
	}
}


TactileLayer *tactile_layer_create(GRect bounds)
{
	Layer *layer = layer_create_with_data(bounds, sizeof(TactileLayer));

	if(layer)
	{
		layer_set_update_proc(layer, tactile_update_proc);

		TactileLayer *tactile = layer_get_data(layer);
		tactile->state = 0;
		tactile->layer = layer;
		tactile->color = GColorWhite;
		tactile->extraDot = false;

		return tactile;
	}

	return NULL;
}
void tactile_layer_destroy(TactileLayer *tactile)
{
	layer_destroy(tactile->layer);
}


Layer *tactile_layer_get_layer(TactileLayer *tactile)
{
	return tactile->layer;
}

void tactile_layer_set_state(TactileLayer *tactile, uint32_t state)
{
	if(tactile->state != state)
	{
		tactile->state = state;
		layer_mark_dirty(tactile->layer);
	}
}
void tactile_layer_set_color(TactileLayer *tactile, GColor color)
{
	tactile->color = color;
	layer_mark_dirty(tactile->layer);
}
void tactile_layer_set_has_extra_dot(TactileLayer *tactile, bool extraDot)
{
	tactile->extraDot = extraDot;
	layer_mark_dirty(tactile->layer);
}
