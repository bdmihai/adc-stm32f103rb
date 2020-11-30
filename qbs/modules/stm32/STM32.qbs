/*_____________________________________________________________________________
 │                                                                            |
 │ COPYRIGHT (C) 2020 Mihai Baneu                                             |
 │                                                                            |
 | Permission is hereby  granted,  free of charge,  to any person obtaining a |
 | copy of this software and associated documentation files (the "Software"), |
 | to deal in the Software without restriction,  including without limitation |
 | the rights to  use, copy, modify, merge, publish, distribute,  sublicense, |
 | and/or sell copies  of  the Software, and to permit  persons to  whom  the |
 | Software is furnished to do so, subject to the following conditions:       |
 |                                                                            |
 | The above  copyright notice  and this permission notice  shall be included |
 | in all copies or substantial portions of the Software.                     |
 |                                                                            |
 | THE SOFTWARE IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF ANY KIND,  EXPRESS |
 | OR   IMPLIED,   INCLUDING   BUT   NOT   LIMITED   TO   THE  WARRANTIES  OF |
 | MERCHANTABILITY,  FITNESS FOR  A  PARTICULAR  PURPOSE AND NONINFRINGEMENT. |
 | IN NO  EVENT SHALL  THE AUTHORS  OR  COPYRIGHT  HOLDERS  BE LIABLE FOR ANY |
 | CLAIM, DAMAGES OR OTHER LIABILITY,  WHETHER IN AN ACTION OF CONTRACT, TORT |
 | OR OTHERWISE, ARISING FROM,  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR  |
 | THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                 |
 |____________________________________________________________________________|
 |                                                                            |
 |  Author: Mihai Baneu                           Last modified: 15.May.2020  |
 |                                                                            |
 |___________________________________________________________________________*/
 
import qbs.FileInfo
import qbs.TextFile
import qbs.Utilities
import 'gcc.js' as Gcc

