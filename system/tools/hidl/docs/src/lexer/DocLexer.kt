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

import java.util.*

object DocLexer : ILexer {

    /**
     * @param str The string should already be padded from the file lexer.
     */
    override fun tokenize(str: String): List<Token> {
        val tokens = mutableListOf<Token>()
        var token: String

        //remove docblock comment indention - must go before others
        val formattedStr = str.lines()
                //remove docblock comment indent prefix (must go before others) ...
                .map { it.replace(Regex("^\\s*\\*[^/]"), "") } //indented prefix except doc_end
                .map { it.replace(Regex("^\\s*\\*$"), "") }    //prefix empty lines remain
                //replace empty lines with something the scanner can pick out ...
                .map { it.replace(Regex("$\\s*^"), TokenGrammar.EMPTY_LINE.value) }
                .joinToString("\n")

        Scanner(formattedStr).use { scanner ->
            while (scanner.hasNext()) {
                token = scanner.next()

                when (token) {
                    TokenGrammar.EMPTY_LINE.value -> tokens.add(TokenGrammar.newToken("", TokenGrammar.EMPTY_LINE))

                    //if part of annotation, add following tag as well
                    TokenGrammar.AT.value -> {
                        tokens.add(TokenGrammar.newToken(token)) //'@'
                        //determine if part of annotation
                        for (annotation in TokenGrammar.docAnnotations()) {
                            if (scanner.hasNext(annotation.value)) {
                                tokens.add(TokenGrammar.newToken(scanner.next()))
                            }
                        }
                    }

                    //default to DocWord
                    else -> {
                        val id = TokenGrammar.getFromValueOrDefault(token)
                        val category = if (id == TokenGrammar.WORD) TokenCategory.DocWord else id.category
                        tokens.add(TokenGrammar.newToken(token, category))
                        //TODO: Perhaps make all docblock words a DocWord
                        //tokens.add(TokenGrammar.newToken(token, TokenCategory.DocWord))
                    }
                }
            }
        }
        return tokens.toList()
    }
}