import qbs.FileInfo

Product {
    name: "cmsis"
    type: "lib"

    Depends { name: "stm32" }
    stm32.includePaths: [
        "ARM/inc",
        "STM32F1xx/inc",
        "STM32F4xx/inc"
    ]

    Group {
        name: "STM32F103xx"
        condition: qbs.targetPlatform == "STM32F103x8" || qbs.targetPlatform == "STM32F103xC"
        files: [
            "ARM/inc/core_cm3.h",
            "STM32F1xx/**/*.h",
            "STM32F1xx/**/*.c"
        ]
    }

    Group {
        name: "STM32F401xx"
        condition: qbs.targetPlatform == "STM32F401xC"
        files: [
            "ARM/inc/core_cm4.h",
            "STM32F4xx/**/*.h",
            "STM32F4xx/**/*.c"
        ]
    }

    files: [
        "ARM/inc/cmsis*.h",
    ]

    Export {
        Depends { name: "stm32" }
        stm32.includePaths: [
            FileInfo.joinPaths(product.sourceDirectory, "/ARM/inc"),
            FileInfo.joinPaths(product.sourceDirectory, "/STM32F1xx/inc"),
            FileInfo.joinPaths(product.sourceDirectory, "/STM32F4xx/inc")
        ]
        stm32.libraryPaths: [ product.destinationDirectory ]
    }
}
