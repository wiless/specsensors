#include <xc.h>
#include <math.h>

double PPB_Calculation(unsigned int ADC, unsigned int ADC_Zero, int nA_per_PPM_x100, double T_float, unsigned long int Part_Number)
{
    double Tkz = 25;
    double Tks = 25;  
    double nA;
    double Cx;
    unsigned long int Rgain = 500000;
    double Zc;
    double Zcf;
    double Zcf_low;
    double Zcf_high;   
    double Sc;
    double Scf;
    double Scf_low;
    double Scf_high;    
       
    if(Part_Number == 110102)
    {
        Zcf_low = 0;
        Zcf_high = 2.375;
        Scf_low = 0.6;
        Scf_high = 0.4;
    }
    else if(Part_Number == 110202)
    {
        Zcf_low = 1.7;
        Zcf_high = 22.5;
        Scf_low = 1.2;
        Scf_high = 0;
    }
    else if(Part_Number == 110303)
    {
        Zcf_low = 0;
        Zcf_high = 0;
        Scf_low = 0.3;
        Scf_high = 0;
    }
    else if(Part_Number == 110601)
    {
        Zcf_low = 0;
        Zcf_high = 15;
        Scf_low = 1.2;
        Scf_high = 0.5;
    }
    else if(Part_Number == 110501)
    {
        Zcf_low = 0;
        Zcf_high = -1.6;
        Scf_low = 0;
        Scf_high = 0.4;
    }
    else if(Part_Number == 110401)
    {
        Zcf_low = 0;
        Zcf_high = 0;
        Scf_low = 0;
        Scf_high = 0.5;
    }
    else if(Part_Number == 110801)
    {
        Zcf_low = 0;
        Zcf_high = 0;
        Scf_low = 0;
        Scf_high = 0;
    }
    else if(Part_Number == 110901)
    {
        Zcf_low = 0;
        Zcf_high = 0;
        Scf_low = 0;
        Scf_high = 0;
    }
    else if(Part_Number == 999036)
    {
        Zcf_low = 0;
        Zcf_high = 0;
        Scf_low = 0;
        Scf_high = 0;
    }
    else;
    
    if(T_float < Tkz)
    {
        Zcf = Zcf_low;
    }
    else
    {
        Zcf = Zcf_high;
    }
    
    if(T_float < Tks)
    {
        Scf = Scf_low;
    }
    else
    {
        Scf = Scf_high; 
    }
       
    nA = ((double)ADC - (double)ADC_Zero) / 65535 * 2.048 / (double)Rgain * 1E9;    //formula for 2.048V reference
//    nA = ((double)ADC - (double)ADC_Zero) / 65535 * 2.5 / (double)Rgain * 1E9;    //formula for 2.5V reference
    Zc = nA - (Zcf * (T_float - Tkz));
    Sc = 1 - (Scf / 100 * (T_float - Tks));
    Cx = Zc * Sc / (double)nA_per_PPM_x100 * 1E5;   //Concentration with temperature compensation   
//    Cx = nA / nA_per_PPM_x100 * 1000; //Concentration without temperature compensation

    return Cx;
}
