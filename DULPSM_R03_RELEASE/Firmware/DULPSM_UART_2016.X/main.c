#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "i2c2.h"
#include "delay.h"
#include "adc.h"
#include "LMP91000.h"
#include "UART2.h"
#include "EEPROM.h"
#include "RTCC.h"
#include "Si7021.h"
#include "PPB.h"

// FBS
#pragma config BWRP = OFF               // Boot Segment Write Protect (Disabled)
#pragma config BSS = OFF                // Boot segment Protect (No boot program flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Write Protect (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Protect (No Protection)

// FOSCSEL
#pragma config FNOSC = FRC              // Oscillator Select (Fast RC Oscillator (FRC))
#pragma config SOSCSRC = ANA            // SOSC Source Type (Analog Mode for use with crystal)
#pragma config LPRCSEL = HP             // LPRC Oscillator Power and Accuracy (High Power, High Accuracy Mode)
#pragma config IESO = OFF               // Internal External Switch Over bit (Internal External Switchover mode disabled (Two-speed Start-up disabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = IO            // CLKO Enable Configuration bit (Port I/O enabled (CLKO disabled))
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary Oscillator configured for high-power operation)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS8192           // Watchdog Timer Postscale Select bits (1:8192)
#pragma config FWPSA = PR32             // WDT Prescaler bit (WDT prescaler ratio of 1:32)
#pragma config FWDTEN = SWON            // Watchdog Timer Enable bits (WDT controlled with the SWDTEN bit setting)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected(windowed WDT disabled))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware, SBOREN bit disabled)
#pragma config PWRTEN = OFF             // Power-up Timer Enable bit (PWRT disabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Use Default SCL1/SDA1 Pins For I2C1)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (RA5 input pin disabled, MCLR pin enabled)

// FICD
#pragma config ICS = PGx3               // ICD Pin Placement Select bits (EMUC/EMUD share PGC3/PGD3)

char Buffer[100];

void Port_init(void)
{
    ANSA = 0b0000000000010000;
    ANSB = 0x0000;

    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    TRISAbits.TRISA4 = 1;
//    TRISAbits.TRISA5 = 1;
    TRISAbits.TRISA6 = 1;
    TRISAbits.TRISA7 = 1;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 1;
//    TRISBbits.TRISB2 = 1;
//    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 1;
//    TRISBbits.TRISB5 = 1;
//    TRISBbits.TRISB6 = 1;
    TRISBbits.TRISB7 = 1;
//    TRISBbits.TRISB8 = 1;
//    TRISBbits.TRISB9 = 1;
    TRISBbits.TRISB10 = 1;
    TRISBbits.TRISB11 = 1;
    TRISBbits.TRISB12 = 1;
    TRISBbits.TRISB13 = 1;
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB15 = 0;
}

void Oscillator_init(void)
{
    RCONbits.SWDTEN = 1;
}

void Unlock(unsigned int Security_Code)
{
    long unsigned int i;
    char user_input;

    UART2_xmits("Enter Unlock Code: ");
    
    i = 0;
    while(i < 100)
    {
        Buffer[i] = '\0';
        i++;
    }

    i = 0;
    while(1)
    {
        if(IFS1bits.U2RXIF)
        {
            IFS1bits.U2RXIF = 0;
            user_input = U2RXREG;
            if(user_input == '\r')
            {
                break;
            }
            else;
            Buffer[i] = user_input;
            i++;
        }
    }

    if(Security_Code != (atoi(Buffer)))
    {
        UART2_xmits("\r\n");
        while(1);
    }
    else;
}

void UART_Entry(void)
{
    unsigned int i;
    char user_input;
    
    ClrWdt();
    i = 0;
    while(i < 100)
    {
        Buffer[i] = '\0';
        i++;
    }

    i = 0;
    while(i < 100)
    {    
        if(IFS1bits.U2RXIF)
        {            
            IFS1bits.U2RXIF = 0;
            user_input = U2RXREG;
            UART2_xmitc(user_input);
            if(user_input == '\r')
            {
                break;
            }
            else
            {
                Buffer[i] = user_input;
            }

            i++;
        }
    }
}

