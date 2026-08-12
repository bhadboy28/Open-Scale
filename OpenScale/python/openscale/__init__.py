__version__='0.1.0'
from .runtime import Runtime
from .model import Model

def load(path, **kwargs):
    return Model(path, Runtime(**kwargs))
