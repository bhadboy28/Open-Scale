class Model:
    def __init__(self,path,runtime): self.path=str(path); self.runtime=runtime; self.loaded=False
    def load(self): self.loaded=True; return self
    def generate(self,prompt,max_tokens=32):
        if not self.loaded:self.load()
        raise NotImplementedError('Model graph execution is the next OpenScale milestone; v0.1 provides the native runtime foundation.')
