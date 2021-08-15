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
import parser.elements.declarations.TypedefDeclarationParser
import writer.formatTextasHTML

class TypedefElement(parser: EntryParser) : AbstractElement(parser) {
    //name, type
    override val declarationParser = parser.declarationParser as TypedefDeclarationParser

    override fun declaration(): String {
        return "typedef ${declarationParser.type} ${declarationParser.name}"
    }

    override fun detailsRows() = "" //not used

    override fun toHTML(): String {
        return """
<div>
  <h3>${declarationParser.name}</h3>
  <pre class="prettyprint devsite-disable-click-to-copy">${declaration()}</pre>
  <div>
    ${formatTextasHTML(docParser.description)}
  </div>
</div>
""".trim()
    }
}