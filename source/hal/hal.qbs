import qbs.FileInfo

Product {
    name: "hal"
    type: "lib"

    Depends { name: "stm32" }
    Depends { name: "cmsis" }
    stm32.includePaths: [
        "STM32F1xx/inc",
        "STM32F4xx/inc"
    ]

    Group {
        name: "STM32F103xx"
        condition: qbs.targetPlatform == "STM32F103x8" || qbs.targetPlatform == "STM32F103xC"
        files: [
            "STM32F1xx/inc/*.h",
            "STM32F1xx/src/*.c"
        ]
    }

    Group {
        name: "STM32F401xx"
        condition: qbs.targetPlatform == "STM32F401xC"
        files: [
            "STM32F4xx/inc/*.h",
            "STM32F4xx/src/*.c"
        ]
    }

    Export {
        Depends { name: "stm32" }
        stm32.includePaths: [ 
            FileInfo.joinPaths(product.sourceDirectory, "/STM32F1xx/inc"), 
            FileInfo.joinPaths(product.sourceDirectory, "/STM32F4xx/inc")
        ]
        stm32.libraryPaths: [ product.destinationDirectory ]
    }
}
