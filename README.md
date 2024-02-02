Three of HRTIM timers are synchroniously generating arbitrary PWM. 
To implement symmetric PWM one has to update CMPx1 and CMPx_2 with the same distance from period's middle.

The update all three timers TIMER_A..C at once is implemented with two steps:
1. the shadow (update) registers CMPx are directly written
2. the dummy MASTER CMP2 register is written with DMA burst, it synchronizes shadows of TIMER_A..C shadow

The MASTER CMP2 register is dummy register, it is not really used in PWM, only one word is send to it over DMA burst
to trigger internal HRTIM update events.
As proof oc concept the direct update of TIMER_A..C CMP happens more frequently than DMA burst.
One can observe with oscilloscope no changes in PWM duty over  TIMER_A..C CMP writes until DMA Burst transfer comes.

ADC flexible trigger added. Two TIMERs are used with longer as PWM period and independent CMPx values.
Timer D CMP2 is Trigger 1 - ADC1 regular (it works, HAL_ADC_ConvCpltCallback() gets fired)
Timer E CMP2 is Trigger 1 - ADC1 injected (seems not to work)