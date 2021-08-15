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

package lexer

import java.text.ParseException
import java.util.*

object HidlLexer : ILexer {

    /**
     * Given the text of a HIDL file, return a list ot tokens.
     * Scanner moves forward only, but can check queued tokens.
     */
    override fun tokenize(str: String): List<Token> {
        val tokens = mutableListOf<Token>()
        //match line to '*/', check for anything after on same line
        val matchToDocEnd = Regex("""(.*)\s+${Regex.escape(TokenGrammar.DOC_END.value)}(.*)$""")

        //pad delimiter tokens so the scanner picks them up.
        val paddedStr = ILexer.padDelimiters(str)

        Scanner(paddedStr).use { scanner ->
            while (scanner.hasNext()) {
                val token = scanner.next()

                when (token) {
                    //strip line comments
                    TokenGrammar.COMMENT_LINE.value -> scanner.nextLine()

                    //strip block comments -- jump to comment close
                    TokenGrammar.COMMENT_START.value -> {
                        if (scanner.findWithinHorizon(Regex.escape(TokenGrammar.DOC_END.value), 0) == null) {
                            throw ParseException("Unable to find closing comment marker", tokens.lastIndex)
                        }
                    }

                    //slurp text between /** and */ into a string,
                    //tokenize string using the doc comment lexer,
                    //append those tokens with the rest of the file tokens.
                    TokenGrammar.DOC_START.value -> {
                        tokens.add(TokenGrammar.newToken(token)) //doc_start
                        //slurp everything until doc_end into a string. but want to keep newline formatting
                        val sb = StringBuilder()
                        while (scanner.hasNextLine()) {
                            val line = scanner.nextLine()

                            val matches = matchToDocEnd.find(line)?.groups
                            if (matches != null) {
                                if (!matches[2]!!.value.isNullOrBlank()) {
                                    throw ParseException("No text after '*/' on same line: ${line}", 0)
                                }
                                //found doc_end
                                sb.append(matches[1]!!.value)
                                break
                            } else {
                                sb.appendln(line)
                            }
                        }
                        //tokenize comment string and append all
                        tokens += DocLexer.tokenize(sb.toString())
                        tokens.add(TokenGrammar.newToken(TokenGrammar.DOC_END.value)) //doc_end
                    }

                    TokenGrammar.AT.value -> {
                        tokens.add(TokenGrammar.newToken(token)) //'@'
                        //determine if part of annotation tag
                        for (annotation in TokenGrammar.annotations()) {
                            if (scanner.hasNext(annotation.value)) {
                                scanner.next() //annotation tag
                                val annotationArgs = StringBuilder()
                                //capture any args: (...)
                                if (scanner.hasNext(Regex.escape(TokenGrammar.PAREN_OPEN.value))) {
                                    while (!scanner.hasNext(Regex.escape(TokenGrammar.PAREN_CLOSE.value))) {
                                        annotationArgs.append(scanner.next()).append(" ")
                                    }
                                    if (!scanner.hasNext()) {
                                        throw ParseException("Unable to find closing annotation paren", tokens.lastIndex)
                                    }
                                    annotationArgs.append(scanner.next()) //')'
                                }
                                //all annotation args are embedded in the token's value
                                tokens.add(TokenGrammar.newToken(identifier = annotation, value = annotationArgs.toString()))
                            }
                        }
                    }

                    else -> tokens.add(TokenGrammar.newToken(token))
                }
            }
        }
        return tokens.toList()
    }
}