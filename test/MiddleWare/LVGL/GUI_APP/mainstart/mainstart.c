/*
 * @Author: lilinng 2464532129@qq.com
 * @Date: 2026-02-27 11:00:26
 * @LastEditTime: 2026-03-01 15:51:52
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\MiddleWare\LVGL\GUI_APP\mainstart\mainstart.c
 * @Description: lvgl动画界面
 */
#include "lvgl.h"
#include "mainstart.h"

//屏幕宽高
#define WIDTH 240
#define HEIGHT 320

//三个obj全局变量初始化
lv_obj_t* speed_label = NULL;
lv_obj_t* speedUp_btn = NULL;
lv_obj_t* speeddown_btn = NULL;

int speed = 0;

/**
 * @description: lvgl事件回调函数
 * @param {lv_event_t*} e
 * @return {void}
 */
void Speed_btn_cb(lv_event_t* e)
{
    //判断obj
    lv_obj_t* target = lv_event_get_target(e);
    if (target == speedUp_btn)
    {
        speed += 30;
    }
    else if (target == speeddown_btn)
    {
        speed -= 30;
    }
    //最后限幅，也可以根据正负判断方向 
    if (speed <= 0)
    {
        speed = 0;
    }
    lv_label_set_text_fmt(speed_label, "Speed: %d rpm", speed);
}

/**
 * @description: speed_label标签创建及其初始化
 * @return {void}
 */
static void Show_SpeedLabel(void)
{
    //创建标签
    speed_label = lv_label_create(lv_scr_act());
    //文本
    lv_label_set_text(speed_label, "Speed: 0 rpm");
    //二参数见lv_conf.h里关于字体的裁剪宏
    lv_obj_set_style_text_font(speed_label, &lv_font_montserrat_18, LV_STATE_DEFAULT);
    //设置样式(本地设置)
    lv_obj_set_style_bg_opa(speed_label, 200, LV_STATE_DEFAULT);    //更改不透明度
    lv_obj_set_style_bg_color(speed_label, lv_color_hex(0x7160e8), LV_STATE_DEFAULT);   //设置颜色
    //对齐
    lv_obj_align(speed_label, LV_ALIGN_CENTER, 0, -HEIGHT / 3);
}

/**
 * @description: speedup_btn obj创建及其初始化
 * @return {void}
 */
static void Show_btn_SpeedUp()
{
    //创建按钮标签
    speedUp_btn = lv_btn_create(lv_scr_act());
    //设置样式
    lv_obj_set_style_bg_color(speedUp_btn, lv_color_hex(0x4480ce), LV_STATE_DEFAULT);
    //对齐
    lv_obj_align(speedUp_btn, LV_ALIGN_CENTER, -WIDTH / 3, 0);

    //创建标签
    static lv_obj_t* speedup_label = NULL;
    speedup_label = lv_label_create(speedUp_btn);
    //设置文本
    lv_label_set_text(speedup_label,"Speed++");
    //设置样式(略)
    //对齐
    lv_obj_align(speedup_label, LV_ALIGN_CENTER, 0, 0);

    //绑定事件
    lv_obj_add_event_cb(speedUp_btn, Speed_btn_cb, LV_EVENT_CLICKED, NULL);
}

/**
 * @description: speeddown_btn obj创建及其初始化
 * @return {void}
 */
static void Show_btn_SpeedDown()
{
    //创建按钮标签
    speeddown_btn = lv_btn_create(lv_scr_act());
    //设置样式
    lv_obj_set_style_bg_color(speeddown_btn, lv_color_hex(0x4480ce), LV_STATE_DEFAULT);
    //对齐
    lv_obj_align(speeddown_btn, LV_ALIGN_CENTER, WIDTH / 3, 0);

    //创建标签
    static lv_obj_t* speeddown_label = NULL;
    speeddown_label = lv_label_create(speeddown_btn);
    //设置文本
    lv_label_set_text(speeddown_label, "Speed--");
    //设置样式(略)
    //对齐
    lv_obj_align(speeddown_label, LV_ALIGN_CENTER, 0, 0);

    //绑定事件
    lv_obj_add_event_cb(speeddown_btn, Speed_btn_cb, LV_EVENT_CLICKED, NULL);

}

/**
 * @description: 主界面
 * @return {void}
 */
void mainstart(void)
{
    Show_SpeedLabel();
    Show_btn_SpeedUp();
    Show_btn_SpeedDown();
}
