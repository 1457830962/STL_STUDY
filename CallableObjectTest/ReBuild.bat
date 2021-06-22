echo off
REM ************************************************************************************************
REM * Script to compile the solutions of VS2008                                                    *
REM * Created by: kandy                                                                      	   *
REM * Created 2019.01.15                                                                           *
REM ************************************************************************************************
 
REM STEP 1: set environment variables:
REM ************************************************************************************************
SET VSCompiler="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\Common7\IDE\devenv.exe"
echo %VSCompiler%
REM ************************************************************************************************
 
REM STEP 2: set compile log
REM ************************************************************************************************
SET CompileLogName="BuildResult.log"
ECHO [%DATE% %Time%] Start compile sequence >%CompileLogName%
ECHO Used compile configuration is %buildAnyCPU% >>%CompileLogName%
REM ************************************************************************************************
 
REM STEP 3: Start compile
REM ************************************************************************************************
SET SolutionFileName="C:\Users\admin\Desktop\CmakeTest\Demo1.sln"

REM 完整编译第一个版本
%VSCompiler% %SolutionFileName% /Rebuild "Release|x64" /Out %CompileLogName%
IF NOT %errorlevel% == 0 ECHO %SolutionFileName% failed!   Error: %errorlevel% >>%CompileLogName%
IF %errorlevel% == 0 ECHO %SolutionFileName% compiled successful >>%CompileLogName%
REM ************************************************************************************************
 
REM STEP 4: If compile failed stop processing:
REM ************************************************************************************************
IF NOT %errorlevel% == 0 PAUSE
 
ECHO [%DATE% %Time%] Finished compile sequence >>%CompileLogName%
REM ************************************************************************************************


REM 完整编译第一个版本
REM %VSCompiler% %SolutionFileName% /build "Release|x64" /Out %CompileLogName%
REM IF NOT %errorlevel% == 0 ECHO %SolutionFileName% failed!   Error: %errorlevel% >>%CompileLogName%
REM IF %errorlevel% == 0 ECHO %SolutionFileName% compiled successful >>%CompileLogName%
REM ************************************************************************************************
 

REM STEP 4: If compile failed stop processing:
REM ************************************************************************************************
IF NOT %errorlevel% == 0 PAUSE
 
ECHO [%DATE% %Time%] Finished compile sequence >>%CompileLogName%
REM ************************************************************************************************

