//============================================================================
// Name        : interrupt_controller.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "csr_map.h"
#include <stdlib.h>
#include <stdio.h>
#include <poll.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#include "hwlib.h"
#include "socal/socal.h"
#include "socal/hps.h"
#include "socal/alt_gpio.h"
using namespace std;

#define HW_REGS_BASE ( ALT_STM_OFST )
#define HW_REGS_SPAN ( 0x04000000 )
#define HW_REGS_MASK ( HW_REGS_SPAN - 1 )

#define USER_IO_DIR     (0x01000000)
#define BIT_LED         (0x01000000)
#define BUTTON_MASK     (0x02000000)
struct altera_poi {
  unsigned int data;
  unsigned int direction;
  unsigned int irq_mask;
  unsigned int edge_capture;
  unsigned int outset;
  unsigned int outclear;

  unsigned short off_data = 0;
  unsigned short off_direction = 1;
  unsigned short off_irq = 2;
  unsigned short off_edge = 3;
  unsigned short off_outset = 4;
  unsigned short off_outclear = 5;
};

#define LED 0xFF203000
#define PB 0xFF205000
#define SW 0xFF200000

int main() {

	tcCSRMap *mpSW = new tcCSRMap(SW);
	tcCSRMap *mpPB = new tcCSRMap(PB);

	tcCSRMap *mpHPS = new tcCSRMap(HW_REGS_BASE);

    int fd = open("/dev/fpgaint0", O_RDWR);
    if (fd < 0) {
        printf("Failed to open device %s\n", "/dev/fpgaint0");
        return 1;
    }
    int     rc = 0;
    int     nfds = 1;
    int     timeout = -1;
    char    buf;
    struct  pollfd fds[1];
    struct timespec current;

    while (1)
    {

        memset(fds, 0, sizeof(fds));
        fds[0].fd = fd;
        fds[0].events = POLLIN;
        fds[0].revents = 0;

        rc = poll(fds, nfds, timeout);
        if (rc < 0)
        {
           printf("poll returned failure\n");
           return -1;
        }
        else
        {
             if (fds[0].revents & POLLIN)
             {
                 read(fds[0].fd, &buf, 1);
                 clock_gettime(CLOCK_REALTIME, &current);
                 printf("%ld.%.9lu secs: triggered by interrupt %hhu\n", current.tv_sec, current.tv_nsec, buf);
                 cout << "SW-data " << mpSW->ReadCtrlReg(0) << endl; // prints !!!Hello World!!!
           		cout << "PB-data " << mpPB->ReadCtrlReg(0) << endl; // prints !!!Hello World!!!

           		cout << "SW-dir " << mpSW->ReadCtrlReg(1) << endl; // prints !!!Hello World!!!
                cout << "PB-dir " << mpPB->ReadCtrlReg(1) << endl; // prints !!!Hello World!!!

                cout << "SW-irq " << mpSW->ReadCtrlReg(2) << endl; // prints !!!Hello World!!!
                cout << "PB-irq " << mpPB->ReadCtrlReg(2) << endl; // prints !!!Hello World!!!


                cout << "SW-edge " << mpSW->ReadCtrlReg(3) << endl; // prints !!!Hello World!!!
                cout << "PB-edge " << mpPB->ReadCtrlReg(3) << endl; // prints !!!Hello World!!!
                mpPB->WriteCtrlReg(3,0xF);
                alt_clrbits_word( ( virtual_base + ( ( uint32_t )( ALT_GPIO1_SWPORTA_DR_ADDR ) & ( uint32_t )( HW_REGS_MASK ) ) ), BIT_LED );
             }
             else
             {
            	 printf("didn't receive POLLIN\n");
             }
         }
          // prints !!!Hello World!!!
    }
	return 0;
}
