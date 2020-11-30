import qbs.FileInfo

Product {
    name: "linker"
    type: "lib"

    Depends { name: "stm32" }

    Group {
        name: "STM32F103x8"
        condition: qbs.targetPlatform == "STM32F103x8"
        files: ["STM32F1xx/STM32F103x8.ld"]
        fileTags: ["linkerscript"]
    }

    Group {
        name: "STM32F103xC"
        condition: qbs.targetPlatform == "STM32F103xC"
        files: ["STM32F1xx/STM32F103xC.ld"]
        fileTags: ["linkerscript"]
    }

    Group {
        name: "STM32F401xC"
        condition: qbs.targetPlatform == "STM32F401xC"
        files: ["STM32F4xx/STM32F401xC.ld"]
        fileTags: ["linkerscript"]
    }

    files: [
        "common.ld"
    ]

    Export {
        Depends { name: "stm32" }
        stm32.libraryPaths: [ product.sourceDirectory ]
    }
}
