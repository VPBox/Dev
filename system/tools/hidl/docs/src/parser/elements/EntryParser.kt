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
import lexer.TokenCategory
import lexer.TokenGrammar
import parser.config
import parser.elements.declarations.*
import parser.peekPreviousToken
import parser.peekToken
import writer.tokenValues
import writer.warn
import java.text.ParseException

//an entry consists of: doc block, annotationParsers, declarationParser (code sig)
//EntryParser contains:
//- docParser
//  * description
//  * docAnnotationParsers
//    - tag (token)
//    - arg?
//    - description
//- annotationParsers
//  * name
//  * value
//- declarationParser
//  * name
//  * members [CompoundDeclarationParser]: type, name, tokens, typedef [CompoundMemberDeclaration]
//  * members [EnumDeclarationParser]: name, value?
//  * type [EnumDeclarationParser]
//  * type [TypedefDeclarationParser]
//  * extendsName [InterfaceDeclarationParser]
//  * extendsVersion [InterfaceDeclarationParser]
//  * prefix [MethodDeclarationParser]
//  * params [MethodDeclarationParser]: ArgEntry: type, name
//  * returns [[MethodDeclarationParser]: ArgEntry: type, name

class IllegalEntryException(msg: String? = null, cause: Throwable? = null) : Exception(msg, cause)

/**
 * An entry inclused the doc comment block, code docAnnotationParsers, and method signature/interface
 */
class EntryParser(iter: ListIterator<Token>, var shouldResetIterator: Boolean = false) : AbstractParser(iter) {

    //doc description, summary, and doc annottions
    lateinit var docParser: DocParser
    //annotation
    val annotationParsers = mutableListOf<AnnotationParser>()
    //declaration - properties depend on declarationParser subclass
    lateinit var declarationParser: AbstractDeclarationParser

    init {
        parseTokens(scanTokens(iter))
        if (shouldResetIterator) resetIterator(iter)
    }

    //start at doc_start, collect until end of declaration
    override fun scanTokens(iter: ListIterator<Token>): List<Token> {
        //queue up doc_start
        //ignore any empty lines that start the doc block
        while (peekPreviousToken(iter)?.identifier == TokenGrammar.EMPTY_LINE) iter.previous()
        //if called after the doc_start was found
        if (peekPreviousToken(iter)?.identifier == TokenGrammar.DOC_START) iter.previous()

        val tokens = mutableListOf<Token>()
        //collect doc block /** ... */
        tokens += scanDocTokens(iter)
        //collect annotations and declaration, nested to semicolon
        tokens += scanDeclarationTokens(iter)
        return tokens
    }

    override fun parseTokens(tokens: List<Token>) {
        val iter = tokens.listIterator()

        /*
         * doc comment block
         */
        do {
            assert(peekToken(iter)!!.identifier == TokenGrammar.DOC_START)
            this.docParser = DocParser(iter) //increments iterator
            assert(peekPreviousToken(iter)!!.identifier == TokenGrammar.DOC_END)

            //if there's consecutive doc blocks, use the last one found in warning mode, otherwise error
            if (peekToken(iter)?.identifier != TokenGrammar.DOC_START) {
                break //good to go
            } else {
                val msg = "Found consecutive doc block after: ${this.docParser.description}"
                if (config.warnOnly) {
                    warn("${msg}\nUsing last found doc block.")
                } else {
                    throw ParseException(msg, this.indexStart)
                }
            }
        } while (true)

        /*
         * annotations (optional)
         */
        while (iter.hasNext() && peekToken(iter)!!.identifier == TokenGrammar.AT) {
            iter.next()
            if (peekToken(iter)?.category == TokenCategory.Annotation) {
                this.annotationParsers.add(AnnotationParser(iter)) //increments iterator
            } else {
                throw ParseException("Unknown annotation tag: ${peekToken(iter)?.value}", this.indexStart)
            }
        }

        /*
         * declaration
         */
        val token = peekToken(iter) ?: throw ParseException("No declaration body available", this.indexStart)

        //check we're not at an annotation
        assert(token.identifier != TokenGrammar.AT && token.category != TokenCategory.Annotation)

        /*
         * known bad starts for a declaration
         */

        if (token.identifier == TokenGrammar.DOC_START) {
            throw ParseException("Bad doc block location:\n${tokenValues(tokens)}", this.indexStart)
        } else if (token.identifier == TokenGrammar.PACKAGE) {
            //usually this means they've used a doc block for the license
            throw IllegalEntryException("Don't document the package declaration") //handled in EntryCollectionParser
        } else if (token.category != TokenCategory.Word
                && token.category != TokenCategory.TypeDef
                && token.category != TokenCategory.Keyword) {
            //sanity check - skip entry or bail
            throw IllegalEntryException("Invalid start for entry declaration: '${token.value}'\n" +
                    "tokens: ${tokenValues(tokens)}")
            //throw ParseException("Invalid start for entry declaration: ${token}\ntoken: ${token.value}\n${tokenValues(tokens)}", this.indexStart)
        }

        this.declarationParser = when (token.identifier) {
            TokenGrammar.INTERFACE -> {
                this.shouldResetIterator = true
                InterfaceDeclarationParser(iter)
            }
            TokenGrammar.ENUM -> EnumDeclarationParser(iter)
            TokenGrammar.TYPEDEF -> TypedefDeclarationParser(iter)
            TokenGrammar.STRUCT, TokenGrammar.UNION -> CompoundDeclarationParser(iter)
            else -> MethodDeclarationParser(iter)
        }

    }
}