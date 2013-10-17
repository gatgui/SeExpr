import sys
import glob
import excons

env = excons.MakeBaseEnv()

if not sys.platform == "win32":
  libtype = ("staticlib" if int(ARGUMENTS.get("static", "0")) != 0 else "sharedlib")
  libs = ["dl", "pthread"]
  defs = []
else:
  libtype = "staticlib"
  libs = []
  defs = ["SEEXPR_WIN32"]

prjs = [
  {"name": "SeExpr",
   "type": libtype,
   "defs": defs,
   "incdirs": ["src/SeExpr", "src/SeExpr/generated"],
   "srcs": glob.glob("src/SeExpr/*.cpp") + glob.glob("src/SeExpr/generated/*.cpp"),
   "libs": libs,
   "install": {"include": glob.glob("src/SeExpr/*.h")}
  }
]

excons.DeclareTargets(env, prjs)
