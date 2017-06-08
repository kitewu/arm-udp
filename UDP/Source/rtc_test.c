/*********************************************************************************************
* File£º	rtc_test.c
* Author:	embest
* Desc£º	RTC_Test
* History:	
*********************************************************************************************/
/*------------------------------------------------------------------------------------------*/
/*                                     include files	                                    */
/*------------------------------------------------------------------------------------------*/
#include "2410lib.h"
#include "rtc_test.h"
char local_time[10];

/*------------------------------------------------------------------------------------------*/
/*                                     global variables                                     */
/*------------------------------------------------------------------------------------------*/
INT8T *day[8] = {" ","SUN","MON","TUE","WED","THR","FRI","SAT"};
INT32T g_nYear,g_nMonth,g_nDate,g_nWeekday,g_nHour,g_nMin,g_nSec;

volatile INT32T f_nIsRtcInt;
volatile UINT32T f_unTickCount;

/*********************************************************************************************
* name:		rtc_init
* func:		rtc init
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void rtc_init(void)
{
    rRTCCON  = rRTCCON  & ~(0xf)  | 0x1;		// No reset, Merge BCD counters, 1/32768, RTC Control enable
    
    rBCDYEAR = rBCDYEAR & ~(0xff) | TESTYEAR;
    rBCDMON  = rBCDMON  & ~(0x1f) | TESTMONTH;
    rBCDDATE = rBCDDATE & ~(0x3f) | TESTDATE;         
    rBCDDAY  = rBCDDAY  & ~(0x7)  | TESTDAY;	// SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
    rBCDHOUR = rBCDHOUR & ~(0x3f) | TESTHOUR;
    rBCDMIN  = rBCDMIN  & ~(0x7f) | TESTMIN;
    rBCDSEC  = rBCDSEC  & ~(0x7f) | TESTSEC;
    
    rRTCCON  = 0x0;								// No reset, Merge BCD counters, 1/32768, RTC Control disable    
}

char* rtc_display(void)
{
    INT32T nTmp; 
	  rRTCCON = 0x01;					// No reset, Merge BCD counters, 1/32768, RTC Control enable
    
		while(1)
		{
				if(rBCDYEAR == 0x99) 
						g_nYear = 0x1999;
				else 
						g_nYear = 0x2000 + rBCDYEAR;

				g_nMonth   = rBCDMON;
				g_nWeekday = rBCDDAY;
				g_nDate    = rBCDDATE;
				g_nHour    = rBCDHOUR;
				g_nMin     = rBCDMIN;
				g_nSec     = rBCDSEC;
				
				if(g_nSec!=nTmp)		// Same time is not display
				{
						nTmp = g_nSec;
						break;
			 }         
		}
		 local_time[0] = g_nHour;
		 local_time[1] = g_nMin;
		 local_time[2] = g_nSec;
		 local_time[3] = g_nWeekday;
		 local_time[4] = g_nMonth;
		 local_time[5] = g_nDate;
		 local_time[6] = g_nYear;
			
		uart_printf(" %02x:%02x:%02x  %10s,  %02x/%02x/%04x\r\n",g_nHour,g_nMin,g_nSec,day[g_nWeekday],g_nMonth,g_nDate,g_nYear);
		return local_time;
}




/*********************************************************************************************
* name:		rtc_display
* func:		display rtc value
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
/*
char* rtc_display(void)
{
    INT32T nTmp; 
           
    uart_printf("\n Display current Date and time: \n");

	rRTCCON = 0x01;					// No reset, Merge BCD counters, 1/32768, RTC Control enable
    uart_printf(" Press any key to exit.\n");
    while(!uart_getkey())
    {    
        while(1)
        {
            if(rBCDYEAR == 0x99) 
                g_nYear = 0x1999;
            else 
                g_nYear = 0x2000 + rBCDYEAR;

            g_nMonth   = rBCDMON;
            g_nWeekday = rBCDDAY;
            g_nDate    = rBCDDATE;
            g_nHour    = rBCDHOUR;
            g_nMin     = rBCDMIN;
            g_nSec     = rBCDSEC;
            
            if(g_nSec!=nTmp)		// Same time is not display
            {
                nTmp = g_nSec;
                break;
           }         
       }
			 hhmmss[0] = g_nHour;
			 hhmmss[1] = g_nMin;
			 hhmmss[2] = g_nSec;
			 hhmmss[3] = g_nMonth;
			 hhmmss[4] = g_nDate;
			 hhmmss[5] = g_nYear;
			 return hhmmss;
       uart_printf(" %02x:%02x:%02x  %10s,  %02x/%02x/%04x\r",g_nHour,g_nMin,g_nSec,day[g_nWeekday],g_nMonth,g_nDate,g_nYear);
   }
	rRTCCON = 0x0;					// No reset, Merge BCD counters, 1/32768, RTC Control disable(for power consumption)
    
    uart_printf("\n\n Exit display.\n");
}
*/

