import os, platform
class Runtime:
    def __init__(self, ram_limit=None, cpu_limit=85, gpu_limit=80, workers=None):
        self.ram_limit=ram_limit; self.cpu_limit=cpu_limit; self.gpu_limit=gpu_limit; self.workers=workers or (os.cpu_count() or 1)
    def info(self): return {'platform':platform.platform(),'cpu_threads':os.cpu_count() or 1,'ram_limit':self.ram_limit,'cpu_limit':self.cpu_limit,'gpu_limit':self.gpu_limit,'workers':self.workers}
    def load(self,path): return Model(path,self)
from .model import Model
