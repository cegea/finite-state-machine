#include <FiniteStateMachine.h>
#include <stdlib.h>

// Generates an external event. Is called once per external event to start
// the state machine executing
void _FSM_ExternalEvent(FSM_StateMachine* self, const FSM_StateMachineConst* self_const, uint8_t newState, void* pEventData)
{
    if (newState == EVENT_IGNORED)
    {
        if (pEventData){
            free(pEventData);
        }
    }
    else{
        // Generate the internal event
        _FSM_InternalEvent(self, newState, pEventData);
    }

    // Execute state machine based on type of state map defined
    if (self_const->state_Map){
        _FSM_StateEngine(self, self_const);
    }
    else{
        _FSM_StateEngineEx(self, self_const);
    }
}

// Generates internal event. Called from within a state function to transition to a new state
void _FSM_InternalEvent(FSM_StateMachine* self, uint8_t newState, void* pEventData)
{
    void* pDataTemp = ((void *)0);

    if (self)
    {
        self->pEventData = pEventData;
        self->eventGEnerated = True;
        self->newState = newState;
    }
}

// The state engine executes the state machine states
void _FSM_StateEngine(FSM_StateMachine* self, const FSM_StateMachineConst* self_const)
{
    void* pDataTemp = ((void *)0);

    if (self && self_const)
    {
        while(self->eventGEnerated)
        {
            // Error check that the new state is valid before proceeding
            if (self->newState < self_const->maxStates)
            {
                // Get the pointers from the state map
                FSM_StateFunc state = self_const->state_Map[self->newState].pStateFunc;

                // Copy of event data pointer
                pDataTemp = self->pEventData;

                // Event data used up, reset the pointer
                self->pEventData = ((void *)0);

                // Event used up, reset the flag
                self->eventGEnerated = False;

                // Switch to the new current state
                self->currentState = self->newState;

                // Execute the state action passing in event data
                if (state)
                {
                    state(self, pDataTemp);
                }

                // If event data was used, the delete it
                if (pDataTemp)
                {
                    free(pDataTemp);
                    pDataTemp = ((void *)0);
                }        
            }
        }
    }
}

// The state engine executes the extended state machine states
void _FSM_StateEngineEx(FSM_StateMachine* self, const FSM_StateMachineConst* self_const)
{
    bool guardResult = True;
    void* pDataTemp = ((void *)0);

    if (self && self_const)
    {
        // While events are being generated keep executing states
        while (self->eventGEnerated)
        {
            // Error check that the new state is valid before proceeding
            if (self->newState < self_const->maxStates)
            {
                // Get the pointers from the extended state map
                FSM_StateFunc state = self_const->stateMapEx[self->newState].pStateFunc;
                FSM_GuardFunc guard = self_const->stateMapEx[self->newState].pGuardFunc;
                FSM_EntryFunc entry = self_const->stateMapEx[self->newState].pEntryFunc;
                FSM_ExitFunc  exit  = self_const->stateMapEx[self->newState].pExitFunc;

                // Copy of event data pointer
                pDataTemp = self->pEventData;

                // Event data used up, reset the pointer
                self->pEventData = ((void *)0);

                // Event used up, reset the flag
                self->eventGEnerated = False;

                // Execute the guard condition
                if (guard)
                {
                    guardResult = guard(self, pDataTemp);
                }

                // If the guard condition succeeds
                if (guardResult == True)
                {
                    // Transition to a new state verification
                    if (self->newState != self->currentState)
                    {
                        // Execute the state exit action on current state before swithing to new state
                        if (exit)
                        {
                            exit(self);
                        }
                        
                        // Execute the state entry action on the new state
                        if (entry)
                        {
                            entry(self, pDataTemp);
                        }

                        // Ensure exit/entry actions didn't call FSM_InternalEvent by accident
                        if (self->eventGEnerated == True)
                        {
                            /* TODO: Rise some error */
                        }                       
                    }

                    // Switch to the new current state
                    self->currentState = self->newState;

                    // Execute the state action passing in event data
                    if (state)
                    {
                        state(self, pDataTemp);
                    }
                }

                // If event data was used, the delete it
                if (pDataTemp)
                {
                    free(pDataTemp);
                    pDataTemp = ((void *)0);
                } 
            }
        }        
    }
}
void _FSM_StateEngineEx(FSM_StateMachine* self, const FSM_StateMachineConst* self_const);

