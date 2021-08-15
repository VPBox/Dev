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
import lexer.TokenGrammar

class TypedefDeclarationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractDeclarationParser(iter) {
    lateinit override var name: String //synonym
    lateinit var type: String          //type declarationParser

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    override fun parseTokens(tokens: List<Token>) {
        assert(tokens.isNotEmpty())
        assert(tokens.first().identifier == TokenGrammar.TYPEDEF)
        assert(tokens.last().identifier == TokenGrammar.SEMICOLON)

        this.name = tokens.get(tokens.size - 2).value
        this.type = tokens.subList(1, tokens.size - 2).map { it.value }.joinToString("")
    }
}