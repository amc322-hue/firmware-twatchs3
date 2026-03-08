/**
 * @file lv_conf.h
 * Configuration file for v8.3.11
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

/*====================
   COLOR SETTINGS
 *====================*/

/*Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888)*/
#define LV_COLOR_DEPTH 16

/*Swap the 2 bytes of RGB565 color. Useful if the display has an 8-bit interface (e.g. SPI)*/
#define LV_COLOR_16_SWAP 1

/*=========================
   MEMORY SETTINGS
 *=========================*/

/*1: use custom malloc/free, 0: use the built-in `lv_mem_alloc()` and `lv_mem_free()`*/
#define LV_MEM_CUSTOM 1
#if LV_MEM_CUSTOM
    #define LV_MEM_CUSTOM_INCLUDE <Arduino.h>   /*Header for the methods*/
    #define LV_MEM_CUSTOM_ALLOC   ps_malloc
    #define LV_MEM_CUSTOM_FREE    free
    #define LV_MEM_CUSTOM_REALLOC realloc
#endif

/*Default memory size for `lv_mem_alloc()` in bytes (used only if LV_MEM_CUSTOM == 0)*/
#define LV_MEM_SIZE (128U * 1024U)          /*[bytes]*/

/*====================
   HAL SETTINGS
 *====================*/

/*Default display refresh period. LVGL will redraw at most this many times in a second*/
#define LV_DISP_DEF_REFR_PERIOD 30      /*[ms]*/

/*Input device read period in milliseconds*/
#define LV_INDEV_DEF_READ_PERIOD 30     /*[ms]*/

/*Use a custom tick source that tells the elapsed time in milliseconds.
 *It removes the need to manually update the tick with `lv_tick_inc()`.*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
    #define LV_TICK_CUSTOM_INCLUDE <Arduino.h>         /*Header for the methods*/
    #define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())    /*Expression evaluating to current system time in ms*/
#endif

/*=======================
 * FEATURE CONFIGURATION
 *=======================*/

/*-----------
 * Widgets
 *----------*/

#define LV_USE_LABEL      1
#define LV_USE_BTN        1
#define LV_USE_TEXTAREA   1
#define LV_USE_SPINNER    1

/*-----------
 * Themes
 *----------*/

/*A simple, monolithic theme*/
#define LV_USE_THEME_DEFAULT 1
#define LV_THEME_DEFAULT_DARK 1
#define LV_THEME_DEFAULT_GROW 1
#define LV_THEME_DEFAULT_TRANSITION_TIME 80

/*-----------
 * Layouts
 *----------*/

#define LV_USE_FLEX 1
#define LV_USE_GRID 1

/*=====================
 *  LOGGING
 *=====================*/

#define LV_USE_LOG 1
#if LV_USE_LOG
    #define LV_LOG_LEVEL LV_LOG_LEVEL_INFO
    #define LV_LOG_PRINTF 1
#endif

/*=========================
 *  FONT USAGE
 *=========================*/

/*Montserrat fonts with ASCII range and some symbols using 4 bpp*/
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_20 1

/*Declare the default font*/
#define LV_FONT_DEFAULT &lv_font_montserrat_14

#endif /*LV_CONF_H*/
