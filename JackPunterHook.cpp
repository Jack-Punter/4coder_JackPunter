#if !defined(FCODER_JACK_PUNTER_HOOKS)
#define FCODER_JACK_PUNTER_HOOKS

#include "JackPunterAsync.cpp"

static void
jp_tick(Application_Links *app, Frame_Info frame_info)
{
    ProfileScope(app, "JP Tick Hook");
    
    /* default_tick */
    code_index_update_tick(app);
    if (tick_all_fade_ranges(app, frame_info.animation_dt)){
        animate_in_n_milliseconds(app, 0);
    }
    /* end default tick */
    
    Buffer_ID buffer = get_buffer_next(app, 0, Access_Read);
    do {
        Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer);
        jp_buffer_data_t *buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment,
                                                         jp_buffer_data_t);
        if (buffer_data && buffer_data->parse_contents && buffer_data->language != 0) {
            jp_parse_custom_highlights(app, buffer);
            buffer_data->parse_contents = false;
        }
        
        // NOTE(jack): returns zero when its the last buffer
        buffer = get_buffer_next(app, buffer, Access_Read);
    } while (buffer);
}

BUFFER_HOOK_SIG(jp_file_save)
CUSTOM_DOC("Jack Punter save file")
{
    ProfileScope(app, "JP File Save");
    clean_trailing_whitespace(app);
    default_file_save(app, buffer_id);
    
    Scratch_Block scratch(app);
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t *buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, jp_buffer_data_t);
    
    // TODO(jack): Replace this with the same approach in the begin buffer for robustness?
    // I.e. call the jp_trigger_parse_buffer async task so the lexer can finish, as the 
    // async lexer gets triggered on the BufferEditRange hook.
    if (buffer_data->language != 0) {
        jp_parse_custom_highlights(app, buffer_id);
    }
    
    return 0;
}

BUFFER_HOOK_SIG(jp_end_buffer){
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment,
                                                     jp_buffer_data_t);
    
    for(int i = 0; i < buffer_data->custom_highlight_array.count; ++i) {
        jp_erase_custom_highlight(app, buffer_data->custom_highlight_array.vals[i]);
    }
    linalloc_clear(&buffer_data->custom_highlights_arena);
    end_buffer_close_jump_list(app, buffer_id);
    return(0);
}

BUFFER_HOOK_SIG(jp_begin_buffer)
CUSTOM_DOC("Jack Punter begin buffer")
{
    //=================== MODIFIED DEFAULT BEGIN BUFFER ===================
    ProfileScope(app, "JP begin buffer");
    
    Scratch_Block scratch(app);
    Managed_Scope scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, scope, jp_buffer_attachment,
                                                     jp_buffer_data_t);
    
    
    b32 treat_as_code = false;
    String_Const_u8 file_name = push_buffer_file_name(app, scratch, buffer_id);
    
    if (file_name.size > 0) {
        String_Const_u8_Array extensions = global_config.code_exts;
        String_Const_u8 ext = string_file_extension(file_name);
        
        for (i32 i = 0; i < extensions.count; ++i) {
            if (string_match(ext, extensions.strings[i])){
                buffer_data->language = jp_get_language_from_extension(app, ext);
                treat_as_code = true;
                break;
            }
        }
    }
    
    Command_Map_ID map_id = (treat_as_code)?(mapid_code):(mapid_file);
    Command_Map_ID *map_id_ptr = scope_attachment(app, scope, buffer_map_id, Command_Map_ID);
    *map_id_ptr = map_id;
    
    Line_Ending_Kind setting = guess_line_ending_kind_from_buffer(app, buffer_id);
    Line_Ending_Kind *eol_setting = scope_attachment(app, scope, buffer_eol_setting, Line_Ending_Kind);
    *eol_setting = setting;
    
    // NOTE(allen): Decide buffer settings
    b32 wrap_lines = true;
    b32 use_lexer = false;
    if (treat_as_code){
        wrap_lines = global_config.enable_code_wrapping;
        use_lexer = true;
    }
    
    String_Const_u8 buffer_name = push_buffer_base_name(app, scratch, buffer_id);
    if (buffer_name.size > 0 && buffer_name.str[0] == '*' && buffer_name.str[buffer_name.size - 1] == '*'){
        wrap_lines = global_config.enable_output_wrapping;
    }
    
    Async_Task *lex_task_ptr = scope_attachment(app, scope, buffer_lex_task, Async_Task);
    if (use_lexer){
        ProfileBlock(app, "begin buffer kick off lexer");
        *lex_task_ptr = async_task_no_dep(&global_async_system, language_do_full_lex_async, make_data_struct(&buffer_id));
    }
    
    {
        b32 *wrap_lines_ptr = scope_attachment(app, scope, buffer_wrap_lines, b32);
        *wrap_lines_ptr = wrap_lines;
    }
    
    if (use_lexer){
        buffer_set_layout(app, buffer_id, layout_virt_indent_index_generic);
    }
    else{
        if (treat_as_code){
            buffer_set_layout(app, buffer_id, layout_virt_indent_literal_generic);
        }
        else{
            buffer_set_layout(app, buffer_id, layout_generic);
        }
    }
    
    //=====================================================================
    
    if (buffer_data->language != 0) {
        buffer_data->custom_highlights_arena = make_arena_system();
        
        if (use_lexer) {
            ProfileBlock(app, "begin buffer kick off parser");
            Async_Task *parser_task_ptr = scope_attachment(app, scope, buffer_parse_keywords_types_task, Async_Task);
            AsyncParserData data = { 0 };
            data.buffer_id = buffer_id;
            data.lexer_task = *lex_task_ptr;
            
            String_Const_u8 buffer_filename = push_buffer_file_name(app, scratch, buffer_id);
            String_Const_u8 ext = string_file_extension(buffer_filename);
            
            //*parser_task_ptr = async_task_no_dep(&global_async_system, jp_parse_keyword_types_async, make_data_struct(&data));
            *parser_task_ptr = async_task_no_dep(&global_async_system, jp_trigger_parse_buffer, make_data_struct(&data));
        }
    }
    
    return 0;
}

