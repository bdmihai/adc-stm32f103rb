Product {
    name: "startup"
    type: "lib"

    Depends { name: "stm32" }

    Group {
        name: "STM32F103xB"
        condition: qbs.targetPlatform == "STM32F103x8"
        files: ["STM32F1xx/STM32F103xB.s"]
    }

    Group {
        name: "STM32F103xE"
        condition: qbs.targetPlatform == "STM32F103xC"
        files: ["STM32F1xx/STM32F103xE.s"]
    }

    Group {
        name: "STM32F401xC"
        condition: qbs.targetPlatform == "STM32F401xC"
        files: ["STM32F4xx/STM32F401xC.s"]
    }

    Export {
        Depends { name: "stm32" }
        stm32.libraryPaths: [ product.destinationDirectory ]
    }
}
