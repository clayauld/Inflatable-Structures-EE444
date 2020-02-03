float DT_NUM = 8;               // Multiple Data array size
float VS = 3.30;            // ADC reference, VSource = 3.3V
float PE = 1.00;            // Pressure Error, 1 = No error
float TM40 = 3.00;            // Temp. multiplier for -40 Degree Range
float TM85 = 1.00;            // Temp. multiplier for 0-85 Degree Range
float TM125 = 3.00;            // Temp. multiplier for +125 Degree Range
float ADCRES = (VS/(4096.00));
float OFFSET0_MIN = 0.088;
float OFFSET0_TYP = 0.200;
float OFFSET0_MAX = 0.313;

float getPressure(float ADCdata, int TM, long useOffset)
{

    double Pmax = 0, Pmin = 0;
    double Vout = (double) ADCdata * ADCRES;
    double ERR = PE * (double)TM * VS * 0.009;

    Pmax = (((Vout + ERR - useOffset)/VS) + 0.095) / 0.009;
    Pmin = (((Vout - ERR - useOffset)/VS) + 0.095) / 0.009;
    Pmax *=  0.145; // 1 kPa =  0.145 psi
    Pmin *=  0.145; // 1 kPa =  0.145 psi
    return (float)Pmax;
}