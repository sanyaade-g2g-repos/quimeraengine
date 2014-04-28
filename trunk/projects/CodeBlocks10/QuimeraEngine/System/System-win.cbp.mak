#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.137                       #
#------------------------------------------------------------------------------#


WORKDIR = %cd%

CC = gcc.exe
CXX = g++.exe
AR = ar.exe
LD = g++.exe
WINDRES = windres.exe

#DebugWin32SharedrtStatic
INC_DEBUGWIN32SHAREDRTSTATIC =  -I$(MAKEPATH)/trunk/headers -I$(MAKEPATH)/trunk/3rdparty/Boost/include -I$(MAKEPATH)trunk/3rdparty/ICU/include
CFLAGS_DEBUGWIN32SHAREDRTSTATIC =  -Wshadow -Wredundant-decls -Wcast-align -Winline -Wmissing-include-dirs -Wswitch-default -std=c++98 -Wall -g -D_DEBUG
RESINC_DEBUGWIN32SHAREDRTSTATIC =  
RCFLAGS_DEBUGWIN32SHAREDRTSTATIC =  
LIBDIR_DEBUGWIN32SHAREDRTSTATIC =  -L..\..\..\..\3rdparty\Boost\bin\Win32\DebugSharedrtStatic
LIB_DEBUGWIN32SHAREDRTSTATIC = 
LDFLAGS_DEBUGWIN32SHAREDRTSTATIC =  
OBJDIR_DEBUGWIN32SHAREDRTSTATIC = $(MAKEPATH)\\trunk\\garbage\\CodeBlocks10\\DebugWin32SharedrtStatic
DEP_DEBUGWIN32SHAREDRTSTATIC = 
OUT_DEBUGWIN32SHAREDRTSTATIC = $(MAKEPATH)\\trunk\\bin\\CodeBlocks10\\DebugWin32SharedrtStatic\\QuimeraEngineSystem.lib
OBJ_DEBUGWIN32SHAREDRTSTATIC = $(OBJDIR_DEBUGWIN32SHAREDRTSTATIC)$(MAKEPATH__)\\trunk\\source\\QObject.o

#DebugWin32SharedrtDynamic
INC_DEBUGWIN32SHAREDRTDYNAMIC =  -I$(MAKEPATH)/trunk/headers -I$(MAKEPATH)/trunk/3rdparty/Boost/include -I$(MAKEPATH)trunk/3rdparty/ICU/include
CFLAGS_DEBUGWIN32SHAREDRTDYNAMIC =  -Wshadow -Wredundant-decls -Wcast-align -Winline -Wmissing-include-dirs -Wswitch-default -std=c++98 -Wall -g -D_DEBUG -DQE_PREPROCESSOR_IMPORTLIB_COMMON -DQE_PREPROCESSOR_IMPORTLIB_TOOLS -DQE_PREPROCESSOR_EXPORTLIB_SYSTEM
RESINC_DEBUGWIN32SHAREDRTDYNAMIC =  
RCFLAGS_DEBUGWIN32SHAREDRTDYNAMIC =  
LIBDIR_DEBUGWIN32SHAREDRTDYNAMIC =  -L$(MAKEPATH)\trunk\3rdparty\Boost\bin\Win32\DebugSharedrtStatic -L$(MAKEPATH)\\trunk\\3rdparty\\ICU\\bin\\Win32\\ReleaseSharedrtDynamic\\MinGW
LIB_DEBUGWIN32SHAREDRTDYNAMIC = $(MAKEPATH)\\trunk\\bin\CodeBlocks10\DebugWin32SharedrtDynamic\QuimeraEngineCommon.dll $(MAKEPATH)\\trunk\\bin\CodeBlocks10\DebugWin32SharedrtDynamic\QuimeraEngineTools.dll -lboost_system-mt-d -lboost_thread-mt-d -licuuc.dll -licudt.dll -licuin.dll
LDFLAGS_DEBUGWIN32SHAREDRTDYNAMIC =  
OBJDIR_DEBUGWIN32SHAREDRTDYNAMIC = $(MAKEPATH)\\trunk\\garbage\\CodeBlocks10\\DebugWin32SharedrtDynamic
DEP_DEBUGWIN32SHAREDRTDYNAMIC = 
OUT_DEBUGWIN32SHAREDRTDYNAMIC = $(MAKEPATH)\\trunk\\bin\\CodeBlocks10\\DebugWin32SharedrtDynamic\\QuimeraEngineSystem.dll
OBJ_DEBUGWIN32SHAREDRTDYNAMIC = $(OBJDIR_DEBUGWIN32SHAREDRTDYNAMIC)$(MAKEPATH__)\\trunk\\source\\QObject.o

all: debugwin32sharedrtstatic debugwin32sharedrtdynamic

clean: clean_debugwin32sharedrtstatic clean_debugwin32sharedrtdynamic

