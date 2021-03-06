#if !defined(FCODER_JACK_PUNTER_ODIN_PARSE)

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

//
// NOTE(jack): Parser Helpers
// TODO(jack): Move these to JackPunterParsing.cpp so they can be used
//             in other [lang]_parse.cpp files
//

function b32
token_pair_inc_non_whitespace(Token_Iterator_Array *it, Token **prev_token,
                              Token **token)
{
    *prev_token = *token;
    if (!token_it_inc_non_whitespace(it)) {
        return false;
    }
    *token = token_it_read(it);
    return true;
}

function b32
token_inc_non_whitespace(Token_Iterator_Array *it, Token **token)
{
    if (!token_it_inc_non_whitespace(it)) {
        return false;
    }
    *token = token_it_read(it);
    return true;
}

function b32
token_dec_non_whitespace(Token_Iterator_Array *it, Token **token)
{
    if (!token_it_dec_non_whitespace(it)) {
        return false;
    }
    *token = token_it_read(it);
    return true;
}

function b32
token_inc_to_kind(Token_Iterator_Array *it, Token **token, Token_Base_Kind kind)
{
    do {
        if (!token_inc_non_whitespace(it, token)) {
            return false;
        }
    } while ((*token)->kind != kind);
    return true;
}

function b32
token_inc_to_first_of_kinds(Token_Iterator_Array *it, Token **token,
                                Token_Base_Kind kind1, Token_Base_Kind kind2)
{
    do {
        if (!token_inc_non_whitespace(it, token)) {
            return false;
        }
    } while (((*token)->kind != kind1 && (*token)->kind != kind2));
    return true;
}

function b32
token_inc_to_sub_kind(Token_Iterator_Array *it, Token **token, Token_Odin_Kind kind)
{
    do {
        if (!token_inc_non_whitespace(it, token)) {
            return false;
        }
    } while ((*token)->sub_kind != kind);
    return true;
}

//
//
//

