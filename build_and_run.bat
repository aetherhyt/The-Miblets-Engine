@echo off
echo =======================================
echo 1. Configuring Miblets Engine...
echo =======================================
cmake -S . -B build
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed!
    exit /b %errorlevel%
)

echo.
echo =======================================
echo 2. Building Miblets Engine...
echo =======================================
cmake --build build --config Debug
if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    exit /b %errorlevel%
)

echo.
echo =======================================
echo 3. Running MibletsGame...
echo =======================================
cd build
.\Game\MibletsGame\Debug\MibletsGame.exe
cd ..
