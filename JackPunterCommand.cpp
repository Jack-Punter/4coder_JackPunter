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
CUSTOM_DOC("Log Custom_keywords")
{
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Read);
    do {
        jp_buffer_data_t *buffer_data = scope_attachment(app, buffer, jp_buffer_attachment, 
                                                         jp_buffer_data_t);

        for (size_t i = 0; i < buffer_data->custom_keywords_end; ++i){
            log_string(app, buffer_data->custom_keywords[i]);
        }

        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
}

CUSTOM_UI_COMMAND_SIG(jp_log_custom_types)
CUSTOM_DOC("Log Custom_keywords")
{
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Read);
    do {
        jp_buffer_data_t *buffer_data = scope_attachment(app, buffer, jp_buffer_attachment, 
                                                         jp_buffer_data_t);

        for (size_t i = 0; i < buffer_data->custom_types_end; ++i){
            log_string(app, buffer_data->custom_types[i]);
        }

        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
}

#endif // FCODER_JACK_PUNTER_COMMANDS