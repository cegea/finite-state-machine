#include <FiniteStateMachine.h>
#include <stdlib.h>
#include <Motor.h>
//#include "CentrifugeTest.h"

// @see https://github.com/endurodave/C_StateMachine
// 
// Other related repos:
// @see https://github.com/endurodave/C_StateMachineWithThreads
// @see https://github.com/endurodave/C_Allocator

// Define motor objects
static Motor motorObj1;
static Motor motorObj2;

// Define two public Motor state machine instances
FSM_DEFINE(Motor1SM, &motorObj1)
FSM_DEFINE(Motor2SM, &motorObj2)

int main(void)
{
    //ALLOC_Init();

    MotorData* data;

    // Create event data
    data = malloc(sizeof(MotorData));
    data->speed = 100;

    // Call MTR_SetSpeed event function to start motor
    FSM_Event(Motor1SM, MTR_SetSpeed, data);

    // Call MTR_SetSpeed event function to change motor speed
    data = malloc(sizeof(MotorData));
    data->speed = 200;
    FSM_Event(Motor1SM, MTR_SetSpeed, data);

    // Get current speed from Motor1SM
    uint8_t currentSpeed = FSM_Get(Motor1SM, MTR_GetSpeed);

    // Stop motor again will be ignored
    FSM_Event(Motor1SM, MTR_Halt, NULL);

    // Motor2SM example
    data = malloc(sizeof(MotorData));
    data->speed = 300;
    FSM_Event(Motor2SM, MTR_SetSpeed, data);
    FSM_Event(Motor2SM, MTR_Halt, NULL);

    //ALLOC_Term();

    return 0;
}