Module {
    condition: false

    property string toolchainPrefix: 'arm-none-eabi-'

    property string assemblerName:        toolchainPrefix + 'gcc'
    property string compilerName:         toolchainPrefix + 'gcc'
    property string cxxCompilerName:      toolchainPrefix + 'c++'
    property string linkerName:           toolchainPrefix + 'ld'
    property string archiverName:         toolchainPrefix + 'ar'
    property string objcopyName:          toolchainPrefix + 'objcopy'
    property string objdumpName:          toolchainPrefix + 'objdump'
    property string sizeName:             toolchainPrefix + 'size'

    property string toolchainPath: ''

    property string assemblerPath:        toolchainPath + assemblerName
    property string compilerPath:         toolchainPath + compilerName
    property string cxxCompilerPath:      toolchainPath + cxxCompilerName
    property string linkerPath:           toolchainPath + linkerName
    property string archiverPath:         toolchainPath + archiverName
    property string objcopyPath:          toolchainPath + objcopyName
    property string objdumpPath:          toolchainPath + objdumpName
    property string sizePath:             toolchainPath + sizeName

    property stringList defines
    property stringList seriesDefines
    property stringList targetDefines

    property pathList includePaths
    property pathList seriesIncludePaths
    property pathList targetIncludePaths

    property stringList asmFlags
    property stringList seriesAsmFlags
    property stringList targetAsmFlags

    property stringList cFlags
    property stringList seriesCFlags
    property stringList targetCFlags

    property stringList cxxFlags
    property stringList seriesCxxFlags
    property stringList targetCxxFlags

    property stringList archiverFlags
    property stringList seriesArchiverFlags
    property stringList targetArchiverFlags

    property stringList linkerFlags
    property stringList seriesLinkerFlags
    property stringList targetLinkerFlags

    property pathList libraryPaths
    property pathList seriesLibraryPaths
    property pathList targetLibraryPaths

    property stringList libraries
    property stringList seriesLibraries
    property stringList targetLibraries

    FileTagger {
        patterns: [ '*.s', '*.S' ]
        fileTags: ['asm']
    }

    FileTagger {
        patterns: ['*.c']
        fileTags: ['c']
    }

    FileTagger {
        patterns: ['*.cpp', '*.cxx', '*.c++', '*.cc']
        fileTags: ['cpp']
    }

    FileTagger {
        patterns: ['*.h', '*.hpp', '*.hxx', '*.h++']
        fileTags: ['hpp']
    }

    FileTagger {
        patterns: ['*.a']
        fileTags: ['lib']
    }

    Rule {
        name: "assembler"
        inputs: ["asm"]

        Artifact {
            fileTags: ["obj"]
            filePath: FileInfo.joinPaths(Utilities.getHash(input.baseDir), input.fileName + ".o")
        }

        prepare: {
            return Gcc.prepareAssembler.apply(Gcc, arguments);
        }
    }

    Rule {
        name: "compiler"
        inputs: ["c"]

        Artifact {
            fileTags: ["obj"]
            filePath: FileInfo.joinPaths(Utilities.getHash(input.baseDir), input.fileName + ".o")
        }

        prepare: {
            return Gcc.prepareCompiler.apply(Gcc, arguments);
        }
    }

    Rule {
        name: "cxxCompiler"
        inputs: ["cpp"]

        Artifact {
            fileTags: ["obj"]
            filePath: FileInfo.joinPaths(Utilities.getHash(input.baseDir), input.fileName + ".o")
        }

        prepare: {
            return Gcc.prepareCxxCompiler.apply(Gcc, arguments);
        }
    }

    Rule {
        name: "archiver"
        inputs: ["obj"]
        multiplex: true

        Artifact {
            fileTags: ["lib"]
            filePath: FileInfo.joinPaths(product.destinationDirectory, 'lib' + product.targetName + '.a')
        }

        prepare: {
            return Gcc.prepareArchiver.apply(Gcc, arguments);
        }
    }

    Rule {
        name: "linker"
        multiplex: true
        inputs: ["lib", "linkerscript"]
        inputsFromDependencies: ["lib", "linkerscript"]
        outputFileTags: ["app", "map", "bin"]
        outputArtifacts: {
            var artifacts = [
                {
                    filePath: FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".elf"),
                    fileTags: ["app"]
                },
                {
                    filePath: FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".map"),
                    fileTags: ["map"]
                },
                {
                    filePath: FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".bin"),
                    fileTags: ["bin"]
                }
            ];
            return artifacts;
        }

        prepare: {
            return Gcc.prepareLinker.apply(Gcc, arguments);
        }
    }

    Scanner {
        inputs: ["linkerscript"]
        recursive: true
        scan: {
            console.debug("scanning linkerscript " + filePath + " for dependencies");
            var retval = [];
            var linkerScript = new TextFile(filePath, TextFile.ReadOnly);
            var regexp = /[\s]*INCLUDE[\s]+(\S+).*/ // "INCLUDE filename"
            var match;
            while (!linkerScript.atEof()) {
                match = regexp.exec(linkerScript.readLine());
                if (match) {
                    var dependencyFileName = match[1];
                    retval.push(dependencyFileName);
                    console.debug("linkerscript " + filePath + " depends on " + dependencyFileName);
                }
            }
            linkerScript.close();
            return retval;
        }
        searchPaths: {
            var retval = [];
            for (var i = 0; i < (product.stm32.libraryPaths || []).length; i++)
                retval.push(product.stm32.libraryPaths[i]);
            var regexp = /[\s]*SEARCH_DIR\((\S+)\).*/ // "SEARCH_DIR(path)"
            var match;
            var linkerScript = new TextFile(input.filePath, TextFile.ReadOnly);
            while (!linkerScript.atEof()) {
                match = regexp.exec(linkerScript.readLine());
                if(match) {
                    var additionalPath = match[1];
                    // path can be quoted to use non-latin letters, remove quotes if present
                    if (additionalPath.startsWith("\"") && additionalPath.endsWith("\""))
                        additionalPath = additionalPath.slice(1, additionalPath.length - 1);
                    retval.push(additionalPath);
                }
            }
            linkerScript.close();
            return retval;
        }
    }
}
