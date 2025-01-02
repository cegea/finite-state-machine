#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#ifndef False
#define False               0
#endif

#ifndef True
#define True                1
#endif

enum { EVENT_IGNORED = 0xFE, CANNOT_HAPPEN = 0xFF };
typedef void NoEventData;

// State machine constant data
typedef struct 
{
    const char* name;
    const unsigned int maxStates;
    const struct FSM_StatesStruct* state_Map;
    const struct FSM_StateStructEx* stateMapEx;
}FSM_StateMachineConst;

// State machine instance data
typedef struct 
{
    const char* name;
    void* pInstance;
    unsigned int newState;
    unsigned int currentState;
    unsigned int eventGEnerated; // bool
    void* pEventData;
}FSM_StateMachine;

// Generic state function signatures
typedef void (*FSM_StateFunc)(FSM_StateMachine* self, void* pEventData);
typedef unsigned int (*FSM_GuardFunc)(FSM_StateMachine* self, void* pEventData);
typedef void (*FSM_EntryFunc)(FSM_StateMachine* self, void* pEventData);
typedef void (*FSM_ExitFunc)(FSM_StateMachine* self);

typedef struct FSM_StatesStruct
{
    FSM_StateFunc pStateFunc;
} FSM_StatesStruct;

typedef struct FSM_StateStructEx
{
    FSM_StateFunc pStateFunc;
    FSM_GuardFunc pGuardFunc;
    FSM_EntryFunc pEntryFunc;
    FSM_ExitFunc  pExitFunc;
} FSM_StateStructEx;

// Public functions
#define FSM_Get(_fsmName_, _getFunc_) _getFunc_(&_fsmName_##Obj)
#define FSM_Event(_fsmName_, _eventFunc_, _eventData_) _eventFunc_(&_fsmName_##Obj, _eventData_)

// Protected functions
#define FSM_InternalEvent(_newState_, _eventData_) _FSM_InternalEvent(self, _newState_, _eventData_)
#define FSM_GetInstance(_instance_) (_instance_*)(self->pInstance);

// Private functions
void _FSM_ExternalEvent(FSM_StateMachine* self, const FSM_StateMachineConst* self_const, unsigned int newState, void* pEventData);
void _FSM_InternalEvent(FSM_StateMachine* self, unsigned int newState, void* pEventData);
void _FSM_StateEngine(FSM_StateMachine* self, const FSM_StateMachineConst* self_const);
void _FSM_StateEngineEx(FSM_StateMachine* self, const FSM_StateMachineConst* self_const);

#define FSM_DECLARE(_fsmName_) extern FSM_StateMachine _fsmName_##Obj;
#define FSM_DEFINE(_fsmName_, _instance_) FSM_StateMachine _fsmName_##Obj = {#_fsmName_, _instance_, 0,0,0,0};
#define EVENT_DECLARE(_eventFunc_, _eventData_) void _eventFunc_(FSM_StateMachine* self, _eventData_* pEventData);
#define EVENT_DEFINE(_eventFunc_, _eventData_) void _eventFunc_(FSM_StateMachine* self, _eventData_* pEventData)
#define STATE_DECLARE(_stateFunc_, _eventData_) static void ST_##_stateFunc_(FSM_StateMachine* self, _eventData_* pEventData);
#define STATE_DEFINE(_stateFunc_, _eventData_) static void ST_##_stateFunc_(FSM_StateMachine* self, _eventData_* pEventData)

#define GET_DECLARE(_getFunc_, _getData_) \
    _getData_ _getFunc_(FSM_StateMachine* self);

#define GET_DEFINE(_getFunc_, _getData_) \
    _getData_ _getFunc_(FSM_StateMachine* self)

#define GUARD_DECLARE(_guardFunc_, _eventData_) \
    static unsigned int GD_##_guardFunc_(FSM_StateMachine* self, _eventData_* pEventData);

#define GUARD_DEFINE(_guardFunc_, _eventData_) \
    static unsigned int GD_##_guardFunc_(FSM_StateMachine* self, _eventData_* pEventData)

#define ENTRY_DECLARE(_entryFunc_, _eventData_) \
    static void EN_##_entryFunc_(FSM_StateMachine* self, _eventData_* pEventData);

#define ENTRY_DEFINE(_entryFunc_, _eventData_) \
    static void EN_##_entryFunc_(FSM_StateMachine* self, _eventData_* pEventData)

#define EXIT_DECLARE(_exitFunc_) \
    static void EX_##_exitFunc_(FSM_StateMachine* self);

#define EXIT_DEFINE(_exitFunc_) \
    static void EX_##_exitFunc_(FSM_StateMachine* self)

#define BEGIN_STATE_MAP(_smName_) \
    static const FSM_StatesStruct _smName_##StateMap[] = { 

#define STATE_MAP_ENTRY(_stateFunc_) \
    { (FSM_StateFunc)_stateFunc_ },

#define END_STATE_MAP(_smName_) \
    }; \
    static const FSM_StateMachineConst _smName_##Const = { #_smName_, \
        (sizeof(_smName_##StateMap)/sizeof(_smName_##StateMap[0])), \
        _smName_##StateMap, NULL };

#define BEGIN_STATE_MAP_EX(_smName_) \
    static const FSM_StateStructEx _smName_##StateMap[] = { 

#define STATE_MAP_ENTRY_EX(_stateFunc_) \
    { (FSM_StatesStruct)_stateFunc_, NULL, NULL, NULL },

#define STATE_MAP_ENTRY_ALL_EX(_stateFunc_, _guardFunc_, _entryFunc_, _exitFunc_) \
    { (FSM_StatesStruct)_stateFunc_, (SM_GuardFunc)_guardFunc_, (SM_EntryFunc)_entryFunc_, (SM_ExitFunc)_exitFunc_ },

#define END_STATE_MAP_EX(_smName_) \
    }; \
    static const FSM_StateMachineConst _smName_##Const = { #_smName_, \
        (sizeof(_smName_##StateMap)/sizeof(_smName_##StateMap[0])), \
        NULL, _smName_##StateMap };

#define BEGIN_TRANSITION_MAP \
    static const unsigned int TRANSITIONS[] = { \

#define TRANSITION_MAP_ENTRY(_entry_) \
    _entry_,

#define END_TRANSITION_MAP(_smName_, _eventData_) \
    }; \
    _FSM_ExternalEvent(self, &_smName_##Const, TRANSITIONS[self->currentState], _eventData_); \
    //C_ASSERT((sizeof(TRANSITIONS)/sizeof(BYTE)) == (sizeof(_smName_##StateMap)/sizeof(_smName_##StateMap[0])));


#endif
