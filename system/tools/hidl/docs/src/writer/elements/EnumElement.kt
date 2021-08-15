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

import parser.elements.EntryParser
import parser.elements.declarations.EnumDeclarationParser
import writer.formatTextasHTML
import writer.htmlEscape

class EnumElement(parser: EntryParser) : AbstractElement(parser) {
    //name [String], type [String], members [EnumMember: name, value?]
    override val declarationParser = parser.declarationParser as EnumDeclarationParser

    override fun declaration(): String {
        return "enum ${declarationParser.name}: ${declarationParser.type}"
    }

    override fun detailsRows(): String {
        //build member rows
        val sb = StringBuilder()
        if (declarationParser.members.isNotEmpty()) {
            sb.append("""
            <tr>
              <td>Members</td>
              <td>
                <table class="function param responsive">
                  <tbody>""")
            //EnumMember => name, value?
            declarationParser.members.forEach { arg ->
                val fieldVal = arg.value?.let { " = ${htmlEscape(it)}" } ?: ""
                val fieldDesc = arg.docParser?.description?.let { formatTextasHTML(it, useParagraphs = false) } ?: ""
                sb.append("""
                <tr>
                  <td>
                    <code>${htmlEscape(arg.name)}${fieldVal}</code>
                  </td>
                  <td>
                    <div>$fieldDesc</div>
                  </td>
                </tr>""")
            }
            sb.append("""
                  </tbody>
                </table>
              </td>
            </tr>""")
        }
        return sb.toString().trim()
    }
}