function void
jp_render_buffer(Application_Links *app, View_ID view_id, Face_ID face_id,
                 Buffer_ID buffer, Text_Layout_ID text_layout_id,
                 Rect_f32 rect)
{
    ProfileScope(app, "JP render buffer");
    
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);
    Rect_f32 prev_clip = draw_set_clip(app, rect);
    
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    
    // NOTE(allen): Cursor shape
    Face_Metrics metrics = get_face_metrics(app, face_id);
    f32 cursor_roundness = metrics.normal_advance*global_config.cursor_roundness;
    f32 mark_thickness = (f32)global_config.mark_thickness;
    
    // NOTE(allen): Token colorizing
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if (token_array.tokens != 0) {
        jp_draw_token_colors(app, text_layout_id, &token_array, buffer);
        
        // NOTE(allen): Scan for TODOs and NOTEs
        if (global_config.use_comment_keyword){
            Comment_Highlight_Pair pairs[] = {
                // NOTE(jack): the index into the theme's color aray (sencond arg to finalize_color)
                // is % ArrayCount([defcolor_comment_pop array])
                // so IMPORTANT will be the same color as NOTE in the 4coder theme as this only specifies 2 colors
                {string_u8_litexpr("NOTE"), finalize_color(defcolor_comment_pop, 0)},
                {string_u8_litexpr("TODO"), finalize_color(defcolor_comment_pop, 1)},
                {string_u8_litexpr("IMPORTANT"), finalize_color(defcolor_comment_pop, 2)},
            };
            draw_comment_highlights(app, buffer, text_layout_id,
                                    &token_array, pairs, ArrayCount(pairs));
        }
    } else {
        paint_text_color_fcolor(app, text_layout_id, visible_range, fcolor_id(defcolor_text_default));
    }
    
    i64 cursor_pos = view_correct_cursor(app, view_id);
    view_correct_mark(app, view_id);
    
    // NOTE(allen): Scope highlight
    if (global_config.use_scope_highlight){
        Color_Array colors = finalize_color_array(defcolor_back_cycle);
        draw_scope_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
    
    if (global_config.use_error_highlight || global_config.use_jump_highlight){
        // NOTE(allen): Error highlight
        String_Const_u8 name = string_u8_litexpr("*compilation*");
        Buffer_ID compilation_buffer = get_buffer_by_name(app, name, Access_Always);
        if (global_config.use_error_highlight){
            draw_jump_highlights(app, buffer, text_layout_id, compilation_buffer,
                                 fcolor_id(defcolor_highlight_junk));
        }
        
        // NOTE(allen): Search highlight
        if (global_config.use_jump_highlight){
            Buffer_ID jump_buffer = get_locked_jump_buffer(app);
            if (jump_buffer != compilation_buffer){
                draw_jump_highlights(app, buffer, text_layout_id, jump_buffer,
                                     fcolor_id(defcolor_highlight_white));
            }
        }
    }
    
    // NOTE(allen): Color parens
    if (global_config.use_paren_helper){
        Color_Array colors = finalize_color_array(defcolor_text_cycle);
        draw_paren_highlight(app, buffer, text_layout_id, cursor_pos, colors.vals, colors.count);
    }
    
    // NOTE(allen): Line highlight
    if (global_config.highlight_line_at_cursor && is_active_view){
        i64 line_number = get_line_number_from_pos(app, buffer, cursor_pos);
        draw_line_highlight(app, text_layout_id, line_number,
                            fcolor_id(defcolor_highlight_cursor_line));
    }
    
    // NOTE(allen): Whitespace highlight
    b64 show_whitespace = false;
    view_get_setting(app, view_id, ViewSetting_ShowWhitespace, &show_whitespace);
    if (show_whitespace){
        if (token_array.tokens == 0){
            draw_whitespace_highlight(app, buffer, text_layout_id, cursor_roundness);
        }
        else{
            draw_whitespace_highlight(app, text_layout_id, &token_array, cursor_roundness);
        }
    }
    
    // NOTE(allen): Cursor
    switch (fcoder_mode){
        case FCoderMode_Original:
        {
            draw_original_4coder_style_cursor_mark_highlight(app, view_id, is_active_view, buffer, text_layout_id, cursor_roundness, mark_thickness);
        }break;
        case FCoderMode_NotepadLike:
        {
            draw_notepad_style_cursor_highlight(app, view_id, buffer, text_layout_id, cursor_roundness);
        }break;
    }
    
    // NOTE(allen): Fade ranges
    paint_fade_ranges(app, text_layout_id, buffer);
    
    // NOTE(allen): put the actual text on the actual screen
    draw_text_layout_default(app, text_layout_id);
    
    draw_set_clip(app, prev_clip);
}

