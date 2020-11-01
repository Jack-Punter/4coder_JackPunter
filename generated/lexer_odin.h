#if !defined(FCODER_LEX_GEN_HAND_WRITTEN_TYPES)
#define FCODER_LEX_GEN_HAND_WRITTEN_TYPES

struct Lexeme_Table_Value{
    Token_Base_Kind base_kind;
    u16 sub_kind;
};

struct Lexeme_Table_Lookup{
    b32 found_match;
    Token_Base_Kind base_kind;
    u16 sub_kind;
};

#endif
typedef u16 Token_Odin_Kind;
enum{
TokenOdinKind_EOF = 0,
TokenOdinKind_Whitespace = 1,
TokenOdinKind_LexError = 2,
TokenOdinKind_BlockComment = 3,
TokenOdinKind_LineComment = 4,
TokenOdinKind_Backslash = 5,
TokenOdinKind_LiteralInteger = 6,
TokenOdinKind_LiteralIntegerHex = 7,
TokenOdinKind_LiteralIntegerOct = 8,
TokenOdinKind_LiteralIntegerDoz = 9,
TokenOdinKind_LiteralIntegerBin = 10,
TokenOdinKind_LiteralFloat = 11,
TokenOdinKind_LiteralString = 12,
TokenOdinKind_LiteralCharacter = 13,
TokenOdinKind_LiteralStringRaw = 14,
TokenOdinKind_KeywordGeneric = 15,
TokenOdinKind_KeywordAttribute = 16,
TokenOdinKind_KeywordDirective = 17,
TokenOdinKind_AttributeOpen = 18,
TokenOdinKind_AttributeClose = 19,
TokenOdinKind_BraceOp = 20,
TokenOdinKind_BraceCl = 21,
TokenOdinKind_ParenOp = 22,
TokenOdinKind_BrackOp = 23,
TokenOdinKind_ParenCl = 24,
TokenOdinKind_BrackCl = 25,
TokenOdinKind_Semicolon = 26,
TokenOdinKind_Colon = 27,
TokenOdinKind_DotDot = 28,
TokenOdinKind_DotDotLess = 29,
TokenOdinKind_ColonColon = 30,
TokenOdinKind_Dot = 31,
TokenOdinKind_Plus = 32,
TokenOdinKind_Minus = 33,
TokenOdinKind_Not = 34,
TokenOdinKind_Tilde = 35,
TokenOdinKind_Star = 36,
TokenOdinKind_And = 37,
TokenOdinKind_Div = 38,
TokenOdinKind_Mod = 39,
TokenOdinKind_LeftLeft = 40,
TokenOdinKind_RightRight = 41,
TokenOdinKind_Less = 42,
TokenOdinKind_LessEq = 43,
TokenOdinKind_Grtr = 44,
TokenOdinKind_GrtrEq = 45,
TokenOdinKind_EqEq = 46,
TokenOdinKind_NotEq = 47,
TokenOdinKind_Carrot = 48,
TokenOdinKind_Or = 49,
TokenOdinKind_AndAnd = 50,
TokenOdinKind_OrOr = 51,
TokenOdinKind_Ternary = 52,
TokenOdinKind_Eq = 53,
TokenOdinKind_ColonEq = 54,
TokenOdinKind_PlusEq = 55,
TokenOdinKind_MinusEq = 56,
TokenOdinKind_StarEq = 57,
TokenOdinKind_DivEq = 58,
TokenOdinKind_ModEq = 59,
TokenOdinKind_LeftLeftEq = 60,
TokenOdinKind_RightRightEq = 61,
TokenOdinKind_Comma = 62,
TokenOdinKind_using = 63,
TokenOdinKind_import = 64,
TokenOdinKind_foreign = 65,
TokenOdinKind_package = 66,
TokenOdinKind_where = 67,
TokenOdinKind_when = 68,
TokenOdinKind_if = 69,
TokenOdinKind_else = 70,
TokenOdinKind_for = 71,
TokenOdinKind_switch = 72,
TokenOdinKind_in = 73,
TokenOdinKind_notin = 74,
TokenOdinKind_not_in = 75,
TokenOdinKind_do = 76,
TokenOdinKind_case = 77,
TokenOdinKind_break = 78,
TokenOdinKind_continue = 79,
TokenOdinKind_fallthrough = 80,
TokenOdinKind_defer = 81,
TokenOdinKind_return = 82,
TokenOdinKind_proc = 83,
TokenOdinKind_struct = 84,
TokenOdinKind_union = 85,
TokenOdinKind_enum = 86,
TokenOdinKind_bit_field = 87,
TokenOdinKind_bit_set = 88,
TokenOdinKind_map = 89,
TokenOdinKind_dynamic = 90,
TokenOdinKind_auto_cast = 91,
TokenOdinKind_cast = 92,
TokenOdinKind_transmute = 93,
TokenOdinKind_distinct = 94,
TokenOdinKind_opaque = 95,
TokenOdinKind_inline = 96,
TokenOdinKind_no_inline = 97,
TokenOdinKind_size_of = 98,
TokenOdinKind_align_of = 99,
TokenOdinKind_offset_of = 100,
TokenOdinKind_type_of = 101,
TokenOdinKind_context = 102,
TokenOdinKind_LiteralTrue = 103,
TokenOdinKind_LiteralFalse = 104,
TokenOdinKind_LiteralNil = 105,
TokenOdinKind_byte = 106,
TokenOdinKind_bool = 107,
TokenOdinKind_b8 = 108,
TokenOdinKind_b16 = 109,
TokenOdinKind_b32 = 110,
TokenOdinKind_b64 = 111,
TokenOdinKind_i8 = 112,
TokenOdinKind_u8 = 113,
TokenOdinKind_i16 = 114,
TokenOdinKind_u16 = 115,
TokenOdinKind_i32 = 116,
TokenOdinKind_u32 = 117,
TokenOdinKind_i64 = 118,
TokenOdinKind_u64 = 119,
TokenOdinKind_i128 = 120,
TokenOdinKind_u128 = 121,
TokenOdinKind_rune = 122,
TokenOdinKind_f16 = 123,
TokenOdinKind_f32 = 124,
TokenOdinKind_f64 = 125,
TokenOdinKind_complex32 = 126,
TokenOdinKind_complex64 = 127,
TokenOdinKind_complex128 = 128,
TokenOdinKind_quaternion128 = 129,
TokenOdinKind_quaternion256 = 130,
TokenOdinKind_int = 131,
TokenOdinKind_uint = 132,
TokenOdinKind_uintptr = 133,
TokenOdinKind_rawptr = 134,
TokenOdinKind_string = 135,
TokenOdinKind_cstring = 136,
TokenOdinKind_any = 137,
TokenOdinKind_typeid = 138,
TokenOdinKind_i8le = 139,
TokenOdinKind_u8le = 140,
TokenOdinKind_i16le = 141,
TokenOdinKind_u16le = 142,
TokenOdinKind_i32le = 143,
TokenOdinKind_u32le = 144,
TokenOdinKind_i64le = 145,
TokenOdinKind_u64le = 146,
TokenOdinKind_i128le = 147,
TokenOdinKind_u128le = 148,
TokenOdinKind_i8be = 149,
TokenOdinKind_u8be = 150,
TokenOdinKind_i16be = 151,
TokenOdinKind_u16be = 152,
TokenOdinKind_i32be = 153,
TokenOdinKind_u32be = 154,
TokenOdinKind_i64be = 155,
TokenOdinKind_u64be = 156,
TokenOdinKind_i128be = 157,
TokenOdinKind_u128be = 158,
TokenOdinKind_len = 159,
TokenOdinKind_cap = 160,
TokenOdinKind_type_info_of = 161,
TokenOdinKind_typeid_of = 162,
TokenOdinKind_swizzle = 163,
TokenOdinKind_complex = 164,
TokenOdinKind_quaternion = 165,
TokenOdinKind_real = 166,
TokenOdinKind_imag = 167,
TokenOdinKind_jmag = 168,
TokenOdinKind_kmag = 169,
TokenOdinKind_conj = 170,
TokenOdinKind_expand_to_tuple = 171,
TokenOdinKind_min = 172,
TokenOdinKind_max = 173,
TokenOdinKind_abs = 174,
TokenOdinKind_clamp = 175,
TokenOdinKind_init_global_temporary_allocator = 176,
TokenOdinKind_copy_slice = 177,
TokenOdinKind_copy_from_string = 178,
TokenOdinKind_copy = 179,
TokenOdinKind_pop = 180,
TokenOdinKind_unoredered_remove = 181,
TokenOdinKind_ordered_remove = 182,
TokenOdinKind_clear = 183,
TokenOdinKind_reserve = 184,
TokenOdinKind_resize = 185,
TokenOdinKind_free = 186,
TokenOdinKind_free_all = 187,
TokenOdinKind_delete_string = 188,
TokenOdinKind_delete_cstring = 189,
TokenOdinKind_delete_dynamic_array = 190,
TokenOdinKind_delete_slice = 191,
TokenOdinKind_delete_map = 192,
TokenOdinKind_delete = 193,
TokenOdinKind_new = 194,
TokenOdinKind_new_clone = 195,
TokenOdinKind_make_slice = 196,
TokenOdinKind_make_dynamic_array = 197,
TokenOdinKind_make_dynamic_array_len = 198,
TokenOdinKind_make_dynamic_array_len_cap = 199,
TokenOdinKind_make_map = 200,
TokenOdinKind_make = 201,
TokenOdinKind_reserve_map = 202,
TokenOdinKind_delete_key = 203,
TokenOdinKind_append_elem = 204,
TokenOdinKind_append_elems = 205,
TokenOdinKind_append_elem_string = 206,
TokenOdinKind_reserve_soa = 207,
TokenOdinKind_append_soa_elem = 208,
TokenOdinKind_append_soa_elems = 209,
TokenOdinKind_append = 210,
TokenOdinKind_append_soa = 211,
TokenOdinKind_append_string = 212,
TokenOdinKind_clear_dynamic_array = 213,
TokenOdinKind_reserve_dynamic_array = 214,
TokenOdinKind_resize_dynamic_array = 215,
TokenOdinKind_incl_elem = 216,
TokenOdinKind_incl_elems = 217,
TokenOdinKind_excl_elem = 218,
TokenOdinKind_excl_elems = 219,
TokenOdinKind_excl_bit_set = 220,
TokenOdinKind_incl = 221,
TokenOdinKind_excl = 222,
TokenOdinKind_card = 223,
TokenOdinKind_Identifier = 224,
TokenOdinKind_align = 225,
TokenOdinKind_packed = 226,
TokenOdinKind_raw_union = 227,
TokenOdinKind_no_nil = 228,
TokenOdinKind_complete = 229,
TokenOdinKind_no_alias = 230,
TokenOdinKind_type = 231,
TokenOdinKind_c_vararg = 232,
TokenOdinKind_assert = 233,
TokenOdinKind_caller_location = 234,
TokenOdinKind_file = 235,
TokenOdinKind_line = 236,
TokenOdinKind_locations = 237,
TokenOdinKind_procedure = 238,
TokenOdinKind_load = 239,
TokenOdinKind_defined = 240,
TokenOdinKind_bounds_check = 241,
TokenOdinKind_no_bounds_check = 242,
TokenOdinKind_partial = 243,
TokenOdinKind_builtin = 244,
TokenOdinKind_export = 245,
TokenOdinKind_static = 246,
TokenOdinKind_deferred_in = 247,
TokenOdinKind_deferred_none = 248,
TokenOdinKind_deferred_out = 249,
TokenOdinKind_require_results = 250,
TokenOdinKind_default_calling_convention = 251,
TokenOdinKind_link_name = 252,
TokenOdinKind_link_prefix = 253,
TokenOdinKind_deprecated = 254,
TokenOdinKind_private = 255,
TokenOdinKind_thread_local = 256,
TokenOdinKind_COUNT = 257,
};
char *token_odin_kind_names[] = {
"EOF",
"Whitespace",
"LexError",
"BlockComment",
"LineComment",
"Backslash",
"LiteralInteger",
"LiteralIntegerHex",
"LiteralIntegerOct",
"LiteralIntegerDoz",
"LiteralIntegerBin",
"LiteralFloat",
"LiteralString",
"LiteralCharacter",
"LiteralStringRaw",
"KeywordGeneric",
"KeywordAttribute",
"KeywordDirective",
"AttributeOpen",
"AttributeClose",
"BraceOp",
"BraceCl",
"ParenOp",
"BrackOp",
"ParenCl",
"BrackCl",
"Semicolon",
"Colon",
"DotDot",
"DotDotLess",
"ColonColon",
"Dot",
"Plus",
"Minus",
"Not",
"Tilde",
"Star",
"And",
"Div",
"Mod",
"LeftLeft",
"RightRight",
"Less",
"LessEq",
"Grtr",
"GrtrEq",
"EqEq",
"NotEq",
"Carrot",
"Or",
"AndAnd",
"OrOr",
"Ternary",
"Eq",
"ColonEq",
"PlusEq",
"MinusEq",
"StarEq",
"DivEq",
"ModEq",
"LeftLeftEq",
"RightRightEq",
"Comma",
"using",
"import",
"foreign",
"package",
"where",
"when",
"if",
"else",
"for",
"switch",
"in",
"notin",
"not_in",
"do",
"case",
"break",
"continue",
"fallthrough",
"defer",
"return",
"proc",
"struct",
"union",
"enum",
"bit_field",
"bit_set",
"map",
"dynamic",
"auto_cast",
"cast",
"transmute",
"distinct",
"opaque",
"inline",
"no_inline",
"size_of",
"align_of",
"offset_of",
"type_of",
"context",
"LiteralTrue",
"LiteralFalse",
"LiteralNil",
"byte",
"bool",
"b8",
"b16",
"b32",
"b64",
"i8",
"u8",
"i16",
"u16",
"i32",
"u32",
"i64",
"u64",
"i128",
"u128",
"rune",
"f16",
"f32",
"f64",
"complex32",
"complex64",
"complex128",
"quaternion128",
"quaternion256",
"int",
"uint",
"uintptr",
"rawptr",
"string",
"cstring",
"any",
"typeid",
"i8le",
"u8le",
"i16le",
"u16le",
"i32le",
"u32le",
"i64le",
"u64le",
"i128le",
"u128le",
"i8be",
"u8be",
"i16be",
"u16be",
"i32be",
"u32be",
"i64be",
"u64be",
"i128be",
"u128be",
"len",
"cap",
"type_info_of",
"typeid_of",
"swizzle",
"complex",
"quaternion",
"real",
"imag",
"jmag",
"kmag",
"conj",
"expand_to_tuple",
"min",
"max",
"abs",
"clamp",
"init_global_temporary_allocator",
"copy_slice",
"copy_from_string",
"copy",
"pop",
"unoredered_remove",
"ordered_remove",
"clear",
"reserve",
"resize",
"free",
"free_all",
"delete_string",
"delete_cstring",
"delete_dynamic_array",
"delete_slice",
"delete_map",
"delete",
"new",
"new_clone",
"make_slice",
"make_dynamic_array",
"make_dynamic_array_len",
"make_dynamic_array_len_cap",
"make_map",
"make",
"reserve_map",
"delete_key",
"append_elem",
"append_elems",
"append_elem_string",
"reserve_soa",
"append_soa_elem",
"append_soa_elems",
"append",
"append_soa",
"append_string",
"clear_dynamic_array",
"reserve_dynamic_array",
"resize_dynamic_array",
"incl_elem",
"incl_elems",
"excl_elem",
"excl_elems",
"excl_bit_set",
"incl",
"excl",
"card",
"Identifier",
"align",
"packed",
"raw_union",
"no_nil",
"complete",
"no_alias",
"type",
"c_vararg",
"assert",
"caller_location",
"file",
"line",
"locations",
"procedure",
"load",
"defined",
"bounds_check",
"no_bounds_check",
"partial",
"builtin",
"export",
"static",
"deferred_in",
"deferred_none",
"deferred_out",
"require_results",
"default_calling_convention",
"link_name",
"link_prefix",
"deprecated",
"private",
"thread_local",
};
