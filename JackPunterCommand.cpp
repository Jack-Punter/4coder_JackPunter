#if !defined(FCODER_JACK_PUNTER_COMMANDS)
#define FCODER_JACK_PUNTER_COMMANDS

CUSTOM_UI_COMMAND_SIG(jp_macro_toggle_recording)
CUSTOM_DOC("Toggle Recording Keyboard Macro")
{
    if (GlobalIsRecordingMacro) {
        keyboard_macro_finish_recording(app);
        GlobalIsRecordingMacro = false;
    } else {
        keyboard_macro_start_recording(app);
        GlobalIsRecordingMacro = true;
    }
}

CUSTOM_UI_COMMAND_SIG(jp_toggle_definition_peek)
CUSTOM_DOC("Toggle Show Definition Peeks")
{
    GlobalShowDefinitionPeeks = !GlobalShowDefinitionPeeks;
}

CUSTOM_UI_COMMAND_SIG(jp_toggle_scope_helpers)
CUSTOM_DOC("Toggle Show Definition Peeks")
{
    GlobalShowScopeHelpers = !GlobalShowScopeHelpers;
}

CUSTOM_UI_COMMAND_SIG(jp_switch_buffer_other)
CUSTOM_DOC("Switch to an open buffer in the other panel")
{
    change_active_panel(app);
    View_ID vid = get_active_view(app, Access_Always);
    view_enqueue_command_function(app, vid, interactive_switch_buffer);
}

CUSTOM_UI_COMMAND_SIG(jp_insert_deref_access)
CUSTOM_DOC("Insert -> to access a pointer's data member")
{
    write_text(app, string_u8_litexpr("->"));
}

CUSTOM_UI_COMMAND_SIG(jp_cut_line)
CUSTOM_DOC("Cut the line that the cursor is on")
{
    View_ID view = get_active_view(app, Access_ReadWriteVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadWriteVisible);
    i64 pos = view_get_cursor_pos(app, view);
    i64 line = get_line_number_from_pos(app, buffer, pos);
    Range_i64 range = get_line_pos_range(app, buffer, line);
    range.end += 1;
    i32 size = (i32)buffer_get_size(app, buffer);
    range.end = clamp_top(range.end, size);
    if (range_size(range) == 0 || buffer_get_char(app, buffer, range.end - 1) != '\n'){
        range.start -= 1;
        range.first = clamp_bot(0, range.first);
    }

    if (clipboard_post_buffer_range(app, 0, buffer, range)) {
        buffer_replace_range(app, buffer, range, string_u8_litexpr(""));
    }
}

