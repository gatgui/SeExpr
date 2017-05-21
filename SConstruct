import os
import re
import sys
import subprocess
import excons
import excons.config
from excons.tools import gl
from excons.tools import dl
from excons.tools import threads
from excons.tools import python
from excons.tools import boost


ARGUMENTS["use-c++11"] = "1"

env = excons.MakeBaseEnv()

use_llvm = (excons.GetArgument("use-llvm", 0, int) != 0)
if use_llvm:
   llvm_incdir, llvm_libdir = excons.GetDirs("llvm", silent=False)
else:
   llvm_incdir, llvm_libdir = None, None

llvm_cfg = None
llvm_libs = None

def LLVMConfig(env):
   global llvm_cfg

   if llvm_cfg is None:
      relpath = "llvm/Config/llvm-config.h"

      llvm_config = None

      if llvm_incdir:
         path = llvm_incdir + "/" + relpath
         if os.path.isfile(path):
            llvm_config = path

      if llvm_config is None:
         incdirs = map(str, env["CPPPATH"])
         if sys.platform != "win32":
            # Add some standard locations
            incdirs.extend(["/usr/local/include", "/usr/include"])
         for incdir in incdirs:
            path = incdir + "/" + relpath
            if os.path.isfile(path):
               llvm_config = path
               break

      if llvm_config is None:
         print("[SeExpr2] Could not find 'llvm/Config/llvm-config.h'")
         sys.exit(1)

      targets = []
      native_target = None
      enable_threads = False
      major = 0
      minor = 0
      e = re.compile(r"#define\s+LLVM_VERSION_(MAJOR|MINOR)\s+(\d+)")
      with open(llvm_config, "r") as f:
         for l in f.readlines():
            l = l.strip()
            m = e.match(l.strip())
            if m:
               if m.group(1) == "MAJOR":
                  major = int(m.group(2))
               elif m.group(1) == "MINOR":
                  minor = int(m.group(2))
            else:
               if "LLVM_NATIVE_ARCH" in l:
                  native_target = l.split("LLVM_NATIVE_ARCH")[1].strip()
               elif "LLVM_ENABLE_THREADS" in l:
                  enable_threads = (int(l.split("LLVM_ENABLE_THREADS")[1].strip()) != 0)

      defpath = os.path.dirname(llvm_config) + "/Targets.def"
      if os.path.isfile(defpath):
         e = re.compile(r"LLVM_TARGET\(([^)]+)\)")
         with open(defpath, "r") as f:
            for l in f.readlines():
               m = e.match(l.strip())
               if m:
                  targets.append(m.group(1))

      llvm_cfg = {"major_version": major,
                  "minor_version": minor,
                  "native_target": native_target,
                  "enable_threads": enable_threads,
                  "targets": targets}

   return llvm_cfg

def LLVMLibs(env, cfg):
   global llvm_libs

   if llvm_libs is None:
      libdir = llvm_libdir
      libprefix = ("lib" if sys.platform != "win32" else "")
      libext = (".a" if sys.platform != "win32" else ".lib")
      if libdir is None:
         target = "%sLLVMSupport%s" % (libprefix, libext)
         libdirs = map(str, env["LIBPATH"])
         if sys.platform != "win32":
            if sys.platform != "darwin":
               libdirs.extend(["/usr/local/lib64", "/usr/local/lib", "/usr/lib64", "/usr/lib"])
            else:
               libdirs.extend(["/usr/local/lib", "/usr/lib"])
            for ld in libdirs:
               path = ld + "/" + target
               if os.path.isfile(path):
                  libdir = ld
                  break
      if libdir is None:
         print("[SeExpr2] Cannot find LLVM libraries.")
      libs = excons.glob("%s/%sLLVM*%s" % (libdir, libprefix, libext))
      e = re.compile(r"%sLLVM(\w+)(AsmParser|AsmPrinter|CodeGen|Desc|Disassembler|Info|Utils)%s" % (libprefix, libext.replace(".", r"\.")))
      llvm_libs = {"base": [],
                   "target": {},
                   "jit": {},
                   "debug": []}
      for lib in libs:
         bn = os.path.basename(lib)
         m = e.match(bn)
         if m and m.group(1) in cfg["targets"]:
            tlibs = llvm_libs["target"].get(m.group(1), [])
            tlibs.append(m.group(1) + m.group(2))
            llvm_libs["target"][m.group(1)] = tlibs
         else:
            ln = os.path.splitext(bn)[0][4 + len(libprefix):]
            if ln.startswith("Debug"):
               llvm_libs["debug"].append(ln)
            elif ln.endswith("JIT"):
               jn = ln[:-3] # strip JIT
               jl = llvm_libs["jit"].get(jn, [])
               jl.append(ln)
               llvm_libs["jit"][jn] = jl
            else:
               llvm_libs["base"].append(ln)
      for jn in llvm_libs["jit"].keys():
         bl = llvm_libs["base"]
         llvm_libs["jit"][jn].extend(filter(lambda x: x.startswith(jn), bl))
         llvm_libs["base"] = filter(lambda x: not x.startswith(jn), bl)

   return llvm_libs

