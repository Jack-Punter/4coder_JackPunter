#if !defined(FCODER_JACK_PUNTER_PARSING)
#define FCODER_JACK_PUNTER_PARSING

function b32
jp_iterate_end_of_next_scope(Application_Links *app, Token_Iterator_Array *it)
{
    b32 failed = false;
    int open_counter = 0;
    // NOTE(jack): Skip until the end of the struct declaration
    Scratch_Block do_memory(app);
    Token* current_token;
    do
    {
        if (!token_it_inc_non_whitespace(it)) {
            DEBUG_MSG_LIT("typedef's existing type was the last token\n");
            failed = true;
        }
        current_token = token_it_read(it);
        if (current_token->kind == TokenBaseKind_ScopeOpen) {
            ++open_counter;
        }
        else if (current_token->kind == TokenBaseKind_ScopeClose) {
            --open_counter;
        }
    } while (!failed && !(current_token->kind == TokenBaseKind_ScopeClose && open_counter == 0));
    return !failed;
}

function Token*
jp_get_next_identifier_token(Application_Links *app, Token_Iterator_Array *it) 
{
    b32 failed = false;
    Token *token;
    do {
        if (!token_it_inc_non_whitespace(it)) {
            DEBUG_MSG_LIT("typedef's existing type was the last token\n");
            failed = true;
        }
        token = token_it_read(it);
    } while (!failed && token->kind != TokenBaseKind_Identifier);

    if(!failed) {
        return token;
    } else {
        return 0;
    }
}

function void
jp_parse_cpp_keywords_types(Application_Links *app, Buffer_ID buffer_id)
{
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);
    // NOTE(jack): "Empty" the current keyword/type arrays for this buffer.
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

            String_Const_u8 token_string = push_buffer_range(app, scratch, buffer_id, Ii64(token));
            
            if (token->kind == TokenBaseKind_Preprocessor && token->sub_kind == TokenCppKind_PPDefine)
            {
                // NOTE(jack): #define [identifier] [keyword/type]
                if (!token_it_inc_non_whitespace(&it)) {
                    DEBUG_MSG_LIT("#define was the last token\n");
                    break;
                }
                Token *di_token = token_it_read(&it);
                String_Const_u8 di_string = push_buffer_range(app, scratch, buffer_id, Ii64(di_token));

                if (!token_it_inc_non_whitespace(&it)) {
                    DEBUG_MSG_LIT("#defined identifier was the last token\n");
                    break;
                }
                Token *dt_token = token_it_read(&it);
                String_Const_u8 dt_string = push_buffer_range(app, scratch, buffer_id, Ii64(dt_token));
                
                String_Const_u8 di_dt_separator = push_buffer_range(app, scratch, buffer_id, 
                                                                    {di_token->pos + di_token->size , dt_token->pos});
               
                // If there is not a newline between di and dt
                if(string_find_first(di_dt_separator, '\n') == di_dt_separator.size) {
                    // NOTE(jack): If the defined to (dt) token is a keyword and the defined identifier
                    // is not already a custom keyword
                    if((dt_token->kind == TokenBaseKind_Keyword && jp_is_type_token(dt_token)) ||
                       jp_is_custom_type(app, dt_string))
                    {
                        jp_push_custom_type(app, buffer_id, di_string);
                    }
                    // NOTE(jack): If the dt_token (keyword) is not a type or it is a custom keyword
                    else if ((dt_token->kind == TokenBaseKind_Keyword && !jp_is_type_token(dt_token)) ||
                              jp_is_custom_keyword(app, dt_string))
                    {
                        jp_push_custom_keyword(app, buffer_id, di_string);
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
                            String_Const_u8 inner_struct_type = push_buffer_range(app, scratch, buffer_id, Ii64(second_token));
                            jp_push_custom_type(app, buffer_id, inner_struct_type);
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
                    String_Const_u8 di_string = push_buffer_range(app, scratch, buffer_id, Ii64(di_token));
                    jp_push_custom_type(app, buffer_id, di_string);

                } else if (token->sub_kind == TokenCppKind_Struct || token->sub_kind == TokenCppKind_Union ||
                           token->sub_kind == TokenCppKind_Class )
                {
                    // NOTE(jack): struct MyStruct {int x, int y};
                    //             union MyUnion { struct {int x, int y}; int a[2]; };
                    if (!token_it_inc_non_whitespace(&it)) {
                        DEBUG_MSG_LIT("union/struct/class was the last token\n");
                        break;
                    }
                    Token *next_token = token_it_read(&it);
                    if(next_token->kind == TokenBaseKind_Identifier) {
                        String_Const_u8 struct_name = push_buffer_range(app, scratch, buffer_id, Ii64(next_token));
                        jp_push_custom_type(app, buffer_id, struct_name);
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

#if 1
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
jp_parse_data_desk_types(Application_Links *app, Buffer_ID buffer_id) 
{
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);
    // NOTE(jack): "Empty" the current keyword/type arrays for this buffer.
    buffer_data->custom_keywords_end = 0;
    buffer_data->custom_types_end = 0;
    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    
    Token_List typeTokens = {};
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            // NOTE(jack): Temporary scratchpad memory arena that gets free'd at scope close
            Scratch_Block scratch(app);
            Token *token = token_it_read(&it);
            if (token->kind == TokenBaseKind_Identifier)
            {
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
                        jp_push_custom_type(app, buffer_data, token_string);
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
#endif

#if 0
/* Code to find Identifier Tokens which are types
This was mostly copied from Ryan Fleury's customisation layer.
He was doing this (#if 0'd out) in the render to colour custom types
however was too slow, if a faster lookup is added to the code index
this is a better solution, however it doesn't properly identify types in
cases like the following:
typedef MyStruct_t {
  ...
} MyStruct;

So if a faster lookup is added, I may convert parsing logic to instead add
Code Index Notes to type identifiers wich flag them as types. 

This would remove the need to store custom types in a buffer attachement
and would just requrie a code index lookup to determine if the token should
be highlighted. I may also be able to leaverage this for keywords, but I
haven't spent too long looking into the Code Index Notes.
*/
function void
jp_fill_buffer_data_with_types(Application_Links *app, Buffer_ID buffer_id)
{
    // NOTE(jack): Uses the Code Index to identify type indentifiers,
    // Does not include typedef's, defines or class
    // typedef struct name {} name; dosent work
    ProfileBlock(app, "jp_fill_buffer_data_with_types");

    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);
    // NOTE(jack): "Empty" the current keyword/type arrays for this buffer.
    buffer_data->custom_keywords_end = 0;
    buffer_data->custom_types_end = 0;

    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            Token *token = token_it_read(&it);
            
            Scratch_Block scratch(app);
            if (token->kind == TokenBaseKind_Identifier)
            {
                String_Const_u8 string = push_buffer_range(app, scratch, buffer_id,
                                                           Ii64(token->pos, token->pos + token->size));

                Code_Index_File* file = code_index_get_file(buffer_id);
                if (file != 0)
                {
                    for (i32 i = 0; i < file->note_array.count; i += 1)
                    {
                        Code_Index_Note* this_note = file->note_array.ptrs[i];
                        if (string_match(this_note->text, string) && 
                            this_note->note_kind == CodeIndexNote_Type)
                        {
                            jp_push_custom_type(app, buffer_data, string);
                        }
                    }
                }
            }
        }
    }
}
#endif

#endif /* FCODER_JACK_PUNTER_PARSING */