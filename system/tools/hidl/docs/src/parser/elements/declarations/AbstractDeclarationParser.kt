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

package parser.elements.declarations

import lexer.Token
import lexer.TokenCategory
import lexer.TokenGrammar
import parser.elements.AbstractParser
import parser.peekPreviousToken
import parser.peekToken
import java.text.ParseException

//can be a method, struct, enum, typedef
abstract class AbstractDeclarationParser(iter: ListIterator<Token>) : AbstractParser(iter) {

    abstract var name: String //user-specified name declarationParser

    //should be segmented in EntryParser, remaining tokens are the declarationParser, may be nested
    override fun scanTokens(iter: ListIterator<Token>): List<Token> {
        val token = peekPreviousToken(iter) ?: throw ParseException("No token before declaration", this.indexStart)

        if (token.category != TokenCategory.Annotation && token.identifier != TokenGrammar.DOC_END)
            throw ParseException("Invalid declaration start", this.indexStart)

        return scanDeclarationTokens(iter)
    }

    /**
     * Takes a delimited separated list and splits entries into list of list<token>.
     * Ignore nested lists using same open/close delimiter.
     * For method param lists, enum members, struct members, etc.
     */
    protected fun scanDelimitedList(iter: ListIterator<Token>,
                                    delimiter: TokenGrammar = TokenGrammar.COMMA,
                                    openDelimiter: TokenGrammar = TokenGrammar.PAREN_OPEN,
                                    closeDelimiter: TokenGrammar = TokenGrammar.PAREN_CLOSE): List<List<Token>> {
        val allFields = mutableListOf<List<Token>>() //top-level list
        //queue up list open
        if (iter.hasPrevious() && peekPreviousToken(iter)!!.identifier == openDelimiter) {
            iter.previous()
        }
        var token = iter.next()
        if (token.identifier != openDelimiter)
            throw ParseException("Expected list start '${openDelimiter}', but got '${token.identifier}'", this.indexStart)

        // collect tokens between open/close delimiters, fields separated by delimiter.
        // ignore if nested, ignore in doc comment
        while (iter.hasNext()) {
            token = peekToken(iter)!! //iter.next()

            if (token.identifier == closeDelimiter) {
                iter.next()
                break
            } else if (token.identifier == delimiter) {
                iter.next()
                continue //skip
            } else {
                //start field entry
                val fieldTokens = mutableListOf<Token>()
                var inDoc = false
                var nestLevel = 0

                while (iter.hasNext()) {
                    token = iter.next()

                    if (token.identifier == TokenGrammar.DOC_START) {
                        inDoc = true
                    } else if (token.identifier == TokenGrammar.DOC_END) {
                        inDoc = false
                    }

                    //check for end of field
                    if ((token.identifier == delimiter || token.identifier == closeDelimiter) && nestLevel == 0 && !inDoc) {
                        break
                    } else {
                        fieldTokens.add(token)
                    }

                    if (token.identifier == openDelimiter) {
                        nestLevel++
                    } else if (token.identifier == closeDelimiter && nestLevel > 0) {
                        nestLevel--
                    }
                }

                //add entry
                allFields.add(fieldTokens)
                //check for end of list
                if (token.identifier == closeDelimiter && nestLevel == 0) {
                    break
                }
            }
        }

        if (!iter.hasPrevious() || peekPreviousToken(iter)!!.identifier != closeDelimiter) {
            throw ParseException("Didn't find closing '${closeDelimiter.value}' for list, found '${peekPreviousToken(iter)!!.value}'", this.indexStart)
        }
        return allFields
    }
}