/*********************************************************************************************
* name:		rtc_set
* func:		set a new g_nDate & time to rtc
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void rtc_set(void)
{
    uart_printf("\n Please input 0x and Two digit then press Enter, such as 0x99.\n");

	while(1)
	{
	    uart_printf(" Year  : ");
	    g_nYear = uart_getintnum();
		if(g_nYear > 0x99 || g_nYear < 0x00 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;
	}
    
	while(1)
	{
	    uart_printf(" Month : ");
	    g_nMonth = uart_getintnum();
		if(g_nMonth > 0x12 || g_nMonth < 0x1 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;
	}

	while(1)
	{
	    uart_printf(" Date  : ");
	    g_nDate = uart_getintnum();
		if(g_nDate > 0x31 || g_nDate < 0x1 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;

	}
          
    uart_printf("\n 1:Sunday 2:Monday 3:Tuesday 4:Wednesday 5:Thursday 6:Friday 7:Saturday\n");
	while(1)
	{
	    uart_printf(" Day of week : ");
	    g_nWeekday = uart_getintnum();
		if(g_nWeekday > 0x7 || g_nWeekday < 0x1 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;

	}
    
	while(1)
	{
	    uart_printf("\n Hour  : ");
	    g_nHour = uart_getintnum();
		if(g_nHour > 0x23 || g_nHour < 0x0 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;
	}
    
	while(1)
	{
	    uart_printf(" Minute: ");
	    g_nMin = uart_getintnum();
		if(g_nMin > 0x59 || g_nMin < 0x0 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;

	}
    
	while(1)
	{
	    uart_printf(" Second: ");
	    g_nSec = uart_getintnum();
		if(g_nSec > 0x59 || g_nSec < 0x0 )
			uart_printf(" The number you input are invalid! Please input again!\n");
		else break;
	}
	    
    rRTCCON  = rRTCCON  & ~(0xf)  | 0x1;		// No reset, Merge BCD counters, 1/32768, RTC Control enable
    
    rBCDYEAR = rBCDYEAR & ~(0xff) | g_nYear;
    rBCDMON  = rBCDMON  & ~(0x1f) | g_nMonth;
    rBCDDAY  = rBCDDAY  & ~(0x7)  | g_nWeekday;	// SUN:1 MON:2 TUE:3 WED:4 THU:5 FRI:6 SAT:7
    rBCDDATE = rBCDDATE & ~(0x3f) | g_nDate;
    rBCDHOUR = rBCDHOUR & ~(0x3f) | g_nHour;
    rBCDMIN  = rBCDMIN  & ~(0x7f) | g_nMin;
    rBCDSEC  = rBCDSEC  & ~(0x7f) | g_nSec;
    
	rRTCCON  = 0x0;								//  No reset, Merge BCD counters, 1/32768, RTC Control disable    
}

/*********************************************************************************************
* name:		rtc_read
* func:		read data from rtc
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void rtc_read(void)
{
	while(1)
	{
		// Read the data from RTC registers
		if(rBCDYEAR == 0x99) 
			g_nYear = 0x1999;
		else 
	    	g_nYear = 0x2000 + rBCDYEAR;
	    	
		g_nMonth   = rBCDMON;
		g_nWeekday = rBCDDAY;
		g_nDate    = rBCDDATE;
		g_nHour    = rBCDHOUR;
		g_nMin     = rBCDMIN;
		g_nSec     = rBCDSEC;
		
		if(g_nSec != 0)
	    	break;
	}	 
}

/*********************************************************************************************
* name:		rtc_alarm_test
* func:		test rtc alarm
* para:		none
* ret:		f_nIsRtcInt = 0 : rtc not work
*	        f_nIsRtcInt = 1 : rtc work fine
* modify:
* comment:		
*********************************************************************************************/  
int rtc_alarm_test(void)  
{
    uart_printf(" RTC Alarm Test for S3C2410 \n");

    rRTCCON  = 0x01;				// No reset, Merge BCD counters, 1/32768, RTC Control enable
    rALMYEAR = rBCDYEAR ;
    rALMMON  = rBCDMON;
    rALMDATE = rBCDDATE  ;
    rALMHOUR = rBCDHOUR ;
    rALMMIN  = rBCDMIN  ;
    rALMSEC  = rBCDSEC + 2; 
    f_nIsRtcInt = 0;
    pISR_RTC = (unsigned int)rtc_int;
    rRTCALM  = 0x7f;				// Global,g_nYear,g_nMonth,Day,g_nHour,Minute,Second alarm enable
    rRTCCON  = 0x0;					// No reset, Merge BCD counters, 1/32768, RTC Control disable
    rINTMSK &= ~(BIT_RTC);

	uart_printf(" %02x:%02x:%02x\n",rBCDHOUR,rBCDMIN,rBCDSEC);        
    delay(21000);					// Delay 2.1s
	uart_printf(" %02x:%02x:%02x\n",rBCDHOUR,rBCDMIN,rBCDSEC);        

    rINTMSK |= BIT_RTC;
    rRTCCON  = 0x0;					// No reset, Merge BCD counters, 1/32768, RTC Control disable    
    return f_nIsRtcInt;
}

