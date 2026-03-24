@echo off
setlocal EnableExtensions EnableDelayedExpansion

set "REPO_ROOT=%~dp0"
pushd "%REPO_ROOT%" >nul 2>&1
if errorlevel 1 (
    echo [bootstrap] Failed to enter repository root.
    exit /b 1
)

call :load_env ".env"

echo [bootstrap] Generating Papyrus project...
xmake papyrus.project papyrus
if errorlevel 1 goto :fail

echo [bootstrap] Building project...
xmake f -m release
xmake
if errorlevel 1 goto :fail

echo [bootstrap] Done.
popd >nul 2>&1
exit /b 0

:load_env
set "ENV_FILE=%~1"
if not exist "%ENV_FILE%" exit /b 0

for /f "usebackq eol=# tokens=1* delims==" %%A in ("%ENV_FILE%") do (
    if not "%%~A"=="" (
        set "KEY=%%~A"
        set "VALUE=%%~B"

        if defined VALUE (
            if "!VALUE:~0,1!"=="\"" if "!VALUE:~-1!"=="\"" set "VALUE=!VALUE:~1,-1!"
            if "!VALUE:~0,1!"=="'" if "!VALUE:~-1!"=="'" set "VALUE=!VALUE:~1,-1!"
        )

        set "!KEY!=!VALUE!"
    )
)
exit /b 0

:fail
echo [bootstrap] Failed.
popd >nul 2>&1
exit /b 1
