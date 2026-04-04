@echo off
setlocal

rem diff.bat

rem Abort if anything is already staged
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

rem temporarily mark untracked files intent-to-add
set "git_add=git add -N ."
%git_add% >nul 2>nul
if errorlevel 1 (
    >&2 echo -- ERROR: "%git_add%" failed; aborting.
    exit /b 1
)

rem sic beyond compare on it
git difftool -y -d --extcmd=bcompare
set "rc=%errorlevel%"

rem remove temporary intent-to-add state
set "git_reset=git reset ."
%git_reset% >nul 2>nul
if errorlevel 1 (
    >&2 echo -- ERROR: "%git_reset%" failed; aborting.
    exit /b 1
)

rem the end
exit /b %rc%
