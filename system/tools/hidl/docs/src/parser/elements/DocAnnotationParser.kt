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
import parser.peekPreviousToken
import parser.peekToken
import java.text.ParseException


class DocAnnotationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractParser(iter) {

    lateinit var tag: TokenGrammar
    var arg: String? = null //some tags have arguments (eg. @param arg desc)
    lateinit var description: String

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    private fun formatValue(tokens: List<Token>): String {
        return if (tokens.isEmpty()) {
            ""
        } else {
            tokens.map {
                when (it.identifier) {
                    TokenGrammar.EMPTY_LINE -> "\n\n"
                    else -> it.value
                }
            }
                    .joinToString(" ")
                    .let { ILexer.unpadDelimiters(it) }
        }
    }

    /**
     * Scan until: doc end token, empty line, another @param
     */
    override fun scanTokens(iter: ListIterator<Token>): List<Token> {
        val tokens = mutableListOf<Token>()
        var token: Token

        //depending how invoked, queue up doc annotation token
        if (peekToken(iter)?.identifier == TokenGrammar.AT) iter.next()
        if (peekPreviousToken(iter)?.category == TokenCategory.DocAnnotation) iter.previous()

        if (peekToken(iter)!!.category != TokenCategory.DocAnnotation)
            throw ParseException("Token sequence must begin with a DocAnnotation", this.indexStart)

        loop@ while (iter.hasNext()) {
            token = iter.next()

            when {
                //descriptions don't span blank lines
                token.identifier == TokenGrammar.EMPTY_LINE -> break@loop

                //if doc block ends or found next annotation tag, back up and bail
                token.identifier == TokenGrammar.DOC_END ||
                token.identifier == TokenGrammar.AT && peekToken(iter)?.category == TokenCategory.DocAnnotation -> {
                    iter.previous()
                    break@loop
                }

                else -> tokens.add(token)
            }
        }
        return tokens
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()
        assert(iter.hasNext())
        val token = iter.next()

        if (token.category != TokenCategory.DocAnnotation)
            throw ParseException("Invalid doc anootation tag: ${token.value}", this.indexStart)

        //annotation tag name (must be in TokenGrammar)
        this.tag = token.identifier

        //annotation tag can take an optional argument
        //so can return (preferred). TODO: check HALs if mandatory
        if (token.identifier == TokenGrammar.PARAM || token.identifier == TokenGrammar.RETURN) {
            if (iter.hasNext()) this.arg = iter.next().value
        }

        //the rest is annotation description
        val descTokens = mutableListOf<Token>()
        while (iter.hasNext()) {
            descTokens.add(iter.next())
        }

        this.description = if (descTokens.isEmpty()) {
            ""
        } else {
            descTokens.map { if (it.identifier == TokenGrammar.EMPTY_LINE) "\n\n" else it.value }
                    .joinToString(" ")
                    .let { ILexer.unpadDelimiters(it) }
        }
    }
}