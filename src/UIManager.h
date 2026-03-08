#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <lvgl.h>

enum UIState {
    UI_STATE_IDLE,
    UI_STATE_RECORDING,
    UI_STATE_THINKING,
    UI_STATE_RESPONSE
};

class UIManager {
public:
    static void begin();
    static void update();
    static void setState(UIState state);
    static void setResponse(const char* text);
    
    // Callbacks for the main loop to handle recording
    static bool isButtonPressed();

private:
    static lv_obj_t* mainScreen;
    static lv_obj_t* statusLabel;
    static lv_obj_t* actionButton;
    static lv_obj_t* responseTextArea;
    static lv_obj_t* spinner;
    
    static UIState currentState;
    static void createIdleScreen();
    static void handleButtonEvent(lv_event_t* e);
};

#endif // UI_MANAGER_H
