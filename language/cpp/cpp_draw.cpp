#if !defined(FCODER_JACK_PUNTER_CPP_DRAW)

function ARGB_Color
jp_get_token_color_cpp(Application_Links *app, Token token, String_Const_u8 lexeme)
{
    ProfileScope(app, "JP Get Token Color CPP");
    ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_text_default));
    switch (token.kind) {
        case TokenBaseKind_Preprocessor: {
            color = fcolor_resolve(fcolor_id(defcolor_preproc));
        } break;
        case TokenBaseKind_Keyword: {
            if (jp_is_type_token(&token)) {
                color = finalize_color(defcolor_keyword, 1);
            } else {
                color = finalize_color(defcolor_keyword, 0);
            }
        } break;
        case TokenBaseKind_Comment: {
            color = fcolor_resolve(fcolor_id(defcolor_comment));
        } break;
        case TokenBaseKind_LiteralString: {
            color = fcolor_resolve(fcolor_id(defcolor_str_constant));
        }break;
        case TokenBaseKind_LiteralInteger: {
            color = fcolor_resolve(fcolor_id(defcolor_int_constant));
        } break;
        case TokenBaseKind_LiteralFloat: {
            color = fcolor_resolve(fcolor_id(defcolor_float_constant));
        } break;
        case TokenBaseKind_Identifier: {
            Highlight_Data lookup_data;
            if (jp_custom_highlight_lookup(app, lexeme, &lookup_data)) {
                switch (lookup_data.type) {
                    case HighlightType_Keyword: {
                        color = fcolor_resolve(fcolor_id(defcolor_keyword));
                    } break;
                    case HighlightType_Type: {
                        color = fcolor_resolve(fcolor_id(defcolor_type));
                    } break;
                    case HighlightType_Function: {
                        color = fcolor_resolve(fcolor_id(defcolor_function));
                    } break;
                    case HighlightType_Macro: {
                        color = fcolor_resolve(fcolor_id(defcolor_macro));
                    } break;
                }
            }
        } break;
    }
    // specifics override generals
    switch (token.sub_kind){
        case TokenCppKind_LiteralTrue:
        case TokenCppKind_LiteralFalse: {
            color = fcolor_resolve(fcolor_id(defcolor_bool_constant));
        }break;
        case TokenCppKind_LiteralCharacter:
        case TokenCppKind_LiteralCharacterWide:
        case TokenCppKind_LiteralCharacterUTF8:
        case TokenCppKind_LiteralCharacterUTF16:
        case TokenCppKind_LiteralCharacterUTF32: {
            color = fcolor_resolve(fcolor_id(defcolor_char_constant));
        } break;
        case TokenCppKind_PPIncludeFile: {
            color = fcolor_resolve(fcolor_id(defcolor_include));
        } break;
    }
    
    return(color);
}

#define FCODER_JACK_PUNTER_CPP_DRAW
#endif