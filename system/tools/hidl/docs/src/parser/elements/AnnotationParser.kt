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

import lexer.Token
import lexer.TokenCategory
import lexer.TokenGrammar
import parser.peekPreviousToken
import parser.peekToken
import java.text.ParseException

class AnnotationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractParser(iter) {

    lateinit var name: TokenGrammar
    lateinit var value: String

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    override fun scanTokens(iter: ListIterator<Token>): List<Token> {
        val tokens = mutableListOf<Token>()
        //depending how called, queue up annotation
        if (peekToken(iter)?.identifier == TokenGrammar.AT) iter.next()
        if (peekPreviousToken(iter)?.category == TokenCategory.Annotation) iter.previous()

        if (peekToken(iter)!!.category != TokenCategory.Annotation)
            throw ParseException("Doc token sequence must begin with an annotation", this.indexStart)

        //just one token, info embedded
        tokens.add(iter.next())
        return tokens
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()
        assert(peekToken(iter)!!.category == TokenCategory.Annotation)
        var token = iter.next()

        this.name = token.identifier
        this.value = parseAnnotationValue(token)
    }

    //capture text between parens
    private fun parseAnnotationValue(token: Token): String {
        return Regex(""".*\((.*)\).*""")
                .matchEntire(token.value)
                ?.groups?.get(1)
                ?.value?.trim()
                ?: ""
    }
}