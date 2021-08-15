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
import parser.elements.declarations.AbstractDeclarationParser
import writer.formatTextasHTML
import writer.htmlEscape

abstract class AbstractElement(parser: EntryParser) {
    protected val docParser = parser.docParser
    protected val annotationParsers = parser.annotationParsers

    abstract protected val declarationParser: AbstractDeclarationParser //specify in subclass
    abstract fun declaration(): String
    abstract fun detailsRows(): String

    open fun toHTML(): String {
        return """
<div>
  <h3>${declarationParser.name}</h3>
  <pre class="prettyprint devsite-disable-click-to-copy">${declaration()}</pre>
  <div>
    ${formatTextasHTML(docParser.description)}
  </div>
  <table class="details responsive">
    <thead>
      <th colspan="2">Details</th>
    </thead>
    <tbody>
      ${detailsRows()}
      ${annotationRows()}
    </tbody>
  </table>
</div>
""".trim()
    }

    private fun annotationRows(): String {
        val sb = StringBuilder()
        if (annotationParsers.isNotEmpty()) {
            sb.append("""
            <tr>
              <td>Annotations</td>
              <td>
                <table class="function param responsive">
                  <tbody>""")
            //AnnotationParser => name:TokenGrammar, value:String
            annotationParsers.forEach { arg ->
                sb.append("""
                <tr>
                  <td>
                    <code>${arg.name.value}</code>
                  </td>
                  <td>
                    <div>${htmlEscape(arg.value)}</div>
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