def RequireLLVM(env):
   cfg = LLVMConfig(env)
   if cfg["major_version"] != 3 or cfg["minor_version"] < 8:
      print("[SeExpr2] Unsupported LLVM version %d.%d." % (cfg["major_version"], cfg["minor_version"]))
      sys.exit(1)
   if cfg["minor_version"] == 8:
      env.Append(CPPDEFINES=["__STDC_LIMIT_MACROS", "__STDC_CONSTANT_MACROS"])
   if llvm_incdir:
      env.Append(CPPPATH=[llvm_incdir])
   if llvm_libdir:
      env.Append(LIBPATH=[llvm_libdir])
   alibs = LLVMLibs(env, cfg)
   llibs = alibs["jit"].get("MC", []) + alibs["target"].get(cfg["native_target"], []) + alibs["base"]
   for l in llibs:
      excons.Link(env, "LLVM%s" % l, static=True, force=True, silent=True)
   if sys.platform != "win32":
      env.Append(LIBS=["curses"])
      if cfg["enable_threads"]:
         env.Append(LIBS=["pthread"])

# Check if editor should be built
buildEditor = ("editor" in COMMAND_LINE_TARGETS)

# Check if python module should be built
buildPython = ("python" in COMMAND_LINE_TARGETS)

# Check if demos should be built
buildDemo = ("demos" in COMMAND_LINE_TARGETS)

# Check if parser sources should be generated
generateParser = False
if int(ARGUMENTS.get("generate-parser", "0")) != 0:
   # Check if we can actually generate parser sources
   generateParser = (excons.Which("flex") and excons.Which("bison") and excons.Which("sed"))


def GenerateMOC(target, source, env):
   cmd = "moc \"%s\" -o \"%s\"" % (source[0], target[0])
   p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
   out, _ = p.communicate()
   if p.returncode != 0:
      excons.WarnOnce("MOC generation failed for '%s'. SeExpr2Editor won't build." % target[0])
      excons.WarnOnce(out.strip())
   
   return None

def RequireQt(env):
   if sys.platform == "darwin":
      qtprefix = excons.GetArgument("with-qt", None)
      if qtprefix:
         env.Append(CPPFLAGS = " -F%s/Frameworks" % qtprefix)
         env.Append(LINKFLAGS = " -F%s/Frameworks -framework QtCore -framework QtGui -framework QtOpenGL" % qtprefix)
      else:
         env.Append(LINKFLAGS = " -framework QtCore -framework QtGui -framework QtOpenGL")
   else:
      qtinc, qtlib = excons.GetDirs("qt")
      if qtinc:
         env.Append(CPPPATH = [qtinc])
      if qtlib:
         env.Append(LIBPATH = [qtlib])
      env.Append(LIBS = ["QtCore", "QtGui", "QtOpenGL"])


GenerateConfig = excons.config.AddGenerator(env, "seexpr", {"SEEXPR_ENABLE_LLVM_BACKEND": ("1" if use_llvm else "0")})
env["BUILDERS"]["GenerateMOC"] = Builder(action=Action(GenerateMOC, "Generating $TARGET ..."), suffix="_moc.cpp")

# Library
libccflags = ""
libcppflags = ""
libdefs = [] # ["__STDC_LIMIT_MACROS"]
if not sys.platform == "win32":
   libcppflags += " -msse4.1 -Wextra -Wno-unused-parameter"
   if sys.platform == "darwin":
      libcppflags += " -Wno-date-time -Wno-deprecated-declarations -Wno-unneeded-internal-declaration"
   else:
      libccflags += " -rdynamic"
