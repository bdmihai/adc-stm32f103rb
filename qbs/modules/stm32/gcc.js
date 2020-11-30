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

function prepareAssembler(project, product, inputs, outputs, input, output, explicitlyDependsOn) {
    var flags = [];
    flags = flags.uniqueConcat(product.stm32.asmFlags);
    flags = flags.uniqueConcat(product.stm32.seriesAsmFlags);
    flags = flags.uniqueConcat(product.stm32.targetAsmFlags);

    var defines = [];
    defines = defines.uniqueConcat(product.stm32.defines);
    defines = defines.uniqueConcat(product.stm32.seriesDefines);
    defines = defines.uniqueConcat(product.stm32.targetDefines);

    var includePaths = [];
    includePaths = includePaths.uniqueConcat(product.stm32.includePaths);
    includePaths = includePaths.uniqueConcat(product.stm32.seriesIncludePaths);
    includePaths = includePaths.uniqueConcat(product.stm32.targetIncludePaths);

    var args = flags;
    args = args.concat(defines.map(function(define) { return '-D' + define }));
    includePaths.forEach(function(path) { args.push("-I", path); });
    args.push("-c", input.filePath);
    args.push("-o", output.filePath);

    var cmd = new Command(product.stm32.assemblerPath, args);
    cmd.workingDirectory = product.sourceDirectory;
    cmd.description = 'assembling ' + input.fileName;
    cmd.highlight = "compiler";
    cmd.jobPool = "compiler";
    return cmd;
}


function prepareCompiler(project, product, inputs, outputs, input, output, explicitlyDependsOn) {
    var flags = [];
    flags = flags.uniqueConcat(product.stm32.cFlags);
    flags = flags.uniqueConcat(product.stm32.seriesCFlags);
    flags = flags.uniqueConcat(product.stm32.targetCFlags);

    var defines = [];
    defines = defines.uniqueConcat(product.stm32.defines);
    defines = defines.uniqueConcat(product.stm32.seriesDefines);
    defines = defines.uniqueConcat(product.stm32.targetDefines);

    var includePaths = [];
    includePaths = includePaths.uniqueConcat(product.stm32.includePaths);
    includePaths = includePaths.uniqueConcat(product.stm32.seriesIncludePaths);
    includePaths = includePaths.uniqueConcat(product.stm32.targetIncludePaths);

    var args = flags;
    args = args.concat(defines.map(function(define) { return '-D' + define }));
    includePaths.forEach(function(path) { args.push("-I", path); });
    args.push("-c", input.filePath);
    args.push("-o", output.filePath);

    var cmd = new Command(product.stm32.compilerPath, args);
    cmd.workingDirectory = product.sourceDirectory;
    cmd.description = 'compiling ' + input.fileName;
    cmd.highlight = "compiler";
    cmd.jobPool = "compiler";
    return cmd;
}

function prepareCxxCompiler(project, product, inputs, outputs, input, output, explicitlyDependsOn) {
    var flags = [];
    flags = flags.uniqueConcat(product.stm32.cxxFlags);
    flags = flags.uniqueConcat(product.stm32.seriesCxxFlags);
    flags = flags.uniqueConcat(product.stm32.targetCxxFlags);

    var defines = [];
    defines = defines.uniqueConcat(product.stm32.defines);
    defines = defines.uniqueConcat(product.stm32.seriesDefines);
    defines = defines.uniqueConcat(product.stm32.targetDefines);

    var includePaths = [];
    includePaths = includePaths.uniqueConcat(product.stm32.includePaths);
    includePaths = includePaths.uniqueConcat(product.stm32.seriesIncludePaths);
    includePaths = includePaths.uniqueConcat(product.stm32.targetIncludePaths);

    var args = flags;
    args = args.concat(defines.map(function(define) { return '-D' + define }));
    includePaths.forEach(function(path) { args.push("-I", path); });
    args.push("-c", input.filePath);
    args.push("-o", output.filePath);

    var cmd = new Command(product.stm32.cxxCompilerPath, args);
    cmd.description = 'compiling ' + input.fileName;
    cmd.highlight = "compiler";
    cmd.jobPool = "compiler";
    return cmd;
}

function prepareArchiver(project, product, inputs, outputs, input, output, explicitlyDependsOn) {
    var flags = [];
    flags = flags.uniqueConcat(product.stm32.archiverFlags);
    flags = flags.uniqueConcat(product.stm32.seriesArchiverFlags);
    flags = flags.uniqueConcat(product.stm32.targetArchiverFlags);

    var args = [flags.join(''), output.filePath];
    args = args.concat(inputs.obj.map(function(obj) { return obj.filePath }));

    var cmd = new Command(product.stm32.archiverPath, args);
    cmd.description = 'archiving ' + output.fileName;
    cmd.highlight = "linker";
    cmd.jobPool = "linker";
    return cmd;
}

function prepareLinker(project, product, inputs, outputs, input, output, explicitlyDependsOn) {
    var flags = [];
    flags = flags.uniqueConcat(product.stm32.linkerFlags);
    flags = flags.uniqueConcat(product.stm32.seriesLinkerFlags);
    flags = flags.uniqueConcat(product.stm32.targetLinkerFlags);

    var libraryPaths = [];
    libraryPaths = libraryPaths.uniqueConcat(product.stm32.libraryPaths);
    libraryPaths = libraryPaths.uniqueConcat(product.stm32.seriesLibraryPaths);
    libraryPaths = libraryPaths.uniqueConcat(product.stm32.targetLibraryPaths);

    var libraries = [];
    libraries = libraries.uniqueConcat(product.stm32.libraries);
    libraries = libraries.uniqueConcat(product.stm32.seriesLibraries);
    libraries = libraries.uniqueConcat(product.stm32.targetLibraries);

    var args = flags;
    args = args.concat(libraryPaths.map(function(path) { return '-L' + path }));
    args = args.concat('-L' + product.destinationDirectory);
    args = args.concat(libraries.map(function(lib) { return '-l' + lib }));
    args.push('-Wl,--start-group');
    args = args.concat(inputs.lib.map(function(lib) { return '-l' + lib.baseName.substr(3, lib.baseName.length) }));
    args.push('-Wl,--end-group');
    args = args.concat(inputs.linkerscript.map(function(linkerscript) { return '-Wl,-T' + linkerscript.filePath }));
    args.push('-o', outputs.app[0].filePath);
    args.push('-Wl,-Map=' + outputs.map[0].filePath);

    var commands = [];
    var cmd = new Command(product.stm32.compilerPath, args); // use the compiler for the final linking
    cmd.description = 'linking ' + outputs.app[0].fileName;
    cmd.highlight = "linker";
    cmd.jobPool = "linker";
    commands.push(cmd);

    args = ["-O", "binary", outputs.app[0].filePath, outputs.bin[0].filePath];
    var cmd = new Command(product.stm32.objcopyPath, args); // generate the binary file
    cmd.description = 'generating binary file for flashing ' + outputs.bin[0].fileName;
    cmd.highlight = "linker";
    cmd.jobPool = "linker";
    commands.push(cmd);

    
    //args = ['-Ax', outputs.app[0].filePath];
    args = [outputs.app[0].filePath];
    var cmd = new Command(product.stm32.sizePath, args); // show the size of the executable total
    cmd.jobPool = "linker";
    cmd.silent = true;
    commands.push(cmd);

    return commands;
}