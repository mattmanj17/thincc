@echo off
rem tdiff.bat
git --no-pager diff
exit /b %errorlevel%
