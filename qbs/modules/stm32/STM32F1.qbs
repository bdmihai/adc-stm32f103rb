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
 
STM32 {
    condition: false

    seriesDefines: [
        'CALL_ARM_SYSTEM_INIT'
    ]

    seriesAsmFlags: [
        '-specs=nosys.specs',
	    '-specs=nano.specs',
	    '-mcpu=cortex-m3',
	    '-march=armv7-m',
	    '-mlittle-endian',
	    '-mthumb',
	    '-masm-syntax-unified',
	    '-ffreestanding',
	    '-fno-exceptions',
	    '-fno-unwind-tables',
        '-ffunction-sections',
        '-fdata-sections',
        '-Wall',
        '-Wextra',
        '-Og',
        '-ggdb'
    ]

    seriesCFlags: [
        '-std=gnu99',
        '-specs=nosys.specs',
	    '-specs=nano.specs',
	    '-mcpu=cortex-m3',
	    '-march=armv7-m',
	    '-mlittle-endian',
	    '-mthumb',
	    '-masm-syntax-unified',
	    '-ffreestanding',
	    '-fno-exceptions',
	    '-fno-unwind-tables',
        '-ffunction-sections',
        '-fdata-sections',
        '-Wall',
        '-Wextra',
        '-Og',
        '-ggdb'
    ]

    seriesCxxFlags: [
        '-std=gnu++11',
        '-specs=nosys.specs',
	    '-specs=nano.specs',
	    '-mcpu=cortex-m3',
	    '-march=armv7-m',
	    '-mlittle-endian',
	    '-mthumb',
	    '-masm-syntax-unified',
	    '-ffreestanding',
	    '-fno-exceptions',
	    '-fno-unwind-tables',
        '-ffunction-sections',
        '-fdata-sections',
        '-fno-exceptions',
	    '-fno-rtti',
	    '-fno-threadsafe-statics',
        '-Wall',
        '-Wextra',
        '-Og',
        '-ggdb'
    ]

    seriesArchiverFlags: [ 
        'c', 
        'r' 
    ]

    seriesLinkerFlags: [
        '-specs=nosys.specs',
	    '-specs=nano.specs',
	    '-mcpu=cortex-m3',
	    '-march=armv7-m',
	    '-mlittle-endian',
	    '-mthumb',
	    '-masm-syntax-unified',
	    '-ffreestanding',
	    '-fno-exceptions',
	    '-fno-unwind-tables',
        '-ffunction-sections',
        '-fdata-sections',
        '-Wall',
        '-Wextra',
        '-Og',
        '-ggdb'
    ]

}