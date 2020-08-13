#if !defined(FCODER_JACK_PUNTER_PARSING)
#define FCODER_JACK_PUNTER_PARSING

function void
jp_parse_keywods_types(Application_Links *app, Buffer_ID buffer_id)
{
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);

    buffer_data->custom_keywords_end = 0;
    buffer_data->custom_types_end = 0;

    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            // NOTE(jack): Temporary scratchpad memory arena that gets free'd at scope close
            Scratch_Block scratch(app);
            Token *token = token_it_read(&it);

            String_Const_u8 token_string = push_buffer_range(app, scratch, buffer_id,
                                                             {token->pos, token->pos + token->size});
            
            // NOTE(jack): #define [identifier] [keywords]
            if (token->kind == TokenBaseKind_Preprocessor && token->sub_kind == TokenCppKind_PPDefine)
            {
                if (!token_it_inc_non_whitespace(&it)) {
                    DEBUG_MSG_LIT("#define was the last token\n");
                    break;
                }
                Token *di_token = token_it_read(&it);
                String_Const_u8 di_string = push_buffer_range(app, scratch, buffer_id,
                                                              {di_token->pos, di_token->pos + di_token->size});

                if (!token_it_inc_non_whitespace(&it)) {
                    DEBUG_MSG_LIT("#defined identifier was the last token\n");
                    break;
                }
                Token *dt_token = token_it_read(&it);
                String_Const_u8 dt_string = push_buffer_range(app, scratch, buffer_id,
                                                              {dt_token->pos, dt_token->pos + dt_token->size});
                
                String_Const_u8 di_dt_separator = push_buffer_range(app, scratch, buffer_id, 
                                                                    {di_token->pos + di_token->size, dt_token->pos});
               
                // If there is not a newline between di and dt
                if(string_find_first(di_dt_separator, '\n') == di_dt_separator.size) {
                    // NOTE(jack): If the defined to (dt) token is a keyword and the defined identifier
                    // is not already a custom keyword
                    if((dt_token->kind == TokenBaseKind_Keyword && jp_is_type_token(dt_token)) ||
                       jp_is_custom_type(app, dt_string))
                    {
                        if (jp_push_custom_type(app, buffer_id, di_string)) {
                            DEBUG_MSG_LIT("New #define type Added: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
                        } else {
                            log_string(app, string_u8_litexpr("Array Full. Couldn't add new #define type: "));
                            log_string(app, di_string);
                            log_string(app, string_u8_litexpr("\n"));
                        }
                    }
                    // NOTE(jack): If the dt_token (keyword) is not a type or it is a custom keyword
                    else if ((dt_token->kind == TokenBaseKind_Keyword && !jp_is_type_token(dt_token)) ||
                             jp_is_custom_keyword(app, dt_string))
                    {
                        if (jp_push_custom_keyword(app, buffer_id, di_string)) {
                            DEBUG_MSG_LIT("New #define keyword Added: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
                        } else {
                            log_string(app, string_u8_litexpr("Array Full. Couldn't add new #define keyword: "));
                            log_string(app, di_string);
                            log_string(app, string_u8_litexpr("\n"));
                        }
                    }
                }

                // NOTE(jack): Decrement just to prevent possibly missing a define
                // occurs after a symbol definition, i.e:
                // #define header_guard
                // #define global static
                b32 result = token_it_dec_non_whitespace(&it);
                // This should never fail as we are mid file.
                Assert(result);
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
                    

                    // NOTE(jack): typedef enum { TILE_NONE, TILE_WALL, ... }
                    if (second_token->sub_kind == TokenCppKind_Enum) {
                        b32 failed = false;
                        // NOTE(jack): Skip until the end of the enum declaration
                        do {
                            if (!token_it_inc_non_whitespace(&it)) {
                                DEBUG_MSG_LIT("typedef's existing type was the last token\n");
                                failed = true;
                            }
                            second_token = token_it_read(&it);
                        } while(!failed && second_token->kind != TokenBaseKind_ScopeClose);
                    }

                    // NOTE(jack): typedef struct { int x, int y } myStruct;
                    // TODO(jack): typedef union { myStruct s; int a[2]; } myUnion;
                    //             typedef enum { RED, GREEN, ...} myEnum;
                    if (second_token->sub_kind == TokenCppKind_Struct || second_token->sub_kind == TokenCppKind_Union)
                    {
                        if (!token_it_inc_non_whitespace(&it)) {
                            DEBUG_MSG_LIT("typedef's existing type was the last token\n");
                            break;
                        }
                        second_token = token_it_read(&it);

                        // NOTE(jack): If the token after 'struct' is an identifier it is a new typename
                        if (second_token->kind == TokenBaseKind_Identifier) {
                            String_Const_u8 inner_struct_type = push_buffer_range(app, scratch, buffer_id,
                                                                                  {second_token->pos, second_token->pos + second_token->size});
                            if (jp_push_custom_type(app, buffer_id, inner_struct_type)) {
                                DEBUG_MSG_LIT("New typedef struct type added: ");
                                DEBUG_MSG_STR(inner_struct_type);
                                DEBUG_MSG_LIT("\n");
                            } else { 
                                log_string(app, string_u8_litexpr("Array Full. Couldn't add new struct typedef type: "));
                                log_string(app, inner_struct_type);
                                log_string(app, string_u8_litexpr("\n"));
                            }
                        } else if (second_token->kind == TokenBaseKind_ScopeOpen) {
                            b32 result = token_it_dec_non_whitespace(&it);
                            // NOTE(jack): we have iterated forward to this point so shouldn't fail.
                            Assert(result);
                        }

                        b32 failed = false;
                        int open_counter = 0;
                        // NOTE(jack): Skip until the end of the struct declaration
                        {
                            Scratch_Block do_memory(app);
                            do {
                                if (!token_it_inc_non_whitespace(&it)) {
                                    DEBUG_MSG_LIT("typedef's existing type was the last token\n");
                                    failed = true;
                                }
                                second_token = token_it_read(&it);
                                String_Const_u8 current_token = push_buffer_range(app, do_memory, buffer_id,
                                                                                  {second_token->pos, second_token->pos + second_token->size});

                                if (second_token->kind == TokenBaseKind_ScopeOpen) {
                                    ++open_counter;
                                } else if (second_token->kind == TokenBaseKind_ScopeClose) {
                                    --open_counter;
                                }
                            } while(!failed && !(second_token->kind == TokenBaseKind_ScopeClose && open_counter == 0));
                        }
                    }
     
                    Token *di_token;
                    b32 failed = false;
                    // NOTE(jack): skip parenthesis operators, and keywords.
                    // Allows 'typedef int (*Sum)(int a, int b)' and
                    // typedef unsigned long int LUINT
                    do {
                        if (!token_it_inc_non_whitespace(&it)) {
                            DEBUG_MSG_LIT("typedef's existing type was the last token\n");
                            failed = true;
                        }
                        di_token = token_it_read(&it);
                    } while (!failed && di_token->kind != TokenBaseKind_Identifier);
                    
                    String_Const_u8 di_string = push_buffer_range(app, scratch, buffer_id,
                                                                  {di_token->pos, di_token->pos + di_token->size});

                    if (di_token->kind == TokenBaseKind_Identifier) {
                        if (jp_push_custom_type(app, buffer_id, di_string)) {
                            DEBUG_MSG_LIT("New typedef type Added: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
                        } else {
                            log_string(app, string_u8_litexpr("Array Full. Couldn't add new typedef type: "));
                            log_string(app, di_string);
                            log_string(app, string_u8_litexpr("\n"));
                        }
                    }

                // NOTE(jack): struct MyStruct {int x, int y};
                //             union MyUnion { struct {int x, int y}; int a[2]; };
                // TODO(jack): may fail on: enum class MyEnum
                } else if (token->sub_kind == TokenCppKind_Struct || token->sub_kind == TokenCppKind_Union) {
                    if (!token_it_inc_non_whitespace(&it)) {
                        DEBUG_MSG_LIT("typedef was the last token\n");
                        break;
                    }
                    Token *next_token = token_it_read(&it);
                    if(next_token->kind == TokenBaseKind_Identifier) {
                        String_Const_u8 struct_name = push_buffer_range(app, scratch, buffer_id,
                                                                    {next_token->pos, next_token->pos + next_token->size});
                        if (jp_push_custom_type(app, buffer_id, struct_name)) {
                            DEBUG_MSG_LIT("New struct keyword Added: ");
                            DEBUG_MSG_STR(struct_name);
                            DEBUG_MSG_LIT("\n");
                        } else { 
                            log_string(app, string_u8_litexpr("Array Full. Couldn't add new struct type: "));
                            log_string(app, struct_name);
                            log_string(app, string_u8_litexpr("\n"));
                        }
                    }
                }
            }

            // We dont want to check whitespace
            if (!token_it_inc_non_whitespace(&it)) {
                break;
            }
        }
    }
}

#endif /* FCODER_JACK_PUNTER_PARSING */