CUSTOM_UI_COMMAND_SIG(jp_copy_line)
CUSTOM_DOC("Copy the line that the cursor is on")
{
    View_ID view = get_active_view(app, Access_ReadVisible);
    Buffer_ID buffer = view_get_buffer(app, view, Access_ReadVisible);
    i64 pos = view_get_cursor_pos(app, view);
    i64 line = get_line_number_from_pos(app, buffer, pos);
    Range_i64 range = get_line_pos_range(app, buffer, line);
    range.end += 1;
    i32 size = (i32)buffer_get_size(app, buffer);
    range.end = clamp_top(range.end, size);
    if (range_size(range) == 0 || buffer_get_char(app, buffer, range.end - 1) != '\n'){
        range.start -= 1;
        range.first = clamp_bot(0, range.first);
    }

    clipboard_post_buffer_range(app, 0, buffer, range);
}
/***********************************************************************************/
/*                            NOTE(jack): TODO LISTING                             */
/***********************************************************************************/

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
                String_Const_u8 comment_text = push_buffer_range(app, scratch, buffer_id, Ii64(token));
                String_Const_u8 todo = string_u8_litexpr("TODO");
                for (i64 index = token->pos;
                     comment_text.size > 0;
                     comment_text = string_skip(comment_text, 1), index += 1)
                {
                    String_Const_u8 prefix = string_prefix(comment_text, todo.size);
                    if(string_match(prefix, todo)) {
                        i64 line_number = get_line_number_from_pos(app, buffer_id, token->pos);
                        insertf(&out, "%.*s:%lld: ", string_expand(buffer_name), line_number);

                        if(user.size != 0) {
                            // NOTE(jack): prefix.size + 1 to skip the open bracket "TODO(jack): "
                            String_Const_u8 todo_user = string_skip(comment_text, prefix.size + 1);
                            todo_user = string_prefix(todo_user, user.size);
                            if (string_match(todo_user, user)) {
                                
                                u64 newline_pos = string_find_first(comment_text, '\n');
                                if ( newline_pos != comment_text.size) {
                                    comment_text = string_prefix(comment_text, newline_pos);
                                }

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

        change_active_panel(app);
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

/***********************************************************************************/
/*                          NOTE(jack): Rectangular Edits                          */
/***********************************************************************************/

function void
replace_in_rect(Application_Links *app, Buffer_ID buffer, View_ID vid,
                      String_Const_u8 replacment)
{
    i64 cursor = view_get_cursor_pos(app, vid);
    i64 mark = view_get_mark_pos(app, vid);

    i64 min_cursor = Min(cursor, mark);
    i64 max_cursor = Max(cursor, mark);

    i64 min_line = get_line_number_from_pos(app, buffer, min_cursor);
    i64 min_line_start = get_line_start_pos(app, buffer, min_line);

    i64 max_line = get_line_number_from_pos(app, buffer, max_cursor);
    i64 max_line_start = get_line_start_pos(app, buffer, max_line);

    i64 min_column = Min(min_cursor - min_line_start,
                         max_cursor - max_line_start);

    i64 max_column = Max(min_cursor - min_line_start,
                         max_cursor - max_line_start);

    // for each line that the cursors mark (inclusive)
    // NOTE(jack): The history group stuff makes the edit undo in a single command.
    History_Group new_history_group = history_group_begin(app, buffer);
    for (i64 line = min_line; line <= max_line; ++line)
    {
        i64 line_start = get_line_start_pos(app, buffer, line);
        Range_i64 line_kill_range = Ii64(line_start + min_column,
                                         line_start + max_column);
        buffer_replace_range(app, buffer, line_kill_range, replacment);
    }
    history_group_end(new_history_group);
}

CUSTOM_UI_COMMAND_SIG(jp_kill_rect)
CUSTOM_DOC("Remove the content in the rectangle bounded by the cursor and the mark")
{
    View_ID vid = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, vid, Access_Always);
    replace_in_rect(app, buffer, vid, SCu8(""));
}

CUSTOM_UI_COMMAND_SIG(jp_replace_rect)
CUSTOM_DOC("Remove the content in the rectangle bounded by the cursor and the mark")
{
    View_ID vid = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, vid, Access_Always);
    
    Query_Bar_Group group(app);
    Query_Bar replace_bar = {};
    replace_bar.prompt = SCu8("Replace: ");
    u8 replace_buffer[KB(1)];
    replace_bar.string.str = replace_buffer;
    replace_bar.string_capacity = sizeof(replace_buffer);

    if (query_user_string(app, &replace_bar) && replace_bar.string.size > 0) {
        replace_in_rect(app, buffer, vid, replace_bar.string);
    }
}

//~ NOTE(jack): Font Size Changing stuff.

CUSTOM_COMMAND_SIG(jp_increase_face_size)
CUSTOM_DOC("Increase the size of the face used by the current buffer.")
{
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Description description = get_face_description(app, face_id);
    ++description.parameters.pt_size;
    try_modify_face(app, face_id, &description);

    Face_Description small_description = get_face_description(app, GlobalSmallCodeFaceID);
    ++small_description.parameters.pt_size;
    try_modify_face(app, GlobalSmallCodeFaceID, &small_description);
}

CUSTOM_COMMAND_SIG(jp_decrease_face_size)
CUSTOM_DOC("Decrease the size of the face used by the current buffer.")
{
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Description description = get_face_description(app, face_id);
    --description.parameters.pt_size;
    try_modify_face(app, face_id, &description);

    Face_Description small_description = get_face_description(app, GlobalSmallCodeFaceID);
    --small_description.parameters.pt_size;
    try_modify_face(app, GlobalSmallCodeFaceID, &small_description);
}

CUSTOM_COMMAND_SIG(jp_mouse_wheel_change_face_size)
CUSTOM_DOC("Reads the state of the mouse wheel and uses it to either increase or decrease the face size.")
{
    local_persist u64 next_resize_time = 0;
    u64 now = system_now_time();
    if (now >= next_resize_time){
        next_resize_time = now + 50*1000;
        Mouse_State mouse = get_mouse_state(app);
        if (mouse.wheel > 0){
            jp_decrease_face_size(app);
        }
        else if (mouse.wheel < 0){
            jp_increase_face_size(app);
        }
    }
}

#endif // FCODER_JACK_PUNTER_COMMANDS