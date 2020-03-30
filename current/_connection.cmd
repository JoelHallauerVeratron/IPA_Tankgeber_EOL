@echo off

set TARGET=K:

set LOGIN=Backup
set PASSWORD="PROD_RHI_BACKUP1"
set HOST=192.168.20.110

net use %TARGET% \\%HOST%\Produktion %PASSWORD% /user:%LOGIN%
echo new drive = %TARGET%

:keep_alive
time /t > %TARGET%\time.txt
echo wait 60 s
sleep 60
goto keep_alive