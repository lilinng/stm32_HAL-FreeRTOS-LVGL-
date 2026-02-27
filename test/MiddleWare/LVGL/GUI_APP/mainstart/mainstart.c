/*
 * @Author: lilinng 2464532129@qq.com
 * @Date: 2026-02-27 11:00:26
 * @LastEditTime: 2026-02-27 16:34:08
 * @FilePath: \test_EIDEd:\MCU\stm32\stm32_practise\VS+HAL\stm32_HAL-FreeRTOS-LVGL-\test\MiddleWare\LVGL\GUI_APP\mainstart\mainstart.c
 * @Description: 
 */
#include "lvgl.h"
#include "mainstart.h"

lv_coord_t main_screen_width;
lv_coord_t main_screen1_height;

void mainstart(void)
{
    main_screen_width = lv_obj_get_width(lv_scr_act());
    main_screen1_height = lv_obj_get_height(lv_scr_act());

    static lv_obj_t* label1_obj = NULL; 
    label1_obj = lv_label_create(lv_scr_act());
    //设置大小和展示方式
    lv_obj_set_size(label1_obj,50,15);
    lv_label_set_long_mode(label1_obj,LV_LABEL_LONG_SCROLL_CIRCULAR);
    //对齐
    lv_obj_align(label1_obj,LV_ALIGN_CENTER,0,-30);
    //文本
    lv_label_set_text(label1_obj,"Welcome to LVLG");
    //背景不透明度和颜色设置
    lv_obj_set_style_bg_opa(label1_obj,255,LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(label1_obj,lv_color_hex(0xd2a8ff),LV_STATE_DEFAULT);
    
    //为某些字体单独着色
    lv_label_set_recolor(label1_obj,true);
    lv_label_set_text(label1_obj,"Welcome to #24acf2 LVGL#");

    static lv_obj_t* User_info_label_obj = NULL;
    User_info_label_obj = lv_label_create(lv_scr_act());
    
    lv_obj_set_align(User_info_label_obj,LV_ALIGN_CENTER);

    lv_label_set_recolor(User_info_label_obj,true);
    lv_label_set_text(User_info_label_obj,"#b01215 User:# #19cc23 lilinng#");
}