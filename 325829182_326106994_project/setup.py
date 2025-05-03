from setuptools import Extension, setup

sources = ["kmeansmodule.c", "kmeans.c"]

module = Extension(
    "symnmf",
    sources=["symnmfmodule.c", "symnmf_algo.c", "matrix_utils.c"],
    extra_compile_args=["-g"],
)
setup(
    name="symnmf",
    version="1.0",
    description="Python wrapper for custom C extension",
    ext_modules=[module],
)
