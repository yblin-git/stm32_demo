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

:: 获取分支名
git rev-parse --abbrev-ref HEAD>>temp.ini
set /p branch=<temp.ini
del temp.ini

:: 获取commitId
git rev-parse HEAD>>temp.ini
set /p commitId=<temp.ini
del temp.ini

:: 获取commit对应时间
(git log --pretty=format:%%cd !commitId! -1 --date=format:"%%Y-%%m--%%d %%H:%%M:%%S")>>temp1.ini
set /p commitTime=<temp1.ini
del temp1.ini

rem 获取当前编译时间 2021-08-08 12：00：00
set compileTime=%date:~0,4%-%date:~5,2%-%date:~8,2% %time:~0,2%:%time:~3,2%:%time:~6,2%
set compileTime=%date:~0,4%%date:~5,2%%date:~8,2%

echo "branch:!branch!"
echo "version:!version!"
echo "commitId:!commitId!"
echo "compileTime:!compileTime!"

@REM :: 判断分支 赋值CustomeId
@REM set dev35_degree_branch=dev-35-degree-old
@REM set dev_branch=developer
@REM set agency_branch=agency
@REM if !branch!==!dev_branch! (
@REM     set CustomeId=0
@REM ) else if !branch!==!dev35_degree_branch! (
@REM     set CustomeId=0
@REM ) else if !branch!==!agency_branch! (
@REM     set CustomeId=2
@REM ) else (
@REM     echo not find branch
@REM     set CustomeId=other
@REM )

cd %dstDir%

::删除旧的bin文件
del OzoneGenerator_*.bin
echo "copy OzoneGenerator.bin --> OzoneGenerator_!version!_!compileTime!.bin"
copy OzoneGenerator.bin OzoneGenerator_!version!_!compileTime!.bin
