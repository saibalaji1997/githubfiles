

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/drivers/Board.h>

#define TASKSTACKSIZE   512

Void task1Fxn(UArg arg0, UArg arg1);
Void task2Fxn(UArg arg0, UArg arg1);

Int resource = 0;
Int finishCount = 0;
UInt32 sleepTickCount;

Task_Struct task1Struct, task2Struct;
Char task1Stack[TASKSTACKSIZE], task2Stack[TASKSTACKSIZE];
Semaphore_Struct semStruct;
Semaphore_Handle semHandle;

/*
 *  ======== main ========
 */
int main()
{
    /* Construct BIOS objects */
    Task_Params taskParams;
    Semaphore_Params semParams;

    /* Call driver init functions */
    Board_init();

    /* Construct writer/reader Task threads */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task1Stack;
    taskParams.priority = 1;
    Task_construct(&task1Struct, (Task_FuncPtr)task1Fxn, &taskParams, NULL);

    taskParams.stack = &task2Stack;
    taskParams.priority = 2;
    Task_construct(&task2Struct, (Task_FuncPtr)task2Fxn, &taskParams, NULL);

    /* Construct a Semaphore object to be use as a resource lock, inital count 1 */
    Semaphore_Params_init(&semParams);
    Semaphore_construct(&semStruct, 1, &semParams);

    /* Obtain instance handle */
    semHandle = Semaphore_handle(&semStruct);

    /* We want to sleep for 10000 microseconds */
    sleepTickCount = 10000 / Clock_tickPeriod;

    BIOS_start();    /* Does not return */
    return(0);
}

/*
 *  ======== task1Fxn ========
 */
Void task1Fxn(UArg arg0, UArg arg1)
{
    UInt32 time;

    for (;;) {
        System_printf("Running task1 function\n");

        if (Semaphore_getCount(semHandle) == 0) {
            System_printf("Sem blocked in task1\n");
        }

        /* Get access to resource */
        Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

        /* Do work by waiting for 2 system ticks to pass */
        time = Clock_getTicks();
        while (Clock_getTicks() <= (time + 1)) {
            ;
        }

        /* Do work on locked resource */
        resource += 1;
        /* Unlock resource */

        Semaphore_post(semHandle);

        Task_sleep(sleepTickCount);
    }
}

/*
 *  ======== task2Fxn ========
 */
Void task2Fxn(UArg arg0, UArg arg1)
{
    for (;;) {
        System_printf("Running task2 function\n");

        if (Semaphore_getCount(semHandle) == 0) {
            System_printf("Sem blocked in task2\n");
        }

        /* Get access to resource */
        Semaphore_pend(semHandle, BIOS_WAIT_FOREVER);

        /* Do work on locked resource */
        resource += 1;
        /* Unlock resource */

        Semaphore_post(semHandle);

        Task_sleep(sleepTickCount);

        finishCount++;
        if (finishCount == 5) {
            System_printf("Calling BIOS_exit from task2\n");
            BIOS_exit(0);
        }
    }
}
