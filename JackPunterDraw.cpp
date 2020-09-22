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
        draw_rect.p0.y -= (metrics.line_height);
        draw_rect.p1.y -= (metrics.line_height);
        // NOTE(jack): the - 1 on the range is to remove the newline character on the end.
        draw_rect.p1.x += metrics.normal_advance * (def_line_range.end - def_line_range.start - 1);

        Rect_f32 draw_clip_rect = rect_intersect(draw_rect, buffer_region);

        draw_set_clip(app, draw_clip_rect);

        Text_Layout_ID def_text_layout_id = text_layout_create(
            app, highlight_data.def_buffer, draw_rect, def_buffer_point);
        draw_rectangle(app, draw_clip_rect, 5.0f, finalize_color(defcolor_back, 0));
        draw_rectangle_outline(app, draw_clip_rect, 5.0f,
                               2.0f, finalize_color(defcolor_highlight, 0));

        if (def_buffer_tokens.tokens != 0) {
            jp_draw_cpp_token_colors(app, def_text_layout_id,
                                     &def_buffer_tokens, highlight_data.def_buffer);
        } else {
            paint_text_color_fcolor(app, def_text_layout_id, def_line_range,
                                    fcolor_id(defcolor_highlight_junk));
        }

        draw_text_layout_default(app, def_text_layout_id);
        draw_set_clip(app, buffer_region);
        text_layout_free(app, text_layout_id);
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
jp_draw_definition_peek(Application_Links *app, Text_Layout_ID text_layout_id,
                        Face_ID face_id, View_ID vid, Buffer_ID buffer)
{
    ProfileScope(app, "JP Draw Macro definition");
    Scratch_Block scratch(app);
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    if(token_array.tokens != 0) {
        i64 cursor_pos = view_get_cursor_pos(app, vid);
        Token_Iterator_Array it = token_iterator_pos(0, &token_array, cursor_pos);
        Token *cursor_token = token_it_read(&it);

        Highlight_Data lookup_data;
        if (jp_custom_highlight_token_lookup(app, cursor_token, buffer, &lookup_data)) {
            Rect_f32 cursor_token_start_rect = text_layout_character_on_screen(
                app, text_layout_id, cursor_token->pos
            );
            if (lookup_data.def_pos != cursor_token->pos) {
                switch (lookup_data.type) {
                    case HighlightType_Macro: {
                        jp_draw_macro_definition(app, text_layout_id, face_id, vid, buffer,
                                                 lookup_data, cursor_token_start_rect);
                    } break;
                }
            }
        }
    }
}

#endif // FCODER_JACK_PUNTER_DRAW   