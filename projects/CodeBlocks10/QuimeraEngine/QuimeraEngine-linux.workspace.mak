#------------------------------------------------------------------------------#
# This makefile was generated by 'cbp2make' tool rev.137                       #
# and then tuned by Kinesis Team to adjust to Quimera Engine project's needs.  #
#------------------------------------------------------------------------------#

WRKDIR = %cd%

buildDebugLinux32SharedrtStatic: core_debuglinux32sharedrtstatic system_debuglinux32sharedrtstatic tools_debuglinux32sharedrtstatic

core_debuglinux32sharedrtstatic:
	$(MAKE) debuglinux32sharedrtstatic -f $(MAKEPATH)trunk/projects/CodeBlocks10/QuimeraEngine/Core/Core-linux.cbp.mak MAKEFILE="$(MAKEPATH)" MAKEFILE__="$(MAKEPATH__)"

system_debuglinux32sharedrtstatic:
	$(MAKE) debuglinux32sharedrtstatic -f $(MAKEPATH)trunk/projects/CodeBlocks10/QuimeraEngine/System/System-linux.cbp.mak MAKEFILE="$(MAKEPATH)" MAKEFILE__="$(MAKEPATH__)"

tools_debuglinux32sharedrtstatic:
	$(MAKE) debuglinux32sharedrtstatic -f $(MAKEPATH)trunk/projects/CodeBlocks10/QuimeraEngine/Tools/Tools-linux.cbp.mak MAKEFILE="$(MAKEPATH)" MAKEFILE__="$(MAKEPATH__)"
	
cleanDebuglinux32SharedrtStatic: clean_core_debuglinux32sharedrtstatic clean_system_debuglinux32sharedrtstatic clean_tools_debuglinux32sharedrtstatic

clean_core_debuglinux32sharedrtstatic: 
	$(MAKE) clean_debuglinux32sharedrtstatic -f $(MAKEPATH)trunk/projects/CodeBlocks10/QuimeraEngine/Core/Core-linux.cbp.mak

clean_system_debuglinux32sharedrtstatic: 
	$(MAKE) clean_debuglinux32sharedrtstatic -f $(MAKEPATH)trunk/projects/CodeBlocks10/QuimeraEngine/System/System-linux.cbp.mak

clean_tools_debuglinux32sharedrtstatic: 
	$(MAKE) clean_debuglinux32sharedrtstatic -f $(MAKEPATH)trunk/projects/CodeBlocks10/QuimeraEngine/Tools/Tools-linux.cbp.mak

.PHONY: clean_core_core clean_system_system clean_tools_tools
