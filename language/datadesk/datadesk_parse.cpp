#if !defined(FCODER_JACK_PUNTER_DATADESK_PARSE)

/* DataDesk syntax:
MyEnum :: enum {
    Values, ...
}
Foo :: struct {
    a : int,
    b : float,
    c : *char,
    d : *void,
    e : struct
    {
        a : int,
        b : float,
        c : *char,
    },
    f : Bar,
}
*/
function void
jp_parse_datadesk_content(Application_Links *app, Arena *scratch,
                          List_Highlight_String_Data *list, Buffer_ID buffer_id)
{
    ProfileScope(app, "jp Parse Data Desk Content");

    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    Token_List typeTokens = {};
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            Token *token = token_it_read(&it);
            if (token->kind == TokenBaseKind_Identifier)
            {   
                Token *defined_token = token;
                String_Const_u8 token_string = push_token_lexeme(app, scratch, buffer_id, token);
                // NOTE(jack): Iterate to the next token;
                if(!token_it_inc_non_whitespace(&it)) {
                    break;
                }

                token = token_it_read(&it);
                // NOTE(jack): If the token after the identifier is a '::'
                // It is a definition;
                if (token->kind == TokenBaseKind_Operator &&
                    token->sub_kind == TokenCppKind_ColonColon)
                {
                    // Increment to the enum/struct/union keyword defining the type;
                    if(!token_it_inc_non_whitespace(&it)) {
                        break;
                    }
                    // If the identifier is defined to be a struct/union/enum we can add it
                    // as a custom type;
                    token = token_it_read(&it);
                    if (token->kind == TokenBaseKind_Keyword && 
                        (token->sub_kind == TokenCppKind_Struct || 
                         token->sub_kind == TokenCppKind_Enum ||
                         token->sub_kind == TokenCppKind_Union))
                    {
                        // NOTE(jack): If it was a type definition, add it as a custom type.
                        highlight_string_list_push(scratch, list, token_string,
                                                   HighlightType_Type, buffer_id, defined_token->pos);
                    }
                    else 
                    {
                        // NOTE(jack): otherwise iterate back so we dont miss anything
                        // This may not be necessary in valid datadesk syntax.
                        // But i would rather be safe
                        if (!token_it_dec_non_whitespace(&it)) {
                            break;
                        }
                    }
                }
                else
                {
                    // NOTE(jack): If the next token wasn't an operator '::',
                    // go back so we dont miss anything
                    if (!token_it_dec_non_whitespace(&it)) {
                        break;
                    }
                }
            }

            if (!token_it_inc_non_whitespace(&it)) {
                break;
            }
        }
    }
}
#define FCODER_JACK_PUNTER_DATADESK_PARSE
#endif 