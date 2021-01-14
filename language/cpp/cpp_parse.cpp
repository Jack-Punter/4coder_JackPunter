#if !defined(FCODER_JACK_PUNTER_CPP_PARSE)

function void
jp_parse_cpp_keywords_types(Application_Links *app, Arena *scratch,
                            List_Highlight_String_Data *list, Buffer_ID buffer_id)
{
    ProfileScope(app, "JP Parse Cpp Keywords Types");

    // NOTE(jack): "Empty" the current keyword/type arrays for this buffer.
    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            Token *token = token_it_read(&it);

            String_Const_u8 token_string = push_token_lexeme(app, scratch, buffer_id, token);
            
            if (token->kind == TokenBaseKind_Preprocessor && token->sub_kind == TokenCppKind_PPDefine)
            {
                // NOTE(jack): #define [identifier] [keyword/type]
                if (!token_it_inc_non_whitespace(&it)) {
                    DEBUG_MSG_LIT("#define was the last token\n");
                    break;
                }
                Token *di_token = token_it_read(&it);
                String_Const_u8 di_string = push_token_lexeme(app, scratch, buffer_id, di_token);

                if (!token_it_inc_non_whitespace(&it)) {
                    DEBUG_MSG_LIT("#defined identifier was the last token\n");
                    break;
                }
                Token *dt_token = token_it_read(&it);
                String_Const_u8 dt_string = push_token_lexeme(app, scratch, buffer_id, dt_token);
                
                String_Const_u8 di_dt_separator = push_buffer_range(app, scratch, buffer_id, 
                                                                    {di_token->pos + di_token->size , dt_token->pos});
               
                // If there is not a newline between di and dt
                if(string_find_first(di_dt_separator, '\n') == di_dt_separator.size) {
                    // NOTE(jack): If the defined to (dt) token is a keyword and is a type
                    if((dt_token->kind == TokenBaseKind_Keyword && jp_is_type_token(dt_token)))
                    {
                        highlight_string_list_push(scratch, list, di_string, HighlightType_Type,
                                                   buffer_id, Ii64_size(di_token->pos, di_token->size));
                    }
                    // NOTE(jack): If the dt_token (keyword) is not a type or it is a custom keyword
                    else if (dt_token->kind == TokenBaseKind_Keyword && !jp_is_type_token(dt_token))
                    {
                        highlight_string_list_push(scratch, list, di_string, HighlightType_Keyword,
                                                   buffer_id, Ii64_size(di_token->pos, di_token->size));
                    }
                } else {
                    // NOTE(jack): If there was a new line we need to decrement to not miss the next line.
                    // #define header_guard
                    // #define global static
                    b32 result = token_it_dec_non_whitespace(&it);
                    // This should never fail as we are mid file.
                    Assert(result);
                }
            }

            if (token->kind == TokenBaseKind_Keyword)
            {
                // NOTE(jack): typedef uint8_t u8
                if (token->sub_kind == TokenCppKind_Typedef)
                {
                    if (!token_it_inc_non_whitespace(&it)) {
                        DEBUG_MSG_LIT("typedef was the last token\n");
                        break;
                    }
                    Token *second_token = token_it_read(&it);
                    
                    // NOTE(jack): typedef volatile i32 register32;
                    if (second_token->sub_kind == TokenCppKind_Volatile) {
                        if (!token_it_inc_non_whitespace(&it)) {
                            DEBUG_MSG_LIT("typedef was the last token\n");
                            break;
                        }
                        second_token = token_it_read(&it);
                    }

                    // NOTE(jack): typedef enum { TILE_NONE, TILE_WALL, ... }
                    if (second_token->sub_kind == TokenCppKind_Enum) {
                        if (!jp_iterate_end_of_next_scope(app, &it)) {
                            break;
                        }
                    }

                    if (second_token->sub_kind == TokenCppKind_Struct ||
                        second_token->sub_kind == TokenCppKind_Union)
                    {
                        // NOTE(jack): typedef struct { int x, int y } myStruct;
                        //             typedef union { myStruct s; int a[2]; } myUnion;
                        //             typedef enum { RED, GREEN, ...} myEnum;
                        if (!token_it_inc_non_whitespace(&it)) {
                            DEBUG_MSG_LIT("typedef's existing type was the last token\n");
                            break;
                        }
                        second_token = token_it_read(&it);

                        // NOTE(jack): If the token after 'struct' is an identifier it is a new typename
                        if (second_token->kind == TokenBaseKind_Identifier) {
                            String_Const_u8 inner_struct_type = push_token_lexeme(app, scratch, buffer_id, second_token);
                            highlight_string_list_push(scratch, list, inner_struct_type, HighlightType_Type,
                                                       buffer_id, Ii64_size(second_token->pos, second_token->size));
                        } else if (second_token->kind == TokenBaseKind_ScopeOpen) {
                            b32 result = token_it_dec_non_whitespace(&it);
                            // NOTE(jack): we have iterated forward to this point so shouldn't fail.
                            Assert(result);
                        }
                        if (!jp_iterate_end_of_next_scope(app, &it)) {
                            break;
                        }
                    }
     
                    // NOTE(jack): skip parenthesis operators, and keywords.
                    // Allows 'typedef int (*Sum)(int a, int b)' and
                    // typedef unsigned long int LUINT
                    Token *di_token = jp_get_next_identifier_token(app, &it);
                    if (!di_token) {
                        break;
                    }
                    String_Const_u8 di_string = push_token_lexeme(app, scratch, buffer_id, di_token);
                    highlight_string_list_push(scratch, list, di_string, HighlightType_Type,
                                               buffer_id, Ii64_size(di_token->pos, di_token->size));
                }
                else if (token->sub_kind == TokenCppKind_Struct || token->sub_kind == TokenCppKind_Union ||
                         token->sub_kind == TokenCppKind_Class || token->sub_kind == TokenCppKind_Enum)
                {
                    // NOTE(jack): struct MyStruct {int x, int y};
                    //             union MyUnion { struct {int x, int y}; int a[2]; };
                    if (!token_it_inc_non_whitespace(&it)) {
                        DEBUG_MSG_LIT("union/struct/class was the last token\n");
                        break;
                    }
                    Token *next_token = token_it_read(&it);
                    if(next_token->kind == TokenBaseKind_Identifier) {
                        String_Const_u8 struct_name = push_token_lexeme(app, scratch, buffer_id, next_token);
                        highlight_string_list_push(scratch, list, struct_name, HighlightType_Type,
                                                   buffer_id, Ii64_size(next_token->pos, next_token->size));
                    }
                }
                // TODO(jack): highlight enum class types.
                // I need to double check the syntax for this as I'm not 100% familiar.
            }

            if (!token_it_inc_non_whitespace(&it)) {
                break;
            }
        }
    }
}

function void
jp_parse_cpp_content(Application_Links *app, Arena *scratch,
                     List_Highlight_String_Data *list, Buffer_ID buffer_id)
{
    ProfileScope(app, "JP Parse Cpp Content");
    jp_parse_cpp_keywords_types(app, scratch, list, buffer_id);
    Used_Note_Kinds kinds = { 0 };
    kinds.CodeIndex_Function = true;
    kinds.CodeIndex_Macro = true;

    jp_parse_using_code_index(app, scratch, list, buffer_id, kinds);
}

#define FCODER_JACK_PUNTER_CPP_PARSE
#endif 