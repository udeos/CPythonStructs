from distutils.core import setup, Extension
setup(
    name="structs",
    version="1.0",
    ext_modules=[
        Extension("structs", [
            "src/structs/structs.c",
            "src/structs/onelinkedlist.c",
        ], )
    ]
)
