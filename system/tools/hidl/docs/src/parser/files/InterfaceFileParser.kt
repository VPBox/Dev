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
import parser.elements.AnnotationParser
import parser.elements.EntryParser
import parser.elements.declarations.InterfaceDeclarationParser
import parser.elements.declarations.MethodDeclarationParser

class InterfaceFileParser(tokens: List<Token>) : AbstractFileParser(tokens) {

    private val interfaceEntry: EntryParser by lazy {
        //returns first matching element (i don't think there can be multiple interface declarations)
        this.entries.find { it.declarationParser is InterfaceDeclarationParser }
                ?: throw ClassNotFoundException("Unable to find an InterfaceDeclarationParser")
    }
    //need this for the cast
    private val interfaceDecl: InterfaceDeclarationParser by lazy {
        interfaceEntry.declarationParser as InterfaceDeclarationParser
    }

    /* top-level properties on the interface itself, ie. not its entries */

    override val name: String get() = interfaceDecl.name
    val extendsName: String? get() = interfaceDecl.extendsName
    val extendsVersion: Float? get() = interfaceDecl.extendsVersion

    //doc
    val description get() = interfaceEntry.docParser.description
    val docAnnotations get() = interfaceEntry.docParser.docAnnotationParsers //tag, arg?, description

    val annotations: List<AnnotationParser> get() = interfaceEntry.annotationParsers //name, value
    val methods: List<EntryParser> by lazy { getEntriesByDeclarationParser<MethodDeclarationParser>() }

    companion object {
        /**
         * Searches tokens for an interface identifier.
         * Maybe not the most accurate measurement, but good enough.
         */
        fun isInterface(tokens: List<Token>): Boolean {
            val iter = tokens.listIterator()
            var token: Token
            var inDoc = false

            while (iter.hasNext()) {
                token = iter.next()
                if (token.identifier == TokenGrammar.DOC_START) {
                    inDoc = true
                    continue
                } else if (token.identifier == TokenGrammar.DOC_END) {
                    inDoc = false
                    continue

                } else if (!inDoc && token.identifier == TokenGrammar.INTERFACE) {
                    if (iter.next().category != TokenCategory.Word) break //no, try again

                    return true
                }
            }
            return false
        }

    }
}