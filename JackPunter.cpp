#if !defined(FCODER_JACK_PUNTER)
#define FCODER_JACK_PUNTER

#include "../4coder_default_include.cpp"

#define DEBUGGING 0

#if DEBUGGING
#define DEBUG_MSG_LIT(str) print_message(app, string_u8_litexpr(str))
#define DEBUG_MSG_STR(str) print_message(app, str)
#else
#define DEBUG_MSG_LIT(str)
#define DEBUG_MSG_STR(str)
#endif


global const i32 custom_highlight_base_size = 32;
global bool GlobalShowDefinitionPeeks = true;
global bool GlobalIsRecordingMacro = false;

global i32 type_token_kinds[] = {
    TokenCppKind_Void,
    TokenCppKind_Bool,
    TokenCppKind_Char,
    TokenCppKind_Int,
    TokenCppKind_Float,
    TokenCppKind_Double,
    TokenCppKind_Long,-
    TokenCppKind_Short,
    TokenCppKind_Unsigned,
    TokenCppKind_Signed,
    TokenCppKind_Const,
    TokenCppKind_Volatile
};

enum HighlightType : u64 {
    HighlightType_Function,
    HighlightType_Macro,
    HighlightType_Type,
    HighlightType_Keyword,
};

struct Highlight_Data {
    HighlightType type;
    Buffer_ID def_buffer;
    i64 def_pos;
};

bool operator==(const Highlight_Data& lhs, const Highlight_Data& rhs) {
    return (lhs.def_buffer == rhs.def_buffer &&
            lhs.def_pos == rhs.def_pos);
}

struct jp_app_data_t {
    // NOTE(jack): This will store the custom highlight information for all buffers
    Base_Allocator* table_allocator;
    Arena global_highlights_arena;
    Table_Data_Data custom_highlight_table;
};

struct jp_buffer_data_t {
    // NOTE(jack): This will store the custom highlight tokens (keys for table lookup)
    // for this buffer
    // cleared on reparse (as well as erased from app table)
    Arena custom_highlights_arena;
    String_Const_u8_Array custom_highlight_array;

    b32 parse_contents = false;
};
CUSTOM_ID(attachment, jp_buffer_attachment);
CUSTOM_ID(attachment, jp_app_attachment);
CUSTOM_ID(attachment, buffer_parse_keywords_types_task);

function b32
jp_is_type_token(Token* token){
    for (int i = 0; i < ArrayCount(type_token_kinds); ++i) {
        if (token->sub_kind == type_token_kinds[i]) {
            return true;
        }
    }
    return false;
}

function b32
jp_insert_custom_highlight(Application_Links *app, String_Const_u8 string,
                           Data data) 
{
    ProfileScope(app, "JP Insert Custom Highlight");
    Assert(data.size == sizeof(Highlight_Data));
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);
    
    //String_Const_u8 perm_string = push_string_copy(&app_data->highlight_table_arena, string);
    Data key = {};
    key.data = string.str;
    key.size = string.size;
    return table_insert(&app_data->custom_highlight_table, key, data);
}

function b32
jp_custom_highlight_lookup(Application_Links *app, String_Const_u8 string, Highlight_Data *out_val)
{
    ProfileScope(app, "JP Custom Highlight Lookup");
    Data table_entry = {0};
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);

    Data key_data = {0};
    key_data.data = string.str;
    key_data.size = string.size;
    b32 read_success = table_read(&app_data->custom_highlight_table, key_data, &table_entry);
    if (read_success) {
        Assert(table_entry.size == sizeof(Highlight_Data));
        *out_val = *(Highlight_Data *)table_entry.data;
    }
    return read_success;
}

function b32
jp_custom_highlight_token_lookup(Application_Links *app, Token *token, Buffer_ID buffer, Highlight_Data *out_val)
{
    ProfileScope(app, "JP Custom Highlight Lookup [Token]");
    Scratch_Block scratch(app);
    String_Const_u8 token_string = push_token_lexeme(app, scratch, buffer, token);
    return jp_custom_highlight_lookup(app, token_string, out_val);
}

function b32
jp_erase_custom_highlight(Application_Links *app, String_Const_u8 string)
{
    ProfileScope(app, "JP Erase Custom Highlight");
    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);
    Data key = {};
    key.data = string.str;
    key.size = string.size;
    return table_erase(&app_data->custom_highlight_table, key);
}

#include "JackPunterParsing.cpp"
// NOTE(jack): Custom Command Definitions
#include "JackPunterCommand.cpp"
#include "JackPunterDraw.cpp"
#include "JackPunterHook.cpp"

#include "../generated/managed_id_metadata.cpp"

#include "JackPunterBindings.cpp"

void
custom_layer_init(Application_Links *app)
{
    Thread_Context *tctx = get_thread_context(app);
    
    // NOTE(allen): setup for default framework
    default_framework_init(app);
    
    // NOTE(allen): default hooks and command maps
    set_all_default_hooks(app);
    set_custom_hook(app, HookID_Tick, jp_tick);
    set_custom_hook(app, HookID_BeginBuffer, jp_begin_buffer);
    set_custom_hook(app, HookID_EndBuffer, jp_end_buffer);
    set_custom_hook(app, HookID_RenderCaller, jp_render_caller);
    set_custom_hook(app, HookID_SaveFile, jp_file_save);
    
    mapping_init(tctx, &framework_mapping);
    
    // NOTE(jack): If None of the custom bindings are working, check that the the config/ file
    // has mapping=""; rather than mapping="choose";
    // 4.1.6 chaned the default setting to ""
    JackPunterBindings(&framework_mapping);

    Managed_Scope global_scope = get_global_managed_scope(app);
    jp_app_data_t* app_data = scope_attachment(app, global_scope, jp_app_attachment, jp_app_data_t);
    app_data->table_allocator = get_base_allocator_system();
    app_data->global_highlights_arena = make_arena_system();
    app_data->custom_highlight_table = make_table_Data_Data(app_data->table_allocator,
                                                           custom_highlight_base_size);

    Highlight_Data not_defined_type = {HighlightType_Type, 0, 0};
    Data nd_type_data = push_data_copy(&app_data->global_highlights_arena,
                                       make_data_struct(&not_defined_type));

    Highlight_Data not_defined_keyword = {HighlightType_Keyword, 0, 0};
    Data nd_keyword_data = push_data_copy(&app_data->global_highlights_arena,
                                          make_data_struct(&not_defined_keyword));
    // CUSTOM TYPE HIGHLIGHTS
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("uint8_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("uint16_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("uint32_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("uint64_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("int8_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("int16_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("int32_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("int64_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("size_t")),
        nd_type_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("auto")),
        nd_type_data
    );
    

    // CUSTOM KEYWORD HIGHLIGHTS
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("override")),
        nd_keyword_data
    );
    jp_insert_custom_highlight(
        app, push_string_copy(&app_data->global_highlights_arena, SCu8("final")),
        nd_keyword_data
    );
}

#endif // FCODER_JACK_PUNTER

// BOTTOM
