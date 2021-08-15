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

package writer.elements

import lexer.TokenGrammar
import parser.config
import parser.elements.EntryParser
import parser.elements.declarations.ArgEntry
import parser.elements.declarations.MethodDeclarationParser
import writer.formatTextasHTML
import writer.htmlEscape
import writer.warn
import java.text.ParseException

class MethodElement(parser: EntryParser) : AbstractElement(parser) {
    override val declarationParser = parser.declarationParser as MethodDeclarationParser

    //prefix, params, returns
    override fun declaration(): String {
        val sb = StringBuilder()
        declarationParser.prefix?.let { sb.append("${it} ") }
        sb.append("${declarationParser.name} (")
        declarationParser.params.forEachIndexed { i, arg ->
            sb.append(htmlEscape("${arg.type} ${arg.name}"))
            if (i < declarationParser.params.size-1) sb.append(", ")
        }
        sb.append(")")
        if (declarationParser.returns.isNotEmpty()) {
            sb.append("\ngenerates (")
            declarationParser.returns.forEachIndexed { i, arg ->
                sb.append(htmlEscape("${arg.type} ${arg.name}"))
                if (i < declarationParser.returns.size-1) sb.append(", ")
            }
            sb.append(")")
        }
        return sb.toString()
    }

    override fun detailsRows(): String {
        return """
${buildPrefixRows()}
${buildParamRows()}
${buildReturnRows()}
""".trim()
    }

    private fun getDocAnnotationDesc(tag: TokenGrammar, arg: ArgEntry): String {
        return docParser.docAnnotationParsers
                .filter { it.tag == tag }
                .firstOrNull { it.arg == arg.name }
                ?.let { formatTextasHTML(it.description, useParagraphs = false) }
                ?: run {
            val msg = "Missing @${tag.value} doc annotation for '${arg.type} ${arg.name}'"
            if (config.warnOnly) {
                warn(msg)
                "" //return empty string if it can't find it
            } else {
                throw ParseException(msg, 0)
            }
        }
    }

    private fun buildPrefixRows(): String {
        val sb = StringBuilder()
        if (declarationParser.prefix != null) {
            sb.append("""
            <tr>
              <td>RPC mode</td>
              <td>
                <table class="function param responsive">
                  <tbody>
                    <tr>
                      <td>
                        <code>${declarationParser.prefix}</code>
                      </td>
                    </tr>
                  </tbody>
                </table>
              </td>
            </tr>""")
        }
        return sb.toString()
    }

    private fun buildParamRows(): String {
        //docParser.docAnnotationParsers //=> tag [TokenGrammer], arg?, description
        val sb = StringBuilder()
        if (declarationParser.params.isNotEmpty()) {
            sb.append("""
            <tr>
              <td>Parameters</td>
              <td>
                <table class="function param responsive">
                  <tbody>""")
            declarationParser.params.forEach { arg ->
                sb.append("""
                <tr>
                  <td>
                    <code>${htmlEscape(arg.name)}</code>
                  </td>
                  <td>
                    <div>${getDocAnnotationDesc(TokenGrammar.PARAM, arg)}</div>
                  </td>
                </tr>""")
            }
            sb.append("""
                  </tbody>
                </table>
              </td>
            </tr>""")
        }
        return sb.toString()
    }

    private fun buildReturnRows(): String {
        val sb = StringBuilder()
        if (declarationParser.returns.isNotEmpty()) {
            sb.append("""
            <tr>
              <td>Generates</td>
              <td>
                <table class="function param responsive">
                  <tbody>""")
            declarationParser.returns.forEach { arg ->
                sb.append("""
                <tr>
                  <td>
                    <code>${htmlEscape(arg.name)}</code>
                  </td>
                  <td>
                    <div>${getDocAnnotationDesc(TokenGrammar.RETURN, arg)}</div>
                  </td>
                </tr>""")
            }
            sb.append("""
                  </tbody>
                </table>
              </td>
            </tr>""")
        }
        return sb.toString()
    }
}