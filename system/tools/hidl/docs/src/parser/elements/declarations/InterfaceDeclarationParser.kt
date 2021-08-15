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

class InterfaceDeclarationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractDeclarationParser(iter) {

    lateinit override var name: String
    var extendsName: String? = null
    var extendsVersion: Float? = null

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    // example format: interface ITunerCallback extends @1.0::ITunerCallback
    override fun parseTokens(tokens: List<Token>) {
        assert(tokens.isNotEmpty())
        assert(tokens.first().identifier == TokenGrammar.INTERFACE)
        assert(tokens.last().identifier == TokenGrammar.SEMICOLON)

        //grab first line of declarationParser
        val sigToks = tokens.takeWhile { it.identifier != TokenGrammar.BRACE_OPEN }
        assert(sigToks[1].category == TokenCategory.Word)
        assert(sigToks.last().category == TokenCategory.Word) //either interface name or extends name

        this.name = sigToks[1].value

        //parse extends info (if exists)
        if (sigToks.any { it.identifier == TokenGrammar.EXTENDS }) {
            this.extendsName = sigToks.last().value
            this.extendsVersion = sigToks.find { it.category == TokenCategory.Number }?.value?.toFloat()
        }
    }
}