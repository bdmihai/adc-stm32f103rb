import qbs

Project {
    name: "adc"
    minimumQbsVersion: "1.16"
    qbsSearchPaths: "../qbs"

    references: [
        "startup/startup.qbs",
        "linker/linker.qbs",
        "cmsis/cmsis.qbs",
        "hal/hal.qbs",
        "freertos/freertos.qbs",
        "uprintf/uprintf.qbs",
        "app/app.qbs"
    ]
}


