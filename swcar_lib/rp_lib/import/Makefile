######################################################################
#  Makefile  (Rev 2.50)
#
#  Standard Makefile Template
#
#  Copyright(C) 1999-2022  Stellar Respree
#
#  Programmed by  Stellar Respree
######################################################################

######################################################################
#   Application Definition  (Pi)
######################################################################
OBJ         = VL53L1X_api VL53L1X_calibration vl53l1_linux_platform
APP			=

AUX_INC     = -I${STHOME}/import
AUX_LIB     =
AUX_OBJ     =


######################################################################
#   Common Library
######################################################################
STHOME      = /home/pi/swcar_app/rp_lib
STINC       = -I${STHOME}/include -I${STHOME}
STLIB       = -L${STHOME}/lib -lpthread \
-lwiringPi
#-lssl -lcrypto -lwebsockets


######################################################################
#   Basic command definition Section
######################################################################
MAKE        = make
CC          = gcc
AR          = ar
RM          = rm -f


######################################################################
#   Debug Flag Section
#   DEBUG           : print debug information while execution
######################################################################
CFLAG       = -g -c
DFLAG       = -D__EXTENSIONS__ -DRASPBIAN #-DDEBUG #-DSYSONLY


######################################################################
#	General Application Compile Section
######################################################################
OBJO	= $(addsuffix .o,${OBJ})
APPO	= $(addsuffix .o,${APP})

%.o:%.c
	${CC} ${CFLAG} ${DFLAG} ${STINC} ${AUX_INC} $<

%.o:%.cpp
	${CC} ${CFLAG} ${DFLAG} ${STINC} ${AUX_INC} $<

all:
	@clear
	@${MAKE} clean
	@echo
#	@${MAKE} ${APP}
	@${MAKE} ${OBJO}
	@echo
	@echo Generation completed...
	@echo

${APP}: ${OBJO} ${APPO}
	@echo
	@echo Generating application [$(basename $@)]...
	@echo ----------------------------------------
	${CC} -o $(subst output,,$@) $(addsuffix .o, $(subst output,,$@)) ${OBJO} ${STLIB}

clean clear:
	${RM} ${OBJO} ${APPO}


######################################################################
#   Install Section
######################################################################
#install:
#	${MAKE} clean
#	cp mymodule.* ../SOURCE
#	dp *.h ../include