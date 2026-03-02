/*
 * @Author: lilinng 2464532129@qq.com
 * @Date: 2026-02-27 11:00:26
 * @LastEditTime: 2026-03-02 17:26:34
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\MiddleWare\LVGL\GUI_APP\mainstart\mainstart.c
 * @Description: lvgl动画界面
 */
#include "lvgl.h"
#include "mainstart.h"

//屏幕宽高
#define WIDTH 240
#define HEIGHT 320

//obj全局变量初始化
static lv_obj_t* label1 = NULL;
static lv_obj_t* label2 = NULL;
static lv_obj_t* bar_obj = NULL;

static uint32_t val = 0;

/**
 * @description: 事件回调函数
 * @param {lv_event_t*} e
 * @return {void}
 */
static void event_cb(lv_event_t* e)
{

}

/**
 * @description: 定时器回调函数,在任务while(1)中被lv_timer_handler()调用,无需定义map等
 * @return {*}
 */
void timer_cb(struct _lv_timer_t* User_timer)
{
    if (val < 100)
    {
        val++;
        //更新显示(bar&label2)
        //此处val不能被lv_bar_get_value替换，相同的数据替换不能触发动画
        lv_bar_set_value(bar_obj, val, LV_ANIM_ON);
        lv_label_set_text_fmt(label2, "%d  %%", lv_bar_get_value(bar_obj));
    }
    else
    {
        lv_label_set_text_fmt(label2, "Finished");

    }
}

/**
 * @description: 初始化label标签:LOADING和%d %%
 * @return {void}
 */
void label_example(void)
{
    //创建LOADING和显示%
    //创建LOADING并初始化
    label1 = lv_label_create(lv_scr_act());
    //对齐
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, -HEIGHT / 3);
    //文本
    lv_label_set_text(label1, "LOADING");

    //创建%
    label2 = lv_label_create(lv_scr_act());
    //对齐
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, HEIGHT / 3);
    //文本
    lv_label_set_text(label2, "  %");

}

/**
 * @description: 初始化进度条标签
 * @return {void}
 */
void bar_example(void)
{
    //创建进度条
    bar_obj = lv_bar_create(lv_scr_act());
    //对齐
    lv_obj_align(bar_obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_anim_time(bar_obj, 100, LV_STATE_DEFAULT);
    //初始化定时器(100个节拍进入一次)
    lv_timer_create(timer_cb, 100, NULL);

}
/**
 * @description: 主界面
 * @return {void}
 */
void mainstart(void)
{
    label_example();
    bar_example();
}
