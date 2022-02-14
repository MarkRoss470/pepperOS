#pragma once

#include <stdint.h>

void checkFunction(uint8_t bus, uint8_t device, uint8_t function);
void checkDevice(uint8_t bus, uint8_t device);
void checkBus(uint8_t bus);
void checkAllBuses();

int lspci(int argc, char *argv[]);
