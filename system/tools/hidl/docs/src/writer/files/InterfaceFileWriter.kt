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

package writer.files

import parser.files.InterfaceFileParser
import writer.elements.MethodElement
import writer.formatTextasHTML

class InterfaceFileWriter(private val parser: InterfaceFileParser) : AbstractParserFileWriter(parser) {

    override var templateResource = "/resources/template/interface.html"

    override fun replaceVars() {
        super.replaceVars()
        replaceVar("header", buildHeader())

        replaceVar("methodDefs") {
            val sb = StringBuilder()
            if (parser.methods.isNotEmpty()) {
                sb.append("<section>\n")
                sb.append("<h2>Methods</h2>\n")
                sb.append(parser.methods.map { MethodElement(it).toHTML() }.joinToString("\n"))
                sb.append("\n</section>\n")
            }
            sb.toString()
        }
    }

    private fun buildHeader(): String {
        return """
<header>
  <div id="api-info-block">
    Package: <code>${parser.packageName}@${parser.packageVersion}</code>
  </div>
  <h1>${parser.name}</h1>
  <code>${buildDeclaration()}</code>
  ${formatTextasHTML(parser.description)}
</header>
""".trim()
    }

    private fun buildDeclaration(): String {
        val sb = StringBuilder()
        sb.append("interface ${parser.name}")
        if (parser.extendsName != null) {
            sb.append(" extends ")
            if (parser.extendsVersion != null) sb.append("@${parser.extendsVersion}::")
            sb.append(parser.extendsName)
        }
        return sb.toString()
    }

    override fun printInfo() {
        super.printInfo()
        println(" InterfaceFileWriter:")
        println("  Interface: ${parser.name} extends ${parser.extendsName}::${parser.extendsVersion}")
        val methodEntries = parser.methods.map { MethodElement(it) }
        println("  method count: ${methodEntries.size}")
    }
}