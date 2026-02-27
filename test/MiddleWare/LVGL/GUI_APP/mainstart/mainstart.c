/*
 * @Author: lilinng 2464532129@qq.com
 * @Date: 2026-02-27 11:00:26
 * @LastEditTime: 2026-02-27 13:35:51
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

    static lv_obj_t* obj1 = NULL;
    obj1 = lv_obj_create(lv_scr_act());
    lv_obj_set_size(obj1,main_screen_width*2/3,main_screen1_height*2/3);
    lv_obj_align(obj1,LV_ALIGN_CENTER,0,0);
    lv_obj_set_style_bg_color(obj1,lv_color_hex(0x1f6feb),LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(obj1,(lv_coord_t)30,LV_STATE_DEFAULT);

    lv_coord_t obj1_width = lv_obj_get_width(lv_scr_act());
    lv_coord_t obj1_height = lv_obj_get_height(lv_scr_act());

    static lv_obj_t* obj2 = NULL;
    obj2 = lv_obj_create(obj1);
    lv_obj_set_size(obj2,obj1_width*1/4,obj1_height*1/5);
    lv_obj_align(obj2,LV_ALIGN_BOTTOM_MID,0,50);
    lv_obj_set_style_bg_color(obj2,lv_color_hex(0xd099fb),LV_STATE_DEFAULT);
}