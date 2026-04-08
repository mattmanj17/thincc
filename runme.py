import subprocess
import sys
from pathlib import Path

def need_f(path):
    if not Path(path).is_file():
        sys.exit(f"File not found: {path}")
def clean_f(path):
    Path(path).unlink(missing_ok=True)

name = "thincc"
src = name + ".c"
obj = name + ".obj"
exe = name + ".exe"

need_f(src)
clean_f(exe)
subprocess.run(
    [
        "cl", "-nologo",
        "/Wall", "/WX",
        src,
        "/link", "/WX",
    ]
)
clean_f(obj)
need_f(exe)
with open(src, "rb") as f:
    result = subprocess.run(
        [exe], 
        stdin=f,
        capture_output=True,
        text=False
    )

print("Testing...")
if Path(src).read_bytes() != result.stdout:
    print("Failed to echo stdin!")
else:
    print("OK!")