#if !defined(FCODER_JACK_PUNTER_COMMANDS)
#define FCODER_JACK_PUNTER_COMMANDS

CUSTOM_UI_COMMAND_SIG(jp_macro_toggle_recording)
CUSTOM_DOC("Toggle Recording Keyboard Macro")
{
    DEBUG_MSG_LIT("jp_macro_toggle_recording");
    if (GlobalIsRecordingMacro) {
        keyboard_macro_finish_recording(app);
        GlobalIsRecordingMacro = false;
    } else {
        keyboard_macro_start_recording(app);
        GlobalIsRecordingMacro = true;
    }
}

CUSTOM_UI_COMMAND_SIG(jp_switch_buffer_other)
CUSTOM_DOC("Switch to an open buffer in the other panel")
{
    DEBUG_MSG_LIT("jp_switch_buffer_other");
    change_active_panel(app);
    View_ID vid = get_active_view(app, Access_Always);
    view_enqueue_command_function(app, vid, interactive_switch_buffer);
}

CUSTOM_UI_COMMAND_SIG(jp_log_custom_keywords)
CUSTOM_DOC("Log_Custom_keywords")
{
    char count_buffer[16] = {};
    snprintf(count_buffer, 16, "%llu / %llu", custom_keywords_end, total_custom_keywords);
    log_string(app, string_u8_litexpr("Array Count: "));
    log_string(app, SCu8(count_buffer));
    log_string(app, string_u8_litexpr("\n"));
    
    for (size_t i = 0; i < custom_keywords_end; ++i){
        log_string(app, custom_keywords[i]);
        log_string(app, string_u8_litexpr("\n"));
    }
}

#endif // FCODER_JACK_PUNTER_COMMANDS