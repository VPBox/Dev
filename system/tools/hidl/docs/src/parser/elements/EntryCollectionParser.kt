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
import parser.config
import writer.warn
import java.text.ParseException

/**
 * Collection of doc entryParsers in a token stream.
 * Typically, like entries are grouped together in a file (eg. Interface def),
 * But could be useful for declarations with fields (enums, structs).
 */
class EntryCollectionParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractParser(iter) {

    var entryParsers = mutableListOf<EntryParser>()

    constructor(tokens: List<Token>) : this(tokens.listIterator())

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) this.resetIterator(iter)
    }

    //use all the tokens
    override fun scanTokens(iter: ListIterator<Token>): List<Token> {
        val tokens = mutableListOf<Token>()
        while (iter.hasNext()) {
            tokens.add(iter.next())
        }
        return tokens
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()
        var token: Token

        //find entry parsers - must start with doc_start
        while(iter.hasNext()) {
            token = iter.next()

            if (token.identifier == TokenGrammar.DOC_START) {
                try {
                    entryParsers.add(EntryParser(iter)) //advances iterator past declaration
                } catch (ex: IllegalEntryException) {
                    if (config.warnOnly) {
                        //bail on current entry but continue
                        warn("${ex.message}, skipping entry")
                    } else {
                        throw ParseException(ex.message, this.indexStart)
                    }
                }
            }
        }
    }
}