#if !defined(FCODER_JACK_PUNTER_LANGUAGE)

#pragma warning(disable : 4065)
#include "generated/lexer_odin.h"
#include "generated/lexer_odin.cpp"

struct List_Highlight_String_Data;

// When a language lexer is generated it will create a Lex_State_[langauge]
// These should be added here, so that we always have enough space
// to store a lexers state. (used in language_do_full_lex_async__inner)
union Generic_Lexer_State {
    Lex_State_Cpp cppState;
    Lex_State_Odin odinState;
};

typedef void (*lex_init_func)(Generic_Lexer_State*, String_Const_u8 /* content * */);
typedef b32 (*lex_breaks_func)(Arena*, Token_List *, Generic_Lexer_State *, i32 );
typedef Token_List (*lex_full_input_func)(Arena *arena, String_Const_u8 input);
typedef ARGB_Color (*get_token_color_func)(Application_Links *app, Token token,
                                           String_Const_u8 lexeme);

typedef void (*parse_content_func)(Application_Links *app, Arena *scratch,
                                   List_Highlight_String_Data *list, Buffer_ID buffer_id);


struct Language
{
    String_Const_u8 name;
    String_Const_u8 ext_string;
    lex_init_func lex_init;
    lex_breaks_func lex_breaks;
    lex_full_input_func lex_full_input;
    get_token_color_func get_token_color;
    parse_content_func parse_content;

    Language *next;
};

struct Language_List
{
    Language *first;
    Language *last;
    
    i64 count;
};

#define FCODER_JACK_PUNTER_LANGUAGE
#endif