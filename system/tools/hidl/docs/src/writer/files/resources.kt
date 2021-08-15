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
import java.io.FileNotFoundException
import java.nio.file.Path

object resources {

    private val resourceCache = mutableMapOf<String, String>() //name(path) => contents

    fun readResourceText(name: String): String {
        return resourceCache.getOrElse(name) {
            val input = javaClass.getResourceAsStream(name)
                    ?: throw FileNotFoundException("Unable to locate file resource: $name")

            val contents = input.bufferedReader().use { it.readText() }
            resourceCache[name] = contents
            contents
        }
    }

    fun copyToFile(resourceName: String, outPath: Path): Boolean {
        val contents = readResourceText(resourceName)

        if (config.lintMode) {
            return false
        } else {
            val dir = outPath.parent.toFile() //dir name
            if (!dir.exists()) dir.mkdirs()
            if (!dir.canWrite()) throw FileSystemException(dir, reason = "No write access to output directory")

            val fp = outPath.toFile()
            fp.bufferedWriter().use { it.write(contents) }
            if (!fp.isFile) throw FileSystemException(fp, reason = "Error writing file")
            return true
        }
    }
}