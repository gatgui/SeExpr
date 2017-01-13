import os
import sys
import glob
import subprocess
import excons
from excons.tools import gl
from excons.tools import dl
from excons.tools import threads

# Check if editor is required
buildEditor = ("editor" in COMMAND_LINE_TARGETS or "SeExprEditor" in COMMAND_LINE_TARGETS)

def GenerateMOC(target, source, env):
   cmd = "moc \"%s\" -o \"%s\"" % (source[0], target[0])
   p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
   out, _ = p.communicate()
   if p.returncode != 0:
      excons.WarnOnce("MOC generation failed for '%s'. SeExprEditor won't build." % target[0])
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

# Library
libdefs = []
if not sys.platform == "win32":
  env.Append(CPPFLAGS=" -msse4.1")
  if sys.platform == "darwin":
     env.Append(CPPFLAGS=" -Wno-unneeded-internal-declaration")
else:
  libdefs.append("SEEXPR_WIN32")
libincs = ["src/SeExpr", "src/SeExpr/generated"]
libsrcs = glob.glob("src/SeExpr/*.cpp") + glob.glob("src/SeExpr/generated/*.cpp")

# Declare targets
prjs = [
   {  "name": "SeExpr_s",
      "type": "staticlib",
      "defs": libdefs,
      "incdirs": libincs,
      "srcs": libsrcs
   }
]

if sys.platform != "win32":
  prjs.append({"name": "SeExpr",
               "type": "sharedlib",
                "defs": libdefs,
                "incdirs": libincs,
                "srcs": libsrcs,
                "custom": [dl.Require, threads.Require]})

if buildEditor:
   env["BUILDERS"]["GenerateMOC"] = Builder(action=Action(GenerateMOC, "Generating $TARGET ..."), suffix="_moc.cpp")

   srcs = filter(lambda x: not x.endswith("_moc.cpp"), glob.glob("src/SeExprEditor/*.cpp"))
   srcs += glob.glob("src/SeExprEditor/generated/*.cpp")

   qtmochdrs = ["src/SeExprEditor/SeExprEdBrowser.h",
                "src/SeExprEditor/SeExprEdColorCurve.h",
                "src/SeExprEditor/SeExprEdColorSwatchWidget.h",
                "src/SeExprEditor/SeExprEdControl.h",
                "src/SeExprEditor/SeExprEdControlCollection.h",
                "src/SeExprEditor/SeExprEdCurve.h",
                "src/SeExprEditor/SeExprEdDialog.h",
                "src/SeExprEditor/SeExprEdFileDialog.h",
                "src/SeExprEditor/SeExprEdGrapher2d.h",
                "src/SeExprEditor/SeExprEditor.h",
                "src/SeExprEditor/SeExprEdPopupDocumentation.h",
                "src/SeExprEditor/SeExprEdShortEdit.h"]
   qtmocsrcs = map(lambda x: str(env.GenerateMOC(x)[0]), qtmochdrs)

   prjs.append({"name": "SeExprEditor",
                "alias": "editor",
                "type": "program",
                "defs": ["SeExprEditor_BUILT_AS_STATIC"],
                "incdirs": ["src/SeExpr", "src/SeExprEditor", "src/SeExprEditor/generated"],
                "srcs": qtmocsrcs + srcs,
                "libs": ["SeExpr_s"],
                "custom": [gl.Require, RequireQt, dl.Require, threads.Require]})

targets = excons.DeclareTargets(env, prjs)

insthdrs = env.Install(excons.OutputBaseDirectory() + "/include", glob.glob("src/SeExpr/*.h"))
env.Depends(targets["SeExpr_s"], insthdrs)
if "SeExpr" in targets:
   env.Depends(targets["SeExpr"], insthdrs)