function void
jp_render_caller(Application_Links *app, Frame_Info frame_info, View_ID view_id)
{
    ProfileScope(app, "jp render caller");
    View_ID active_view = get_active_view(app, Access_Always);
    b32 is_active_view = (active_view == view_id);
    
    Rect_f32 region = draw_background_and_margin(app, view_id, is_active_view);
    Rect_f32 prev_clip = draw_set_clip(app, region);
    
    Buffer_ID buffer = view_get_buffer(app, view_id, Access_Always);
    Face_ID face_id = get_face_id(app, buffer);
    Face_Metrics face_metrics = get_face_metrics(app, face_id);
    f32 line_height = face_metrics.line_height;
    f32 digit_advance = face_metrics.decimal_digit_advance;
    
    // NOTE(allen): file bar
    b64 showing_file_bar = false;
    if (view_get_setting(app, view_id, ViewSetting_ShowFileBar, &showing_file_bar) && showing_file_bar){
        Rect_f32_Pair pair = layout_file_bar_on_top(region, line_height);
        draw_file_bar(app, view_id, buffer, face_id, pair.min);
        region = pair.max;
    }
    
    Buffer_Scroll scroll = view_get_buffer_scroll(app, view_id);
    
    Buffer_Point_Delta_Result delta = delta_apply(app, view_id,
                                                  frame_info.animation_dt, scroll);
    if (!block_match_struct(&scroll.position, &delta.point)){
        block_copy_struct(&scroll.position, &delta.point);
        view_set_buffer_scroll(app, view_id, scroll, SetBufferScroll_NoCursorChange);
    }
    if (delta.still_animating){
        animate_in_n_milliseconds(app, 0);
    }
    
    // NOTE(allen): query bars
    region = default_draw_query_bars(app, region, view_id, face_id);
    
    // NOTE(allen): FPS hud
    if (show_fps_hud){
        Rect_f32_Pair pair = layout_fps_hud_on_bottom(region, line_height);
        draw_fps_hud(app, frame_info, face_id, pair.max);
        region = pair.min;
        animate_in_n_milliseconds(app, 1000);
    }
    
    // NOTE(allen): layout line numbers
    Rect_f32 line_number_rect = {};
    if (global_config.show_line_number_margins){
        Rect_f32_Pair pair = layout_line_number_margin(app, buffer, region, digit_advance);
        line_number_rect = pair.min;
        region = pair.max;
    }
    
    // NOTE(allen): begin buffer render
    Buffer_Point buffer_point = scroll.position;
    Text_Layout_ID text_layout_id = text_layout_create(app, buffer, region, buffer_point);
    
    // NOTE(allen): draw line numbers
    if (global_config.show_line_number_margins){
        draw_line_number_margin(app, view_id, buffer, face_id, text_layout_id, line_number_rect);
    }
    
    // NOTE(jack): Call my copy of the default renderer which does custom highlighting
    jp_render_buffer(app, view_id, face_id, buffer, text_layout_id, region);
    
    if (is_active_view && GlobalShowDefinitionPeeks) {
        jp_draw_definition_helpers(app, text_layout_id, face_id, view_id, buffer);
    }
    
    if (/*is_active_view && */GlobalShowScopeHelpers) {
        jp_draw_scope_helpers(app, text_layout_id, face_id, view_id, buffer);
    }
    
    F4_RenderDividerComments(app, buffer, view_id, text_layout_id);

    text_layout_free(app, text_layout_id);
    draw_set_clip(app, prev_clip);
}

//
//
//


