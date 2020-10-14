#if !defined(FCODER_JACK_PUNTER_PARSING)
#define FCODER_JACK_PUNTER_PARSING

struct Highlight_String_Data {
    String_Const_u8 string;
    Highlight_Data data;
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
highlight_string_list_push(Arena *arena, List_Highlight_String_Data *list, String_Const_u8 string,
                           HighlightType type, Buffer_ID buffer, i64 pos)
{
    Highlight_String_Data highlight_data = {};
    highlight_data.string = string;
    highlight_data.data.type = type;
    highlight_data.data.def_buffer = buffer;
    highlight_data.data.def_pos = pos;

    // Add the data to the list
    Node_Highlight_String_Data *node = push_array(arena, Node_Highlight_String_Data, 1);
    sll_queue_push(list->first, list->last, node);
    node->data = highlight_data;
    list->node_count += 1;
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
        Highlight_Data table_entry;
        // NOTE(jack): If the string is not already a key in the table;
        if(!jp_custom_highlight_lookup(app, node->data.string, &table_entry)) {
            Result.vals[array_index] = push_string_copy(arena, node->data.string);

            // NOTE(jack): Make a Data object from the temporarily allocated data
            // Then add a copy of that to the buffer arena for long term storage
            Data temp_highlight_data = make_data_struct(&node->data.data);
            Data perm_highlight_data = push_data_copy(arena, temp_highlight_data);
            jp_insert_custom_highlight(app, Result.vals[array_index], perm_highlight_data);

            ++array_index;
        } else {
            // TODO(jack): store something to indicate that there are overloads?
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

struct Used_Note_Kinds {
    bool CodeIndex_Function: 1;
    bool CodeIndex_Type: 1;
    bool CodeIndex_Macro: 1;
};

// TODO(jack): we probably want to be smarter here for performance reasons,
// This will currently attempt to push a function when it is declared, and
// when it is called, resulting in a lot of string comparisons].
function void
jp_parse_using_code_index(Application_Links *app, Arena *scratch, List_Highlight_String_Data *list,
                          Buffer_ID buffer_id, Used_Note_Kinds used_note_kinds)
{
    // NOTE(jack): Uses the Code Index to identify function indentifiers,
    ProfileScope(app, "jp_parse_using_code_index");

    Token_Array token_array = get_token_array_from_buffer(app, buffer_id);
    if (token_array.tokens != 0)
    {
        Token_Iterator_Array it = token_iterator_index(0, &token_array, 0);
        Token *prev_token = 0;
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
                            if (used_note_kinds.CodeIndex_Function && this_note->note_kind == CodeIndexNote_Function)
                            {
                                /* TODO(jack): in the following case the definition of PushSize_ is the first line:
                                    #define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type))
                                    #define PushArray(Arena, Count, type) (type *)PushSize_(Arena, (Count) * sizeof(type))
                                    void *
                                    PushSize_(memory_arena *Arena, memory_index Size)
                                    { ... }
                                */
                                highlight_string_list_push(scratch, list, string,
                                                           HighlightType_Function, buffer_id, token->pos);
                            } else if(used_note_kinds.CodeIndex_Type && this_note->note_kind == CodeIndexNote_Type) {
                                highlight_string_list_push(scratch, list, string,
                                                           HighlightType_Type, buffer_id, token->pos);
                            } else if (used_note_kinds.CodeIndex_Macro &&
                                       (prev_token && prev_token->sub_kind == TokenCppKind_PPDefine) &&
                                       this_note->note_kind == CodeIndexNote_Macro)
                            {
                                highlight_string_list_push(scratch, list, string,
                                                           HighlightType_Macro, buffer_id, token->pos);
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
            prev_token = token;
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
    List_Highlight_String_Data list {};
    buffer_data->language->parse_content(app, scratch, &list, buffer_id);

    // NOTE(jack): Fill buffer's list of custom highlights
    buffer_data->custom_highlight_array = finalize_highlight_string_list(app, &buffer_data->custom_highlights_arena,
                                                                         &list);
}

#include "language/odin/odin_parse.cpp"
#include "language/cpp/cpp_parse.cpp"
#include "language/datadesk/datadesk_parse.cpp"

#endif /* FCODER_JACK_PUNTER_PARSING */