/*********************************************************************************************
* name:		rtc_tick_test
* func:		test rtc tick
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void rtc_tick_test(void)
{
    uart_printf("\n RTC Tick interrupt(1 Second) test for S3C2410 \n");
    uart_printf(" Press any key to exit.\n"); 
        
    pISR_TICK = (unsigned)rtc_tick;
    f_unTickCount  = 1;
    rINTMSK  &= ~(BIT_TICK); 
    rRTCCON   = 0x0;            // No reset[3], Merge BCD counters[2], BCD clock select XTAL[1], RTC Control disable[0]
    rTICNT    = (1<<7) + 127;   // Tick time interrupt enable, Tick time count value 127
                                // Period = (n + 1) / 128 second    n:Tick time count value(1~127)
    uart_getch();
    uart_printf("\n");
    rINTMSK  |= BIT_TICK;
    rRTCCON   = 0x0;            // No reset[3], Merge BCD counters[2], BCD clock select XTAL[1], RTC Control disable[0]
}

/*********************************************************************************************
* name:		rtc_check
* func:		check rtc working or not
* para:		none
* ret:		cRtcAlarm = 0:
*			cRtcAlarm = 1: ok
* modify:
* comment:		
*********************************************************************************************/
char rtc_check(void)
{
	INT8T cRtcAlarm = 0;
	INT8T cYn = 0x59;
   
	// Check RTC
	while((cYn == 0x0d)|(cYn == 0x59)|(cYn == 0x79)|(cRtcAlarm == 0))
	{ cYn = 'N';
		// Get data from keybroad
/*
		while(1)
		{
			uart_printf(" RTC Check(Y/N)? ");
			cYn = uart_getch();
			uart_printf("%c\n",cYn);
			if(cYn == 'Y' || cYn == 'y' || cYn == 'N' || cYn == 'n')
				break;
			else 
				uart_printf(" Please chose Y or N\n");
		}*/
		if((cYn == 0x0d)|(cYn == 0x59)|(cYn == 0x79))
			cRtcAlarm = rtc_alarm_test();			// Test rtc alarm
		else break;
		if (cRtcAlarm) break;
	}
	return cRtcAlarm;   
}

/*********************************************************************************************
* name:		rtc_test
* func:		rtc test main function
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
char* rtc_test(void)
{
	INT32T	nKey;
	
	// User interface
	//uart_printf("\n RTC Test Example\n"); 

	// Check if rtc work normally
	/*
	if(!rtc_check())
		uart_printf(" Please check RTC, maybe it's Wrong!. \n");
	else
	{
		//uart_printf(" 0. RTC Time Setting    1. Only RTC Display\n");
		/*
	 	while(1)
		{
			uart_printf(" Please Selet : ");
			nKey = uart_getintnum();
			if(nKey > 0x1 || nKey < 0x0)
				uart_printf(" The number you input are invalid! Please input again!\n");
			else break;
		}*/
		/*
		nKey = 0x01;
	    if(!nKey)
	    {
			rtc_init(); 
	    	rtc_set();						// Set a new g_nDate and time
	    }
	}*/
	//rtc_init(); 
  //rtc_set();						// Set a new g_nDate and time
	return rtc_display();
	/*
  while(1){
	ptr= rtc_display();							// Display current time
	uart_printf("\n %d %d %d %d %d %d -----------------\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
	delay(20000);*/
}
	
	//rtc_tick_test();

//	uart_printf(" End.\n");


/*********************************************************************************************
* name:		rtc_int
* func:		rtc interrupt handler
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void  __irq  rtc_int(void)
{
	ClearPending(BIT_RTC);   

    uart_printf(" RTC Alarm Interrupt O.K.\n");
    f_nIsRtcInt = 1;  
}

/*********************************************************************************************
* name:		rtc_tick
* func:		rtc tick
* para:		none
* ret:		none
* modify:
* comment:		
*********************************************************************************************/
void  __irq  rtc_tick(void)
{
	ClearPending(BIT_TICK);   

	uart_printf(" %03d seconds\r",f_unTickCount++);
}
