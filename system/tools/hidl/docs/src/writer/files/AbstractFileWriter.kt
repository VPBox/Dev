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
import java.nio.file.Path

abstract class AbstractFileWriter() {
    abstract val baseName: String
    abstract val templateResource: String
    abstract val path: Path

    //init with template file contents on first use
    var outStr: String = ""
        get() = if (field.isEmpty()) resources.readResourceText(this.templateResource) else field

    open fun replaceVars() {}

    fun replaceVar(varName: String, newValue: String) {
        outStr = outStr.replace(Regex("\\\$\\{?$varName}?"), newValue)
    }

    fun replaceVar(varName: String, newValue: () -> String) {
        replaceVar(varName, newValue())
    }

    fun writeToFile(): Boolean {
        replaceVars()
        onWrite()

        if (config.lintMode) {
            return false
        } else {
            val dir = this.path.parent.toFile() //dir name
            if (!dir.exists()) dir.mkdirs()
            if (!dir.canWrite()) throw FileSystemException(dir, reason = "No write access to output directory")

            val fp = this.path.toFile()
            fp.bufferedWriter().use { it.write(this.outStr) }
            if (!fp.isFile) throw FileSystemException(fp, reason = "Error writing file")
            return true
        }
    }

    open fun onWrite() {}

    open fun printInfo() {
        println("Name: ${this.baseName}")
        println(" AbstractFileWriter:")
        println("  class: ${javaClass.simpleName}")
        println("  dest: ${this.path}")
    }
}