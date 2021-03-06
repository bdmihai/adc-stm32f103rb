import qbs.FileInfo

Product {
    name: "uprintf"
    type: "lib"

    Depends { name: "stm32" }
    stm32.includePaths: [ "inc" ]

    files: [
        "inc/*.h",
        "src/*.c"
    ]

    Export {
        Depends { name: "stm32" }
        stm32.includePaths: [ FileInfo.joinPaths(product.sourceDirectory, "/inc") ]
        stm32.libraryPaths: [ product.destinationDirectory ]
    }
}