else:
   libdefs.extend(["SEEXPR_WIN32", "_CRT_SECURE_NO_WARNINGS", "_CRT_NONSTDC_NO_DEPRECATE", "NOMINMAX"])
   # disable:
   # - 4267: size_t to int warnings
   # - 4244: double to float, __int64 to int warnings
   # - 4005: macro redifinitions
   libcppflags += " -wd4267 -wd4244 -wd4005"
libincs = ["src/SeExpr", "src/SeExpr/generated"]

# Only support static linking so far
def SeExpr2Name():
   return "SeExpr2"

def SeExpr2Path():
   name = SeExpr2Name()
   if sys.platform == "win32":
      libname = name + ".lib"
   else:
      libname = "lib" + name + ".a"
   return excons.OutputBaseDirectory() + "/lib/" + libname

def RequireSeExpr2(env):
   if libcppflags:
      env.Append(CPPFLAGS=libcppflags)
   if libccflags:
      env.Append(CCFLAGS=libcflags)
   if libdefs:
      env.Append(CPPDEFINES=libdefs)
   excons.Link(env, SeExpr2Path(), static=True, force=True, silent=True)
   if use_llvm:
      RequireLLVM(env)
   if sys.platform != "win32":
      dl.Require(env)

Export("SeExpr2Name SeExpr2Path RequireSeExpr2")

GenerateConfig("src/SeExpr/ExprConfig.h", "src/SeExpr/ExprConfig.h.in")

if generateParser:
   env.Command("ExprParserLexIn.cpp", "src/SeExpr/ExprParserLex.l",
               "flex -o$TARGET $SOURCE")
   env.Command("src/SeExpr/generated/ExprParserLex.cpp", "ExprParserLexIn.cpp",
               "sed -e \"s/SeExpr2wrap(n)/SeExpr2wrap()/g\" -e \"s/yy/SeExpr2/g\" -e \"s/YY/SeExprYY/g\" $SOURCE > $TARGET")
   env.Command(["y.tab.c", "y.tab.h"], "src/SeExpr/ExprParser.y",
               "bison --defines --verbose --fixed-output-files -p SeExpr2 $SOURCE")
   env.Command("src/SeExpr/generated/ExprParser.tab.h", "y.tab.h",
               "sed -e \"s/yy/SeExpr2/g\" -e \"s/YY/SeExprYY/g\" $SOURCE > $TARGET")
   env.Command("src/SeExpr/generated/ExprParser.cpp", "y.tab.c",
               "sed -e \"s/yy/SeExpr2/g\" -e \"s/YY/SeExprYY/g\" $SOURCE > $TARGET")
else:
   env.Command("src/SeExpr/generated/ExprParserLex.cpp", "windows7/SeExpr/generated/ExprParserLex.cpp", Copy("$TARGET", "$SOURCE"))
   env.Command("src/SeExpr/generated/ExprParser.tab.h", "windows7/SeExpr/generated/ExprParser.tab.h", Copy("$TARGET", "$SOURCE"))
   env.Command("src/SeExpr/generated/ExprParser.cpp", "windows7/SeExpr/generated/ExprParser.cpp", Copy("$TARGET", "$SOURCE"))

# Collect library sources after potential parser generation so that Glob get the generated files
libsrcs = excons.glob("src/SeExpr/*.cpp")
if not use_llvm:
   libsrcs = filter(lambda x: os.path.basename(x) != "ExprLLVMCodeGeneration.cpp", libsrcs)
libsrcs += Glob("src/SeExpr/generated/*.cpp")

# Python module
python_prefix = "%s/%s/SeExprPy" % (python.ModulePrefix(), python.Version())

# Declare targets
prjs = [
   {  "name": "SeExpr2",
      "type": "staticlib",
      "desc": "SeExpr static library",
      "alias": "lib",
      "ccflags": libccflags,
      "cppflags": libcppflags,
      "defs": libdefs,
      "incdirs": libincs,
      "srcs": libsrcs,
      "custom": ([RequireLLVM] if use_llvm else [])
   },
]

if buildDemo:
   prjs.append({"name": "demos",
                "type": "testprograms",
                "srcs": ["src/demos/asciiCalculator.cpp", "src/demos/asciiGraph.cpp"],
                "custom": [RequireSeExpr2]})

if buildPython:
   prjs.append({"name": "core",
                "type": "dynamicmodule",
                "alias": "python",
                "prefix": python_prefix,
                "ext": python.ModuleExtension(),
                "incdirs": ["src/SeExpr/parser"],
                "srcs": excons.glob("src/py/*.cpp") + excons.glob("src/SeExpr/parser/*.cpp"),
                "install": {python_prefix: ["src/py/__init__.py", "src/py/utils.py"]},
                "custom": [boost.Require(libs=["python"]), python.SoftRequire, dl.Require, threads.Require]})

