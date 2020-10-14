#if !defined(FCODER_JACK_PUNTER_ODIN_DRAW)

// Common
function b32 odin_is_builtin_type(Token *token)
{
    return TokenOdinKind_byte <= token->sub_kind &&
        token->sub_kind <= TokenOdinKind_u128be;
}

function b32 odin_is_builtin_proc(Token *token)
{
    return TokenOdinKind_len <= token->sub_kind &&
        token->sub_kind <= TokenOdinKind_card;
}

function b32 odin_is_directive(Token *token)
{
    return TokenOdinKind_align <= token->sub_kind &&
        token->sub_kind <= TokenOdinKind_partial;
}

function b32 odin_is_attribute(Token *token)
{
    return TokenOdinKind_builtin <= token->sub_kind &&
        token->sub_kind <= TokenOdinKind_thread_local;
}

// Highlight
static ARGB_Color jp_get_token_color_odin(Application_Links *app, Token token, String_Const_u8 lexeme)
{
    ARGB_Color color = fcolor_resolve(fcolor_id(defcolor_text_default));
    switch (token.kind) {
        case TokenBaseKind_Preprocessor: {
            color = fcolor_resolve(fcolor_id(defcolor_preproc));
        } break;

        case TokenBaseKind_Keyword: {
            if (odin_is_directive(&token) || odin_is_attribute(&token))
                color = fcolor_resolve(fcolor_id(defcolor_preproc));
            else if (odin_is_builtin_type(&token))
                color = fcolor_resolve(fcolor_id(defcolor_type));
            else if (odin_is_builtin_proc(&token))
                color = fcolor_resolve(fcolor_id(defcolor_function));
            else
                color = fcolor_resolve(fcolor_id(defcolor_keyword));
        } break;

        case TokenBaseKind_Comment: {
            color = fcolor_resolve(fcolor_id(defcolor_comment));
        } break;

        case TokenBaseKind_LiteralString: {
            color = fcolor_resolve(fcolor_id(defcolor_str_constant));
        } break;

        case TokenBaseKind_LiteralInteger: {
            color = fcolor_resolve(fcolor_id(defcolor_int_constant));
        } break;

        case TokenBaseKind_LiteralFloat: {
            color = fcolor_resolve(fcolor_id(defcolor_float_constant));
        } break;

        case TokenBaseKind_Identifier: {
            if (odin_is_builtin_type(&token))
                color = fcolor_resolve(fcolor_id(defcolor_type));
            else if (odin_is_builtin_proc(&token))
                color = fcolor_resolve(fcolor_id(defcolor_function));
        } break;
    }
    return(color);
}

#define FCODER_JACK_PUNTER_ODIN_DRAW
#endif