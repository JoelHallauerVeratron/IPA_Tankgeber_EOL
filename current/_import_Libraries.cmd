@echo off
@set SOURCE=\Projects\Libraries\LibTesterGui\LibTesterGui_Test\current
@set TARGET=.
@echo on
copy %SOURCE%\LibTesterGui_0001* %TARGET%
copy %SOURCE%\LibTesterGui_0003* %TARGET%
copy %SOURCE%\LibTesterGui_0007* %TARGET%

copy %SOURCE%\LibTesterGui_0009* %TARGET%
copy %SOURCE%\LibTesterGui_Mgr* %TARGET%

@echo off
@set SOURCE=\Projects\Libraries\LibDutCom\Test\current
@set TARGET=.
@echo on
copy %SOURCE%\LibDutCom_0056.dll %TARGET%

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
@set LIBRARY=LibNfc
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set LIBRARY=LibIntermecPrinter
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%

@echo off
@set SOURCE=\Projects\OceanLink\OL_D85\Label\PrinterTest\current
@set TARGET=.
@echo on
copy %SOURCE%\LabelPrt09* %TARGET%

@echo off
@set LIBRARY=LibWin32Prt
@set SOURCE=\Projects\Libraries\CVI\%LIBRARY%\current\zz_to_distribute_TM88
@set TARGET=.
@echo on
copy %SOURCE%\* %TARGET%



Pause
