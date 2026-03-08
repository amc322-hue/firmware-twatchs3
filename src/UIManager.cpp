#include "UIManager.h"
#include <Arduino.h>

lv_obj_t* UIManager::mainScreen = nullptr;
lv_obj_t* UIManager::statusLabel = nullptr;
lv_obj_t* UIManager::actionButton = nullptr;
lv_obj_t* UIManager::responseTextArea = nullptr;
lv_obj_t* UIManager::spinner = nullptr;
UIState UIManager::currentState = UI_STATE_IDLE;

static bool buttonPressed = false;

void UIManager::begin() {
    mainScreen = lv_scr_act();
    lv_obj_set_style_bg_color(mainScreen, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_set_style_bg_opa(mainScreen, LV_OPA_COVER, 0);

    // Title
    lv_obj_t* title = lv_label_create(mainScreen);
    lv_label_set_text(title, "PickleRick AI");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(title, lv_palette_main(LV_PALETTE_LIME), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Status Label
    statusLabel = lv_label_create(mainScreen);
    lv_label_set_text(statusLabel, "Ready to Hack");
    lv_obj_align(statusLabel, LV_ALIGN_TOP_MID, 0, 40);

    // Action Button (Invisible large area for touch)
    actionButton = lv_btn_create(mainScreen);
    lv_obj_set_size(actionButton, 200, 100);
    lv_obj_align(actionButton, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(actionButton, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_set_style_radius(actionButton, 50, 0);
    
    lv_obj_t* btnLabel = lv_label_create(actionButton);
    lv_label_set_text(btnLabel, "Hold to Speak");
    lv_obj_center(btnLabel);

    lv_obj_add_event_cb(actionButton, [](lv_event_t* e) {
        lv_event_code_t code = lv_event_get_code(e);
        if (code == LV_EVENT_PRESSED) {
            buttonPressed = true;
            UIManager::setState(UI_STATE_RECORDING);
        } else if (code == LV_EVENT_RELEASED || code == LV_EVENT_PRESS_LOST) {
            buttonPressed = false;
        }
    }, LV_EVENT_ALL, NULL);

    // Response Text Area (Scrollable)
    responseTextArea = lv_textarea_create(mainScreen);
    lv_obj_set_size(responseTextArea, 220, 120);
    lv_obj_align(responseTextArea, LV_ALIGN_BOTTOM_MID, 0, -10);
    lv_textarea_set_text(responseTextArea, "");
    lv_obj_add_flag(responseTextArea, LV_OBJ_FLAG_HIDDEN);
    lv_obj_set_style_text_font(responseTextArea, &lv_font_montserrat_14, 0);

    // Spinner (for Thinking state)
    spinner = lv_spinner_create(mainScreen, 1000, 60);
    lv_obj_set_size(spinner, 50, 50);
    lv_obj_align(spinner, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(spinner, LV_OBJ_FLAG_HIDDEN);
}

void UIManager::update() {
    lv_timer_handler();
}

void UIManager::setState(UIState state) {
    currentState = state;
    
    switch (state) {
        case UI_STATE_IDLE:
            lv_label_set_text(statusLabel, "Ready to Hack");
            lv_obj_clear_flag(actionButton, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(spinner, LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(responseTextArea, LV_OBJ_FLAG_HIDDEN);
            break;
            
        case UI_STATE_RECORDING:
            lv_label_set_text(statusLabel, "Recording...");
            lv_obj_set_style_bg_color(actionButton, lv_palette_main(LV_PALETTE_RED), 0);
            break;
            
        case UI_STATE_THINKING:
            lv_label_set_text(statusLabel, "Thinking...");
            lv_obj_add_flag(actionButton, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(spinner, LV_OBJ_FLAG_HIDDEN);
            break;
            
        case UI_STATE_RESPONSE:
            lv_label_set_text(statusLabel, "Response:");
            lv_obj_add_flag(spinner, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(responseTextArea, LV_OBJ_FLAG_HIDDEN);
            break;
    }
}

void UIManager::setResponse(const char* text) {
    lv_textarea_set_text(responseTextArea, text);
    setState(UI_STATE_RESPONSE);
}

bool UIManager::isButtonPressed() {
    return buttonPressed;
}
