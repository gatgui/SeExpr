import os
import re
import sys
import glob
import subprocess
import excons
from excons.tools import gl
from excons.tools import dl
from excons.tools import threads


excons.SetArgument("use-c++11", 1)


# Check if editor is required
buildEditor = ("editor" in COMMAND_LINE_TARGETS or "SeExpr2Editor" in COMMAND_LINE_TARGETS)

def GenerateConfig(target, source, env):
   with open(str(source[0]), "r") as src:
      with open(str(target[0]), "w") as dst:
         e = re.compile("@([^@]+)@")
         for line in src.readlines():
            m = e.search(line)
            if m is not None:
               opt = m.group(1)
               if opt == "SEEXPR_ENABLE_LLVM_BACKEND":
                  # LLVM backend build not yet supported
                  line = line.replace(m.group(0), "0")
               else:
                  excons.WarnOne("Unsupported config option '%s'" % opt)
            dst.write(line)
   return None

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


env = excons.MakeBaseEnv()
env["BUILDERS"]["GenerateConfig"] = Builder(action=Action(GenerateConfig, "Generating $TARGET ...", suffix=".h", src_suffix=".h.in"))
env["BUILDERS"]["GenerateMOC"] = Builder(action=Action(GenerateMOC, "Generating $TARGET ..."), suffix="_moc.cpp")

# Library
libdefs = [] # ["__STDC_LIMIT_MACROS"]
if not sys.platform == "win32":
   env.Append(CPPFLAGS=" -msse4.1 -Wno-unused-parameter")
   if sys.platform == "darwin":
      #env.Append(CPPFLAGS=" -Wno-unneeded-internal-declaration")
      env.Append(CCFLAGS=" -Wno-date-time")
   else:
      env.Append(CCFLAGS=" -rdynamic")
else:
   libdefs.append("SEEXPR_WIN32")
libincs = ["src/SeExpr", "src/SeExpr/generated"]
libsrcs = glob.glob("src/SeExpr/*.cpp") + glob.glob("src/SeExpr/generated/*.cpp")
libsrcs.remove("src/SeExpr/ExprLLVMCodeGeneration.cpp")

env.GenerateConfig("src/SeExpr/ExprConfig.h.in")

# Declare targets
prjs = [
   {  "name": "SeExpr2_s",
      "type": "staticlib",
      "defs": libdefs,
      "incdirs": libincs,
      "srcs": libsrcs
   }
]

if sys.platform != "win32":
    prjs.append({"name": "SeExpr2",
                 "type": "sharedlib",
                 "defs": libdefs,
                 "incdirs": libincs,
                 "srcs": libsrcs,
                 "custom": [dl.Require, threads.Require]})

if buildEditor:
   srcs = filter(lambda x: not x.endswith("_moc.cpp"), glob.glob("src/ui/*.cpp"))
   srcs += glob.glob("src/ui/generated/*.cpp")

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
                "incdirs": ["src/SeExpr", "src/ui", "src/ui/generated"],
                "srcs": qtmocsrcs + srcs,
                "libs": ["SeExpr2_s"],
                "custom": [gl.Require, RequireQt, dl.Require, threads.Require]})

targets = excons.DeclareTargets(env, prjs)

insthdrs = env.Install(excons.OutputBaseDirectory() + "/include/SeExpr2", Glob("src/SeExpr/*.h"))
env.Depends(targets["SeExpr2_s"], insthdrs)
if "SeExpr2" in targets:
   env.Depends(targets["SeExpr2"], insthdrs)
