/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include <stdlib.h>
//#define TAB_SIZE 889
#define TAB_SIZE 38

volatile uint16_t* frequency = (uint16_t*) 0x40000000; //16 bits!
volatile uint8_t* volume = (uint8_t*) 0x40030000; //3 bits! volume < 8

//const uint16_t frequencies[TAB_SIZE] = {568, 536, 1080, 808, 672, 1072, 800,
//										679, 672, 568, 1136, 856, 720, 712,
//										848, 711, 720, 216, 536, 1072, 808,
//										672, 1080, 799, 680, 672, 640, 720,
//										720, 808, 800, 855, 896, 1072, 264}; //pacman
/*const uint16_t frequencies[TAB_SIZE] = {0, 128, 8, 16, 8, 0, 8, 0, 552, 8, 8, 8, 136,
									16, 104, 104, 104, 104, 8, 16, 8, 0, 8, 0, 136, 8,
									8, 8, 136, 16, 696, 208, 208, 208, 8, 1104, 8, 0, 8,
									0, 136, 8, 8, 8, 136, 1240, 104, 104, 104, 104, 8,
									1656, 8, 0, 8, 0, 176, 8, 128, 8, 176, 176, 176, 176,
									8, 224, 232, 232, 1104, 1104, 1104, 1104, 1104, 216,
									1104, 1104, 8, 8, 344, 8, 208, 208, 208, 208, 208, 232,
									208, 208, 552, 192, 200, 192, 200, 192, 0, 464, 464, 552,
									736, 736, 736, 736, 8, 0, 368, 8, 8, 8, 8, 736, 736, 696, 136, 136, 8, 552, 552, 552, 8, 0, 8, 520, 520, 8, 8, 520, 136, 136, 136, 552, 584, 208, 624, 624, 624, 624, 216, 8, 0, 8, 8, 136, 136, 552, 552, 8, 8, 520, 416, 552, 16, 0, 8, 520, 624, 8, 8, 208, 208, 736, 208, 152, 152, 696, 1104, 832, 32, 1040, 80, 8, 1104, 8, 0, 136, 1240, 104, 104, 104, 104, 176, 1656, 8, 0, 8, 0, 176, 8, 128, 8, 176, 176, 176, 176, 216, 216, 232, 1104, 280, 1104, 352, 1104, 1104, 216, 224, 216, 280, 0, 352, 0, 208, 208, 624, 208, 232, 8, 208, 0, 192, 200, 192, 192, 192, 192, 8, 464, 552, 0, 736, 736, 736, 736, 8, 0, 368, 8, 8, 8, 736, 736, 136, 696, 136, 136, 8, 552, 552, 552, 8, 0, 520, 520, 8, 8, 520, 520, 416, 136, 552, 560, 208, 624, 624, 616, 8, 624, 208, 8, 152, 8, 136, 136, 136, 136, 8, 552, 312, 520, 416, 416, 416, 8, 520, 832, 832, 8, 208, 208, 0, 736, 696, 696, 184, 8, 0, 8, 0, 8, 0, 8, 176, 8, 176, 176, 176, 176, 176, 8, 696, 464, 552, 280, 520, 352, 8, 216, 224, 216, 216, 280, 0, 0, 344, 352, 0, 8, 8, 8, 0, 8, 56, 8, 0, 8, 176, 176, 880, 176, 176, 8, 8, 552, 120, 696, 552, 232, 392, 152, 8, 120, 8, 152, 0, 0, 464, 464, 8, 696, 696, 624, 624, 552, 552, 696, 696, 8, 8, 624, 136, 0, 8, 552, 88, 696, 696, 624, 624, 552, 552, 696, 96, 96, 8, 928, 928, 8, 16, 1040, 96, 832, 832, 552, 832, 416, 832, 104, 312, 8, 8, 208, 152, 624, 624, 96, 96, 104, 416, 416, 416, 416, 416, 104, 416, 416, 416, 416, 416, 416, 416, 0, 128, 8, 1104, 832, 0, 1040, 0, 136, 8, 8, 8, 136, 1240, 104, 104, 104, 104, 176, 1656, 8, 0, 8, 0, 176, 8, 128, 8, 176, 176, 176, 176, 8, 224, 232, 1104, 1104, 1104, 352, 1104, 1104, 1104, 224, 216, 8, 1112, 352, 8, 208, 208, 208, 208, 232, 8, 208, 8, 192, 192, 200, 192, 192, 192, 8, 464, 552, 0, 736, 736, 736, 736, 8, 0, 368, 8, 8, 8, 736, 736, 696, 696, 136, 136, 8, 552, 552, 552, 8, 0, 520, 520, 8, 8, 520, 520, 136, 136, 8, 552, 624, 624, 624, 624, 624, 624, 624, 8, 152, 8, 136, 136, 0, 552, 552, 8, 8, 416, 0, 0, 8, 0, 208, 624, 624, 8, 208, 208, 208, 736, 624, 152, 464, 1104, 0, 832, 8, 0, 136, 8, 8, 8, 8, 1240, 1240, 104, 104, 104, 8, 176, 0, 1640, 8, 0, 8, 176, 0, 8, 8, 176, 176, 176, 176, 216, 216, 232, 1104, 1104, 1104, 1104, 1104, 1104, 216, 224, 216, 280, 1096, 352, 1096, 208, 208, 208, 208, 232, 616, 208, 0, 192, 200, 192, 200, 192, 192, 464, 464, 552, 0, 736, 736, 736, 736, 8, 0, 184, 8, 8, 8, 736, 736, 696, 696, 624, 136, 552, 552, 552, 552, 8, 0, 520, 520, 8, 8, 520, 136, 416, 136, 552, 584, 624, 624, 624, 624, 8, 616, 208, 8, 152, 8, 136, 136, 136, 136, 552, 552, 208, 520, 416, 416, 0, 8, 832, 832, 832, 8, 208, 208, 0, 736, 696, 184, 0, 0, 0, 8, 0, 0, 0, 8, 176, 8, 176, 736, 176, 16, 176, 8, 464, 464, 552, 552, 520, 352, 8, 216, 224, 216, 280, 8, 8, 0, 352, 8, 0, 8, 0, 8, 0, 8, 0, 8, 176, 8, 176, 880, 176, 176, 176, 8, 528, 552, 696, 176, 552, 232, 160, 152, 464, 120, 8, 152, 8, 0, 464, 192, 8, 696, 0, 624, 8, 552, 96, 696, 88, 8, 8, 624, 8, 0, 552, 552, 8, 696, 8, 624, 8, 552, 96, 696, 96, 8, 8, 928, 136, 0, 0, 1040, 96, 832, 832, 824, 8, 416, 832, 312, 104, 8, 8, 208, 8, 624, 624, 96, 96, 104, 0, 416, 416, 416, 416, 520, 416, 416, 416, 416, 416, 416, 416, 120, 392, 696, 1104, 832, 8, 1040, 16, 8, 8, 8, 8, 136, 1240, 104, 104, 104, 104, 176, 1656, 1656, 32, 0, 88, 176, 8, 128, 8, 176, 176, 176, 176, 1392, 224, 232, 1104, 280, 1104, 1104, 1104, 216, 216, 1104, 216, 8, 1096, 352, 0, 208, 208, 624, 208, 232, 8, 208, 0, 192, 192, 8, 192, 192, 192, 8, 464, 552, 0, 736, 736, 736, 736, 8, 0, 184, 8, 8, 8, 736, 736, 696, 696, 136, 136, 8, 557, 0};
*/
const uint16_t frequencies[TAB_SIZE] = {300, 300, 299, 300, 400, 300, 400, 400,
										300, 300, 300, 300, 400, 400, 400, 400, 500,
										500, 600, 600, 700, 800, 800, 900, 1000, 1000,
										1100, 1100, 1100, 1100, 1100, 1100, 1100, 1100,
										1100, 1100, 1100, 1028};

int main()
{
    init_platform();
    *volume = 1;
//    srand(12398429);
//    //CLK IS SET TO 100 MHz
//    for(int i =0; i<=750; i++){
//        *frequency = rand()%750;
//        for(int j = 0; j<=(1000000); j++){
//            asm volatile("nop");
//        }
//    }

    // for(int i =0; i<=750; i++){
    //     *frequency = 750 -i;
    //     for(int j = 0; j<=(1000000); j++){
    //         asm volatile("nop");
    //     }
    // }
    int cycles = 30000;
     for(int i =0; i<=7500; i++){
         *frequency = frequencies[i%TAB_SIZE];
         for(int j = 0; j<=(cycles); j++){
             asm volatile("nop");
         }
     }
    xil_printf("\nFrequency set to %d Hz\n",*frequency);
    xil_printf("Volume set to %d\n", *volume);


    cleanup_platform();
    return 0;
}
