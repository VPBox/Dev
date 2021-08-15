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
import java.text.ParseException

data class PackageInfo(val name: String, val version: Float)

/**
 * Find and parse package info. Throw error if it can't find a valid declarationParser format.
 * Example format: package android.hardware.audio@2.0;
 */
fun parsePackageInfo(tokens: List<Token>): PackageInfo {
    val iter: ListIterator<Token> = tokens.listIterator()
    var token: Token

    while (iter.hasNext()) {
        token = iter.next()

        if (token.identifier == TokenGrammar.PACKAGE) {
            //collect namespace
            val pkgNameToks = mutableListOf<Token>()
            while (iter.hasNext()) {
                token = iter.next()
                if (token.identifier != TokenGrammar.AT && token.identifier != TokenGrammar.SEMICOLON) {
                    pkgNameToks.add(token)
                } else {
                    break
                }
            }
            val pkgName = pkgNameToks.map { it.value }.joinToString("")

            //step through format and test syntax
            if (token.identifier != TokenGrammar.AT) break
            token = iter.next()
            if (token.category != TokenCategory.Number) break //version
            val pkgVer = token.value.toFloat()
            token = iter.next()
            if (token.identifier != TokenGrammar.SEMICOLON) break

            //hooray, a proper package format
            return PackageInfo(pkgName, pkgVer)
        }
    }
    throw ParseException("Unable to find a valid package declaration", 0)
}