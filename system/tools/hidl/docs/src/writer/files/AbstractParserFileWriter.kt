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

import parser.config
import parser.files.AbstractFileParser
import writer.elements.CompoundElement
import writer.elements.EnumElement
import writer.elements.TypedefElement
import writer.getOutPath
import java.nio.file.Path

abstract class AbstractParserFileWriter(private val parser: AbstractFileParser) : AbstractFileWriter() {

    override val baseName = parser.name
    override val templateResource: String by lazy { "/resources/template/${this.baseName}.html" }
    override val path: Path by lazy { getOutPath(parser, config.outDir) }

    override fun replaceVars() {
        replaceVar("name", parser.name)

        replaceVar("propertyDefs") {
            val sb = StringBuilder()

            if (parser.typedefs.isNotEmpty()
                    || parser.enums.isNotEmpty()
                    || parser.structs.isNotEmpty()
                    || parser.unions.isNotEmpty()) {
                sb.append("<section>\n")
                sb.append("<h2>Properties</h2>\n")
                //typedefs
                sb.append(parser.typedefs.map { TypedefElement(it).toHTML() }.joinToString("\n"))
                //enums
                sb.append(parser.enums.map { EnumElement(it).toHTML() }.joinToString("\n"))
                //structs
                sb.append(parser.structs.map { CompoundElement(it).toHTML() }.joinToString("\n"))
                //unions
                sb.append(parser.unions.map { CompoundElement(it).toHTML() }.joinToString("\n"))
                sb.append("\n</section>\n")
            }
            sb.toString()
        }
    }

    override fun printInfo() {
        super.printInfo()
        println(" AbstractParserFileWriter:")
        println("  package: ${parser.packageName}")
        println("  package ver: ${parser.packageVersion}")
        println("  enums count: ${parser.enums.size}")
        println("  structs count: ${parser.structs.size}")
        println("  unions count: ${parser.unions.size}")
        println("  typedefs count: ${parser.typedefs.size}")
    }
}