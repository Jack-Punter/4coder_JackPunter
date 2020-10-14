#if !defined(FCODER_JACK_PUNTER_ODIN_PARSE)

function void
jp_parse_odin_content(Application_Links *app, Arena *scratch,
                      List_Highlight_String_Data *list, Buffer_ID buffer_id)
{
    // TODO(jack): This may be temporary? will be replaced with an ODIN specific parser.
    // I know that i'm kind of replacing the code_index stuff with the table that the list
    // fills but it allows me to use extra information so I'll likely stick with this approach,
    // unless I need to implement Code_Index_Nests for a language
    Used_Note_Kinds kinds = {};
    kinds.CodeIndex_Function = true;
    kinds.CodeIndex_Type = true;
    kinds.CodeIndex_Macro = true;
    jp_parse_using_code_index(app, scratch, list, buffer_id, kinds);
}

#define FCODER_JACK_PUNTER_ODIN_PARSE
#endif 