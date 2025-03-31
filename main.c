/* ========================================
 *
 * PSoC Thermistor Lab
 *
 * ========================================
*/
#include "project.h"
#include "math.h"
#include "float.h"

// Variable declarations
uint32 loop_counter, LCD_write_counter, thermistor_R;
uint16 R_1_result, thermistor_result;
int16 thermistor_mV, R_1_mV, R_1_mV_f, temperature;
float R_1_result_f, R_1_mv_f, thermistor_mV_f, ratio_f, voltage_f;
float thermistor_RA_f, thermistor_RB_f;
float current_mA_f, temperature_K_f;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Power up and start the ADC and mux
    ADC_DelSig_1_Start();
    AMux_1_Start();
    
    // Power up the LCD and initialize the starting position
    LCD_Start();
    LCD_Position(0, 0);
    
    // Flash title on LCD display for 1 second
    LCD_PrintString("Thermistor Test");
    CyDelay(1000);
    
    LCD_Position(0, 0);
    

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    for(;;)
    {
        loop_counter++;
        LCD_write_counter++;
        
        P0_1_Write(1);
        
        AMux_1_Select(0);
        ADC_DelSig_1_StartConvert();
        
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
        
        P0_1_Write(0);
        
        R_1_result = ADC_DelSig_1_GetResult16();
        R_1_result_f = (float)(R_1_result);
        
        R_1_mV = (int16)ADC_DelSig_1_CountsTo_mVolts(R_1_result);
        R_1_mV_f = (float)R_1_mV;
        
        current_mA_f = R_1_mV_f / 10000.;
        
        AMux_1_Select(1);
        ADC_DelSig_1_StartConvert();
        
        ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT);
        
        thermistor_result = ADC_DelSig_1_GetResult16();
        
        thermistor_mV = (int16)ADC_DelSig_1_CountsTo_mVolts(thermistor_result);
        thermistor_mV_f = (float)thermistor_mV;
        
        thermistor_RA_f = thermistor_mV_f / current_mA_f;
        
        thermistor_R = Thermistor_1_GetResistance(R_1_mV, thermistor_mV);
        thermistor_RB_f = (float)thermistor_R;
        
        temperature = Thermistor_1_GetTemperature(thermistor_R);
        
        if (LCD_write_counter++ > 1000)
        {
            LCD_write_counter = 0;
            P0_0_Write(1);
            LCD_IsReady();
            LCD_Position(0, 0);
            LCD_PrintString("loop cnt:      ");
            LCD_Position(0, 10);
            LCD_PrintU32Number(loop_counter);
            LCD_Position(1, 0);
            LCD_PrintString("       degrees");
            LCD_Position(1, 0);
            LCD_PrintNumber(temperature / 100);
            LCD_Position(1, 2);
            LCD_PrintString(".");
            LCD_PrintNumber(temperature % 100);
            P0_0_Write(0);
        }
    }
}

/* [] END OF FILE */
