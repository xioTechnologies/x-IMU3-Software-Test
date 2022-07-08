#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-32MZ2048EFG124.mk)" "nbproject/Makefile-local-32MZ2048EFG124.mk"
include nbproject/Makefile-local-32MZ2048EFG124.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=32MZ2048EFG124
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/sd/ff.c ../src/sd/diskio.c ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_device_ds30.c ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_ds30.c ../src/framework/usb/src/dynamic/usb_device_cdc_acm_ds30.c ../src/framework/usb/src/dynamic/usb_device_cdc_ds30.c ../src/framework/usb/src/dynamic/usb_device_ds30.c ../src/uart_32.c ../src/main_sd.c ../src/timer_32.c ../src/comm.c ../src/app.c ../src/ds30_fifo.c ../src/spi_32.c ../src/spi_sw.c ../src/board_32MZ2048EFG124.c ../src/sd/ffunicode.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/809997941/ff.o ${OBJECTDIR}/_ext/809997941/diskio.o ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o ${OBJECTDIR}/_ext/1360937237/uart_32.o ${OBJECTDIR}/_ext/1360937237/main_sd.o ${OBJECTDIR}/_ext/1360937237/timer_32.o ${OBJECTDIR}/_ext/1360937237/comm.o ${OBJECTDIR}/_ext/1360937237/app.o ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o ${OBJECTDIR}/_ext/1360937237/spi_32.o ${OBJECTDIR}/_ext/1360937237/spi_sw.o ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o ${OBJECTDIR}/_ext/809997941/ffunicode.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/809997941/ff.o.d ${OBJECTDIR}/_ext/809997941/diskio.o.d ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d ${OBJECTDIR}/_ext/1360937237/uart_32.o.d ${OBJECTDIR}/_ext/1360937237/main_sd.o.d ${OBJECTDIR}/_ext/1360937237/timer_32.o.d ${OBJECTDIR}/_ext/1360937237/comm.o.d ${OBJECTDIR}/_ext/1360937237/app.o.d ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d ${OBJECTDIR}/_ext/1360937237/spi_32.o.d ${OBJECTDIR}/_ext/1360937237/spi_sw.o.d ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d ${OBJECTDIR}/_ext/809997941/ffunicode.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/809997941/ff.o ${OBJECTDIR}/_ext/809997941/diskio.o ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o ${OBJECTDIR}/_ext/1360937237/uart_32.o ${OBJECTDIR}/_ext/1360937237/main_sd.o ${OBJECTDIR}/_ext/1360937237/timer_32.o ${OBJECTDIR}/_ext/1360937237/comm.o ${OBJECTDIR}/_ext/1360937237/app.o ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o ${OBJECTDIR}/_ext/1360937237/spi_32.o ${OBJECTDIR}/_ext/1360937237/spi_sw.o ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o ${OBJECTDIR}/_ext/809997941/ffunicode.o

