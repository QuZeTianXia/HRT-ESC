#ifndef PWM_H
#define PWM_H

void PWM_Init(void);

void Back_Pressure_Up_Slow(void);
void Front_Pressure_Up_Slow(void);
void Back_Pressure_Up_Quick(void);
void Front_Pressure_Up_Quick(void);
void ESC_Release(void);
void Back_Pressure_Down_Slow(void);
void Front_Pressure_Down_Slow(void);
void Back_Pressure_Down_Quick(void);
void Front_Pressure_Down_Quick(void);
void Back_Release(void);
void Front_Release(void);
void Front_Keep(void);
void Back_Keep(void);
#endif
