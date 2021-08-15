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
import lexer.TokenGrammar
import parser.peekPreviousToken
import parser.peekToken
import java.text.ParseException

/**
 * Start parsing at position in an iterator. Find the end, collect the results.
 * @param iter An iterator of a list of tokens, starting at a position to parse.
 */
abstract class AbstractParser(iter: ListIterator<Token>) {

    val indexStart: Int

    init {
        while (iter.hasNext() && peekToken(iter)?.identifier == TokenGrammar.EMPTY_LINE) {
            iter.next() //skip over beginning empty lines
        }
        require(iter.hasNext()) { "Iterator is empty" }
        indexStart = iter.nextIndex()
    }

    /**
     * Do something with the tokens.
     */
    abstract fun parseTokens(tokens: List<Token>)

    /**
     * Determine end of token sequence, collect tokens from here to there.
     */
    abstract fun scanTokens(iter: ListIterator<Token>): List<Token>

    protected fun scanDocTokens(iter: ListIterator<Token>): List<Token> {
        val tokens = mutableListOf<Token>()
        var token: Token

        //ignore any empty lines that start the doc block (if called after doc_start)
        while (peekPreviousToken(iter)?.identifier == TokenGrammar.EMPTY_LINE) iter.previous()

        //queue up doc_start if called after
        if (peekPreviousToken(iter)?.identifier == TokenGrammar.DOC_START) iter.previous()

        if (peekToken(iter)!!.identifier != TokenGrammar.DOC_START)
            throw ParseException("Doc comment blocks must begin with ${TokenGrammar.DOC_START.value}", this.indexStart)

        tokens.add(iter.next()) //doc_start

        while (iter.hasNext()) {
            token = iter.next()
            tokens.add(token)
            if (token.identifier == TokenGrammar.DOC_END) {
                break
            } else if (token.identifier == TokenGrammar.DOC_START) {
                throw ParseException("Nested doc comments not allowed", this.indexStart)
            }
        }

        if (peekPreviousToken(iter)?.identifier != TokenGrammar.DOC_END) {
            throw ParseException("Unable to find doc comment end", this.indexStart)
        }
        return tokens
    }

    /**
     * Collect annotations (optional) to end of declaration (code body), may be nested.
     */
    protected fun scanDeclarationTokens(iter: ListIterator<Token>): List<Token> {
        val tokens = mutableListOf<Token>()
        var token: Token
        var nestLevel = 0
        var inDoc = false

        while (iter.hasNext()) {
            token = iter.next()
            tokens.add(token)

            if (token.identifier == TokenGrammar.DOC_START) {
                inDoc = true
            } else if (token.identifier == TokenGrammar.DOC_END) {
                inDoc = false
            }

            if (inDoc) {
                continue
            } else if (token.identifier == TokenGrammar.BRACE_OPEN) {
                nestLevel++
            } else if (token.identifier == TokenGrammar.BRACE_CLOSE) {
                nestLevel--
            } else if (token.identifier == TokenGrammar.SEMICOLON && nestLevel == 0) {
                break
            }
        }
        assert(tokens.last().identifier == TokenGrammar.SEMICOLON)
        return tokens
    }

    fun resetIterator(iter: ListIterator<Token>) {
        while (iter.hasPrevious() && iter.previousIndex() >= indexStart) {
            iter.previous()
        }
        assert(iter.nextIndex() == this.indexStart)
    }
}