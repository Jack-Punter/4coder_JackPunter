#if !defined(FCODER_JACK_PUNTER_DRAW)
#define FCODER_JACK_PUNTER_DRAW

function void
jp_draw_token_colors(Application_Links *app, Text_Layout_ID text_layout_id, Token_Array *array,  Buffer_ID buffer)
{
    ProfileScope(app, "jp_draw_cpp_token_colors");
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    i64 first_index = token_index_from_pos(array, visible_range.first);
    Token_Iterator_Array it = token_iterator_index(0, array, first_index);

    Managed_Scope scope = buffer_get_managed_scope(app, buffer);
    jp_buffer_data_t *buffer_attachment = scope_attachment(app, scope, jp_buffer_attachment, jp_buffer_data_t);
    Language *lang = buffer_attachment->language;
    for (;;)
    {
        Token *token = token_it_read(&it);
        if (token->pos >= visible_range.one_past_last){
            break;
        }

        Scratch_Block scratch(app);
        Range_i64 r = Ii64(token);
        String_Const_u8 lexeme = push_buffer_range(app, scratch, buffer, r);
        ARGB_Color argb = lang->get_token_color(app, *token, lexeme);

        paint_text_color(app, text_layout_id, r, argb);
        if (!token_it_inc_all(&it)) {
            break;
        }
    }
}

function ARGB_Color
jp_get_token_color_from_identifier_info(Application_Links *app, String_Const_u8 lexeme)
{
    ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_text_default));;
    Highlight_Data lookup_data;
    if (jp_custom_highlight_lookup(app, lexeme, &lookup_data)) {
        switch (lookup_data.type) {
            case HighlightType_Function: {
                color = fcolor_resolve(fcolor_id(defcolor_function));
            } break;
            case HighlightType_Keyword: {
                color = fcolor_resolve(fcolor_id(defcolor_keyword));
            } break;
            case HighlightType_Macro: {
                color = fcolor_resolve(fcolor_id(defcolor_macro));
            } break;
            case HighlightType_Type: {
                color = fcolor_resolve(fcolor_id(defcolor_type));
            } break;
        }
    }
    return color;
}

// TODO(jack): IMPORTANT, redo all uses of HighlightData.name_range.start

