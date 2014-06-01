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
   def GenerateMOC(src):
      pass
   
   def RequireQt(env):
      env.Append(LINKFLAGS = " -framework QtCore -framework QtGui -framework QtOpenGL")

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
