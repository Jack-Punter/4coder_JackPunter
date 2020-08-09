#if !defined(FCODER_JACK_PUNTER_PARSING)
#define FCODER_JACK_PUNTER_PARSING

function void
jp_parse_keywods_types(Application_Links *app, Buffer_ID buffer_id)
{
    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;) {
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
                       jp_is_custom_type(dt_string))
                    {
                        if (jp_push_custom_type(di_string)) {
                            DEBUG_MSG_LIT("New #define type Added: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
                        } else {
                            DEBUG_MSG_LIT("Array Full. Couldn't add new #define keyword: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
                        }
                    }
                    else if ((dt_token->kind == TokenBaseKind_Keyword && !jp_is_type_token(dt_token)) ||
                             jp_is_custom_keyword(dt_string))
                    {
                        if (jp_push_custom_keyword(di_string)) {
                            DEBUG_MSG_LIT("New #define keyword Added: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
                        } else {
                            DEBUG_MSG_LIT("Array Full. Couldn't add new #define keyword: ");
                            DEBUG_MSG_STR(di_string);
                            DEBUG_MSG_LIT("\n");
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
                if (token->sub_kind == TokenCppKind_Typedef) {
                    if (!token_it_inc_non_whitespace(&it)) {
                        DEBUG_MSG_LIT("typedef was the last token\n");
                        break;
                    }
                    //Token *dt_token = token_it_read(&it);
                    //String_Const_u8 dt_string = push_buffer_range(app, scratch, buffer_id,
                    //                                            {dt_token->pos, dt_token->pos + dt_token->size});
                    // Is this a valid Typedef
                    //if(dt_token->kind == TokenBaseKind_Keyword || 
                    //  (dt_token->kind == TokenBaseKind_Identifier && jp_is_custom_type(dt_string)))
                    //{   
                        Token *di_token;
                        b32 failed = false;
                        // skip parenthesis and operators. allows 'typedef int (*Sum)(int a, int b)'
                        // TODO(jack): Do I want function pointer typedefs to be keyword highlighted?
                        // TODO(jack): typdef struct {int x, int y} MyStruct;
                        do {
                            if (!token_it_inc_non_whitespace(&it)) {
                                DEBUG_MSG_LIT("typedef's existing type was the last token\n");
                                failed = true;
                            }
                            di_token = token_it_read(&it);
                        } while (!failed && di_token->kind != TokenBaseKind_Identifier);

                        if (failed) {
                            break;
                        }

                        String_Const_u8 di_string = push_buffer_range(app, scratch, buffer_id,
                                                                    {di_token->pos, di_token->pos + di_token->size});
                        if (di_token->kind == TokenBaseKind_Identifier) {
                            if (jp_push_custom_type(di_string)) {
                                DEBUG_MSG_LIT("New typedef type Added: ");
                                DEBUG_MSG_STR(di_string);
                                DEBUG_MSG_LIT("\n");
                            } else { 
                                DEBUG_MSG_LIT("Array Full. Couldn't add new typedef type: ");
                                DEBUG_MSG_STR(di_string);
                                DEBUG_MSG_LIT("\n");
                            }
                        }
                    //}
                // NOTE(jack): struct MyStruct {int x, int y};
                } else if (token->sub_kind == TokenCppKind_Struct) {
                    if (!token_it_inc_non_whitespace(&it)) {
                        DEBUG_MSG_LIT("typedef was the last token\n");
                        break;
                    }
                    Token *next_token = token_it_read(&it);
                    if(next_token->kind == TokenBaseKind_Identifier) {
                        String_Const_u8 struct_name = push_buffer_range(app, scratch, buffer_id,
                                                                    {next_token->pos, next_token->pos + next_token->size});
                        if (jp_push_custom_type(struct_name)) {
                            DEBUG_MSG_LIT("New struct keyword Added: ");
                            DEBUG_MSG_STR(struct_name);
                            DEBUG_MSG_LIT("\n");
                        } else { 
                            DEBUG_MSG_LIT("Array Full. Couldn't add new struct keyword: ");
                            DEBUG_MSG_STR(struct_name);
                            DEBUG_MSG_LIT("\n");
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