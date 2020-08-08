#if !defined(FCODER_JACK_PUNTER_DRAW)
#define FCODER_JACK_PUNTER_DRAW

function ARGB_Color
jp_get_token_color_cpp(Token token, String_Const_u8 lexeme)
{
    ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_text_default));
    switch (token.kind){
        case TokenBaseKind_Preprocessor:
        {
            color = fcolor_resolve(fcolor_id(defcolor_preproc));
        }break;
        case TokenBaseKind_Keyword:
        {
            color = fcolor_resolve(fcolor_id(defcolor_keyword));
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
            if(jp_is_custom_keyword(lexeme)) {
                color = fcolor_resolve(fcolor_id(defcolor_keyword));
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

        String_Const_u8 lexeme = push_buffer_range(app, scratch, buffer,
                                                   {token->pos, token->pos + token->size});
        ARGB_Color argb = jp_get_token_color_cpp(*token, lexeme);

        paint_text_color(app, text_layout_id, Ii64_size(token->pos, token->size), argb);
        if (!token_it_inc_all(&it)) {
            break;
        }
    }
}

#endif // FCODER_JACK_PUNTER_DRAW