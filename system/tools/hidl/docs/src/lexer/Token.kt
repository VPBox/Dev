/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package lexer

data class Token(val identifier: TokenGrammar, val value: String, val category: TokenCategory)

enum class TokenCategory {
    Annotation, Delimiter, Keyword, Number, Op, TypeDef, Word, DocComment, DocAnnotation, DocWord
}

enum class TokenGrammar(val value: String, val category: TokenCategory) {

    WORD("", TokenCategory.Word),
    INTEGER("", TokenCategory.Number),
    DECIMAL("", TokenCategory.Number),

    AT("@", TokenCategory.Delimiter),
    EMPTY_LINE("EMPTY_LINE", TokenCategory.Delimiter), //sub in doc string, don't include value in token

    DOC_START("/**", TokenCategory.Delimiter),
    DOC_END("*/", TokenCategory.Delimiter),
    COMMENT_LINE("//", TokenCategory.Delimiter),
    COMMENT_START("/*", TokenCategory.Delimiter),

    // HIDL grammar: {android}/system/tools/hidl/hidl-gen_l.ll
    ENUM("enum", TokenCategory.TypeDef),
    EXTENDS("extends", TokenCategory.Keyword),
    GENERATES("generates", TokenCategory.Keyword),
    IMPORT("import", TokenCategory.Keyword),
    INTERFACE("interface", TokenCategory.TypeDef),
    PACKAGE("package", TokenCategory.Keyword),
    STRUCT("struct", TokenCategory.TypeDef),
    TYPEDEF("typedef", TokenCategory.TypeDef),
    UNION("union", TokenCategory.TypeDef),
    BITFIELD("bitfield", TokenCategory.TypeDef),
    VEC("vec", TokenCategory.TypeDef),
    REF("ref", TokenCategory.TypeDef),
    ONEWAY("oneway", TokenCategory.Keyword),

    BOOL("bool", TokenCategory.TypeDef),
    INT8_T("int8_t", TokenCategory.TypeDef),
    UINT8_T("uint8_t", TokenCategory.TypeDef),
    INT16_T("int16_t", TokenCategory.TypeDef),
    UINT16_T("uint16_t", TokenCategory.TypeDef),
    INT32_T("int32_t", TokenCategory.TypeDef),
    UINT32_T("uint32_t", TokenCategory.TypeDef),
    INT64_T("int64_t", TokenCategory.TypeDef),
    UINT64_T("int8_t", TokenCategory.TypeDef),
    FLOAT("float", TokenCategory.TypeDef),
    DOUBLE("double", TokenCategory.TypeDef),

    DEATH_RECIPIENT("death_recipient", TokenCategory.TypeDef),
    HANDLE("handle", TokenCategory.TypeDef),
    MEMORY("memory", TokenCategory.TypeDef),
    POINTER("pointer", TokenCategory.TypeDef),
    STRING("string", TokenCategory.TypeDef),

    FMQ_SYNC("fmq_sync", TokenCategory.TypeDef),
    FMQ_UNSYNC("fmq_unsync", TokenCategory.TypeDef),

    PAREN_OPEN("(", TokenCategory.Delimiter),
    PAREN_CLOSE(")", TokenCategory.Delimiter),
    CHEVRON_OPEN("<", TokenCategory.Delimiter),
    CHEVRON_CLOSE(">", TokenCategory.Delimiter),
    BRACE_OPEN("{", TokenCategory.Delimiter),
    BRACE_CLOSE("}", TokenCategory.Delimiter),
    BRACKET_OPEN("[", TokenCategory.Delimiter),
    BRACKET_CLOSE("]", TokenCategory.Delimiter),
    COLON(":", TokenCategory.Delimiter),
    SEMICOLON(";", TokenCategory.Delimiter),
    COMMA(",", TokenCategory.Delimiter),
    PERIOD(".", TokenCategory.Delimiter),
    EQUAL("=", TokenCategory.Op),
    PLUS("+", TokenCategory.Op),
    MINUS("-", TokenCategory.Op),
    MULTIPLY("*", TokenCategory.Op),
    DIVIDE("/", TokenCategory.Op),
    MOD("%", TokenCategory.Op),
    BITWISE_AND("&", TokenCategory.Op),
    BITWISE_OR("|", TokenCategory.Op),
    BITWISE_XOR("^", TokenCategory.Op),
    LSHIFT("<<", TokenCategory.Op),
    RSHIFT(">>", TokenCategory.Op),
    LOGICAL_AND("&&", TokenCategory.Op),
    LOGICAL_OR("||", TokenCategory.Op),
    NEGATE("!", TokenCategory.Op),
    COMPLEMENT("~", TokenCategory.Op),
    LEQ("<=", TokenCategory.Op),
    GEQ(">=", TokenCategory.Op),
    EQUALITY("==", TokenCategory.Op),
    NEQUALITY("!=", TokenCategory.Op),
    QUESTION("?", TokenCategory.Op),

