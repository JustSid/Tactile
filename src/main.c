//
//  main.c
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

#include <pebble.h>
#include "tactile_layer.h"

#ifndef WHITE_ON_BLACK
#define WHITE_ON_BLACK 1
#endif

#ifndef AMPM_INDICATOR
#define AMPM_INDICATOR 1
#endif


static Window *window;
static TactileLayer *hour_layer;
static TactileLayer *minute_layer;

// Logic: http://i.imgur.com/3C1WbuL.jpg
// Bits 0-4 are the four dots in clock wise rotation
uint32_t time_lookup[12] = {
	0,
	(1 << 0),
	(1 << 1),
	(1 << 3),
	(1 << 2),
	(1 << 0) | (1 << 1),
	(1 << 1) | (1 << 3),
	(1 << 2) | (1 << 3),
	(1 << 0) | (1 << 2),
	(1 << 0) | (1 << 3),
	(1 << 1) | (1 << 2),
	(1 << 0) | (1 << 1)| (1 << 2)| (1 << 3),
};

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
	int hour = tick_time->tm_hour;
	bool pm = false;

	if(hour >= 12)
	{
		hour -= 12;
		pm = true;
	}

	uint32_t hour_state = time_lookup[hour] | (pm ? (1 << 4) : 0);

	tactile_layer_set_state(hour_layer, hour_state);
	tactile_layer_set_state(minute_layer, time_lookup[tick_time->tm_min / 5]);
}


static void window_load(Window *window)
{
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	GRect hourBounds = bounds;
	hourBounds.size.h /= 2;

	GRect minuteBounds = hourBounds;
	minuteBounds.origin.y += hourBounds.size.h;


	minute_layer = tactile_layer_create(minuteBounds);
	hour_layer = tactile_layer_create(hourBounds);
	
#if AMPM_INDICATOR
	tactile_layer_set_has_extra_dot(hour_layer, true);
#endif

#if WHITE_ON_BLACK
	tactile_layer_set_color(hour_layer, GColorWhite);
	tactile_layer_set_color(minute_layer, GColorWhite);
#else
	tactile_layer_set_color(hour_layer, GColorBlack);
	tactile_layer_set_color(minute_layer, GColorBlack);
#endif

	layer_add_child(window_layer, tactile_layer_get_layer(minute_layer));
	layer_add_child(window_layer, tactile_layer_get_layer(hour_layer));


	// Update the state immedaitely to not have the bubbles
	// empty for a bit when the watchface is activated
	time_t current = time(NULL);
	tick_handler(localtime(&current), 0);
}

static void window_unload(Window *window)
{
	tactile_layer_destroy(hour_layer);
	tactile_layer_destroy(minute_layer);
}

static void init()
{
	window = window_create();

	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});

#if WHITE_ON_BLACK
	window_set_background_color(window, GColorBlack);
#else
	window_set_background_color(window, GColorWhite);
#endif

	window_set_fullscreen(window, true);
	window_stack_push(window, true);

	tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit()
{
	window_destroy(window);
}


int main(void)
{
	init();
	app_event_loop();
	deinit();
}
