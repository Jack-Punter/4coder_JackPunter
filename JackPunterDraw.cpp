#if !defined(FCODER_JACK_PUNTER_DRAW)
#define FCODER_JACK_PUNTER_DRAW

function ARGB_Color
jp_get_token_color_cpp(Application_Links *app, Token token, String_Const_u8 lexeme)
{
    ProfileScope(app, "JP Get Token Color CPP");
    ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_text_default));
    switch (token.kind){
        case TokenBaseKind_Preprocessor:
        {
            color = fcolor_resolve(fcolor_id(defcolor_preproc));
        }break;
        case TokenBaseKind_Keyword:
        {
            if (jp_is_type_token(&token)){
                color = finalize_color(defcolor_keyword, 1);
            } else {
                color = finalize_color(defcolor_keyword, 0);
            }
        }break;
        case TokenBaseKind_Comment:
        {
            color = fcolor_resolve(fcolor_id(defcolor_comment));
        }break;
        case TokenBaseKind_LiteralString:
        {
            color = fcolor_resolve(fcolor_id(defcolor_str_constant));
        }break;
        case TokenBaseKind_LiteralInteger:
        {
            color = fcolor_resolve(fcolor_id(defcolor_int_constant));
        }break;
        case TokenBaseKind_LiteralFloat:
        {
            color = fcolor_resolve(fcolor_id(defcolor_float_constant));
        }break;
        case TokenBaseKind_Identifier:
        {
            Highlight_Data lookup_data;
            if (jp_custom_highlight_lookup(app, lexeme, &lookup_data)) {
                switch (lookup_data.type)
                {
                case HighlightType_Keyword:
                    color = finalize_color(defcolor_keyword, 0);
                    break;
                case HighlightType_Type:
                    color = finalize_color(defcolor_keyword, 1);
                    break;
                case HighlightType_Function:
                    color = finalize_color(defcolor_keyword, 2);
                    break;
                case HighlightType_Macro:
                    color = finalize_color(defcolor_preproc, 1);
                break;
                }
            }
        }break;
    }
    // specifics override generals
    switch (token.sub_kind){
        case TokenCppKind_LiteralTrue:
        case TokenCppKind_LiteralFalse:
        {
            color = fcolor_resolve(fcolor_id(defcolor_bool_constant));
        }break;
        case TokenCppKind_LiteralCharacter:
        case TokenCppKind_LiteralCharacterWide:
        case TokenCppKind_LiteralCharacterUTF8:
        case TokenCppKind_LiteralCharacterUTF16:
        case TokenCppKind_LiteralCharacterUTF32:
        {
            color = fcolor_resolve(fcolor_id(defcolor_char_constant));
        }break;
        case TokenCppKind_PPIncludeFile:
        {
            color = fcolor_resolve(fcolor_id(defcolor_include));
        }break;
    }
    
    return(color);
}

function void
jp_draw_cpp_token_colors(Application_Links *app, Text_Layout_ID text_layout_id, Token_Array *array,  Buffer_ID buffer){
    ProfileScope(app, "jp_draw_cpp_token_colors");
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    i64 first_index = token_index_from_pos(array, visible_range.first);
    Token_Iterator_Array it = token_iterator_index(0, array, first_index);
    for (;;)
    {
        Token *token = token_it_read(&it);
        if (token->pos >= visible_range.one_past_last){
            break;
        }

        Scratch_Block scratch(app);
        Range_i64 r = Ii64(token);
        String_Const_u8 lexeme = push_buffer_range(app, scratch, buffer, r);
        ARGB_Color argb = jp_get_token_color_cpp(app, *token, lexeme);

        paint_text_color(app, text_layout_id, r, argb);
        if (!token_it_inc_all(&it)) {
            break;
        }
    }
}

