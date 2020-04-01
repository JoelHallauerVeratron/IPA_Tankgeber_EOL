@set CVIDIR=c:\program files\ni\cvi71
@set CVIPROJPATH=c:\Projects\NMEA2K\Kapazitiver_Tankgeber_EOL\current\LinkUp_EOL.prj
@set CVIPROJDIR=c:\Projects\NMEA2K\Kapazitiver_Tankgeber_EOL\current
@set CVIPROJNAME=LinkUp_EOL.prj
@set CVITARGETPATH=c:\Projects\NMEA2K\Kapazitiver_Tankgeber_EOL\current\LinkUp_EOL.exe
@set CVITARGETDIR=c:\Projects\NMEA2K\Kapazitiver_Tankgeber_EOL\current
@set CVITARGETNAME=LinkUp_EOL.exe
@set CVIBUILDCONFIG=Release
@set CVIVXIPNPDIR=C:\VXIPNP\winnt
@set CVIIVIDIR=C:\Program Files\IVI
@set CVIWINDIR=C:\WINDOWS
@set CVISYSDIR=C:\WINDOWS\system32
del "%CVIPROJDIR%\Build_Date.h"
my_date_time.exe -pP > "%CVIPROJDIR%\Build_Date.h"
zz_update_svn_ids.cmd
