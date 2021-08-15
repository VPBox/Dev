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
import writer.tokenValues
import java.text.ParseException

/**
 * Used for Structs and Unions
 */
class CompoundDeclarationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractDeclarationParser(iter) {
    lateinit var type: TokenGrammar
    lateinit override var name: String
    var members = mutableListOf<IMemberDeclaration>() //defined below

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()
        var token = iter.next()
        assert(token.identifier == TokenGrammar.STRUCT || token.identifier == TokenGrammar.UNION)
        assert(tokens.last().identifier == TokenGrammar.SEMICOLON)

        //type - struct or union
        this.type = token.identifier

        //name
        token = iter.next()
        if (token.category != TokenCategory.Word)
            throw ParseException("Invalid struct name: ${tokenValues(tokens)}", this.indexStart)
        this.name = token.value

        //parse each semicolon-delimited statement
        scanDelimitedList(iter,
                delimiter = TokenGrammar.SEMICOLON,
                openDelimiter = TokenGrammar.BRACE_OPEN,
                closeDelimiter = TokenGrammar.BRACE_CLOSE)
                .forEach {
                    var docParser: DocParser? = null
                    var statementTokens = it.toMutableList()
                    if (statementTokens.isEmpty())
                        throw ParseException("Invalid statement in: ${tokenValues(tokens)}", this.indexStart)

                    //If doc, extract doc tokens and parse, and remove from statement tokens
                    if (statementTokens.first().identifier == TokenGrammar.DOC_START) {
                        val idx = statementTokens.indexOfFirst { it.identifier == TokenGrammar.DOC_END }
                        if (idx == -1) throw ParseException("Unable to find doc_end", this.indexStart)
                        val docTokens = statementTokens.subList(0, idx+1)
                        docParser = DocParser(docTokens.listIterator())
                        statementTokens = statementTokens.subList(idx+1, statementTokens.size)
                    }

                    if (statementTokens.isEmpty())
                        throw ParseException("Invalid statement in: ${tokenValues(tokens)}", this.indexStart)

                    when(statementTokens.first().identifier) {
                        TokenGrammar.STRUCT, TokenGrammar.UNION -> {
                            assert(statementTokens.first().category == TokenCategory.TypeDef)
                            this.members.add(CompoundMemberDeclaration(
                                    typeDef = statementTokens.first().identifier,
                                    type = statementTokens.get(1).value,
                                    name = statementTokens.last().value,
                                    docParser = docParser,
                                    tokens = statementTokens.subList(2, statementTokens.size-1)
                            ))
                        }
                        TokenGrammar.ENUM -> {
                            assert(statementTokens.size > 1)
                            this.members.add(MemberDeclaration(
                                    type = statementTokens.first().value,
                                    name = statementTokens.get(1).value,
                                    docParser = docParser,
                                    tokens = statementTokens
                            ))
                        }
                        else -> {
                            this.members.add(MemberDeclaration(
                                    type = statementTokens.first().value,
                                    name = statementTokens.last().value,
                                    docParser = docParser,
                                    tokens = statementTokens
                            ))
                        }
                    }
                }
    }
}

interface IMemberDeclaration {
    val type: String
    val name: String
    val docParser: DocParser?
    val tokens: List<Token> //TODO: doesn't seem needed
}

class MemberDeclaration(override val type: String,
                        override val name: String,
                        override val docParser: DocParser?,
                        override val tokens: List<Token>) : IMemberDeclaration

class CompoundMemberDeclaration(override val type: String,
                                override val name: String,
                                override val docParser: DocParser?,
                                override val tokens: List<Token>,
                                val typeDef: TokenGrammar) : IMemberDeclaration
