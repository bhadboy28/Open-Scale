# OpenScale on Windows (MSYS2 UCRT64)

1. Install MSYS2 UCRT64 and make sure these files exist:
   - `C:\msys64\ucrt64\bin\g++.exe`
   - `C:\msys64\ucrt64\bin\mingw32-make.exe`
2. Install CMake and ensure `C:\Program Files\CMake\bin\cmake.exe` exists.
3. In PowerShell from the OpenScale folder:

```powershell
$env:Path += ";C:\Program Files\CMake\bin;C:\msys64\ucrt64\bin"
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_MAKE_PROGRAM=C:/msys64/ucrt64/bin/mingw32-make.exe
cmake --build build --parallel 2
ctest --test-dir build --output-on-failure
.\build\openscale.exe hardware
.\build\openscale.exe modules
```

If CMake says the compiler or make program cannot be found, run `Test-Path` on the two executable paths above. Do not run `g++ main.cpp` because OpenScale is a CMake project and has multiple translation units.
