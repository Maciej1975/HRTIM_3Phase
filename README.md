Three of HRTIM timers are synchroniously generating symmetric PWM. 
Problem to be solved is how to update all three timers CMP registers at once. Writing directly CMPx registry can cause inconsistency of data when only some of CPMx are written and the next period of PWM begins before rest of CMPx are written too.