    PKG_SCOPE("::", TokenCategory.Delimiter),

    // vts tags
    ENTRY("entry", TokenCategory.Annotation),
    EXIT("exit", TokenCategory.Annotation),
    CALLFLOW("callflow", TokenCategory.Annotation),
    EXPORT("export", TokenCategory.Annotation),

    // javadoc tags. Not all supported in HIDL
    // http://docs.oracle.com/javase/1.5.0/docs/tooldocs/windows/javadoc.html#javadoctags
    // https://docs.google.com/document/d/1zHzOtvI9vIADPmI211F_tXQUd-w6lP5u-Y98lvxPvYQ/view#heading=h.aopuaezi1is6
    AUTHOR("author", TokenCategory.DocAnnotation),
    CODE("code", TokenCategory.DocAnnotation),
    DOC_ROOT("docRoot", TokenCategory.DocAnnotation),
    DEPRECATED("deprecated", TokenCategory.DocAnnotation),
    EXCEPTION("exception", TokenCategory.DocAnnotation),
    INHERIT_DOC("inheritDoc", TokenCategory.DocAnnotation),
    LINK("link", TokenCategory.DocAnnotation),
    LINK_PLAIN("linkplain", TokenCategory.DocAnnotation),
    LITERAL("literal", TokenCategory.DocAnnotation),
    PARAM("param", TokenCategory.DocAnnotation),
    RETURN("return", TokenCategory.DocAnnotation),
    SEE("see", TokenCategory.DocAnnotation),
    SERIAL("serial", TokenCategory.DocAnnotation),
    SERIAL_DATA("serialData", TokenCategory.DocAnnotation),
    SERIAL_FIELD("serialField", TokenCategory.DocAnnotation),
    SINCE("since", TokenCategory.DocAnnotation),
    THROWS("throws", TokenCategory.DocAnnotation),
    VALUE("value", TokenCategory.DocAnnotation),
    VERSION("version", TokenCategory.DocAnnotation);

    companion object {
        private val map = TokenGrammar.values().associateBy(TokenGrammar::value)
        private val matchInt = Regex("\\d+")
        private val matchFloat = Regex("\\d+[.]\\d+")

        fun getFromValue(value: String): TokenGrammar? {
            return map[value]
        }

        fun getFromValueOrDefault(value: String): TokenGrammar {
            return getFromValue(value) ?: when {
                matchInt.matches(value) -> INTEGER
                matchFloat.matches(value) -> DECIMAL
                else -> WORD
            }
        }

        fun newToken(value: String): Token {
            val tokenGrammar = getFromValueOrDefault(value)
            return Token(tokenGrammar, value, tokenGrammar.category)
        }

        fun newToken(value: String, category: TokenCategory): Token {
            val tokenGrammar = getFromValueOrDefault(value)
            return Token(tokenGrammar, value, category)
        }

        fun newToken(value: String, identifier: TokenGrammar): Token {
            return Token(identifier, value, identifier.category)
        }

        fun annotations(): List<TokenGrammar> {
            return TokenGrammar.values().filter { it.category == TokenCategory.Annotation }
        }

        fun docAnnotations(): List<TokenGrammar> {
            return TokenGrammar.values().filter { it.category == TokenCategory.DocAnnotation }
        }
    }
}