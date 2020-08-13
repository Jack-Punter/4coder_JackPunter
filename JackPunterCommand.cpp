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
            log_string(app, string_u8_litexpr("\n"));
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
            log_string(app, string_u8_litexpr("\n"));
        }
        
        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
}

function void
jp_list_todos(Application_Links *app, Buffer_ID buffer_id, String_Const_u8 user) {
    String_Const_u8 todos_name = string_u8_litexpr("*todo*");
    Buffer_ID todos_buffer = get_buffer_by_name(app, todos_name, Access_Always);
    if (!buffer_exists(app, todos_buffer)) {
        todos_buffer = create_buffer(app, todos_name, BufferCreate_AlwaysNew);
        buffer_set_setting(app, todos_buffer, BufferSetting_Unimportant, true);
        buffer_set_setting(app, todos_buffer, BufferSetting_ReadOnly, true);
        // TODO(jack): I might want to enable this?
        //buffer_set_setting(app, decls_buffer, BufferSetting_WrapLine, false);
    } else {
        clear_buffer(app, todos_buffer);
        buffer_send_end_signal(app, todos_buffer);
    }

    Scratch_Block scratch(app);
    String_Const_u8 buffer_name = push_buffer_unique_name(app, scratch, buffer_id);

    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if(token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        Cursor insertion_cursor = make_cursor(push_array(scratch, u8, KB(256)), KB(256));
        Buffer_Insertion out = begin_buffer_insertion_at_buffered(app, todos_buffer, 0, &insertion_cursor);
        
        do {
            Token *token = token_it_read(&it);
            if (token->kind == TokenBaseKind_Comment)
            {
                // NOTE(jack): I beleive that this sets a reset point in the memory arena*
                Temp_Memory temp = begin_temp(scratch);
                String_Const_u8 comment_text = push_buffer_range(app, scratch, buffer_id,
                                                                 {token->pos, token->pos + token->size});
                String_Const_u8 todo = string_u8_litexpr("TODO(");
                for (i64 index = token->pos;
                     comment_text.size > 0;
                     comment_text = string_skip(comment_text, 1), index += 1)
                {
                    String_Const_u8 prefix = string_prefix(comment_text, todo.size);
                    if(string_match(prefix, todo)) {
                        i64 line_number = get_line_number_from_pos(app, buffer_id, token->pos);
                        insertf(&out, "%.*s:%lld: ", string_expand(buffer_name), line_number);
                        if(user.size != 0) {
                            String_Const_u8 todo_user = string_skip(comment_text, prefix.size);
                            todo_user = string_prefix(todo_user, user.size);
                            if (string_match(todo_user, user)) {
                                insert_string(&out, comment_text);
                                insertc(&out, '\n');
                                log_string(app, comment_text);
                                log_string(app, string_u8_litexpr("\n"));
                            }
                        } else {
                            insert_string(&out, comment_text);
                            insertc(&out, '\n');
                            log_string(app, comment_text);
                            log_string(app, string_u8_litexpr("\n"));
                        }
                    }
                }
                end_temp(temp);
            }
        } while (token_it_inc_non_whitespace(&it));
        
        end_buffer_insertion(&out);

        View_ID view = get_active_view(app, Access_Always);
        view_set_buffer(app, view, todos_buffer, 0);
        
        lock_jump_buffer(app, todos_name);
    }
}

function void
jp_list_todos(Application_Links *app, String_Const_u8 user) {
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Read);
    do {
        jp_list_todos(app, buffer, user);
        // NOTE(jack): returns zero when its the last buffer
        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
}

CUSTOM_UI_COMMAND_SIG(jp_list_todos_all_users_all_buffers)
CUSTOM_DOC("List todos from all buffers for all users") {
    jp_list_todos(app, String_Const_u8{});
}

CUSTOM_UI_COMMAND_SIG(jp_list_todos_all_users_current_buffer)
CUSTOM_DOC("List todos from current buffer for all users") {
    View_ID vid = get_active_view(app, Access_Always);

    Buffer_ID buffer = view_get_buffer(app, vid, Access_Always);
    jp_list_todos(app, buffer, String_Const_u8{});
}

CUSTOM_UI_COMMAND_SIG(jp_list_todos_current_user_all_buffers)
CUSTOM_DOC("List todos from all buffers for current user") {
    jp_list_todos(app, global_config.user_name);
}

CUSTOM_UI_COMMAND_SIG(jp_list_todos_current_user_current_buffer)
CUSTOM_DOC("List todos from current buffer for current user") {
    View_ID vid = get_active_view(app, Access_Always);

    Buffer_ID buffer = view_get_buffer(app, vid, Access_Always);
    jp_list_todos(app, buffer, global_config.user_name);
}

#endif // FCODER_JACK_PUNTER_COMMANDS