/**************************************************************************Main**********************************************************************/   
int main(void)
{
    unsigned long int i;
    unsigned long int j;
    unsigned long int k;
    char Serial_Number_[13];   
    char Part_Number_[7];
    unsigned long int Part_Number;
    char Gas_[10];
    char Date_Code_[5];
    char Sensitivity_Code_[8];
    char user_input;
    unsigned int T;
    unsigned int RH;
    double T_float;
    double RH_float;
    int Temperature_Offset_x1000;
    unsigned long int counter = 0;
    
    unsigned int LMP91000_B01;
    unsigned int LMP91000_B10;
    unsigned int LMP91000_B11;
    unsigned int LMP91000_B12;

    unsigned int Security_Code = 12345;

    unsigned int ADC_OC;
    unsigned int ADC_Zero;
    unsigned int T_Zero;
    unsigned int RH_Zero;
    unsigned int ADC_Span;
    unsigned int T_Span;
    unsigned int RH_Span;
    unsigned int PPM_Span_x100;
    unsigned int Temp_PPM_Span_x100;
    int Range;
    unsigned int ADC;
    double PPB;   
    int nA_per_PPM_x100;   
    unsigned int Readout_delay = 0;
    unsigned int YRTEN;
    unsigned int YRONE;
    unsigned int MTHTEN;
    unsigned int MTHONE;
    unsigned int DAYTEN;
    unsigned int DAYONE;
    unsigned int HRTEN;
    unsigned int HRONE;
    unsigned int MINTEN;
    unsigned int MINTEN_old;
    unsigned int MINONE;
    unsigned int MINONE_old;
    unsigned int SECTEN;
    unsigned int SECTEN_old;
    unsigned int SECONE;
    unsigned int SECONE_old;

    I2C2_init();
    Port_init();
    UART2_init();
    ADC_init();
    Oscillator_init();
/**************************************************************Power-On Startup Sequence**************************************************************/
    if(RCONbits.POR | RCONbits.BOR)
    {
        RCONbits.POR = 0;
        RCONbits.BOR = 0;
        
        RTCC_init();
        RTCC_start();

        ADC_OC = readNVM(12);
        Temperature_Offset_x1000 = readNVM(20);
        ADC_Zero = readNVM(30);
        T_Zero = readNVM(32);
        RH_Zero = readNVM(34);
        ADC_Span = readNVM(40);
        T_Span = readNVM(42);
        RH_Span = readNVM(44);
        PPM_Span_x100 = readNVM(46);
        LMP91000_B01 = readNVM(50);
        LMP91000_B10 = readNVM(52);
        LMP91000_B11 = readNVM(54);
        LMP91000_B12 = readNVM(56);
        nA_per_PPM_x100 = readNVM(70);
  
        i = 0;
        while(i < 13)
        {
            Serial_Number_[i] = readNVM(300 + (i*2));  
            i++;
        }
        i = 0;
        while(i < 7)
        {
            Part_Number_[i] = readNVM(330 + (i*2));  
            i++;
        }
        Part_Number = atol(Part_Number_);
        i = 0;
        while(i < 10)
        {
            Gas_[i] = readNVM(350 + (i*2));  
            i++;
        }
        i = 0;
        while(i < 5)
        {
            Date_Code_[i] = readNVM(370 + (i*2));  
            i++;
        }
        i = 0;
        while(i < 8)
        {
            Sensitivity_Code_[i] = readNVM(380 + (i*2));  
            i++;
        }

        LMP91000_Unlock();
        LMP91000_Write_0x10(LMP91000_B10);
        LMP91000_Write_0x11(LMP91000_B11);
        LMP91000_Write_0x12(LMP91000_B12);                                                                     
    }
    else;

    if(RCONbits.WDTO)
    {
        UART2_xmits("\r\n");
        
        U2MODEbits.UARTEN = 0;
        U2MODEbits.USIDL = 0;
        U2MODEbits.WAKE = 1;
        U2BRG = 25;	
        IEC1bits.U2RXIE = 1;
        U2MODEbits.UARTEN = 1;	
        U2STAbits.UTXEN = 1;
        RCONbits.WDTO = 0;
        RCONbits.SWDTEN = 0;
        delay(1000);
        Sleep();
        while(1);
    }
    else; 
/************************************************************************Run Mode********************************************************************/    
    while(1)
    {      
        if(counter > 0)
        {
            UART2_xmits("Enter output interval(1, 10, 60, 600): ");
            UART_Entry();

            Readout_delay = atoi(Buffer);
            UART2_xmits("\r\n");
        }
        else
        {
            SECONE_old = 0;
            SECTEN_old = 0;
            MINONE_old = 0;
            MINTEN_old = 0;
            Readout_delay = 0;
        }

        do
        {          
            ADC = ADC_Read(16);
            T = Si7021_Read_T();
            T_float = Si7021_Calculate_T(T, Temperature_Offset_x1000);  
            RH = Si7021_Read_RH();
            RH_float = Si7021_Calculate_RH(RH);
            if(RH_float > 100)
            {
                RH_float = 100;
            }
            else if(RH_float < 0)
            {
                RH_float = 0;
            }
            else;
            PPB = PPB_Calculation(ADC, ADC_Zero, nA_per_PPM_x100, T_float, Part_Number);

            UART2_xmits(Serial_Number_);  
            
            ltoa(Buffer, (long int)PPB, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmits(Buffer);
            
            itoa(Buffer, (int)T_float, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmits(Buffer);
            
            utoa(Buffer, (unsigned int)RH_float, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmits(Buffer);
                        
            utoa(Buffer, ADC, 10);
            delay(1000);  
            UART2_xmits(", ");
            UART2_xmits(Buffer);
            
            utoa(Buffer, T, 10);              
            delay(1000);
            UART2_xmits(", ");
            UART2_xmits(Buffer);

            utoa(Buffer, RH, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmits(Buffer);

            while(1)
            {               
                RCFGCALbits.RTCPTR = 0;
                MINTEN = RTCVAL;
                MINTEN = (0b0000000000000111) & (MINTEN >> 12);
                RCFGCALbits.RTCPTR = 0;
                MINONE = RTCVAL;
                MINONE = (0b0000000000001111) & (MINONE >> 8);
                RCFGCALbits.RTCPTR = 0;
                SECTEN = RTCVAL;
                SECTEN = (0b0000000000000111) & (SECTEN >> 4);
                RCFGCALbits.RTCPTR = 0;
                SECONE = RTCVAL;
                SECONE = (0b0000000000001111) & (SECONE >> 0);
                               
                if(Readout_delay < 1)    
                {
                    break;
                }
                else if(Readout_delay == 1)
                {
                    if(SECONE != SECONE_old) 
                    {
                        SECONE_old = SECONE;
                        break;
                    }
                }
                else if(Readout_delay < 60)
                {
                    if(SECTEN != SECTEN_old) 
                    {
                        SECTEN_old = SECTEN;
                        break;
                    }
                }
                else if(Readout_delay < 600)
                {
                    if(MINONE != MINONE_old) 
                    {
                        MINONE_old = MINONE;
                        break;
                    }
                }
                else
                {
                    if(MINTEN != MINTEN_old) 
                    {
                        MINTEN_old = MINTEN;
                        break;
                    }
                }                           
                ClrWdt();               
            }

            RCFGCALbits.RTCPTR = 3;
            YRTEN = RTCVAL;
            YRTEN = (0b0000000000001111) & (YRTEN >> 4);
            RCFGCALbits.RTCPTR = 3;
            YRONE = RTCVAL;
            YRONE = (0b0000000000001111) & (YRONE >> 0);
            RCFGCALbits.RTCPTR = 2;
            MTHTEN = RTCVAL;
            MTHTEN = (0b0000000000000001) & (MTHTEN >> 12);
            RCFGCALbits.RTCPTR = 2;
            MTHONE = RTCVAL;
            MTHONE = (0b0000000000001111) & (MTHONE >> 8);
            RCFGCALbits.RTCPTR = 2;
            DAYTEN = RTCVAL;
            DAYTEN = (0b0000000000000011) & (DAYTEN >> 4);
            RCFGCALbits.RTCPTR = 2;
            DAYONE = RTCVAL;
            DAYONE = (0b0000000000001111) & (DAYONE >> 0);
            RCFGCALbits.RTCPTR = 1;
            HRTEN = RTCVAL;
            HRTEN = (0b0000000000000011) & (HRTEN >> 4);
            RCFGCALbits.RTCPTR = 1;
            HRONE = RTCVAL;
            HRONE = (0b0000000000001111) & (HRONE >> 0);
                           
            utoa(Buffer, DAYTEN, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmitc(Buffer[0]);
            utoa(Buffer, DAYONE, 10);
            delay(1000);
            UART2_xmits(Buffer);                
            utoa(Buffer, HRTEN, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmitc(Buffer[0]);
            utoa(Buffer, HRONE, 10);
            delay(1000);
            UART2_xmits(Buffer);               
            utoa(Buffer, MINTEN, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmitc(Buffer[0]);
            utoa(Buffer, MINONE, 10);
            delay(1000);
            UART2_xmits(Buffer);               
            utoa(Buffer, SECTEN, 10);
            delay(1000);
            UART2_xmits(", ");
            UART2_xmitc(Buffer[0]);
            utoa(Buffer, SECONE, 10);
            delay(1000);
            UART2_xmits_NewLine(Buffer);
            
            if(IFS1bits.U2RXIF)
            {
                IFS1bits.U2RXIF = 0;
                user_input = U2RXREG;
                if(user_input == 'r')
                {
                    while(1);
                }
                else if(user_input == 'R')
                {
                    while(1);
                }
                else;
            }  
        }while(counter > 0);
    
    counter++;    
    
    user_input = U2RXREG;
/**************************************************************Set Factory Span***********************************************************************/     
    if(user_input == 'S')
    {
        Unlock(Security_Code);

        UART2_xmits("\r\nEnter span gas value in PPM: ");

        UART_Entry();

        Temp_PPM_Span_x100 = (unsigned int)(100 * atof(Buffer));
        Temp_PPM_Span_x100 = Temp_PPM_Span_x100 & 0xFFFF;
        if(Temp_PPM_Span_x100 > 60000)
        {
            UART2_xmits_NewLine("\r\nError, span not saved ");
            while(1); 
        }
        else if(Temp_PPM_Span_x100 < 10)
        {
            UART2_xmits_NewLine("\r\nError, span not saved ");
            while(1); 
        }
        else;
  
        UART2_xmits("\r\nSetting span...");
        
        T = Si7021_Read_T();
        T_float = Si7021_Calculate_T(T, Temperature_Offset_x1000);
        if(T_float > 30)
        {
            UART2_xmits_NewLine("Error, span not saved ");
            while(1); 
        }
        else if(T_float < 0)
        {
            UART2_xmits_NewLine("Error, span not saved ");
            while(1); 
        }
        else;

        ADC = ADC_Read_Oversample();
        if(ADC > 64535)
        {
            UART2_xmits_NewLine("Error, span not saved ");
            while(1); 
        }
        else if(ADC < 1000)
        {
            UART2_xmits_NewLine("Error, span not saved ");
            while(1); 
        }
        else;
        
        if((Part_Number == 110102))
        {
            Range = 20;
        }
        else if((Part_Number == 110303) | (Part_Number == 110601) | (Part_Number == 110202) | (Part_Number == 110801))
        {
            Range = 5;
        }
        else if((Part_Number == 110501) | (Part_Number == 110401) | (Part_Number == 110901) | (Part_Number == 999036))
        {
            Range = -5;
        }
        else
        {
            Range = 0;
        }
       
        if((Part_Number == 110102) | (Part_Number == 110202) | (Part_Number == 110601) | (Part_Number == 110801) | (Part_Number == 110303))
        {    
            i = 0;
            while(i < 65535)
            { 
                PPB = PPB_Calculation(ADC, ADC_Zero, nA_per_PPM_x100, T_float, Part_Number);
                if((10*(double)Temp_PPM_Span_x100) < (PPB - Range))
                {
                    nA_per_PPM_x100 = nA_per_PPM_x100 + 1; 
                }
                else if((10*(double)Temp_PPM_Span_x100) > (PPB + Range))
                {
                    nA_per_PPM_x100 = nA_per_PPM_x100 - 1;
                }
                else
                {
                    writeNVM(70,nA_per_PPM_x100);

                    break;
                }
                ClrWdt();
                i++;
            }       
        }
        else if((Part_Number == 110501) | (Part_Number == 110401) | (Part_Number == 110901) | (Part_Number == 999036))
        {
            i = 0;
            while(i < 65535)
            { 
                PPB = PPB_Calculation(ADC, ADC_Zero, nA_per_PPM_x100, T_float, Part_Number);
                if((10*(double)Temp_PPM_Span_x100) > (PPB - Range))
                {
                    nA_per_PPM_x100 = nA_per_PPM_x100 + 1; 
                }
                else if((10*(double)Temp_PPM_Span_x100) < (PPB + Range))
                {
                    nA_per_PPM_x100 = nA_per_PPM_x100 - 1;
                }
                else
                {
                    writeNVM(70,nA_per_PPM_x100);

                    break;
                }
                ClrWdt();
                i++;
            }           
        }
        else
        {
            UART2_xmits_NewLine("Error, span not saved ");
            while(1);
        }       
        ADC_Span = ADC;

        writeNVM (40,ADC_Span);
        
        T_Span = T;

        writeNVM(42,T_Span);

        RH_Span = Si7021_Read_RH();

        writeNVM(44,RH_Span);
        
        PPM_Span_x100 = Temp_PPM_Span_x100;
        writeNVM(46,PPM_Span_x100);
        
        UART2_xmits_NewLine("done");
    }
/**************************************************************Set Factory Zero***********************************************************************/    
    else if(user_input == 'Z')
    {
        Unlock(Security_Code);

        UART2_xmits("\r\nSetting zero...");
        
        T = Si7021_Read_T();
        T_float = Si7021_Calculate_T(T, Temperature_Offset_x1000);
        if(T_float > 30)
        {
            UART2_xmits_NewLine("Error, zero not saved ");
            while(1); 
        }
        else if(T_float < 0)
        {
            UART2_xmits_NewLine("Error, zero not saved ");
            while(1); 
        }
        else;

        ADC = ADC_Read_Oversample();
        
        if(Part_Number == 110102)
        {
            Range = 1000;
        }
        if(Part_Number == 110501)
        {
            Range = 10000;
        }
        else
        {
            Range = 20000;
        }
        
        if(ADC > (ADC_OC + Range))
        {
            UART2_xmits_NewLine("Error, zero not saved ");
            while(1);
        }
        else if(ADC < (ADC_OC - Range))
        {
            UART2_xmits_NewLine("Error, zero not saved ");
            while(1);
        }
        else
        {
            ADC_Zero = ADC;

            writeNVM(30,ADC_Zero);

            T_Zero = T;

            writeNVM(32,T_Zero);

            RH_Zero = Si7021_Read_RH();

            writeNVM(34,RH_Zero);
            
            UART2_xmits_NewLine("done");
        }
    }
/******************************************************************Read LMP Values************************************************************************/    
    else if(user_input == 'l')
    {   
        UART2_xmits("Enter LMP91000 Register 0x10= ");
        LMP91000_B10 = LMP91000_Read(0x10);
        utoa(Buffer, LMP91000_B10, 10);
        delay(1000);
        UART2_xmits(Buffer);
        UART2_xmits("\r\nEnter LMP91000 Register 0x11= ");
        LMP91000_B11 = LMP91000_Read(0x11);
        utoa(Buffer, LMP91000_B11, 10);
        delay(1000);
        UART2_xmits(Buffer);
        UART2_xmits("\r\nEnter LMP91000 Register 0x12= ");
        LMP91000_B12 = LMP91000_Read(0x12);
        utoa(Buffer, LMP91000_B12, 10);
        delay(1000);
        UART2_xmits(Buffer);
        UART2_xmits("\r\n");
    }    
/******************************************************************Set LMP Values************************************************************************/    
    else if(user_input == 'L')
    {
        Unlock(Security_Code);

        UART2_xmits("\r\nEnter LMP91000 Register 0x10: ");
        UART_Entry();
        LMP91000_B10 = (unsigned int)atoi(Buffer);
        LMP91000_B10 = LMP91000_B10 & 0xFFFF;
        writeNVM(52,LMP91000_B10);        
        
        UART2_xmits("\r\nEnter LMP91000 Register 0x11: ");
        UART_Entry();
        LMP91000_B11 = (unsigned int)atoi(Buffer);
        LMP91000_B11 = LMP91000_B11 & 0xFFFF;
        writeNVM(54,LMP91000_B11);
        
        UART2_xmits("\r\nEnter LMP91000 Register 0x12: ");
        UART_Entry();
        LMP91000_B12 = (unsigned int)atoi(Buffer);
        LMP91000_B12 = LMP91000_B12 & 0xFFFF;                                                       //Remove these lines
        writeNVM(56,LMP91000_B12); 
        
        UART2_xmits("\r\n");
        
        LMP91000_Unlock();
        LMP91000_Write_0x10(LMP91000_B10);
        LMP91000_Write_0x11(LMP91000_B11);
        LMP91000_Write_0x12(LMP91000_B12); 
    } 
/***************************************************************Set Temperature Offset********************************************************************/    
    else if(user_input == 'T')
    {
        Unlock(Security_Code);     
                       
        UART2_xmits("\r\nEnter Temperature_Offset: ");
        UART_Entry();
        Temperature_Offset_x1000 = (int)(1000 * atof(Buffer));
        Temperature_Offset_x1000 = Temperature_Offset_x1000 & 0xFFFF;
        writeNVM(20,Temperature_Offset_x1000);        
        
        UART2_xmits("\r\n");
    }
/*****************************************************************Set Factory Defaults**********************************************************************/    
    else if(user_input == 'B')
    {    
        Unlock(Security_Code);
        
        UART2_xmits("\r\nRemove Sensor and Scan: ");
        UART_Entry();       
        i = 0;
        while(i < 12)
        {
            Serial_Number_[i] = Buffer[i];
            i++;
        } 
        Serial_Number_[i] = '\0';
        i++;
        while(i < 19)
        {
            Part_Number_[i-13] = Buffer[i];
            i++;
        }
        Part_Number_[i-13] = '\0';
        i++;       
        while(i < 22)
        {
            Gas_[i-20] = Buffer[i];
            i++;
        }
        while(i < 30)
        {
            if(Buffer[i] != ' ')
            {
                Gas_[i-20] = Buffer[i];
            }
            else
            {               
                break;
            }
            i++;
        }
        Gas_[i-20] = '\0';
        i++;       
        k = i + 4;
        j = 0;
        while(i < k)
        {
            Date_Code_[j] = Buffer[i];
            j++;
            i++;
        } 
        Date_Code_[j] = '\0';
        i++;
        k = i + 7;
        j = 0;
        while(i < k)
        {
            Sensitivity_Code_[j] = Buffer[i];
            j++;
            i++;
        } 
        Sensitivity_Code_[j] = '\0';
              
        i = 0;
        while(i < 13)
        {
            writeNVM ((300 + (i * 2)),Serial_Number_[i]);
            i++;
        } 
        i = 0;
        while(i < 7)
        {
            writeNVM ((330 + (i * 2)),Part_Number_[i]);
            i++;
        } 
        i = 0;
        while(i < 5)
        {
            writeNVM ((370 + (i * 2)),Date_Code_[i]);
            i++;
        }
        i = 0;
        while(i < 8)
        {
            writeNVM ((380 + (i * 2)),Sensitivity_Code_[i]);
            i++;
        }

        nA_per_PPM_x100 = (int)(100 * atof(Sensitivity_Code_));
        nA_per_PPM_x100 = nA_per_PPM_x100 & 0xFFFF;
        writeNVM(70,nA_per_PPM_x100);
               
        i = 0;
        while(i < 10)
        {
            Gas_[i] = '\0';
            i++;
        }
        LMP91000_B10 = 3;
        LMP91000_B11 = 176;
        LMP91000_B12 = 3; 
        Part_Number = atol(Part_Number_);
        if(Part_Number == 110102)
        {
            LMP91000_B11 = 177; 
            
            Gas_[0] = 'C';
            Gas_[1] = 'O';         
        }
        else if(Part_Number == 110202)
        {
            LMP91000_B10 = 27;
            LMP91000_B11 = 179; 
            
            Gas_[0] = 'E';
            Gas_[1] = 't';
            Gas_[2] = 'O';
            Gas_[3] = 'H';     
        }
        else if(Part_Number == 110303)
        {
            LMP91000_B11 = 176; 
            
            Gas_[0] = 'H';
            Gas_[1] = '2';
            Gas_[2] = 'S';         
        }
        else if(Part_Number == 110601)
        {
            LMP91000_B11 = 182;
            
            Gas_[0] = 'S';
            Gas_[1] = 'O';
            Gas_[2] = '2';        
        }
        else if(Part_Number == 110501)
        {
            LMP91000_B11 = 166;
            
            Gas_[0] = 'N';
            Gas_[1] = 'O';
            Gas_[2] = '2';          
        }
        else if(Part_Number == 110401)
        {
            LMP91000_B11 = 161; 
            
            Gas_[0] = 'O';
            Gas_[1] = '3';         
        }
        else if(Part_Number == 110801)
        {
            LMP91000_B11 = 181; 
            
            Gas_[0] = 'I';
            Gas_[1] = 'A';
            Gas_[2] = 'Q';         
        }
        else if(Part_Number == 110901)
        {
            LMP91000_B11 = 166;
            
            Gas_[0] = 'R';
            Gas_[1] = 'E';
            Gas_[2] = 'S';
            Gas_[3] = 'P';          
        }
        else if(Part_Number == 999036)
        {
            LMP91000_B11 = 161;
            
            Gas_[0] = 'O';
            Gas_[1] = '3';
            Gas_[2] = '/';
            Gas_[3] = 'N';
            Gas_[4] = 'O';
            Gas_[5] = '2';           
        }
        else;
               
        i = 0;
        while(i < 10)
        {
            writeNVM ((350 + (i * 2)),Gas_[i]);
            i++;
        }
        
        writeNVM(52,LMP91000_B10);               
        writeNVM(54,LMP91000_B11);
        writeNVM(56,LMP91000_B12); 
        
        LMP91000_Unlock();
        LMP91000_Write_0x10(LMP91000_B10);
        LMP91000_Write_0x11(LMP91000_B11);
        LMP91000_Write_0x12(LMP91000_B12);
        
        UART2_xmits("\r\nSetting OC...");       
        ADC = ADC_Read_Oversample();
        if(ADC > 32867)
        {
            UART2_xmits_NewLine("Error, zero not saved ");
            while(1);
        }
        else if(ADC < 32667)
        {
            UART2_xmits_NewLine("Error, zero not saved ");
            while(1);
        }
        else
        {
            ADC_OC = ADC;
            writeNVM(12,ADC_OC);
            UART2_xmits_NewLine("done");
            
            UART2_xmits("Setting zero...");
            ADC_Zero = ADC;
            writeNVM(30,ADC_Zero);
            T_Zero = Si7021_Read_T();
            writeNVM(32,T_Zero);
            RH_Zero = Si7021_Read_RH();
            writeNVM(34,RH_Zero);           
            UART2_xmits_NewLine("done");
        }
    }       
/****************************************************************EEPROM Readout********************************************************************/    
    else if(user_input == 'e')
    {       
        UART2_xmits_NewLine("                               EEPROM Values");          
        
        itoa(Buffer, nA_per_PPM_x100, 10);
        delay(1000);
        UART2_xmits("nA_per_PPM_x100 = ");
        UART2_xmits_NewLine(Buffer);

        utoa(Buffer, ADC_OC, 10);
        delay(1000);
        UART2_xmits("ADC_OC = ");
        UART2_xmits_NewLine(Buffer);
        
        utoa(Buffer, ADC_Zero, 10);
        delay(1000);
        UART2_xmits("ADC_Zero = ");
        UART2_xmits_NewLine(Buffer);
        
        utoa(Buffer, ADC_Span, 10);
        delay(1000);
        UART2_xmits("ADC_Span = ");
        UART2_xmits_NewLine(Buffer);
        
        utoa(Buffer, PPM_Span_x100, 10);
        delay(1000);
        UART2_xmits("PPM_Span_x100 = ");
        UART2_xmits_NewLine(Buffer);       
                
        itoa(Buffer, Temperature_Offset_x1000, 10);
        delay(1000);
        UART2_xmits("Temperature_Offset_x1000 = ");
        UART2_xmits_NewLine(Buffer);        

        utoa(Buffer, T_Zero, 10);
        delay(1000);
        UART2_xmits("T_Zero = ");
        UART2_xmits_NewLine(Buffer);

        utoa(Buffer, RH_Zero, 10);
        delay(1000);
        UART2_xmits("RH_Zero = ");
        UART2_xmits_NewLine(Buffer);
        
        utoa(Buffer, T_Span, 10);
        delay(1000);
        UART2_xmits("T_Span = ");
        UART2_xmits_NewLine(Buffer);

        utoa(Buffer, RH_Span, 10);
        delay(1000);
        UART2_xmits("RH_Span = ");
        UART2_xmits_NewLine(Buffer);
        
        utoa(Buffer, LMP91000_B10, 10);
        delay(1000);
        UART2_xmits("LMP91000 Register 0x10 = ");
        UART2_xmits_NewLine(Buffer);        
        
        utoa(Buffer, LMP91000_B11, 10);
        delay(1000);
        UART2_xmits("LMP91000 Register 0x11 = ");
        UART2_xmits_NewLine(Buffer);
        
        utoa(Buffer, LMP91000_B12, 10);
        delay(1000);
        UART2_xmits("LMP91000 Register 0x12 = ");
        UART2_xmits_NewLine(Buffer);  
        
        UART2_xmits("Barcode = ");
        UART2_xmits(Serial_Number_);
        UART2_xmitc(' ');
        UART2_xmits(Part_Number_);
        UART2_xmitc(' ');
        UART2_xmits(Gas_);
        UART2_xmitc(' ');
        UART2_xmits(Date_Code_);
        UART2_xmitc(' ');
        UART2_xmits_NewLine(Sensitivity_Code_);
        
        UART2_xmits("Serial_Number_ = ");
        UART2_xmits_NewLine(Serial_Number_);
        
        UART2_xmits("Part_Number = ");
        UART2_xmits_NewLine(Part_Number_);
        
        UART2_xmits("Gas = ");
        UART2_xmits_NewLine(Gas_);
        
        UART2_xmits("Date_Code = ");
        UART2_xmits_NewLine(Date_Code_);
        
        UART2_xmits("Sensitivity_Code = ");
        UART2_xmits_NewLine(Sensitivity_Code_);
    }  
    else;
    
    if(user_input != 'c')
    {
        U2MODEbits.UARTEN = 0;
        U2MODEbits.USIDL = 0;
        U2MODEbits.WAKE = 1;
        U2BRG = 25;	
        IEC1bits.U2RXIE = 1;
        U2MODEbits.UARTEN = 1;	
        U2STAbits.UTXEN = 1;
        RCONbits.WDTO = 0;
        RCONbits.SWDTEN = 0;
        delay(1000);
        Sleep();
        while(1);
    }
    else;
    }
    while(1);
    return 0;
}