# Source Files
SOURCEFILES=../src/sd/ff.c ../src/sd/diskio.c ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_device_ds30.c ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_ds30.c ../src/framework/usb/src/dynamic/usb_device_cdc_acm_ds30.c ../src/framework/usb/src/dynamic/usb_device_cdc_ds30.c ../src/framework/usb/src/dynamic/usb_device_ds30.c ../src/uart_32.c ../src/main_sd.c ../src/timer_32.c ../src/comm.c ../src/app.c ../src/ds30_fifo.c ../src/spi_32.c ../src/spi_sw.c ../src/board_32MZ2048EFG124.c ../src/sd/ffunicode.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-32MZ2048EFG124.mk dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MZ2048EFG124
MP_LINKER_FILE_OPTION=,--script="..\lkr\p32MZ2048EFG124_ds30_bl_4p_mf.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/809997941/ff.o: ../src/sd/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/809997941" 
	@${RM} ${OBJECTDIR}/_ext/809997941/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/809997941/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/809997941/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/809997941/ff.o.d" -o ${OBJECTDIR}/_ext/809997941/ff.o ../src/sd/ff.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/809997941/diskio.o: ../src/sd/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/809997941" 
	@${RM} ${OBJECTDIR}/_ext/809997941/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/809997941/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/809997941/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/809997941/diskio.o.d" -o ${OBJECTDIR}/_ext/809997941/diskio.o ../src/sd/diskio.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o: ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_device_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/867312845" 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d" -o ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_device_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o: ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/867312845" 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d" -o ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o: ../src/framework/usb/src/dynamic/usb_device_cdc_acm_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1021102206" 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d" -o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o ../src/framework/usb/src/dynamic/usb_device_cdc_acm_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o: ../src/framework/usb/src/dynamic/usb_device_cdc_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1021102206" 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d" -o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o ../src/framework/usb/src/dynamic/usb_device_cdc_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o: ../src/framework/usb/src/dynamic/usb_device_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1021102206" 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d" -o ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o ../src/framework/usb/src/dynamic/usb_device_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/uart_32.o: ../src/uart_32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart_32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart_32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/uart_32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/uart_32.o.d" -o ${OBJECTDIR}/_ext/1360937237/uart_32.o ../src/uart_32.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/main_sd.o: ../src/main_sd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main_sd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main_sd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main_sd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/main_sd.o.d" -o ${OBJECTDIR}/_ext/1360937237/main_sd.o ../src/main_sd.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/timer_32.o: ../src/timer_32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer_32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer_32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer_32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/timer_32.o.d" -o ${OBJECTDIR}/_ext/1360937237/timer_32.o ../src/timer_32.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/comm.o: ../src/comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/comm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/comm.o.d" -o ${OBJECTDIR}/_ext/1360937237/comm.o ../src/comm.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/ds30_fifo.o: ../src/ds30_fifo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d" -o ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o ../src/ds30_fifo.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/spi_32.o: ../src/spi_32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/spi_32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/spi_32.o.d" -o ${OBJECTDIR}/_ext/1360937237/spi_32.o ../src/spi_32.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/spi_sw.o: ../src/spi_sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_sw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_sw.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/spi_sw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/spi_sw.o.d" -o ${OBJECTDIR}/_ext/1360937237/spi_sw.o ../src/spi_sw.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o: ../src/board_32MZ2048EFG124.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d" -o ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o ../src/board_32MZ2048EFG124.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/809997941/ffunicode.o: ../src/sd/ffunicode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/809997941" 
	@${RM} ${OBJECTDIR}/_ext/809997941/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/809997941/ffunicode.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/809997941/ffunicode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_SIMULATOR=1  -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/809997941/ffunicode.o.d" -o ${OBJECTDIR}/_ext/809997941/ffunicode.o ../src/sd/ffunicode.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
