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

package parser.elements

import lexer.ILexer
import lexer.Token
import lexer.TokenCategory
import lexer.TokenGrammar
import parser.peekToken

class DocParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractParser(iter) {

    val description: String by lazy { formatDescription(this.descTokens) }
    var docAnnotationParsers = mutableListOf<DocAnnotationParser>()

    private var descTokens = mutableListOf<Token>()

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    private fun formatDescription(tokens: List<Token>): String {
        return tokens
                .filterNot { it.identifier == TokenGrammar.DOC_START }
                .filterNot { it.identifier == TokenGrammar.DOC_END }
                .map {
                    when (it.identifier) {
                        TokenGrammar.EMPTY_LINE -> "\n\n"
                        else -> it.value
                    }
                }
                .joinToString(" ")
                .let { ILexer.unpadDelimiters(it) }
    }

    override fun scanTokens(iter: ListIterator<Token>): List<Token> {
        //keep doc_start and doc_end tokens /** ... */
        return scanDocTokens(iter)
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()
        var token = iter.next() //doc_start

        assert(token.identifier == TokenGrammar.DOC_START)
        assert(tokens.last().identifier == TokenGrammar.DOC_END)

        loop@ while(iter.hasNext()) {
            token = iter.next()

            when {
                token.identifier == TokenGrammar.AT && peekToken(iter)?.category == TokenCategory.DocAnnotation -> {
                    docAnnotationParsers.add(DocAnnotationParser(iter))  //increments iterator
                }
                token.identifier == TokenGrammar.DOC_END -> break@loop
                else -> this.descTokens.add(token)
            }
        }
    }
}