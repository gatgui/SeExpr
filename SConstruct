import sys
import glob
import excons

env = excons.MakeBaseEnv()

if not sys.platform == "win32":
  libtype = ("staticlib" if int(ARGUMENTS.get("static", "0")) != 0 else "sharedlib")
  libs = ["dl", "pthread"]
else:
  libtype = "staticlib"

prjs = [
  {"name": "SeExpr",
   "type": libtype,
   "incdirs": ["src/SeExpr", "src/SeExpr/generated"],
   "srcs": glob.glob("src/SeExpr/*.cpp") + glob.glob("src/SeExpr/generated/*.cpp"),
   "libs": libs,
   "install": {"include": glob.glob("src/SeExpr/*.h")}
  }
]

excons.DeclareTargets(env, prjs)