else
${OBJECTDIR}/_ext/809997941/ff.o: ../src/sd/ff.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/809997941" 
	@${RM} ${OBJECTDIR}/_ext/809997941/ff.o.d 
	@${RM} ${OBJECTDIR}/_ext/809997941/ff.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/809997941/ff.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/809997941/ff.o.d" -o ${OBJECTDIR}/_ext/809997941/ff.o ../src/sd/ff.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/809997941/diskio.o: ../src/sd/diskio.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/809997941" 
	@${RM} ${OBJECTDIR}/_ext/809997941/diskio.o.d 
	@${RM} ${OBJECTDIR}/_ext/809997941/diskio.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/809997941/diskio.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/809997941/diskio.o.d" -o ${OBJECTDIR}/_ext/809997941/diskio.o ../src/sd/diskio.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o: ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_device_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/867312845" 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o.d" -o ${OBJECTDIR}/_ext/867312845/drv_usbhs_device_ds30.o ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_device_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o: ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/867312845" 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o.d" -o ${OBJECTDIR}/_ext/867312845/drv_usbhs_ds30.o ../src/framework/driver/usb/usbhs/src/dynamic/drv_usbhs_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o: ../src/framework/usb/src/dynamic/usb_device_cdc_acm_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1021102206" 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o.d" -o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_acm_ds30.o ../src/framework/usb/src/dynamic/usb_device_cdc_acm_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o: ../src/framework/usb/src/dynamic/usb_device_cdc_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1021102206" 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o.d" -o ${OBJECTDIR}/_ext/1021102206/usb_device_cdc_ds30.o ../src/framework/usb/src/dynamic/usb_device_cdc_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o: ../src/framework/usb/src/dynamic/usb_device_ds30.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1021102206" 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d 
	@${RM} ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o.d" -o ${OBJECTDIR}/_ext/1021102206/usb_device_ds30.o ../src/framework/usb/src/dynamic/usb_device_ds30.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/uart_32.o: ../src/uart_32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart_32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/uart_32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/uart_32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/uart_32.o.d" -o ${OBJECTDIR}/_ext/1360937237/uart_32.o ../src/uart_32.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/main_sd.o: ../src/main_sd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main_sd.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main_sd.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main_sd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/main_sd.o.d" -o ${OBJECTDIR}/_ext/1360937237/main_sd.o ../src/main_sd.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/timer_32.o: ../src/timer_32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer_32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/timer_32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/timer_32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/timer_32.o.d" -o ${OBJECTDIR}/_ext/1360937237/timer_32.o ../src/timer_32.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/comm.o: ../src/comm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/comm.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/comm.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/comm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/comm.o.d" -o ${OBJECTDIR}/_ext/1360937237/comm.o ../src/comm.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/app.o: ../src/app.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/app.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/app.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/app.o.d" -o ${OBJECTDIR}/_ext/1360937237/app.o ../src/app.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/ds30_fifo.o: ../src/ds30_fifo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/ds30_fifo.o.d" -o ${OBJECTDIR}/_ext/1360937237/ds30_fifo.o ../src/ds30_fifo.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/spi_32.o: ../src/spi_32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_32.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_32.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/spi_32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/spi_32.o.d" -o ${OBJECTDIR}/_ext/1360937237/spi_32.o ../src/spi_32.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/spi_sw.o: ../src/spi_sw.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_sw.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/spi_sw.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/spi_sw.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/spi_sw.o.d" -o ${OBJECTDIR}/_ext/1360937237/spi_sw.o ../src/spi_sw.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o: ../src/board_32MZ2048EFG124.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o.d" -o ${OBJECTDIR}/_ext/1360937237/board_32MZ2048EFG124.o ../src/board_32MZ2048EFG124.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
${OBJECTDIR}/_ext/809997941/ffunicode.o: ../src/sd/ffunicode.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/809997941" 
	@${RM} ${OBJECTDIR}/_ext/809997941/ffunicode.o.d 
	@${RM} ${OBJECTDIR}/_ext/809997941/ffunicode.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/809997941/ffunicode.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../src" -I"../src/sd" -I"../src/framework" -mno-float -O1 -MMD -MF "${OBJECTDIR}/_ext/809997941/ffunicode.o.d" -o ${OBJECTDIR}/_ext/809997941/ffunicode.o ../src/sd/ffunicode.c    -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -mdfp=${DFP_DIR}  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../lib/ds30Loader_32MZ2048EFG124_4p_custom.a ../lib/ds30SdCardLoader_32MZ2048EFG124_4p_custom.a  ../lkr/p32MZ2048EFG124_ds30_bl_4p_mf.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g -mdebugger -D__MPLAB_DEBUGGER_SIMULATOR=1 -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\lib\ds30Loader_32MZ2048EFG124_4p_custom.a ..\lib\ds30SdCardLoader_32MZ2048EFG124_4p_custom.a      -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)      -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=__MPLAB_DEBUGGER_SIMULATOR=1,--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk  ../lib/ds30Loader_32MZ2048EFG124_4p_custom.a ../lib/ds30SdCardLoader_32MZ2048EFG124_4p_custom.a ../lkr/p32MZ2048EFG124_ds30_bl_4p_mf.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -O1 -mno-float -o dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}    ..\lib\ds30Loader_32MZ2048EFG124_4p_custom.a ..\lib\ds30SdCardLoader_32MZ2048EFG124_4p_custom.a      -DXPRJ_32MZ2048EFG124=$(CND_CONF)  -no-legacy-libc  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=2048,--no-code-in-dinit,--no-dinit-in-serial-mem,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp=${DFP_DIR}
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/ds30SdCardLoader_32MZ2048EFG124.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/32MZ2048EFG124
	${RM} -r dist/32MZ2048EFG124

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