function void
jp_draw_macro_definition(Application_Links *app, Text_Layout_ID text_layout_id,
                         Face_ID face_id, View_ID vid, Buffer_ID buffer,
                         Highlight_Data highlight_data, Rect_f32 popup_anchor)
{
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    Face_Metrics metrics = get_face_metrics(app, face_id);

    Rect_f32 buffer_region = view_get_buffer_region(app, vid);

    i64 def_line = get_line_number_from_pos(app, highlight_data.def_buffer,
                                            highlight_data.def_pos);

    Range_i64 def_line_range = get_line_pos_range(app, highlight_data.def_buffer, def_line);
    Buffer_Point def_buffer_point = {def_line, 0};
    Token_Array def_buffer_tokens = get_token_array_from_buffer(app, highlight_data.def_buffer);

    auto draw_macro_definition = [&]() {
        Rect_f32 draw_rect = popup_anchor;
        f32 single_line_endX = draw_rect.x1 + metrics.normal_advance *
                               (def_line_range.end - def_line_range.start - 1);

        draw_rect.x1 = Min(buffer_region.x1, single_line_endX);
        draw_rect.y1 -= metrics.line_height;
        // NOTE(jack): buffer_line_y_difference return top(A) - top(B) in pixels
        f32 y_difference = buffer_line_y_difference(app, buffer, rect_width(draw_rect), face_id,
                                                    /*A*/ def_line, /*B*/ def_line + 1);
        draw_rect.y0 += y_difference;

        // NOTE(jack): Actually draw the text to the screen
        Text_Layout_ID def_text_layout_id = text_layout_create(
            app, highlight_data.def_buffer, draw_rect, def_buffer_point
        );

        draw_set_clip(app, draw_rect);
        
        draw_rectangle(app, draw_rect, 5.0f, finalize_color(defcolor_back, 0));
        draw_rectangle_outline(app, draw_rect, 5.0f,
                               2.0f, finalize_color(defcolor_highlight, 0));

        if (def_buffer_tokens.tokens != 0) {
            jp_draw_cpp_token_colors(app, def_text_layout_id,
                                     &def_buffer_tokens, highlight_data.def_buffer);
        } else {
            paint_text_color_fcolor(app, def_text_layout_id, def_line_range,
                                    fcolor_id(defcolor_text_default));
        }

        draw_text_layout_default(app, def_text_layout_id);
        draw_set_clip(app, buffer_region);
        text_layout_free(app, def_text_layout_id);
    };
    // If the definition is in the current buffer (and visible) we just draw an outline around it
    if (highlight_data.def_buffer == buffer &&
        range_contains(visible_range, def_line_range.start))
    {
        Rect_f32 draw_rect = text_layout_character_on_screen(app, text_layout_id,
                                                             def_line_range.start);
        Rect_f32 end_rect = text_layout_character_on_screen(app, text_layout_id,
                                                            def_line_range.end);

        if (draw_rect.y1 == end_rect.y1) {
            draw_rect.x1 = end_rect.x1 - metrics.normal_advance;
        } else {
            draw_rect.y1 = end_rect.y1;
            draw_rect.x1 = buffer_region.x1;
        }
        Rect_f32 final_draw_rect = rect_intersect(draw_rect, buffer_region);
        if (final_draw_rect == draw_rect) {
            draw_rectangle_outline(app, draw_rect, 5.0f, 2.0f,
                                   finalize_color(defcolor_highlight, 0));
        } else {
            draw_macro_definition();
        }
    } else {
        draw_macro_definition();
    }
}

