#include <FreeRTOS.h>
#include <task.h>

#include <ecl/assert.h>

extern int main();

// Somehow linker drops this function if LTO is enabled
__attribute__((used))
void vTaskSwitchContext( void );

// TODO: find better place for this handlers?
void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
    (void) xTask;
    (void) pcTaskName;
    //ecl::cout << "Stack is overflowed by: " << (char *)pcTaskName << ecl::endl;
    for(;;);
}

void vAssertCalled(const char *file, int line)
{
    (void) file;
    (void) line;

    ecl_assert_failed("",
                      "<see FreeRTOS sources>\n",
                      file,
                      NULL,
                      line);

    for(;;);
}

// FreeRTOS doesn't require special init procedure
void kernel_init()
{
    return;
}

// Required since main prototype is not suitable for xTaskCreate
void freertos_main_runner(void *arg)
{
    (void) arg;
    main();

    // Shouldn't get here TODO: improve error handling
    for(;;);
}

void kernel_main()
{
    freertos_main_runner(NULL);
    int ret = xTaskCreate(freertos_main_runner,
                          "main",
                          MAIN_STACK_SIZE / sizeof(StackType_t),
                          NULL,
                          tskIDLE_PRIORITY,
                          NULL);

    if (ret == pdPASS) {
        vTaskStartScheduler();
    }

    // Shouldn't get here TODO: improve error handling
    for(;;);
}
