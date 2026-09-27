import os
import sys
import subprocess
from typing import Optional, Dict, Any

class Runtime:
    def __init__(self):
        # Locate the compiled openscale.exe binary
        base_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "build"))
        self.exe_path = os.path.join(base_dir, "openscale.exe")
        
        if not os.path.exists(self.exe_path):
            alt_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "OpenScale", "build", "openscale.exe"))
            if os.path.exists(alt_path):
                self.exe_path = alt_path
            else:
                raise FileNotFoundError(f"OpenScale executable not found at {self.exe_path}. Did you compile it?")
            
        self.env = os.environ.copy()
        for p in [r"C:\msys64\ucrt64\bin", r"C:\msys64\mingw64\bin", os.path.dirname(self.exe_path)]:
            if os.path.isdir(p) and p not in self.env.get("PATH", ""):
                self.env["PATH"] = p + ";" + self.env.get("PATH", "")
                
    def load(self, model_path: str):
        return NativeModel(self.exe_path, self.env, model_path)
        
    def hardware(self) -> str:
        try:
            res = subprocess.run([self.exe_path, "hardware"], capture_output=True, text=True, env=self.env, timeout=5)
            return res.stdout.strip()
        except Exception as e:
            return f"Hardware query error: {e}"

    def modules(self) -> str:
        try:
            res = subprocess.run([self.exe_path, "modules"], capture_output=True, text=True, env=self.env, timeout=5)
            return res.stdout.strip()
        except Exception as e:
            return f"Modules query error: {e}"

    def info(self) -> str:
        hw = self.hardware()
        return f"OpenScale Native Engine ({hw})"

class NativeModel:
    def __init__(self, exe_path: str, env: dict, model_path: str):
        self.exe_path = exe_path
        self.env = env
        self.model_path = os.path.abspath(model_path)
        self.model_name = os.path.basename(model_path)
        
    def plan(self) -> str:
        try:
            res = subprocess.run([self.exe_path, "plan", self.model_path], capture_output=True, text=True, env=self.env, timeout=10)
            return res.stdout.strip() or res.stderr.strip()
        except Exception as e:
            return f"Plan error: {e}"

    def generate(self, prompt: str, max_tokens: int = 2048) -> str:
        try:
            res = subprocess.run([self.exe_path, "run", self.model_path, "--prompt", prompt], capture_output=True, text=True, env=self.env, timeout=10)
            out = res.stdout.strip()
            if not out and res.stderr:
                out = res.stderr.strip()
            return out
        except Exception as e:
            return f"Execution error: {e}"
