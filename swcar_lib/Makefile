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
OBJ         =
APP			= swcar

AUX_INC     =
AUX_LIB     =
AUX_OBJ     =


######################################################################
#   Common Library
######################################################################
STHOME      = /home/pi/swcar_lib/rp_lib
STINC       = -I${STHOME}/include -I${STHOME}
STLIB       = -L${STHOME}/lib -lpthread \
-L${STHOME} -lrp_smartcar \
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
AUX_OBJO= $(addsuffix .o,${AUX_OBJ})

%.o:%.c
	${CC} ${CFLAG} ${DFLAG} ${STINC} ${AUX_INC} $<

%.o:%.cpp
	${CC} ${CFLAG} ${DFLAG} ${STINC} ${AUX_INC} $<

all:
	@clear
	@${MAKE} clean
	@echo
	@${MAKE} ${APP}
	@echo
	@echo Generation completed...
	@echo

${APP}: ${OBJO} ${APPO}
	@echo
	@${MAKE} -C rp_lib
	@echo ----------------------------------------
	@echo Generating application [$(basename $@)]...
	@echo ----------------------------------------
	${CC} -o $(subst output,,$@) $(addsuffix .o, $(subst output,,$@)) ${OBJO} ${STLIB}

clean clear:
	${RM} ${APP} ${OBJO} ${APPO}


######################################################################
#   Install Section
######################################################################
#install:
#	${MAKE} clean
#	cp mymodule.* ../SOURCE
#	dp *.h ../include
