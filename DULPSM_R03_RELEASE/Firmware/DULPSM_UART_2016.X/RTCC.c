#include <xc.h>

void RTCC_init(void)
{
    __builtin_write_RTCWEN();
    RCFGCALbits.RTCEN = 0;
    RTCPWCbits.RTCCLK = 1;
    RTCPWCbits.RTCOUT = 2;

    RCFGCALbits.RTCPTR = 3;
    RTCVAL = 19;        // YEAR                 X X X X X X X X YRTEN3 YRTEN2 YRTEN1 YRTEN0 YRONE3 YRONE2 YRONE1 YRONE0
    RTCVAL = 4608;      // MONTH-1/DAY-1        X X X MTHTEN0 MTHONE3 MTHONE2 MTHONE1 MTHONE0 X X DAYTEN1 DAYTEN0 DAYONE3 DAYONE2 DAYONE1 DAYONE0
    RTCVAL = 0;         // WEEKDAY/HOURS        X X X X X WDAY2 WDAY1 WDAY0 X X HRTEN1 HRTEN0 HRONE3 HRONE2 HRONE1 HRONE0
    RTCVAL = 0;         // MINUTES/SECONDS      X MINTEN2 MINTEN1 MINTEN0 MINONE3 MINONE2 MINONE1 MINONE0 X SECTEN2 SECTEN1 SECTEN0 SECONE3 SECONE2 SECONE1 SECONE0

    RCFGCALbits.RTCEN = 1;
    RCFGCALbits.RTCWREN = 0;
}

void RTCC_start(void)
{
    __builtin_write_RTCWEN();
    RCFGCALbits.RTCEN = 0;
    RTCPWCbits.RTCCLK = 1;
    RTCPWCbits.RTCOUT = 2;
    RCFGCALbits.RTCPTR = 3;
    RTCVAL = 0b0000000000000000;    // YEAR            X X X X X X X X YRTEN3 YRTEN2 YRTEN1 YRTEN0 YRONE3 YRONE2 YRONE1 YRONE0
    RTCVAL = 0b0000000000000000;    // MONTH-1/DAY-1   X X X MTHTEN0 MTHONE3 MTHONE2 MTHONE1 MTHONE0 X X DAYTEN1 DAYTEN0 DAYONE3 DAYONE2 DAYONE1 DAYONE0
    RTCVAL = 0b0000000000000000;    // WEEKDAY/HOURS   X X X X X WDAY2 WDAY1 WDAY0 X X HRTEN1 HRTEN0 HRONE3 HRONE2 HRONE1 HRONE0
    RTCVAL = 0b0000000000000000;    // MINUTES/SECONDS X MINTEN2 MINTEN1 MINTEN0 MINONE3 MINONE2 MINONE1 MINONE0 X SECTEN2 SECTEN1 SECTEN0 SECONE3 SECONE2 SECONE1 SECONE0
    RCFGCALbits.RTCEN = 1;
    RCFGCALbits.RTCWREN = 0;
}
