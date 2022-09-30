@echo off
:: 变量延迟执行
setlocal enabledelayedexpansion
:: 在该目录执行git命令获取版本号（也就是git仓库目录）  传参为脚本的第2个参数
set baseDir=%~dp0
:: 将版本信息写入的目标文件夹 传参为脚本的第1个参数
set dstDir=MDK-ARM

cd %baseDir%../
if exist temp.ini del temp.ini
:: 获取版本号
git describe --tags --abbrev=0 >>temp.ini
set /p version=<temp.ini
del temp.ini

echo "version:!version!"
@REM set remain=%version%
@REM set offset=0
@REM :loop
@REM for /f "tokens=1* delims=_" %%a in ("%remain%") do (
@REM     ::输出第一个分段(令牌)
@REM     echo %%a
@REM     rem 将截取剩下的部分赋给变量remain，其实这里可以使用延迟变量开关
@REM     set remain=%%b
@REM     set /a offset=offset+1
@REM     if %offset%==2 (
@REM         set sub_version=%%a
@REM     )
@REM )
@REM ::如果还有剩余,则继续分割
@REM if defined remain goto :loop

@REM echo sub_version: !sub_version!

cd %baseDir%
if exist version.h del version.h
@REM echo "echo "!sub_version:~1!">>version.h"
echo "echo "!version!">>version.h"
echo #define VERSION									"!version!">>version.h