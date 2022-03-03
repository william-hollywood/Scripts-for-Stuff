echo on
set repotomerge=%1
shift
set name=%1
shift
set url=%1
shift
set srcbranch=%1
shift
set oldcd=%CD%

cd %repotomerge%

git remote add %name% %url%

git fetch %name%

if "%srcbranch%"=="" (git checkout remotes/%name%/master) else (git checkout remotes/%name%/%srcbranch%)

set temp="..\%name%"

mkdir %temp%

move * %temp%

move %temp% .\
