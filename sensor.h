#pragma once

#include <avr/io.h>

#define sensor_on(port, mask) ((port) &= ~(mask))
#define sensor_off(port, mask) ((port) |= (mask))

#define sensor_init(ddr, port, mask) ((ddr) |= (mask)); sensor_off((port), (mask));
