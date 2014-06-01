import sys
import glob
import excons
from excons.tools import gl

env = excons.MakeBaseEnv()

qtmochdrs = ["SeExprEdBrowser.h",
             "SeExprEdColorCurve.h",
             "SeExprEdControlCollection.h",
             "SeExprEdControl.h",
             "SeExprEdCurve.h",
             "SeExprEdDialog.h",
             "SeExprEditor.h",
             "SeExprEdFileDialog.h",
             "SeExprEdGrapher2d.h",
             "SeExprEdPopupDocumentation.h",
             "SeExprEdShortEdit.h"]

if sys.platform == "darwin":
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
      env.Append(LINKFLAGS = " -framework QtCore -framework QtGui -framework QtOpenGL")

for hdr in qtmochdrs:
   GenerateMOC("src/SeExprEditor/%s" % hdr)

if not sys.platform == "win32":
  libtype = ("staticlib" if int(ARGUMENTS.get("static", "0")) != 0 else "sharedlib")
  libs = ["dl", "pthread"]
  defs = []
  eddefs = []
else:
  libtype = "staticlib"
  libs = []
  defs = ["SEEXPR_WIN32"]
  eddefs = ["SeExprEditor_BUILT_AS_STATIC"]

prjs = [
  {"name": "SeExpr",
   "type": libtype,
   "defs": defs,
   "incdirs": ["src/SeExpr", "src/SeExpr/generated"],
   "srcs": glob.glob("src/SeExpr/*.cpp") + glob.glob("src/SeExpr/generated/*.cpp"),
   "libs": libs,
   "install": {"include": glob.glob("src/SeExpr/*.h")}
  },
  {"name": "SeExprEditor",
   "type": "program",
   "defs": eddefs,
   "incdirs": ["src/SeExpr", "src/SeExprEditor", "src/SeExprEditor/generated"],
   "srcs": glob.glob("src/SeExprEditor/*.cpp") + glob.glob("src/SeExprEditor/generated/*.cpp"),
   "libs": ["SeExpr"],
   "custom": [gl.Require, RequireQt]
  }
]

excons.DeclareTargets(env, prjs)


