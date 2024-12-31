#ifndef _MOTOR_H
#define _MOTOR_H

#include <DataTypes.h>
#include <FiniteStateMachine.h>

// Motor object structure
typedef struct
{
    uint8_t currentSpeed;
} Motor;

// Event data structure
typedef struct
{
    uint8_t speed;
} MotorData;

// State machine event functions
EVENT_DECLARE(MTR_SetSpeed, MotorData)
EVENT_DECLARE(MTR_Halt, NoEventData)

// Public accessor
GET_DECLARE(MTR_GetSpeed, uint8_t);

#endif // _MOTOR_H
