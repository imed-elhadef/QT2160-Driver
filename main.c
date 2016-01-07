/**
 * @file    main.c
 *
 *
 * @author  Imed ELHADEF
 *
 * @email     imed_elhadef@hotmail.fr
 *          
 * @date    20-05-2015
 *
 *
 * @brief    I2C communication between ARM CORTEX-A8 and AT42QT2160 PCU: ARM-CORTEX A8 @ 1.2 GHz
 */

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//*****Includes************//
#include <stdio.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "AT42QT2160.h"

#define ERREXIT(str) {printf("err %s, %s\n", str, strerror(errno));}

struct QT2160 qt2160;
int rc2;
int fdchange;
const char *fn2;
char value[2];
int QTStatus[5];
int id;

#define die_if(a, msg) do { do_die_if( a , msg, __LINE__); } while(0);
void do_die_if(int b, char* msg, int line)
{
	if(!b)
		return;
	fprintf(stderr, "Error at line %d: %s\n", line, msg);
	fprintf(stderr, "sysmsg: %s\n", strerror(errno));
	exit(1);
}

 int write_to_qt2160(struct QT2160 *e, int addr, char data )
{
	int i;
	for(i=0 ; i<16; i++)
	{
		fflush(stdout);
		die_if(qt2160_write_byte(e, addr, data), "write error");
                addr++;
	}
	fprintf(stderr, "\n\n");
	return 0;
}


 int read_from_qt2160(struct QT2160 *e, int addr, int size, int *buf)
{
	int i;
	
	for(i = 0; i < size; ++i, ++addr)
	{
		(*buf) = qt2160_read_byte(e, addr);
		//printf("%.2x ", *buf);
		buf++;
		//fflush(stdout);
	}
	//fprintf(stderr, "\n\n");
	return 0;
}

 void Init_Change_Status (void)
{
  /* export */  
  fn2 = "/sys/class/gpio/export";
  fdchange = open(fn2, O_WRONLY); if(fdchange < 0)  ERREXIT("open export")
  rc2 = write(fdchange, "147", 3); if(rc2 != 3) ERREXIT("write export")// NB: GPH2_1 --> 147 
  close(fdchange);
  
  /* direction */
  fn2 = "/sys/class/gpio/gpio147/direction";
  fdchange = open(fn2, O_RDWR);  if(fdchange < 0)  ERREXIT("open direction")
  rc2 = write(fdchange, "in", 2);if(rc2 != 2) ERREXIT("write direction")
  close(fdchange);

 /* Reading the value */
  fn2 = "/sys/class/gpio/gpio147/value";
  fdchange = open(fn2, O_RDONLY);  if(fdchange < 0)  ERREXIT("open value")
  
 }

void Read_Change_Status (void)
 {
  
  /* Read the file from the beginning */
    lseek(fdchange, 0, SEEK_SET); 
 /* Get the value */
    read(fdchange, value, 2);
  
 // if Change pin status is low
  if(value[0] == '0')
   { 
     // Change pin status low       
      read_from_qt2160(&qt2160, GENERAL_STATUS_REG, 5, QTStatus);//
      usleep(50000);
        //Reset was enabled
       if (QTStatus[0] & MASK_RESET)
           //Restart Configuration
          { 
             printf ("Restart Config!!!\n");
             //Disable slider mode 
             qt2160_write_byte(&qt2160, SLIDER_CONTROL_REG, 0x00); 
            //Configure 3 pins GPIO as output
            qt2160_write_byte(&qt2160, GPIO_DIRECTION_REG, GPIO_OUTPUT); 
            //Send calibration command---------------// 
            qt2160_write_byte(&qt2160, CALIBRATION_REG, CALIBRATION_VALUE);//
          }

       else if ( (QTStatus[1]) && (QTStatus[1] != 0xFF))
           {
               switch (QTStatus[1])
                  {
                   case 0x01:
                    printf ("Key 0 was pressed!!!\n");
                    
                   break;

                  case 0x02:
                   
                    printf ("Key 1 was pressed!!!\n");
                
                  break;

                  case 0x04:
                   
                   printf ("Key 2 was pressed!!!\n");
                   
                  break;

                  case 0x08: 
    
                   printf ("Key 3 was pressed!!!\n");
                  break;

                  case 0x10:
                   
                   printf ("Key 4 was pressed!!!\n");
                  break;

                  default :
                   printf("Invalid Value!!!\n" );

                   }
             }  
        
       else if ((QTStatus[2]) && (QTStatus[2] != 0xFF))
           {
           switch (QTStatus[2])
                  {
                   case 0x01:
                   printf ("Key 8 was pressed!!!\n");
                   break;

                   case 0x02:
                   printf ("Key 9 was pressed!!!\n");
                   break;

                   case 0x04:
                   printf ("Key 10 was pressed!!!\n");
                   break;

                   default :
                   printf("Invalid Value!!!\n" );

                   }
             
           }          
       
  }
 
 }
 
 void Unexport_Change_Status (void)

{
  // unexport //
  close(fdchange);
  fn2 = "/sys/class/gpio/unexport";
  fdchange = open(fn2, O_WRONLY); if(fdchange < 0)  ERREXIT("open unexport")
  rc2 = write(fdchange, "147", 3); if(rc2 != 3) ERREXIT("write unexport")
  close(fdchange);
 }



int main() 

{

/*------------------------------- Initialisation -------------------------------------*/  
 Init_Change_Status();//Init Change Pin
 qt2160_open("/dev/i2c/1", QT2160_ADR, &qt2160);//Open the device /dev/i2c/1

/*1-After a reset/power-up, wait for CHANGE to go low, indicating the QT2160 has initialized and is ready to
  communicate.*/

//Check the device (QT2160) communication
  do
 {
  id = qt2160_read_byte(&qt2160, CHIP_ID_REG);
 }
  while (id != QT2160_ID);
 
  printf ("Device checked with sucess!!!\n");
  
  //Disable slider mode----------- 
  qt2160_write_byte(&qt2160, SLIDER_CONTROL_REG, 0x00); 
  //Configure 3 pins GPIO OUT---------
  qt2160_write_byte(&qt2160, GPIO_DIRECTION_REG, GPIO_OUTPUT); 
 //Config Negative threshold---------
 // write_to_qt2160(&qt2160, NEGATIVE_THRESHOLD_REG, 0x0A);
  printf ("Configuration with sucess!!!\n");

  //Send calibration command---------------// 
  qt2160_write_byte(&qt2160, CALIBRATION_REG, CALIBRATION_VALUE);
  sleep(1);
 
 
  while (1)
  {
   Read_Change_Status(); 
  }

   return 0;	
}

 
