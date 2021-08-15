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
import parser.elements.DocParser
import parser.peekToken
import writer.tokenValues
import java.text.ParseException

class EnumDeclarationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractDeclarationParser(iter) {

    lateinit override var name: String
    lateinit var type: String
    var members = mutableListOf<EnumMember>()

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()
        var token = iter.next()
        assert(token.identifier == TokenGrammar.ENUM)
        assert(tokens.last().identifier == TokenGrammar.SEMICOLON)

        //name
        token = iter.next()
        if (token.category != TokenCategory.Word)
            throw ParseException("Invalid enum name: ${tokenValues(tokens)}", this.indexStart)
        this.name = token.value

        token = iter.next() //':'
        if (token.identifier != TokenGrammar.COLON)
            throw ParseException("Invalid enum type syntax: ${tokenValues(tokens)}", this.indexStart)

        //type: can be type name or package
        assert(iter.hasNext())
        val sb = StringBuilder()
        while (iter.hasNext() && peekToken(iter)!!.identifier != TokenGrammar.BRACE_OPEN) {
            sb.append(iter.next().value)
        }
        this.type = sb.toString()

        //members
        //convert iterator sequence of comma separated tokens to list of lists
        scanDelimitedList(iter, openDelimiter = TokenGrammar.BRACE_OPEN, closeDelimiter = TokenGrammar.BRACE_CLOSE)
                .forEach {
                    var statementTokens = it.toMutableList()
                    var docParser: DocParser? = null
                    assert(statementTokens.isNotEmpty())

                    //If doc, extract doc tokens and parse, and remove from statement tokens
                    if (statementTokens.first().identifier == TokenGrammar.DOC_START) {
                        val idx = statementTokens.indexOfFirst { it.identifier == TokenGrammar.DOC_END }
                        if (idx == -1) throw ParseException("Unable to find doc_end", this.indexStart)

                        val docTokens = statementTokens.subList(0, idx+1)
                        docParser = DocParser(docTokens.listIterator())
                        statementTokens = statementTokens.subList(idx+1, statementTokens.size)
                    }
                    if (statementTokens.isEmpty())
                        throw ParseException("Invalid member in enum: ${tokenValues(tokens)}", this.indexStart)
                    val member = EnumMember(statementTokens)
                    member.docParser = docParser
                    this.members.add(member)
                }
    }
}

//split member: name [= value]
class EnumMember(tokens: List<Token>) {
    val name: String
    var value: String? = null
    var docParser: DocParser? = null

    init {
        assert(tokens.isNotEmpty())
        this.name = tokens.first().value

        //check for assignment, take right side
        if (tokens.any { it.identifier == TokenGrammar.EQUAL }) {
            this.value = tokens.takeLastWhile { it.identifier != TokenGrammar.EQUAL }
                    .map { it.value }
                    .joinToString(" ")
        }
    }
}