function void
jp_draw_macro_definition(Application_Links *app, Text_Layout_ID text_layout_id,
                         Face_ID face_id, View_ID vid, Buffer_ID buffer,
                         Highlight_Data highlight_data, Rect_f32 popup_anchor)
{
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    Face_Metrics metrics = get_face_metrics(app, face_id);

    Rect_f32 buffer_region = view_get_buffer_region(app, vid);

    i64 def_line = get_line_number_from_pos(app, highlight_data.def_buffer,
                                            highlight_data.name_range.start);

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
            jp_draw_token_colors(app, def_text_layout_id,
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

ARGB_Color set_color_alpha(ARGB_Color col, u8 alpha)
{
    ARGB_Color Result = col;
    Result &= 0x00FFFFFF;
    Result |= alpha << 24;
    return Result;
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
        if (!(highlight_data.param_range == Range_i64{0, 0})) {
            String_Const_u8 params = push_buffer_range(app, scratch, highlight_data.def_buffer,
                                                      highlight_data.param_range);
            i64 start = string_find_first(params, '(') + 1;
            //i64 end = string_find_last(params, ')');
            String_Const_u8 param_no_paren = string_substring(params, Ii64_size(start, params.size-1));
            function_parameter_list = string_split(scratch, param_no_paren, (u8*)",", 1);
        } else 
        {
        it = token_iterator_pos(0, &def_buffer_tokens, highlight_data.name_range.start);
        // NOTE(jack): loop from the definition token forwards
        {
            b32 found_close_paren = false;
            i32 paren_opens = 0;
            i64 transient_single_param_start = {0};
            // TODO(jack): This can probably be done cleaner
            // using the Code_Index_Nests
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
                            push_buffer_range(app, scratch, highlight_data.def_buffer,
                                              {transient_single_param_start, token->pos});
                        String_Const_u8 condensed_param_string = string_condense_whitespace(scratch, param_string);
                        string_list_push(scratch, &function_parameter_list, condensed_param_string);

                        found_close_paren = true;
                    }
                } else if(token->sub_kind == TokenCppKind_Comma) {
                    String_Const_u8 param_string =
                        push_buffer_range(app, scratch, highlight_data.def_buffer,
                                          {transient_single_param_start, token->pos});
                    String_Const_u8 condensed_param_string = string_condense_whitespace(scratch, param_string);
                    string_list_push(scratch, &function_parameter_list, condensed_param_string);

                    transient_single_param_start = token->pos + 1;
                }
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
    ProfileScope(app, "JP Draw definition helpers");
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

                    found_peek_target = is_macro_peek_target(lookup_data.type, open_count, close_count) ||
                        is_function_peek_target(lookup_data.type, open_count, close_count);
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
                    } else if (peek_target->kind == TokenBaseKind_StatementClose) {
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
            !(lookup_data.def_buffer == buffer && lookup_data.name_range.start == peek_target->pos))
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

struct Scope_Helper_Node {
    Scope_Helper_Node *next;
    
    Range_i64 scope_header_range;
    Range_i64 scope_line_range;
    Range_i64 enclosure;
};

struct Scope_Helper_List {
    Scope_Helper_Node *first;
    Scope_Helper_Node *last;
    i32 node_count;
};

function void
scope_helper_list_push(Arena *arena, Scope_Helper_List *list, Range_i64 line_range, Range_i64 header_range,
                       Range_i64 enclosure)
{
    Scope_Helper_Node *node  = push_array(arena, Scope_Helper_Node, 1);
    node->scope_header_range = header_range;
    node->scope_line_range   = line_range;
    node->enclosure          = enclosure;
    sll_queue_push(list->first, list->last, node);
    ++list->node_count;
}

global i64 GlobalMinimumLinesForScopeHelper = 15;

function void
fill_nest_helper_list(Application_Links *app, Arena * arena, Scope_Helper_List *helper_list,
                      Buffer_ID buffer, i64 cursor_pos, Rect_f32 buffer_region)
{
    // TODO(jack): Use find_surrounding_nest to allow multi line helpers?
    Range_i64_Array enclosures = get_enclosure_ranges(app, arena, buffer, cursor_pos,
                                                      RangeHighlightKind_CharacterHighlight);
                                                      //FindNest_Paren);
    // Iterate backwards so that the list is created from top level down.
    for(int i = enclosures.count - 1; i >= 0; --i)
    {
        Range_i64 line_range = get_line_range_from_pos_range(app, buffer,
                                                             enclosures.ranges[i]);
        Range_i64 scope_header_range = get_line_pos_range(app, buffer, line_range.start);
        
        u8 header_buffer[256] = { 0 };
        buffer_read_range(app, buffer, scope_header_range, header_buffer);
        String_Const_u8 header_string = {header_buffer, (u64)(scope_header_range.end - scope_header_range.start)};
        int non_whitespace_count = 0;
        for(u32 idx = 0; idx < header_string.size; ++idx){
            if(!character_is_whitespace(header_string.str[idx])) {
                ++non_whitespace_count;
            }
        }
        //if(range_size(scope_header_range) == 1) {
        if (non_whitespace_count == 1) {
            // NOTE(jack): go to the previous line if scope open is on its own line
            scope_header_range = get_line_pos_range(app, buffer, line_range.start - 1);
        }
        // NOTE(jack): We will add all helpers, and let the drawing code decide if it should draw or not.
        // This way we dont have to worry about pushing blank helpers to keep indentation correct on
        // vertical helpers
        scope_helper_list_push(arena, helper_list, line_range, scope_header_range, enclosures.ranges[i]);
    }
}

function Range_i64
range_remove_whitespace_ends(Token_Array *token_array, Range_i64 range)
{
    Range_i64 Result = range;
    Token_Iterator_Array it = token_iterator_pos(0, token_array, range.start);
    Token *token = token_it_read(&it);
    if(token->kind == TokenBaseKind_Whitespace) {
        if (!token_it_inc_non_whitespace(&it)) {
            return Result;
        }
        token = token_it_read(&it);
        Result.start = token->pos;
    }
    it = token_iterator_pos(0, token_array, range.end);
    token = token_it_read(&it);
    if(token->kind == TokenBaseKind_Whitespace) {
        if (!token_it_dec_non_whitespace(&it)) {
            // NOTE(jack): As we can't break out of the loop if this fails,
            // (which i dont think it can given how we get here),
            // we will return the result as is (with end == range.end)
            return Result;
        } else {
            token = token_it_read(&it);
            Result.end = token->pos + token->size;
        }
    }
    return Result;
}

function void
jp_draw_scope_helpers(Application_Links *app, Text_Layout_ID text_layout_id,
                      Face_ID face_id, View_ID vid, Buffer_ID buffer)
{
    ProfileScope(app, "jp Draw vertical scope helpers");
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    Face_Metrics metrics = get_face_metrics(app, face_id);
    i64 cursor_pos = view_get_cursor_pos(app, vid);
    if(token_array.tokens != 0)
    {
        Scratch_Block scratch(app);
        Rect_f32 buffer_region = view_get_buffer_region(app, vid);
        
        Scope_Helper_List scope_helpers = {};
        
        fill_nest_helper_list(app, scratch, &scope_helpers, buffer, cursor_pos, buffer_region);
        // NOTE(jack): Draw the scope helpers
        i32 node_index = 0;
        for (Scope_Helper_Node *node = scope_helpers.first;
             node != 0; node = node->next, ++node_index)
        {
            i64 range_start_line_pos = get_line_start_pos(app, buffer, node->scope_line_range.start);
            i64 range_end_line_pos = get_line_start_pos(app, buffer, node->scope_line_range.end);
            
            Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
            b32 range_start_line_visible = range_contains(visible_range, range_start_line_pos);
            b32 range_end_line_visible = range_contains(visible_range, range_end_line_pos);
            if (!(node->scope_header_range.start == 0 && node->scope_header_range.start == 0) &&
                 (node->scope_line_range.start != node->scope_line_range.end))
            {   
                node->scope_header_range = range_remove_whitespace_ends(&token_array, node->scope_header_range);
                Face_Metrics small_metrics = get_face_metrics(app, GlobalSmallCodeFaceID);
                i32 indent_amount = global_config.virtual_whitespace_regular_indent;
                Range_f32 buffer_y_range = rect_range_y(buffer_region);

                Range_f32 y_range = {};
                // NOTE(jack): text_layout_line_on_screen returns the y range (in pixels) of a given
                // line in the text layout 
                y_range.start = text_layout_line_on_screen(app, text_layout_id, node->scope_line_range.start).end;
                y_range.end = text_layout_line_on_screen(app, text_layout_id, node->scope_line_range.end).start;


                //NOte
                b32 UseVerticalScopeHelpers = !(range_start_line_visible || range_end_line_visible);
                b32 UseTrailingScopeHelpers = !range_start_line_visible && range_end_line_visible;
                if (UseVerticalScopeHelpers)
                {
                    if (node->scope_line_range.end - node->scope_line_range.start >= GlobalMinimumLinesForScopeHelper)
                    {
                        // NOTE(jack): The amount of x added per scope helper
                        f32 xOffset_per_index = (metrics.normal_advance * indent_amount);

                        // NOTE(jack): the constant xOffset for all scope helpers
                        f32 xOffset = buffer_region.x0 + ((xOffset_per_index - metrics.line_height) / 2.f);                        
                        Rect_f32 text_clip = buffer_region;
                        text_clip.y0 = y_range.start;
                        text_clip.y1 = y_range.end;
                        Vec2_f32 draw_string_start = {
                            xOffset + node_index * xOffset_per_index,
                            y_range.start,
                        };
                        String_Const_u8 helper_string = push_buffer_range(app, scratch, buffer, node->scope_header_range);
                        helper_string = string_condense_whitespace(scratch, helper_string);
                        
                        // NOTE(jack) advance the start as the text is rendered upwards
                        f32 string_length = helper_string.size * small_metrics.normal_advance;
                        
                        if (string_length < (y_range.end - y_range.start)) {
                            // Start y is the midpoint of the y_range + half the string length
                            draw_string_start.y = (y_range.end + y_range.start + string_length) / 2.0f;
                        } else {
                            draw_string_start.y = Min(draw_string_start.y + string_length,
                                                      text_clip.y1);
                        }
                        ARGB_Color helper_color = finalize_color(defcolor_comment, 0);
                        helper_color = set_color_alpha(helper_color, 0x80);
                        
                        Rect_f32 old_clip = draw_set_clip(app, text_clip);
                        draw_string_oriented(app, GlobalSmallCodeFaceID, helper_color, helper_string,
                                             draw_string_start, 0, V2f32(0.f, -1.f));
                        draw_set_clip(app, old_clip);
                    }
                }
                if (UseTrailingScopeHelpers)
                {
                    Range_i64 scope_close_line_range = get_line_pos_range(app, buffer, node->scope_line_range.end);
                    scope_close_line_range = range_remove_whitespace_ends(&token_array, scope_close_line_range);
                    Token *token = get_token_from_pos(app, buffer, scope_close_line_range.end - 1);
                    if (token->kind == TokenBaseKind_ScopeClose || token->kind == TokenBaseKind_StatementClose)
                    {
                        String_Const_u8 helper_string = push_buffer_range(app, scratch, buffer,
                                                                            node->scope_header_range);
                        helper_string = string_condense_whitespace(scratch, helper_string);
                        Vec2_f32 draw_pos = text_layout_character_on_screen(app, text_layout_id,
                                                                                scope_close_line_range.end).p0;
                        draw_pos.x += metrics.normal_advance;
                        draw_pos.y += (metrics.line_height - small_metrics.line_height) / 2.0f;
                        ARGB_Color helper_color = fcolor_resolve(fcolor_id(defcolor_comment));
                        Vec2_f32 next = draw_string(app, GlobalSmallCodeFaceID, string_u8_litexpr(" <- "), draw_pos,
                                                    helper_color);
                        helper_color = set_color_alpha(helper_color, 0x80);
                        draw_string(app, GlobalSmallCodeFaceID, helper_string, next, helper_color);
                    }
                }
                
                Rect_f32 scope_line;
                scope_line.p0 = { buffer_region.x0 + indent_amount * metrics.normal_advance * node_index, y_range.start };
                scope_line.p1 = { buffer_region.x0 + indent_amount * metrics.normal_advance * node_index, y_range.end };
                scope_line.x0 += metrics.normal_advance / 2.0f;
                scope_line.x1 = scope_line.x0 + 1;
                ARGB_Color scope_line_color = set_color_alpha(finalize_color(defcolor_comment, 0), 0x60);
                draw_rectangle(app, scope_line, 0, scope_line_color);
                
            }
            
            ARGB_Color comment_color = finalize_color(defcolor_comment, 0);
            if (range_start_line_visible) {
                Rect_f32 open_pos = text_layout_character_on_screen(app, text_layout_id, node->enclosure.start);
                draw_string(app, face_id, string_u8_litexpr("{"), open_pos.p0, comment_color);
            }
            
            if (range_end_line_visible) {
                Rect_f32 end_pos = text_layout_character_on_screen(app, text_layout_id, node->enclosure.end);
                end_pos.x0 -= metrics.normal_advance;
                draw_string(app, face_id, string_u8_litexpr("}"), end_pos.p0, comment_color);
            }

        }
    }
}


//~ NOTE(rjf): Divider Comments

static void
F4_RenderDividerComments(Application_Links *app, Buffer_ID buffer, View_ID view,
                         Text_Layout_ID text_layout_id)
{
    ProfileScope(app, "[Fleury] Divider Comments");
    
    String_Const_u8 divider_comment_signifier = string_u8_litexpr("//~");
    
    Token_Array token_array = get_token_array_from_buffer(app, buffer);
    Range_i64 visible_range = text_layout_get_visible_range(app, text_layout_id);
    Scratch_Block scratch(app);
    
    if(token_array.tokens != 0)
    {
        i64 first_index = token_index_from_pos(&token_array, visible_range.first);
        Token_Iterator_Array it = token_iterator_index(0, &token_array, first_index);
        
        Token *token = 0;
        for(;;)
        {
            token = token_it_read(&it);
            
            if(token->pos >= visible_range.one_past_last || !token || !token_it_inc_non_whitespace(&it))
            {
                break;
            }
            
            if(token->kind == TokenBaseKind_Comment)
            {
                Rect_f32 comment_first_char_rect =
                    text_layout_character_on_screen(app, text_layout_id, token->pos);
                
                Range_i64 token_range =
                {
                    token->pos,
                    token->pos + (token->size > (i64)divider_comment_signifier.size
                                  ? (i64)divider_comment_signifier.size
                                  : token->size),
                };
                
                u8 token_buffer[256] = {0};
                buffer_read_range(app, buffer, token_range, token_buffer);
                String_Const_u8 token_string = { token_buffer, (u64)(token_range.end - token_range.start) };
                
                if(string_match(token_string, divider_comment_signifier))
                {
                    // NOTE(rjf): Render divider line.
                    Rect_f32 rect =
                    {
                        comment_first_char_rect.x0,
                        comment_first_char_rect.y0-2,
                        10000,
                        comment_first_char_rect.y0,
                    };
                    f32 roundness = 4.f;
                    draw_rectangle(app, rect, roundness, fcolor_resolve(fcolor_id(defcolor_comment)));
                }
                
            }
            
        }
        
    }
}

#include "language/odin/odin_draw.cpp"
#include "language/cpp/cpp_draw.cpp"


#endif // FCODER_JACK_PUNTER_DRAW