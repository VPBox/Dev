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

import java.io.File

interface ILexer {
    fun tokenize(str: String): List<Token>

    fun tokenize(file: File): List<Token> {
        return this.tokenize(file.readText())
    }

    companion object {

        fun padDelimiters(str: String): String {
            val sb = StringBuilder()
            val delimiters = TokenGrammar.values()
                    .filter { it.category == TokenCategory.Delimiter }
                    .filter { it != TokenGrammar.COMMENT_START } //don't convert '/**' to '/* *'
                    .map { it.value } //return string representation

            str.lineSequence().forEach { line ->
                var newLine = line
                for (token in delimiters) {
                    newLine = newLine.replace(token, " $token ")
                }
                //delimiter corrections
                newLine = unpadDecimal(newLine) //'nn . nn' => 'n.n'
                newLine = newLine.replace(":\\s+:".toRegex(), TokenGrammar.PKG_SCOPE.value)  //': :' => '::'
                //squeeze multi-char ops with chevrons
                newLine = newLine.replace("<\\s+<".toRegex(), TokenGrammar.LSHIFT.value)
                newLine = newLine.replace(">\\s+>".toRegex(), TokenGrammar.RSHIFT.value)
                newLine = newLine.replace("<\\s+=".toRegex(), TokenGrammar.LEQ.value)
                newLine = newLine.replace(">\\s+=".toRegex(), TokenGrammar.GEQ.value)

                sb.appendln(newLine)
            }
            return sb.toString()
        }

        /**
         * Replace 'nn . nn' with 'n.n'
         * Doesn't take into account decimals with missing a prefix or suffix, e.g. '9.' or '.9'
         */
        private fun unpadDecimal(str: String): String {
            var newStr = str
            Regex("(\\d+)\\s*\\.\\s*(\\d+)").findAll(newStr).forEach { matchResult ->
                val n1 = matchResult.groups[1]?.value
                val n2 = matchResult.groups[2]?.value
                if (n1 != null && n2 != null) {
                    newStr = newStr.replace("${n1}\\s*\\.\\s*${n2}".toRegex(), "${n1}.${n2}")
                }
            }
            return newStr
        }

        /**
         * Clean up the padded and tokenized doc block (reverse padDelimiters)
         */
        fun unpadDelimiters(str: String): String {
            var newStr = str
            val delimiters = TokenGrammar.values()
                    .filter { it.category == TokenCategory.Delimiter }
                    .map { it.value } //return string representation

            for (token in delimiters) {
                newStr = newStr.replace(" $token ", token)
            }
            //special case
            newStr = newStr.replace(Regex("\\s+\\.\\s*$"), ".") //end-of-line sentence periods
            newStr = newStr.replace(",", ", ") //give comma some breathing room
            return newStr
        }

    }
}