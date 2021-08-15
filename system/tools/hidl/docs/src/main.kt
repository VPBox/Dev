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

import lexer.HidlLexer
import lexer.Token
import parser.LOG_NAME
import parser.config
import parser.files.AbstractFileParser
import parser.files.InterfaceFileParser
import parser.files.TypesFileParser
import writer.files.*
import java.nio.file.Paths
import java.text.ParseException

fun main(args: Array<String>) {
    config.parseArgs(args)
    if (config.verbose) println("$LOG_NAME args: ${config}")

    val indexWriter = IndexFileWriter() //collects parser entries

    /*
     * parse and write HAL files
     */
    for (fp in config.files) {
        println("$LOG_NAME Parsing input: $fp")

        val tokens = HidlLexer.tokenize(fp)
        val (parser, writer) = parseAndGetWriter(tokens)

        indexWriter.addEntry(parser)

        try {
            //since lazily evaluated, check here for parse errors
            if (writer.writeToFile()) println("$LOG_NAME Wrote file: ${writer.path}")

        } catch (ex: ParseException) {
            if (config.warnOnly) {
                System.err.println("$LOG_NAME Error parsing file, skipping: $fp")
                continue
            } else {
                System.err.println("$LOG_NAME Error parsing file: $fp")
                throw ex
            }
        } finally {
            if (config.verbose) writer.printInfo()
        }
    }

    /*
     * non-HAL file
     */
    if (indexWriter.writeToFile()) println("$LOG_NAME Wrote index: ${indexWriter.path}")

    val cssPath = Paths.get("${config.outDir}/assets/style.css")
    if (resources.copyToFile("/resources/assets/style.css", cssPath)) {
        println("$LOG_NAME Copied resource file: $cssPath")
    }
}

fun parseAndGetWriter(tokens: List<Token>): Pair<AbstractFileParser, AbstractParserFileWriter> {
    val parser: AbstractFileParser
    val writer: AbstractParserFileWriter
    if (InterfaceFileParser.isInterface(tokens)) {
        parser = InterfaceFileParser(tokens)
        writer = InterfaceFileWriter(parser)
    } else {
        parser = TypesFileParser(tokens)
        writer = TypesFileWriter(parser)
    }
    return Pair(parser, writer)
}