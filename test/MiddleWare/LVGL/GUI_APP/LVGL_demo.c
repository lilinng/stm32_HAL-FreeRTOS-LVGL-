/*
 * @Author: userName userEmail
 * @Date: 2026-02-24 21:14:15
 * @LastEditTime: 2026-02-27 11:09:30
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\MiddleWare\LVGL\GUI_APP\LVGL_demo.c
 * @Description: 用于测试FreeRTOS和LVGL的demo
 */
/************************INCLUDE***************************************/
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
#include "mainstart.h"
#include "usart.h"

/*****启动任务参数*****/
#define START_TASK_PRIORITY 1
#define START_TASK_STACK_SIZE 128
TaskHandle_t Start_Task_Handle;

/*****Lvgl_Task1参数*****/
#define LVGL_TASK1_PRIORITY 2
#define LVGL_TASK1_STACK_SIZE 1024
TaskHandle_t Lvgl_Task_Handle;

/*****Led_Task参数*****/
#define LED_TASK_PRIORITY 3
#define LED_TASK_STACK_SIZE 128
TaskHandle_t Led_Task_Handle;

// 触摸坐标全局变量
int16_t touch_x = 0;
int16_t touch_y = 0;
bool touch_pressed = false;


void LVGL_Printf_CallBack(const char * buf)
{
    HAL_UART_Transmit(&huart1,(uint8_t*)buf,strlen(buf),HAL_MAX_DELAY);
}

/**
 * @description:lvgl_task1任务 
 * @param {void*} pvParameters
 * @return {void}
 */
void Lvgl_Task1(void* pvParameters)
{
    //LED10闪烁是因为FSMC复用: PD8------> FSMC_D13
    lv_init();
    lv_log_register_print_cb(LVGL_Printf_CallBack);
    LV_LOG_INFO("LVGL init success!");
    lv_port_disp_init();
     mainstart();
    while (1) 
    {
        lv_timer_handler();
        vTaskDelay(5);  //无需使用pd,配置为1kHz
    }
}

/**
 * @description:led_task任务,用于简单闪烁判断系统运行状态 
 * @param {void*} pvParameters
 * @return {void}
 */
void Led_Task(void* pvParameters)
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_7);
        vTaskDelay(500);
    }
}


/**
 * @description:启动任务用于创建其他任务并在末尾删除自身 
 * @param {void*} pvParameters
 * @return {void}
 */
void Start_Task(void* pvParameters)
{
    //进入临界区
    taskENTER_CRITICAL();
    xTaskCreate(
                (TaskFunction_t)Lvgl_Task1,
                (char*)"lvgl_task1",
                (StackType_t)LVGL_TASK1_STACK_SIZE,
                (void*)NULL,
                (UBaseType_t)LVGL_TASK1_PRIORITY,
                (TaskHandle_t*)&Lvgl_Task_Handle
                );
    // xTaskCreate(
    //             (TaskFunction_t)Led_Task,
    //             (char*)"led_task",
    //             (uint32_t)LED_TASK_STACK_SIZE,
    //             (void*)NULL,
    //             (UBaseType_t)LED_TASK_PRIORITY,
    //             (TaskHandle_t*)&Led_Task_Handle
    //             );
                                 
    //退出临界区                
    taskEXIT_CRITICAL();
    /**************一定记得删除自身******************** */
    vTaskDelete(NULL);
}

/**
 * @description:启动FreeRTOS 
 * @return {void}
 */
void FreeRTOS_Start(void)
{

    xTaskCreate(
                (TaskFunction_t)Start_Task,
                (char*)"start_task",
                (StackType_t)START_TASK_STACK_SIZE,
                (void*)NULL,
                (UBaseType_t)START_TASK_PRIORITY,
                (TaskHandle_t*)&Start_Task_Handle
                );
    xTaskCreate(
                (TaskFunction_t)Led_Task,
                (char*)"led_task",
                (StackType_t)LED_TASK_STACK_SIZE,
                (void*)NULL,
                (UBaseType_t)LED_TASK_PRIORITY,
                (TaskHandle_t*)&Led_Task_Handle
    );
    //启动调度器
    vTaskStartScheduler();     
}