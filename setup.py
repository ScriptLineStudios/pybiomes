import glob
from setuptools import setup, Extension

cubiomes_sources = [
    f.replace("\\", "/")
    for f in glob.glob("src/external/cubiomes/**/*.c", recursive=True)
    if "tests" not in f.replace("\\", "/").split("/")
    and not f.endswith("tests.c")
]

ext_modules = [
    Extension(
        name="pybiomes",
        sources=["src/bind.c"] + cubiomes_sources,
        include_dirs=[
            "src/external/cubiomes",
            "src/external/cubiomes/features",
            "src/external/cubiomes/loot",
            "src/external/cubiomes/loot/cjson",
        ],
        extra_compile_args=["-O3", "-g"],
        libraries=["m"],
    )
]

setup(
    ext_modules=ext_modules,
)