if buildEditor:
   if generateParser:
      env.Command("ExprSpecParserLexIn.cpp", "src/ui/ExprSpecParserLex.l",
                  "flex -o$TARGET $SOURCE")
      env.Command("src/ui/generated/ExprSpecParserLex.cpp", "ExprSpecParserLexIn.cpp",
                  "sed -e \"s/ExprSpecwrap(n)/ExprSpecwrap()/g\" -e \"s/yy/ExprSpec/g\" -e \"s/YY/ExprSpecYY/g\" $SOURCE > $TARGET")
      env.Command(["y.tab.c", "y.tab.h"], "src/ui/ExprSpecParser.y",
                  "bison --defines --verbose --fixed-output-files -p ExprSpec $SOURCE")
      env.Command("src/ui/generated/ExprSpecParser.tab.h", "y.tab.h",
                  "sed -e \"s/yy/ExprSpec/g\" -e \"s/YY/ExprSpecYY/g\" $SOURCE > $TARGET")
      env.Command("src/ui/generated/ExprSpecParser.cpp", "y.tab.c",
                  "sed -e \"s/yy/ExprSpec/g\" -e \"s/YY/ExprSpecYY/g\" $SOURCE > $TARGET")
   else:
      env.Command("src/ui/generated/ExprSpecParserLex.cpp", "windows7/ui/generated/ExprSpecParserLex.cpp", Copy("$TARGET", "$SOURCE"))
      env.Command("src/ui/generated/ExprSpecParser.tab.h", "windows7/ui/generated/ExprSpecParser.tab.h", Copy("$TARGET", "$SOURCE"))
      env.Command("src/ui/generated/ExprSpecParser.cpp", "windows7/ui/generated/ExprSpecParser.cpp", Copy("$TARGET", "$SOURCE"))

   srcs = filter(lambda x: not x.endswith("_moc.cpp"), excons.glob("src/ui/*.cpp"))
   srcs += Glob("src/ui/generated/*.cpp")

   qtmochdrs = ["src/ui/ExprBrowser.h",
                "src/ui/ExprColorCurve.h",
                "src/ui/ExprColorSwatch.h",
                "src/ui/ExprControl.h",
                "src/ui/ExprControlCollection.h",
                "src/ui/ExprCurve.h",
                "src/ui/ExprDialog.h",
                "src/ui/ExprFileDialog.h",
                "src/ui/ExprGrapher2d.h",
                "src/ui/ExprEditor.h",
                "src/ui/ExprPopupDoc.h",
                "src/ui/ExprShortEdit.h",
                "src/ui/ExprDeepWater.h"]
   qtmocsrcs = map(lambda x: str(env.GenerateMOC(x)[0]), qtmochdrs)

   prjs.append({"name": "SeExpr2Editor",
                "alias": "editor",
                "type": "program",
                "defs": ["SeExprEditor_BUILT_AS_STATIC"],
                "incdirs": ["src/ui", "src/ui/generated"],
                "srcs": qtmocsrcs + srcs,
                "custom": [RequireSeExpr2, gl.Require, RequireQt, dl.Require, threads.Require]})

build_opts = """SEEXPR OPTIONS
  generate-parser=0|1 : Generate parser sources using flex/bison if available. [0]
  use-llvm=0|1        : Build with LLVM backend.                               [0]
  with-llvm=<str>     : LLVM prefix.                                           []
  with-llvm-inc=<str> : LLVM includes path.                                    [<prefix>/include]
  with-llvm-lib=<str> : LLVM includes path.                                    [<prefix>/lib]
  with-qt=<str>       : Qt prefix.                                             []
  with-qt-inc=<str>   : Qt includes path.                                      [<prefix>/include]
  with-qt-lib=<str>   : Qt libraries path.                                     [<prefix>/lib]"""

excons.AddHelpOptions(seexpr=build_opts)
excons.AddHelpTargets({"python": "SeExpr python module",
                       "editor": "SeExpr expression editor",
                       "demos": "SeExpr sample programs"})

targets = excons.DeclareTargets(env, prjs)

insthdrs = env.Install(excons.OutputBaseDirectory() + "/include/SeExpr2", Glob("src/SeExpr/*.h"))
env.Depends(targets["SeExpr2"], insthdrs)

