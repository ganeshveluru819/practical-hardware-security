/*
    This file is part of the ChipWhisperer Example Targets
    Copyright (C) 2012-2017 NewAE Technology Inc.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"
#include <stdint.h>
#include <stdlib.h>

#include "simpleserial.h"


// TVLA TEST A: multiply across all 16 bytes
uint8_t tvla_test_A(uint8_t* pt, uint8_t len)
{
    int i = 0;
    int product = 1;
    uint8_t result[16] = {0};
    
	trigger_high();

    for(i=0; i < len; i++)
    {
        product = product * pt[i];
    }
    
	trigger_low();
    
    
    for(i = 15; i >= 0; i--)
    {
        result[i] = product & 0xFF;
        product = product >> 8;
    }
    
	simpleserial_put('r', 16, result);
	return 0x00;
}

// TVLA TEST B: XOR across all 16 bytes
uint8_t tvla_test_B(uint8_t* pt, uint8_t len)
{
    int i = 0;
    int xor  = 0;
    uint8_t result[16] = {0};
    
	trigger_high();

    for(i=0; i < len; i++)
    {
        xor = xor ^ pt[i];
    }
    
	trigger_low();
    
    
    for(i = 15; i >= 0; i--)
    {
        result[i] = xor & 0xFF;
        xor = xor >> 8;
    }
    
	simpleserial_put('r', 16, result);
	return 0x00;
}

// TVLA TEST C: modular multiplication across all 16 bytes
uint8_t tvla_test_C(uint8_t* pt, uint8_t len)
{
    int i = 0;
    int res  = 1;
    uint8_t result[16] = {0};
    
	trigger_high();

    for(i=0; i < len; i++)
    {
        res = (res*pt[i])%0xFB;
    }
    
	trigger_low();
    
    
    for(i = 15; i >= 0; i--)
    {
        result[i] = res & 0xFF;
        res = res >> 8;
    }
    
	simpleserial_put('r', 16, result);
	return 0x00;
}


int main(void)
{
    platform_init();
	init_uart();
	trigger_setup();

	simpleserial_init();
    
	simpleserial_addcmd('a', 16, tvla_test_A);
    simpleserial_addcmd('b', 16, tvla_test_B);  
    simpleserial_addcmd('c', 16, tvla_test_C);
    
	while(1)
		simpleserial_get();
}