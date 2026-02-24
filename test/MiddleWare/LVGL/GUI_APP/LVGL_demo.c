/*
 * @Author: userName userEmail
 * @Date: 2026-02-24 21:14:15
 * @LastEditTime: 2026-02-25 01:33:05
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\MiddleWare\LVGL\GUI_APP\LVGL_demo.c
 * @Description: 用于测试FreeRTOS和LVGL的demo
 */
/************************INCLUDE***************************************/
#include "stm32f1xx_hal.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lvgl.h"
#include "lv_port_disp_template.h"
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

/**
 * @description: lvgl任务中调用的显示示例
 * @return {void}
 */
void lvgl_demo_example(void)
{
    static lv_obj_t *rect = NULL;
    if(rect == NULL) {
        rect = lv_obj_create(lv_scr_act());
        lv_obj_set_size(rect, 40, 40);
        lv_obj_set_style_bg_color(rect, lv_color_hex(0x00FF00), 0); // 绿色
        lv_obj_align(rect, LV_ALIGN_TOP_LEFT, 20, 50);
    }

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, rect);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, 20, 180);
    lv_anim_set_time(&a, 1500);
    lv_anim_set_playback_time(&a, 1500);
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_start(&a);
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
    lv_port_disp_init();
    
    lvgl_demo_example();

    while (1) 
    {
        lv_timer_handler();
        vTaskDelay(pdMS_TO_TICKS(5));
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
    xTaskCreate(
                (TaskFunction_t)Led_Task,
                (char*)"led_task",
                (uint32_t)LED_TASK_STACK_SIZE,
                (void*)NULL,
                (UBaseType_t)LED_TASK_PRIORITY,
                (TaskHandle_t*)&Led_Task_Handle
                );
                                 
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

    //启动调度器
    vTaskStartScheduler();     
}