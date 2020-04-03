@echo off
@set SOURCE=\Projects\Libraries\LibTesterGui\LibTesterGui_Test\current
@set TARGET=.
@echo on
copy %SOURCE%\LibTesterGui_0001* %TARGET%
copy %SOURCE%\LibTesterGui_0004* %TARGET%

copy %SOURCE%\LibTesterGui_0009* %TARGET%
copy %SOURCE%\LibTesterGui_Mgr* %TARGET%

@echo off
@set SOURCE=\Projects\Libraries\LibDutCom\Test\current
@set TARGET=.
@echo on
copy %SOURCE%\LibDutCom_0055.dll %TARGET%

@echo off
@set SOURCE=\Projects\Libraries\LibDutCom\0000\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=NumPanel
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=LibSelectorPanel
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=AlphaPanel
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=LibErrorPanel
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=LibNetworking
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=SerialNumber
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@echo on
copy %SOURCE%\* %TARGET%


@echo off
@set LIBRARY=LibWin32Prt
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute_TM88
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%



Pause
