# Makefile.win32 for LibDutCom_0032 Win32/NMAKE

#CFG = Win32 Debug
CFG = Win32 Release

!if "$(CFG)" != "Win32 Release" && "$(CFG)" != "Win32 Debug"
!message Invalid configuration "$(CFG)" specified.
!message You can specify a configuration when running NMAKE
!message by defining the macro CFG on the command line. For example:
!message
!message NMAKE /f "Makefile.msc" CFG="Win32 Debug"
!message
!message Possible choices for configuration are:
!message
!message "Win32 Release" (based on "Win32 (x86) Library")
!message "Win32 Debug" (based on "Win32 (x86) Library")
!message
!error An invalid configuration is specified.
!endif

ObjSuf        = obj
SrcSuf        = c
ExeSuf        = exe
DllSuf        = dll


#
# set output file name @@@
#
TARGET = LibDutCom_0055

#
# add other project specific files as necessary @@@
#
ADDITIONALO   = Uds.$(ObjSuf)\
                Uds_private.$(ObjSuf)\
                Uds_IoControl.$(ObjSuf)\
                Uds_StoredData.$(ObjSuf)\
                ProdInterlock.$(ObjSuf)


#
# set additional include paths for cl.exe @@@
#
H_INCLUDES = -I"C:\PROGRA~1\Kvaser\CANLIB\INC"

#
# set additional library paths for link.exe @@@
#
LIB_INCLUDES = -LIBPATH:"C:\PROGRA~1\Kvaser\CANLIB\Lib\MS"


#############################################################
#
#
#  No further modification below here needed !!!
#
#
#############################################################


# Win32 system with Microsoft Visual C/C++
!include <win32.mak>
CC            = $(cc)
CXX           = $(cc)
CXXFLAGS      = -nologo -Gd -DWIN32 -D_WIN32 -D_WINDOWS -D_WIN32_WINNT=0x0400 \
                $(H_INCLUDES)
LD            = $(link)

!if "$(CFG)" == "Win32 Release"
CXXOPT        = -O2 -MD
LDOPT         = -opt:ref
RUNTIMELIBS   = msvcrt.lib kernel32.lib
!elseif "$(CFG)" == "Win32 Debug"
RUNTIMELIBS   = msvcrtd.lib kernel32.lib
!if "$(RUNTIME)" == "Debug"
CXXOPT        = -Z7 -MDd
!else
CXXOPT        = -Z7 -MD
!endif
LDOPT         = -debug
!endif

EXTRAFLAGS    = -G5


LDFLAGS       = $(LDOPT) $(conlflags) -nologo $(LIB_INCLUDES)
LDFLAGS       = $(LDOPT) -nologo $(LIB_INCLUDES)
SOFLAGS       = $(dlllflags:-pdb:none=)

CANLIB        = canlib32.lib
LIBS          = $(RUNTIMELIBS) $(CANLIB)

#------------------------------------------------------------------------------

LIBDUTCOMO    = LibDutCom.$(ObjSuf) LibDutCom_private.$(ObjSuf)
LIBDUTCOMS    = LibDutCom.$(SrcSuf) LibDutCom_private.$(SrcSuf)
LIBDUTCOMSO   = $(TARGET).$(DllSuf)

DEVSPECO      = DeviceSpecific.$(ObjSuf)
DEVSPECS      = DeviceSpecific.$(SrcSuf)

TRACEO        = Trace.$(ObjSuf)
TRACES        = Trace.$(SrcSuf)

QUEUEO        = Queue.$(ObjSuf)
QUEUES        = Queue.$(SrcSuf)

SVNIDSO       = SvnIds.$(ObjSuf)

OBJS          = $(LIBDUTCOMO) $(DEVSPECO) $(TRACEO) $(QUEUEO) $(SVNIDSO) $(ADDITIONALO)


all:            $(LIBDUTCOMSO)

$(LIBDUTCOMSO): $(OBJS)
                zz_update_svn_ids.cmd
                $(LD) $(SOFLAGS) $(LDFLAGS) $(OBJS) $(LIBS) -out:$(LIBDUTCOMSO)
                copy $(LIBDUTCOMSO) ..\..\Test\current
                rem @del *.obj *.lib *.def *.exp >nul 2>&1
                dumpbin /all /rawdata:none $(LIBDUTCOMSO) > $(TARGET).map
                @del LibDutCom.obj *.lib >nul 2>&1
                @echo +----------------------------
                @echo I $(LIBDUTCOMSO) done
                @echo +----------------------------

clean:
      @del *.obj *.lib *.def *.exp >nul 2>&1

###
.$(SrcSuf).$(ObjSuf):
        $(CXX) $(CXXFLAGS) $(EXTRAFLAGS) $(CXXOPT) -c $<

