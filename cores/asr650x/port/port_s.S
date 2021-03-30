@******************************************************************************
@                            EXTERN PARAMETERS
@******************************************************************************

.extern  g_active_task
.extern  g_preferred_ready_task
.extern  krhino_stack_ovf_check

@******************************************************************************
@                            EXPORT FUNCTIONS
@******************************************************************************

.global  cpu_intrpt_save
.global  cpu_intrpt_restore
.global  cpu_task_switch
.global  cpu_intrpt_switch

@******************************************************************************
@                                 EQUATES
@******************************************************************************

.equ SCB_ICSR,        0xE000ED04 @ Interrupt Control and State Register.
.equ ICSR_PENDSVSET,  0x10000000 @ Value to trigger PendSV exception.

.equ SHPR3_PRI_14_15, 0xE000ED20 @ System Handler Priority Register 3 (PendSV + SysTick).
.equ SHPR3_PRI_LVL,   0xC0C00000 @ PendSV + SysTick priority level (lowest).

@******************************************************************************
@                        CODE GENERATION DIRECTIVES
@******************************************************************************
.text
.align 2
.thumb
.syntax unified

@******************************************************************************
@ Functions:
@     size_t cpu_intrpt_save(void);
@     void cpu_intrpt_restore(size_t cpsr);
@******************************************************************************
.thumb_func
cpu_intrpt_save:
    MRS     R0, PRIMASK
    CPSID   I
    BX      LR

.thumb_func
cpu_intrpt_restore:
    MSR     PRIMASK, R0
    BX      LR

@******************************************************************************
@ Functions:
@     void cpu_intrpt_switch(void);
@     void cpu_task_switch(void);
@******************************************************************************
.thumb_func
cpu_task_switch:
    LDR     R0, =SCB_ICSR
    LDR     R1, =ICSR_PENDSVSET
    STR     R1, [R0]
    BX      LR

.thumb_func
cpu_intrpt_switch:
    LDR     R0, =SCB_ICSR
    LDR     R1, =ICSR_PENDSVSET
    STR     R1, [R0]
    BX      LR

.end

