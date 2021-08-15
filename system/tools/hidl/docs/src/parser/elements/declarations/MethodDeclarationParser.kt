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
import writer.tokenValues
import java.text.ParseException

data class ArgEntry(val type: String, val name: String)

class MethodDeclarationParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractDeclarationParser(iter) {
    lateinit override var name: String

    var prefix: String? = null
    var params = mutableListOf<ArgEntry>()
    var returns = mutableListOf<ArgEntry>()

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    // [prefix] <name> ( [typedef] [name], ...) [generates] ( [typedef], [name], ...);
    override fun parseTokens(tokens: List<Token>) {
        assert(tokens.last().identifier == TokenGrammar.SEMICOLON)
        val iter = tokens.listIterator()
        var token = iter.next()

        //grab prefix (typedef or keyword), if there
        if (token.category == TokenCategory.TypeDef || token.category == TokenCategory.Keyword) {
            this.prefix = token.value
            token = iter.next()
        }

        //name
        if (token.category != TokenCategory.Word) {
            throw ParseException("Invalid declarationParser name: '${token.value}'\nBad Tokens: ${tokenValues(tokens)}",
                    this.indexStart)
        }
        this.name = token.value

        assert(iter.hasNext())
        token = iter.next()

        //parse arg list
        if (token.identifier == TokenGrammar.PAREN_OPEN) {
            addArgs(iter, this.params)
            assert(iter.hasNext())
            token = iter.next()
        }

        //parse return list
        if (token.identifier == TokenGrammar.GENERATES) {
            addArgs(iter, this.returns)
            assert(iter.hasNext())
            token = iter.next()
        }

        assert(token.identifier == TokenGrammar.SEMICOLON)
    }

    /**
     * Arguments are provided in the form: (type1 name1, type2 name2, ...)
     */
    private fun addArgs(iter: ListIterator<Token>, list: MutableList<ArgEntry>) {
        val argLists = this.scanDelimitedList(iter) //return list of list<token>
        for (argList in argLists) {
            list.add(formatArgEntry(argList)) //use tokens to create ArgEntry
        }
    }

    private fun formatArgEntry(paramTokens: List<Token>): ArgEntry {
        val type = paramTokens.takeWhile { it != paramTokens.last() } //but-last
                .map { it.value }
                .joinToString("")
        val name = paramTokens.last().value
        return ArgEntry(type, name)
    }
}