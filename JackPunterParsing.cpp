#if !defined(FCODER_JACK_PUNTER_PARSING)
#define FCODER_JACK_PUNTER_PARSING

struct Highlight_String_Data {
    String_Const_u8 string;
    HighlightType type;
};

struct Node_Highlight_String_Data {
    Node_Highlight_String_Data *next;
    Highlight_String_Data data;
};

struct List_Highlight_String_Data {
    Node_Highlight_String_Data *first;
    Node_Highlight_String_Data *last;
    i32 node_count;
};

function void
highlight_string_list_push(Arena *arena, List_Highlight_String_Data *list,
                          Highlight_String_Data data) {
    Node_Highlight_String_Data *node = push_array(arena, Node_Highlight_String_Data, 1);
    sll_queue_push(list->first, list->last, node);
    node->data = data;
    list->node_count += 1;
}

function void
highlight_string_list_push(Arena *arena, List_Highlight_String_Data *list,
                          String_Const_u8 String, HighlightType type)
{
    Highlight_String_Data highlight_data = {};
    highlight_data.string = String;
    highlight_data.type = type;
    highlight_string_list_push(arena, list, highlight_data);
}

function String_Const_u8_Array
finalize_highlight_string_list(Application_Links *app, Arena *arena, 
                                List_Highlight_String_Data *list)
{
    ProfileScope(app, "Finailze highlight string list");
    String_Const_u8_Array Result = {};
    Result.vals = push_array(arena, String_Const_u8, list->node_count);
    Result.count = list->node_count;
    i32 array_index = 0;
    for (Node_Highlight_String_Data *node = list->first; node != 0; node = node->next) {
        if(jp_custom_highlight_lookup(app, node->data.string) == HighlightType_None) {
            Result.vals[array_index] = push_string_copy(arena, node->data.string);
            jp_insert_custom_highlight(app, Result.vals[array_index], node->data.type);
            ++array_index;
        } else {
            --Result.count;
        }
    }
    return Result;
}

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
                    // NOTE(jack): If the defined to (dt) token is a keyword and is a type
                    if((dt_token->kind == TokenBaseKind_Keyword && jp_is_type_token(dt_token)))
                    {
                        highlight_string_list_push(scratch, list, di_string, HighlightType_Type);
                    }
                    // NOTE(jack): If the dt_token (keyword) is not a type or it is a custom keyword
                    else if (dt_token->kind == TokenBaseKind_Keyword && !jp_is_type_token(dt_token))
                    {
                        highlight_string_list_push(scratch, list, di_string, HighlightType_Keyword);
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
                            highlight_string_list_push(scratch, list, inner_struct_type, HighlightType_Type);
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
                    highlight_string_list_push(scratch, list, di_string, HighlightType_Type);
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
                        highlight_string_list_push(scratch, list, struct_name, HighlightType_Type);
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
jp_parse_data_desk_types(Application_Links *app, Arena *scratch,
                         List_Highlight_String_Data *list, Buffer_ID buffer_id) 
{
    ProfileScope(app, "jp Parse Data Desk Types");

    // NOTE(jack): "Empty" the current keyword/type arrays for this buffer.
    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    
    Token_List typeTokens = {};
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            // NOTE(jack): Temporary scratchpad memory arena that gets free'd at scope close
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
                        highlight_string_list_push(scratch, list, token_string, HighlightType_Type);
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

/* The function Code_Index code can also be used for getting identifiers that are types.
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

// TODO(jack): we probably want to be smarter here for performance reasons,
// This will currently attempt to push a function when it is declared, and
// when it is called, resulting in a lot of string comparisons].
function void
jp_fill_buffer_data_with_functions_macros(Application_Links *app, Arena *scratch,
                                   List_Highlight_String_Data *list, Buffer_ID buffer_id)
{
    // NOTE(jack): Uses the Code Index to identify function indentifiers,
    ProfileScope(app, "jp_fill_buffer_data_with_functions");

    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        for (;;)
        {
            Token *token = token_it_read(&it);
            
            if (token->kind == TokenBaseKind_Identifier)
            {
                String_Const_u8 string = push_token_lexeme(app, scratch, buffer_id, token);

                Code_Index_File* file = code_index_get_file(buffer_id);
                if (file != 0)
                {
                    for (i32 i = 0; i < file->note_array.count; i += 1)
                    {
                        Code_Index_Note* this_note = file->note_array.ptrs[i];
                        if (string_match(this_note->text, string)) {
                            if (this_note->note_kind == CodeIndexNote_Function) {
                                highlight_string_list_push(scratch, list, string, HighlightType_Function);
                            } else if (this_note->note_kind == CodeIndexNote_Macro) {
                                highlight_string_list_push(scratch, list, string, HighlightType_Macro);
                            }
                            // NOTE(jack): We have found the note for this token,
                            // so we can break out of the note loop.
                            break;
                        }
                    }
                }
            }

            if (!token_it_inc_non_whitespace(&it)) {
                break;
            }
        }
    }
}

function void
jp_parse_custom_highlights(Application_Links *app, Buffer_ID buffer_id)
{
    ProfileScope(app, "Jp Parse Custom Highlights");
    Managed_Scope buffer_scope = buffer_get_managed_scope(app, buffer_id);
    jp_buffer_data_t* buffer_data = scope_attachment(app, buffer_scope, jp_buffer_attachment, 
                                                     jp_buffer_data_t);

    Scratch_Block scratch(app);
    // NOTE(jack): remove all of this buffers custom highlights
    for(int i = 0; i < buffer_data->custom_highlight_array.count; ++i) {
        jp_erase_custom_highlight(app, buffer_data->custom_highlight_array.vals[i]);
    }
    linalloc_clear(&buffer_data->custom_highlights_arena);

    // NOTE(jack): parse the buffers filling the list of custom highlight tokens.
    //List_String_Const_u8 list = {};

    List_Highlight_String_Data list {};

    String_Const_u8 buffer_filename = push_buffer_file_name(app, scratch, buffer_id);
    String_Const_u8 ext = string_file_extension(buffer_filename);

    if (string_match(ext, string_u8_litexpr("ds"))) {
        jp_parse_data_desk_types(app, scratch, &list, buffer_id);
    } else {
        jp_parse_cpp_keywords_types(app, scratch, &list, buffer_id);
        jp_fill_buffer_data_with_functions_macros(app, scratch, &list, buffer_id );
    }

    // NOTE(jack): Fill buffer's list of custom highlights
    buffer_data->custom_highlight_array = finalize_highlight_string_list(app, &buffer_data->custom_highlights_arena,
                                                                         &list);
}

#endif /* FCODER_JACK_PUNTER_PARSING */