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

package parser

import java.io.File
import java.nio.file.Files
import java.nio.file.Path
import java.nio.file.Paths
import kotlin.system.exitProcess

const val LOG_NAME = "[hidl-doc]"

fun printUsage() {
    println("""
Usage: hidl-doc [-i path]
 -i=path  Add input HAL file or directory to parse
 -o=dir   Output directory of generated HTML
 -x=path  Exclude file or directory from files to parse
 -v       Verbose mode, print parsing info
 -h       Print this help and exit
 Error modes:
 -w       Warn on errors instead of exiting
 -l       Lint. Warn-only and do not generate files
""".trim())
}

object config {
    val files = mutableListOf<File>()
    lateinit var outDir: Path
    var verbose = false
    var lintMode = false
    var warnOnly = false

    override fun toString(): String {
        return """
verbose: $verbose
warnOnly: $warnOnly
outDir: $outDir
files: $files
"""
    }

    private const val HAL_EXTENSION = ".hal"

    fun parseArgs(args: Array<String>) {
        if (args.isEmpty()) {
            printUsage()
            exitProcess(1)
        }

        val dirPathArgs = mutableListOf<Path>()
        val filePathArgs = mutableListOf<Path>()
        val excludedPathArgs = mutableListOf<Path>()
        var maybeOutDir: Path? = null

        val iter = args.iterator()

        //parse command-line arguments
        while (iter.hasNext()) {
            var arg = iter.next()

            when (arg) {
                "-i" -> {
                    val path = Paths.get(iter.next())
                    if (Files.isDirectory(path)) dirPathArgs.add(path) else filePathArgs.add(path)
                }
                "-x" -> excludedPathArgs.add(Paths.get(iter.next()).toAbsolutePath())
                "-o" -> maybeOutDir = Paths.get(iter.next())
                "-v" -> verbose = true
                "-l" -> { lintMode = true; warnOnly = true }
                "-w" -> warnOnly = true
                "-h" -> {
                    printUsage()
                    exitProcess(0)
                }
                else -> {
                    System.err.println("Unknown option: $arg")
                    printUsage()
                    exitProcess(1)
                }
            }
        }

        if (maybeOutDir == null) {
            System.err.println("Error: No output directory supplied (-o)")
            exitProcess(1)
        }
        outDir = maybeOutDir

        //collect files (explicitly passed and search directories)
        val allFiles = mutableListOf<File>()

        //add individual files
        filePathArgs.filterNot { excludedPathArgs.contains(it.toAbsolutePath()) }
                .map { it.toFile() }.map { fp ->
            if (!fp.isFile || !fp.canRead() || !fp.absolutePath.toLowerCase().endsWith(HAL_EXTENSION)) {
                System.err.println("Error: Invalid $HAL_EXTENSION file: ${fp.path}")
                exitProcess(1)
            }
            fp
        }.map { allFiles.add(it) }

        //check directory args
        dirPathArgs.map { it.toFile() }
                .map { findFiles(it, allFiles, HAL_EXTENSION, excludedPathArgs) }

        //consolidate duplicates
        allFiles.distinctBy { it.canonicalPath }
                .forEach { files.add(it) }

        if (files.isEmpty()) {
            System.err.println("Error: Can't find any $HAL_EXTENSION files")
            exitProcess(1)
        }
    }

    /**
     * Recursively search for files in a directory matching an extension and add to files.
     */
    private fun findFiles(dir: File, files: MutableList<File>, ext: String, excludedPaths: List<Path>) {
        if (!dir.isDirectory || !dir.canRead()) {
            System.err.println("Invalid directory: ${dir.path}, aborting")
            exitProcess(1)
        }
        dir.listFiles()
                .filterNot { excludedPaths.contains(it.toPath().toAbsolutePath()) }
                .forEach { fp ->
                    if (fp.isDirectory) {
                        findFiles(fp, files, ext, excludedPaths)
                    } else if (fp.absolutePath.toLowerCase().endsWith(ext)) {
                        files.add(fp)
                    }
                }
    }
}