function void
jp_parse_odin_content(Application_Links *app, Arena *scratch,
                      List_Highlight_String_Data *list, Buffer_ID buffer_id)
{
    ProfileScope(app, "JP Parse Odin Content");
    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        Token *prev_token = token_it_read(&it);
        if (!token_it_inc_non_whitespace(&it)) {
            DEBUG_MSG_LIT("File was empty");
            return;
        }

        for (;;)
        {
            Token *token = token_it_read(&it);
            if (prev_token->kind == TokenBaseKind_Identifier)
            {
                if (token->sub_kind == TokenOdinKind_ColonColon) {
                    if(!token_inc_non_whitespace(&it, &token)) {
                        i64 line = get_line_number_from_pos(app, buffer_id, prev_token->pos);
                        String_Const_u8 file = push_buffer_file_name(app, scratch, buffer_id);
                        String_Const_u8 text = push_u8_stringf(scratch, "'::' was the last token on line [%s:%d]\n",
                                                               file.str, line);
                        DEBUG_MSG_STR(text);
                        break;
                    }
                    
                    Range_i64 identifier_range = Ii64_size(prev_token->pos, prev_token->size);
                    String_Const_u8 identifier_name = push_buffer_range(app, scratch, buffer_id, identifier_range);
                    
                    if (token->sub_kind == TokenOdinKind_proc || token->sub_kind == TokenOdinKind_inline ||
                        token->sub_kind == TokenOdinKind_no_inline)
                    {
                        Range_i64 param_range = { 0 };
                        if (!token_inc_to_first_of_kinds(&it, &token, TokenBaseKind_ParentheticalOpen,
                                                         TokenBaseKind_ScopeOpen))
                        {
                            i64 line = get_line_number_from_pos(app, buffer_id, prev_token->pos);
                            String_Const_u8 file = push_buffer_file_name(app, scratch, buffer_id);
                            String_Const_u8 text = push_u8_stringf(scratch, "'::' was the last token on line [%s:%d]\n",
                                                                file.str, line);
                            DEBUG_MSG_STR(text);
                            break;
                        }

                        // NOTE(jack): It is not an overload set.
                        if (token->kind == TokenBaseKind_ParentheticalOpen) {
                            //param_range.start = token->pos;
                            if(find_surrounding_nest(app, buffer_id, token->pos + token->size,
                                                     FindNest_Paren, &param_range))
                            {
                                // NOTE(jack): We dont want to include the ')' in the range
                                --param_range.end;
                                
                                enum { None = 0, Proc, ProcType } ProcKind = {};
                                i64 line_end_pos = get_line_end_pos_from_pos(app, buffer_id, param_range.end);
                                Token_Iterator_Array tmp_it = token_iterator_pos(0, &token_array, line_end_pos);
                                Token *tmp_Token = token_it_read(&tmp_it);
                                if (tmp_Token->kind == TokenBaseKind_Whitespace) {
                                    AssertAlways(token_dec_non_whitespace(&tmp_it, &tmp_Token));
                                }
                                if (tmp_Token->sub_kind == TokenOdinKind_Semicolon) {
                                    AssertAlways(token_dec_non_whitespace(&tmp_it, &tmp_Token));
                                }
                                if (tmp_Token->kind == TokenBaseKind_ScopeOpen || 
                                    tmp_Token->kind == TokenBaseKind_ScopeClose || //one-liner functions
                                    tmp_Token->sub_kind == TokenOdinKind_Minus)
                                {
                                    ProcKind = Proc;
                                } else {
                                    if(!token_it_inc_non_whitespace(&tmp_it)) {
                                        ProcKind = ProcType;
                                    } else {
                                        tmp_Token = token_it_read(&tmp_it);
                                        if(tmp_Token->kind == TokenBaseKind_ScopeOpen) {
                                            ProcKind = Proc;
                                        } else {
                                            ProcKind = ProcType;
                                        }
                                    }
                                }

                                if (ProcKind == Proc) {
                                    Highlight_String_Data highlight_data = { 0 };
                                    highlight_data.string = identifier_name;
                                    highlight_data.data.type = HighlightType_Function;
                                    highlight_data.data.def_buffer = buffer_id;
                                    highlight_data.data.name_range = identifier_range;
                                    highlight_data.data.param_range = param_range;
                                    highlight_string_list_push(scratch, list, highlight_data);
                                } else if (ProcKind == ProcType) {
                                    // TODO(jack): Do we want to add a flag and the parameter info to this highlight data,
                                    // so that, we can mark any indentifier defined as this type can be marked as a function?
                                    highlight_string_list_push(scratch, list, identifier_name, 
                                                            HighlightType_Type, buffer_id, identifier_range);
                                }
                                it = token_iterator_pos(0, &token_array, line_end_pos);
                            }
                        } else { 
                            // NOTE(jack): Overload set
                            highlight_string_list_push(scratch, list, identifier_name,
                                                        HighlightType_Function, buffer_id, identifier_range);
                        }
                    } else if (token->sub_kind == TokenOdinKind_enum || token->sub_kind == TokenOdinKind_struct ||
                               token->sub_kind == TokenOdinKind_union || token->sub_kind == TokenOdinKind_distinct ||
                               token->sub_kind == TokenOdinKind_type || odin_is_builtin_type(token))
                    {
                        highlight_string_list_push(scratch, list, identifier_name, HighlightType_Type,
                                                   buffer_id, identifier_range);
                    } else {
                        Highlight_Data data;
                        // Dont use the general lookup as it wont include definitions from this file.
                        String_Const_u8 string = push_token_lexeme(app, scratch, buffer_id, token);
                        if(jp_get_existing_identifier_data(app, string, list, buffer_id, &data)) {
                            highlight_string_list_push(scratch, list, identifier_name, data.type,
                                                       buffer_id, identifier_range);
                        }
                    }

                } else if (token->sub_kind == TokenOdinKind_Colon) {
                    if(!token_inc_non_whitespace(&it, &token)) {
                        DEBUG_MSG_LIT("':' was last identifier in file");
                    }
                }
            }

            if (!token_pair_inc_non_whitespace(&it, &prev_token, &token)) {
                break;
            }
        }
    }

    // TODO(jack): This may be temporary? will be replaced with an ODIN specific parser.
    // I know that i'm kind of replacing the code_index stuff with the table that the list
    // fills but it allows me to use extra information so I'll likely stick with this approach,
    // unless I need to implement Code_Index_Nests for a language
    Used_Note_Kinds kinds = {};
    kinds.CodeIndex_Function = false;
    kinds.CodeIndex_Type = false;
    kinds.CodeIndex_Macro = true;
    jp_parse_using_code_index(app, scratch, list, buffer_id, kinds);
}

#define FCODER_JACK_PUNTER_ODIN_PARSE
#endif 