function void
jp_draw_function_params(Application_Links *app, Text_Layout_ID text_layout_id,
                        Face_ID face_id, View_ID vid, Buffer_ID buffer,
                        Highlight_Data highlight_data, i32 cursor_param_index)
{
    Token_Array def_buffer_tokens = get_token_array_from_buffer(app, highlight_data.def_buffer);
    
    if (def_buffer_tokens.tokens != 0)
    {
        Scratch_Block scratch(app);

        i64 cursor_pos = view_get_cursor_pos(app, vid);
        Token_Iterator_Array it = token_iterator_pos(0, &def_buffer_tokens, cursor_pos);

        List_String_Const_u8 function_parameter_list = {};
        it = token_iterator_pos(0, &def_buffer_tokens, highlight_data.def_pos);
        // NOTE(jack): loop from the definition token forwards
        {
            b32 found_close_paren = false;
            i32 paren_opens = 0;
            i64 transient_single_param_start = {0};
            while (!found_close_paren)
            {
                if (!token_it_inc_non_whitespace(&it)) {
                    break;
                }
                Token *token = token_it_read(&it);

                if (token->kind == TokenBaseKind_ParentheticalOpen) {
                    if(paren_opens == 0) {
                        transient_single_param_start = token->pos + 1;
                    }
                    ++paren_opens;
                } else if (token->kind == TokenBaseKind_ParentheticalClose) {
                    // NOTE(jack): If we reach the closing paren of the function
                    if (--paren_opens == 0) {
                        String_Const_u8 param_string =
                            push_buffer_range(app, scratch, buffer,
                                             {transient_single_param_start, token->pos});
                        String_Const_u8 condensed_param_string = string_condense_whitespace(scratch, param_string);
                        string_list_push(scratch, &function_parameter_list, condensed_param_string);

                        found_close_paren = true;
                    }
                } else if(token->sub_kind == TokenCppKind_Comma) {
                    String_Const_u8 param_string =
                        push_buffer_range(app, scratch, buffer,
                                          {transient_single_param_start, token->pos});
                    String_Const_u8 condensed_param_string = string_condense_whitespace(scratch, param_string);
                    string_list_push(scratch, &function_parameter_list, condensed_param_string);

                    transient_single_param_start = token->pos + 1;
                }
            }
        }
        if (cursor_param_index <= function_parameter_list.node_count) {
            i64 line_after_cursor = 1 + get_line_number_from_pos(app, buffer, cursor_pos);
            i64 line_after_cursor_start = get_line_pos_range(app, buffer, line_after_cursor).start;
            String_Const_u8 after_cursor = push_buffer_range(app, scratch, buffer,
                                                            {cursor_pos, line_after_cursor_start});
            String_Const_u8 after_cursor_condensed = string_condense_whitespace(scratch, after_cursor);
            // NOTE(jack): if it is only whitespace, between cursor and newline
            if(after_cursor_condensed.size == 0) {
                List_String_Const_u8 draw_string_list = function_parameter_list;
                String_Const_u8 first_peek_param = {};
                {
                i32 parameter_index = 0;
                    for (Node_String_Const_u8 *node = function_parameter_list.first;
                        node != 0; node = node->next)
                    {
                        if(parameter_index == cursor_param_index) {
                            first_peek_param = node->string;

                            draw_string_list.first = node->next;
                            draw_string_list.total_size -= node->string.size;
                            --draw_string_list.node_count;
                            break;
                        } else {
                            draw_string_list.total_size -= node->string.size;
                            --draw_string_list.node_count;
                        }
                        ++parameter_index;
                    }
                }
                String_Const_u8 peek_string = {};
                if (draw_string_list.node_count > 0) {
                    peek_string = string_list_flatten(scratch, draw_string_list, SCu8(", "),
                                        StringSeparator_BeforeFirst, StringFill_NoTerminate);
                }
                
                Face_Metrics metrics = get_face_metrics(app, face_id);
                Vec2_f32 draw_pos = text_layout_character_on_screen(app, text_layout_id, cursor_pos).p0;
                draw_pos.x += metrics.normal_advance;

                ARGB_Color peek_col = finalize_color(defcolor_text_default, 0);
                auto set_color_alpha = [](ARGB_Color col, u8 alpha) -> ARGB_Color {
                    ARGB_Color Result = col;
                    Result &= 0x00FFFFFF;
                    Result |= alpha << 24;
                    return Result;
                };

                peek_col = set_color_alpha(peek_col, 0xC0);
                draw_pos = draw_string(app, face_id, first_peek_param, draw_pos, peek_col);
                peek_col = set_color_alpha(peek_col, 0x80);
                draw_pos = draw_string(app, face_id, peek_string, draw_pos, peek_col);
                draw_string(app, face_id, SCu8(");"), draw_pos, peek_col);
            }
        }
    }
}

