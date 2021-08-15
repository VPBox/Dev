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
import parser.elements.declarations.CompoundDeclarationParser
import parser.elements.declarations.CompoundMemberDeclaration
import writer.formatTextasHTML

// used for structs and unions
class CompoundElement(parser: EntryParser): AbstractElement(parser) {
    //type, name, members [IMemberDeclaration]
    override val declarationParser = parser.declarationParser as CompoundDeclarationParser

    override fun declaration(): String {
        val sb = StringBuilder()
        sb.append("${declarationParser.type.value} ${declarationParser.name} {")
        declarationParser.members.forEachIndexed { i, arg ->
            val typedef = if (arg is CompoundMemberDeclaration) "${arg.typeDef.value} " else ""
            sb.append("${typedef}${arg.type} ${arg.name}")
            if (i < declarationParser.members.size-1) sb.append("; ")
        }
        sb.append("}")
        return sb.toString()
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
            //type, name, tokens, typedef
            declarationParser.members.forEach { arg ->
                val fieldDesc = arg.docParser?.description?.let { formatTextasHTML(it, useParagraphs = false) } ?: ""
                sb.append("""
                <tr>
                  <td>
                    <code>${arg.name}</code>
                  </td>
                  <td>
                    <div>${fieldDesc}</div>
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