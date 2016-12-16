@echo off & setlocal

cd "%~dp0" || exit /b 1

for /d %%d in ("build-VideoIO*" "build-probe*") do rd /s /q "%%~d"
del /q ".\qmake\*pro.user*"
del /q ".\data\output.*"
for /d /r ".\install" %%d in ("lib") do (
    if exist "%%~d" (
        del /q "%%~d\*.dll"
        del /q "%%~d\*.exp"
    )
)
