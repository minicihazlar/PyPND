from distutils.core import setup, Extension
import os, sys, shutil, glob

base_path = os.getcwd()
os.chdir("../src")

module = Extension("pnd", sources = ["pnd.cpp"],
                          include_dirs = ["../../../source/pnd/src/common"],
                          libraries = ["pndriver", "pthread", "dl", "rt"],
                          library_dirs = ["../../lib/linux32_native/build/lib"],
                          extra_link_args = ["-m32", "-fmessage-length=0", "-MD", "-MP"],
                          extra_compile_args = ["-DTOOL_CHAIN_GNU", "-DPLF_PNDRIVER_LINUX", "-m32",  "-MMD", "-fmessage-length=0"])
 
setup(name = "pnd",
      version = "1.0",
      description = "Python binding for PN Driver",
      ext_modules = [module])

shutil.move("build","../linux32_native")
os.chdir(base_path)
for file in glob.glob(r"build/lib.*/*.so"):
	shutil.copy(file, "../src_python")