#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
#	DebugWin32SharedrtStatic   ~~~   DebugWin32SharedrtStatic   ~~~   DebugWin32SharedrtStatic   ~~~   DebugWin32SharedrtStatic   ~~~   DebugWin32SharedrtStatic   ~~~       #  
#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
	
before_debugwin32sharedrtstatic: 
	cmd /c if not exist $(MAKEPATH)\\trunk\\bin\\CodeBlocks10\\DebugWin32SharedrtStatic md $(MAKEPATH)\\trunk\\bin\\CodeBlocks10\\DebugWin32SharedrtStatic
	cmd /c if not exist $(OBJDIR_DEBUGWIN32SHAREDRTSTATIC)$(MAKEPATH__)\\trunk\\source md $(OBJDIR_DEBUGWIN32SHAREDRTSTATIC)$(MAKEPATH__)\\trunk\\source

after_debugwin32sharedrtstatic: 

debugwin32sharedrtstatic: before_debugwin32sharedrtstatic out_debugwin32sharedrtstatic after_debugwin32sharedrtstatic

out_debugwin32sharedrtstatic: $(OBJ_DEBUGWIN32SHAREDRTSTATIC) $(DEP_DEBUGWIN32SHAREDRTSTATIC)
	$(AR) rcs $(OUT_DEBUGWIN32SHAREDRTSTATIC) $(OBJ_DEBUGWIN32SHAREDRTSTATIC)

$(OBJDIR_DEBUGWIN32SHAREDRTSTATIC)$(MAKEPATH__)\\trunk\\source\\QObject.o: $(MAKEPATH)\\trunk\\source\\QObject.cpp
	$(CXX) $(CFLAGS_DEBUGWIN32SHAREDRTSTATIC) $(INC_DEBUGWIN32SHAREDRTSTATIC) -c $(MAKEPATH)\\trunk\\source\\QObject.cpp -o $(OBJDIR_DEBUGWIN32SHAREDRTSTATIC)$(MAKEPATH__)\\trunk\\source\\QObject.o

clean_debugwin32sharedrtstatic: 


#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
#	DebugWin32SharedrtDynamic   ~~~   DebugWin32SharedrtDynamic   ~~~   DebugWin32SharedrtDynamic   ~~~   DebugWin32SharedrtDynamic   ~~~   DebugWin32SharedrtDynamic   ~~~       #  
#----------------------------------------------------------------------------------------------------------------------------------------------------------------------------#
	
before_debugwin32sharedrtdynamic: 
	cmd /c if not exist $(MAKEPATH)\\trunk\\bin\\CodeBlocks10\\DebugWin32SharedrtDynamic md $(MAKEPATH)\\trunk\\bin\\CodeBlocks10\\DebugWin32SharedrtDynamic
	cmd /c if not exist $(OBJDIR_DEBUGWIN32SHAREDRTDYNAMIC)$(MAKEPATH__)\\trunk\\source md $(OBJDIR_DEBUGWIN32SHAREDRTDYNAMIC)$(MAKEPATH__)\\trunk\\source

after_debugwin32sharedrtdynamic: 

debugwin32sharedrtdynamic: before_debugwin32sharedrtdynamic out_debugwin32sharedrtdynamic after_debugwin32sharedrtdynamic

out_debugwin32sharedrtdynamic: $(OBJ_DEBUGWIN32SHAREDRTDYNAMIC) $(DEP_DEBUGWIN32SHAREDRTDYNAMIC)
	$(LD) -shared $(LDFLAGS_DEBUGWIN32SHAREDRTDYNAMIC) $(LIBDIR_DEBUGWIN32SHAREDRTDYNAMIC) $(OBJ_DEBUGWIN32SHAREDRTDYNAMIC) $(LIB_DEBUGWIN32SHAREDRTDYNAMIC) -o $(OUT_DEBUGWIN32SHAREDRTDYNAMIC) 

$(OBJDIR_DEBUGWIN32SHAREDRTDYNAMIC)$(MAKEPATH__)\\trunk\\source\\QObject.o: $(MAKEPATH)\\trunk\\source\\QObject.cpp
	$(CXX) $(CFLAGS_DEBUGWIN32SHAREDRTDYNAMIC) $(INC_DEBUGWIN32SHAREDRTDYNAMIC) -c $(MAKEPATH)\\trunk\\source\\QObject.cpp -o $(OBJDIR_DEBUGWIN32SHAREDRTDYNAMIC)$(MAKEPATH__)\\trunk\\source\\QObject.o

clean_debugwin32sharedrtdynamic: 
	
	
.PHONY: before_debugwin32sharedrtstatic after_debugwin32sharedrtstatic clean_debugwin32sharedrtstatic before_debugwin32sharedrtdynamic after_debugwin32sharedrtdynamic clean_debugwin32sharedrtdynamic

