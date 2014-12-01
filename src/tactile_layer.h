//
//  tactile_layer.h
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

#ifndef _TACTILE_LAYER_H_
#define _TACTILE_LAYER_H_

#include <pebble.h>

typedef struct
{
	Layer *layer;
	uint32_t state;
	GColor color;
	bool extraDot;
} TactileLayer;

TactileLayer *tactile_layer_create(GRect bounds);
void tactile_layer_destroy(TactileLayer *tactile);

Layer *tactile_layer_get_layer(TactileLayer *tactile);
void tactile_layer_set_state(TactileLayer *tactile, uint32_t state);
void tactile_layer_set_color(TactileLayer *tactile, GColor color);
void tactile_layer_set_has_extra_dot(TactileLayer *tactile, bool extraDot);

#endif /* _TACTILE_LAYER_H_ */
