/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

#define     ADC_MIN     (0)
#define     ADC_MAX     (255)
#define     D_RATIO     (16)

uint32 adcValue = 0;

// Decimation filter
uint32 acc1 = 0;
uint32 acc2 = 0;
uint32 last1 = 0;
uint32 last2 = 0;
uint32 iter = 0;
uint32 dValue = 0;

int main() {
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    Comp_Start();
    IDAC_Start();
    AMux_Start();

    /* CyGlobalIntEnable; */ /* Uncomment this line to enable global interrupts. */
    for (;;) {
        // IDAC value scanning
        {
            uint32 idac;
            
            AMux_Connect(0);
            for (idac = ADC_MIN; idac <= ADC_MAX; idac++) {
                IDAC_SetValue(idac);
                CyDelayUs(11);
                if (!Comp_GetCompare()) {
                    break;
                }
            }
            adcValue = idac;
        }
        IDAC_SetValue(0);
        AMux_DisconnectAll();
        
        // 2nd order decimator
        acc1 += adcValue;
        acc2 += acc1;
        if (++iter >= D_RATIO) {
            uint32 diff1;
            uint32 diff2;
            
            diff2 = acc2 - last2;
            last2 = acc2;
            diff1 = diff2 - last1;
            last1 = diff2;
            dValue = diff1;
            iter = 0;
        }
    }
}

/* [] END OF FILE */
