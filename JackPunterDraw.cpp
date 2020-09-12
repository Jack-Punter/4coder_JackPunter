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
            if (jp_is_custom_type(app, lexeme)) {
                color = finalize_color(defcolor_keyword, 1);
            } else if (jp_is_custom_keyword(app, lexeme)) {
                color = finalize_color(defcolor_keyword, 0);
            } else if (jp_is_function(app, lexeme)) {
                color = finalize_color(defcolor_keyword, 2);
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

#endif // FCODER_JACK_PUNTER_DRAW