function void
jp_draw_definition_helpers(Application_Links *app, Text_Layout_ID text_layout_id,
                           Face_ID face_id, View_ID vid, Buffer_ID buffer)
{
    ProfileScope(app, "JP Draw Macro definition");
    Scratch_Block scratch(app);
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if(token_array.tokens != 0)
    {
        bool found_peek_target = false;
        i32 cursor_parameter_index = 0;
        Highlight_Data lookup_data = {};
        Token *peek_target;

        {
            i64 cursor_pos = view_get_cursor_pos(app, vid);
            Token_Iterator_Array it = token_iterator_pos(0, &token_array, cursor_pos);
            peek_target = token_it_read(&it);
            i32 open_count = 0;
            i32 close_count = 0;

            while (!found_peek_target)
            {
                peek_target = token_it_read(&it);
                // If the token has highlight data, and the cursor is inside its parameters prenthesis
                if (jp_custom_highlight_token_lookup(app, peek_target, buffer, &lookup_data))
                {
                    auto is_macro_peek_target = [](HighlightType type, i32 open_count, i32 close_count) -> bool {
                        // If the token is a macro
                        // and the cursor is on the token: (close_count == 0 && open_count == 0)
                        // or within the parameters brackets: (open_count > close_count)
                        // then the it's a valid peek token
                        return (type == HighlightType_Macro &&
                                ((close_count == 0 && open_count == 0) ||
                                 (open_count > close_count)));
                    };

                    auto is_function_peek_target = [](HighlightType type, i32 open_count, i32 close_count) -> bool {
                        // If the token is a fucntion
                        // and the cursor is within the parameters brackets: (open_count > close_count)
                        return (type == HighlightType_Function && open_count > close_count);
                    };

                    found_peek_target = is_macro_peek_target(lookup_data.type,open_count, close_count) ||
                                        is_function_peek_target(lookup_data.type,open_count, close_count);
                }
                else
                {
                    // NOTE(jack): These kinds/sub_kinds are the break condition so that we don't
                    // iterate back to the beginning of the buffer
                    if (peek_target->kind == TokenBaseKind_Preprocessor ||
                        peek_target->kind == TokenBaseKind_ScopeClose ||
                        peek_target->sub_kind == TokenCppKind_Semicolon)
                    {
                        break;
                    }

                    if (peek_target->kind == TokenBaseKind_ParentheticalClose) {
                        close_count++;
                    } else if (peek_target->kind == TokenBaseKind_ParentheticalOpen) {
                        open_count++;
                    } else if (peek_target->sub_kind == TokenCppKind_Comma) {
                        cursor_parameter_index++;
                    }
                }
                if(!token_it_dec_non_whitespace(&it)) {
                    break;
                }
            }
        }
        // If a peek target has been found and it isn't the definition
        if (found_peek_target && 
            !(lookup_data.def_buffer == buffer && lookup_data.def_pos == peek_target->pos))
        {
            Rect_f32 popup_anchor_rect = text_layout_character_on_screen(
                app, text_layout_id, peek_target->pos
            );

            switch (lookup_data.type) {
                case HighlightType_Function: {
                    jp_draw_function_params(app, text_layout_id, face_id, vid, buffer,
                                            lookup_data, cursor_parameter_index);
                } break;
                case HighlightType_Macro: {
                    jp_draw_macro_definition(app, text_layout_id, face_id, vid, buffer,
                                             lookup_data, popup_anchor_rect);
                } break;
            }
        }
    }
}

#endif // FCODER_JACK_PUNTER_DRAW   