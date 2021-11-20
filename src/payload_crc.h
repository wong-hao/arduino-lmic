//https://github.com/Lora-net/sx1302_hal/blob/master/tools/payload_tools/payload_crc.c
//https://github.com/Lora-net/sx1302_hal/blob/4b42025d1751e04632c0b04160e0d29dbbb222a5/libloragw/src/loragw_sx1302.c#L1970

#pragma once

#include <stdint.h>

/* -------------------------------------------------------------------------- */
/* --- STAGE ---------------------- */


static void usage(void);

uint16_t sx1302_lora_payload_crc(const uint8_t* data, uint8_t size);

void remove_spaces(char* str);

void lora_crc16(const char data, int* crc);