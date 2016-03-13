import sys
import glob
import excons
from excons.tools import gl
from excons.tools import dl
from excons.tools import threads

env = excons.MakeBaseEnv()

# Library
if not sys.platform == "win32":
  libtype = ("staticlib" if excons.GetArgument("static", "0", int) != 0 else "sharedlib")
  defs = []
  env.Append(CPPFLAGS=" -msse4.1")
  if sys.platform == "darwin":
     env.Append(CPPFLAGS=" -Wno-unneeded-internal-declaration")
else:
  libtype = "staticlib"
  defs = ["SEEXPR_WIN32"]

# Editor
def GenerateMOC(hdr):
   import os
   import subprocess
   dn, bn = os.path.split(hdr)
   bn, ext = os.path.splitext(bn)
   cmd = "moc \"%s/%s%s\" -o \"%s/%s_moc.cpp\"" % (dn, bn, ext, dn, bn)
   p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
   out, _ = p.communicate()
   if p.returncode != 0:
      raise Exception("MOC failed for %s" % hdr)

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

qtmochdrs = ["SeExprEdBrowser.h",
             "SeExprEdColorCurve.h",
             "SeExprEdColorSwatchWidget.h",
             "SeExprEdControlCollection.h",
             "SeExprEdControl.h",
             "SeExprEdCurve.h",
             "SeExprEdDialog.h",
             "SeExprEditor.h",
             "SeExprEdFileDialog.h",
             "SeExprEdGrapher2d.h",
             "SeExprEdPopupDocumentation.h",
             "SeExprEdShortEdit.h"]

for hdr in qtmochdrs:
   GenerateMOC("src/SeExprEditor/%s" % hdr)

eddefs = []
if libtype == "staticlib":
   eddefs.append("SeExprEditor_BUILT_AS_STATIC")

edrequires = [gl.Require, RequireQt]
if libtype == "staticlib":
   edrequires.extend([dl.Require, threads.Require])

# Declare targets
prjs = [
  {"name": "SeExpr",
   "type": libtype,
   "defs": defs,
   "incdirs": ["src/SeExpr", "src/SeExpr/generated"],
   "srcs": glob.glob("src/SeExpr/*.cpp") + glob.glob("src/SeExpr/generated/*.cpp"),
   "custom": ([] if libtype == "staticlib" else [dl.Require, threads.Require]),
   "install": {"include": glob.glob("src/SeExpr/*.h")}
  },
  {"name": "SeExprEditor",
   "type": "program",
   "defs": eddefs,
   "incdirs": ["src/SeExpr", "src/SeExprEditor", "src/SeExprEditor/generated"],
   "srcs": glob.glob("src/SeExprEditor/*.cpp") + glob.glob("src/SeExprEditor/generated/*.cpp"),
   "libs": ["SeExpr"],
   "custom": edrequires
  }
]

excons.DeclareTargets(env, prjs)

Default("SeExpr")