Token_List language_buffer_edit_range__relex(Application_Links *app, Buffer_ID buffer_id, Arena *arena, String_Const_u8 text)
{
    Language *language = buffer_get_language(app, buffer_id);
    return language->lex_full_input(arena, text);
}

BUFFER_EDIT_RANGE_SIG(language_buffer_edit_range)
{
    // buffer_id, new_range, original_size
    ProfileScope(app, "[Language] Buffer Edit Range");
    
    Range_i64 old_range = Ii64(old_cursor_range.min.pos, old_cursor_range.max.pos);
    buffer_shift_fade_ranges(buffer_id, old_range.max, (new_range.max - old_range.max));
    
    {
        code_index_lock();
        Code_Index_File *file = code_index_get_file(buffer_id);
        if (file != 0){
            code_index_shift(file, old_range, range_size(new_range));
        }
        code_index_unlock();
    }
    
    i64 insert_size = range_size(new_range);
    i64 text_shift = replace_range_shift(old_range, insert_size);
    
    Scratch_Block scratch(app);
    
    Managed_Scope scope = buffer_get_managed_scope(app, buffer_id);
    Async_Task *lex_task_ptr = scope_attachment(app, scope, buffer_lex_task, Async_Task);
    
    Base_Allocator *allocator = managed_scope_allocator(app, scope);
    b32 do_full_relex = false;
    
    if (async_task_is_running_or_pending(&global_async_system, *lex_task_ptr)){
        async_task_cancel(app, &global_async_system, *lex_task_ptr);
        buffer_unmark_as_modified(buffer_id);
        do_full_relex = true;
        *lex_task_ptr = 0;
    }
    
    Token_Array *ptr = scope_attachment(app, scope, attachment_tokens, Token_Array);
    if (ptr != 0 && ptr->tokens != 0){
        ProfileBlockNamed(app, "attempt resync", profile_attempt_resync);
        
        i64 token_index_first = token_relex_first(ptr, old_range.first, 1);
        i64 token_index_resync_guess =
            token_relex_resync(ptr, old_range.one_past_last, 32);
        
        if (token_index_resync_guess - token_index_first >= 4000){
            do_full_relex = true;
        }
        else{
            Token *token_first = ptr->tokens + token_index_first;
            Token *token_resync = ptr->tokens + token_index_resync_guess;
            
            Range_i64 relex_range = Ii64(token_first->pos, token_resync->pos + token_resync->size + text_shift);
            String_Const_u8 partial_text = push_buffer_range(app, scratch, buffer_id, relex_range);
            
            Token_List relex_list = language_buffer_edit_range__relex(app, buffer_id, scratch, partial_text);
            if (relex_range.one_past_last < buffer_get_size(app, buffer_id)){
                token_drop_eof(&relex_list);
            }
            
            Token_Relex relex = token_relex(relex_list, relex_range.first - text_shift, ptr->tokens, token_index_first, token_index_resync_guess);
            
            ProfileCloseNow(profile_attempt_resync);
            
            if (!relex.successful_resync){
                do_full_relex = true;
            }
            else{
                ProfileBlock(app, "apply resync");
                
                i64 token_index_resync = relex.first_resync_index;
                
                Range_i64 head = Ii64(0, token_index_first);
                Range_i64 replaced = Ii64(token_index_first, token_index_resync);
                Range_i64 tail = Ii64(token_index_resync, ptr->count);
                i64 resynced_count = (token_index_resync_guess + 1) - token_index_resync;
                i64 relexed_count = relex_list.total_count - resynced_count;
                i64 tail_shift = relexed_count - (token_index_resync - token_index_first);
                
                i64 new_tokens_count = ptr->count + tail_shift;
                Token *new_tokens = base_array(allocator, Token, new_tokens_count);
                
                Token *old_tokens = ptr->tokens;
                block_copy_array_shift(new_tokens, old_tokens, head, 0);
                token_fill_memory_from_list(new_tokens + replaced.first, &relex_list, relexed_count);
                for (i64 i = 0, index = replaced.first; i < relexed_count; i += 1, index += 1){
                    new_tokens[index].pos += relex_range.first;
                }
                for (i64 i = tail.first; i < tail.one_past_last; i += 1){
                    old_tokens[i].pos += text_shift;
                }
                block_copy_array_shift(new_tokens, ptr->tokens, tail, tail_shift);
                
                base_free(allocator, ptr->tokens);
                
                ptr->tokens = new_tokens;
                ptr->count = new_tokens_count;
                ptr->max = new_tokens_count;
                
                buffer_mark_as_modified(buffer_id);
            }
        }
    }
    
    if (do_full_relex){
        *lex_task_ptr = async_task_no_dep(&global_async_system, language_do_full_lex_async,
                                          make_data_struct(&buffer_id));
    }
    
    // no meaning for return
    return(0);
}

#endif // FCODER_JACK_PUNTER_HOOKS