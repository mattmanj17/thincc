@echo off
setlocal

rem commit.bat

rem Abort if anything is staged
set "git_diff=git diff --cached --quiet"
%git_diff%
set "rc=%errorlevel%"
if "%rc%"=="1" (
    >&2 echo -- ERROR: staged changes exist; aborting.
    exit /b 1
)
if not "%rc%"=="0" (
    >&2 echo -- ERROR: "%git_diff%" failed; aborting.
    exit /b %rc%
)

rem git status
set "git_status=git status --short"
%git_status%
set "rc=%errorlevel%"
if errorlevel 1 (
    >&2 echo -- ERROR: "%git_status%" failed; aborting.
    exit /b %rc%
)

rem git add
set "git_add=git add -A"
%git_add%
set "rc=%errorlevel%"
if errorlevel 1 (
    >&2 echo -- ERROR: "%git_add%" failed; aborting.
    exit /b %rc%
)

rem Exit if nothing has changed
%git_diff%
set "rc=%errorlevel%"
if "%rc%"=="0" (
    >&2 echo -- nothing to commit; exiting.
    goto reset
)
if not "%rc%"=="1" (
    >&2 echo -- ERROR: "%git_diff%" failed; aborting.
    goto reset
)

rem confirmation
set "confirm="
set /p "confirm=Commit all changes? [y/N]: "
if /i not "%confirm:~0,1%"=="y" (
    >&2 echo -- commit canceled; exiting.
    set "rc=0"
    goto reset
)

rem git commit
git commit
set "rc=%errorlevel%"

rem git reset
:reset
set "git_reset=git reset ."
%git_reset% >nul 2>nul
if errorlevel 1 (
    >&2 echo -- ERROR: "%git_reset%" failed; aborting.
    exit /b 1
)

rem the end
exit /b %rc%
