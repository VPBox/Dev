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

package parser.files

import lexer.Token
import lexer.TokenCategory
import lexer.TokenGrammar
import parser.elements.EntryCollectionParser
import parser.elements.EntryParser
import parser.elements.declarations.CompoundDeclarationParser
import parser.elements.declarations.EnumDeclarationParser
import parser.elements.declarations.TypedefDeclarationParser
import parser.peekPreviousToken
import parser.peekToken
import java.text.ParseException

/**
 * Parses package info and all entries (determined by doc start/end tokens).
 * Adds empty doc tokens for required types.
 * This class shouldn't be instantiated on it's own.
 */
abstract class AbstractFileParser(tokens: List<Token>) {

    private val packageInfo: PackageInfo by lazy { parsePackageInfo(tokens) }

    abstract val name: String
    val packageName: String get() = packageInfo.name
    val packageVersion: Float get() = packageInfo.version

    protected val entries: List<EntryParser> by lazy {
        assert(tokens.isNotEmpty())
        //add empty docblocks
        EntryCollectionParser(insertDocsForRequiredTypes(tokens)).entryParsers
    }

    val enums: List<EntryParser> by lazy { getEntriesByDeclarationParser<EnumDeclarationParser>() }
    val typedefs: List<EntryParser> by lazy { getEntriesByDeclarationParser<TypedefDeclarationParser>() }
    val structs: List<EntryParser> by lazy { getEntriesByCompoundDeclarationParser(TokenGrammar.STRUCT) }
    val unions: List<EntryParser> by lazy { getEntriesByCompoundDeclarationParser(TokenGrammar.UNION) }

    protected inline fun <reified T> getEntriesByDeclarationParser(): List<EntryParser> {
        return entries.filter { it.declarationParser is T }
    }

    private fun getEntriesByCompoundDeclarationParser(identifier: TokenGrammar): List<EntryParser> {
        return getEntriesByDeclarationParser<CompoundDeclarationParser>()
                .filter { (it.declarationParser as CompoundDeclarationParser).type == identifier }
    }

    private val REQUIRED_DOC_TYPES = listOf(
            TokenGrammar.INTERFACE,
            TokenGrammar.ENUM,
            TokenGrammar.STRUCT,
            TokenGrammar.UNION,
            TokenGrammar.TYPEDEF)

    /**
     * Insert doc block before the undocumented types we want to show up.
     */
    private fun insertDocsForRequiredTypes(tokens: List<Token>): List<Token> {
        val tokensCopy = mutableListOf<Token>()
        val iter = tokens.listIterator()
        var token: Token
        var inDoc = false

        while (iter.hasNext()) {
            token = iter.next()
            tokensCopy.add(token)

            if (token.identifier == TokenGrammar.DOC_START) {
                inDoc = true
                continue
            } else if (token.identifier == TokenGrammar.DOC_END) {
                inDoc = false
                continue

            } else if (!inDoc && token.identifier in REQUIRED_DOC_TYPES) {
                //make sure it's not a reference to a Generic: <name>
                if (peekToken(iter)?.identifier == TokenGrammar.CHEVRON_CLOSE) {
                    continue
                }

                val idx = indexInsertionPointforDocTokens(tokensCopy)
                if (idx != -1) {
                    val removedTokens = mutableListOf<Token>()
                    repeat(idx) {
                        removedTokens.add(tokensCopy.removeAt(tokensCopy.size-1))
                    }
                    tokensCopy.add(TokenGrammar.newToken(TokenGrammar.DOC_START.value))
                    tokensCopy.add(TokenGrammar.newToken(TokenGrammar.DOC_END.value))
                    removedTokens.reversed().forEach { tokensCopy.add(it) }
                }
            }
        }
        return tokensCopy.toList()
    }

    /**
     * @return -1 if documented, otherwise the index count backwards for where
     *          to begin insertion of doc tokens.
     */
    private fun indexInsertionPointforDocTokens(tokens: List<Token>): Int {
        val iter = tokens.reversed().listIterator()
        var token: Token
        var idx = 0

        iter.next() //skip keyword token
        while (iter.hasNext()) {
            token = iter.next()
            if (token.identifier == TokenGrammar.AT || token.category == TokenCategory.Annotation) {
                idx++
                continue //skip annotations
            } else {
                return if (token.identifier == TokenGrammar.DOC_END) -1 else idx+1
            }
        }
        throw ParseException("Empty token list", 0)
    }
}