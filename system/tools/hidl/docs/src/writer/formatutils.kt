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

package writer

import lexer.Token
import parser.files.AbstractFileParser
import java.io.File
import java.nio.file.Path
import java.nio.file.Paths

fun warn(msg: String) {
    System.err.println("WARNING: $msg")
}

/**
 * Get values of tokens, useful for debugging.
 */
fun tokenValues(tokens: List<Token>): String {
    return tokens.map { it.value }.joinToString("|")
}

/**
 * Escape string for HTML.
 */
fun htmlEscape(string: String): String {
    val out = StringBuilder(Math.max(16, string.length))
    string.toCharArray().forEach { c ->
        if (c.toInt() > 127 || c == '"' || c == '<' || c == '>' || c == '&' || c == '$' || c == '{' || c == '}') {
            out.append("&#")
            out.append(c.toInt())
            out.append(';')
        } else {
            out.append(c)
        }
    }
    return out.toString()
}

/**
 * Used to display description text.
 */
fun formatTextasHTML(string: String, useParagraphs: Boolean = true): String {
    if (string.isEmpty()) return string

    val sb = StringBuilder()
    if (useParagraphs) sb.append("<p>")
    //match and replace empty lines
    val replaceText = if (useParagraphs) "</p>\n<p>" else "<br>\n"
    sb.append(htmlEscape(string.trim()).replace(Regex("\\s*\n\n\\s*"), replaceText))
    if (useParagraphs) sb.append("</p>")
    return sb.toString()
}

private val summaryRegex = Regex("\\.|\n\n") //match period or empty line

/**
 * Given a block of description text, return the first sentence.
 */
fun getDescSummaryText(string: String): String {
    return if (string.isEmpty()) {
        string
    } else {
        val s = string.trimStart() // remove any beginning empty lines/whitespace
        val sb = StringBuilder(summaryRegex.split(s)[0])
        if (sb[sb.length - 1] != '.') sb.append(".") // add period, if needed
        formatTextasHTML(sb.toString())
    }
}

/**
 * Return the out file path for a given parser.
 */
fun getOutPath(parser: AbstractFileParser, outDir: Path): Path {
    val pkgPath = parser.packageName.replace(".", File.separator)
    val dirPath = "${outDir}${File.separator}${pkgPath}${File.separator}${parser.packageVersion}"
    return Paths.get("${dirPath}${File.separator}${parser.name}.html")
}