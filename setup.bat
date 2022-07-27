@echo off

:: Name of virtual environment
set VENV_NAME=hin

:: Remember current working directory
pushd .

:: Deactivate any virtual environment the script is running in
if defined VIRTUAL_ENV call deactivate

:: Install virtualenvwrapper-win if not installed yet
C:\Python38\Scripts\pip install virtualenv virtualenvwrapper-win poetry > nul 2>&1

:: Create virtual environment if it does not exist
call lsvirtualenv | findstr /b /e %VENV_NAME% > nul || (
    call mkvirtualenv -a C:\HIN\interview %VENV_NAME%
)

:: Activate virtual environment
call workon %VENV_NAME%

:: Navigate to the virtual environment's site-packages directory
call cdsitepackages

:: Add directories to path file if they are not there yet
for %%d in (
    C:\HIN\interview\app\lib\win64\dll
    C:\HIN\interview\app\lib\win64\dll\Debug
    C:\HIN\interview\app\py
    C:\HIN\interview
) do (
    findstr /b /e %%d virtualenv_path_extensions.pth > nul 2>&1 || (
        call add2virtualenv %%d
    )
)

:: Go to project's root directory
call cdproject

:: Install required Python dependencies
echo y | poetry cache clear . --all
poetry install

:: Deactivate virtual environment
call deactivate

:: Restore old working directory
popd

:: Restore default color scheme in case pip messes it up
color
