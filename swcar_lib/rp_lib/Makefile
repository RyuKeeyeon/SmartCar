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
OBJ         = hal sio gps
APP			= librp_smartcar

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
APPO	=
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
#	@${MAKE} -C import
	@echo ----------------------------------------
	@echo Generating application [$(basename $@)]...
	@echo ----------------------------------------
#	${CC} -o $(subst output,,$@) $(addsuffix .o, $(subst output,,$@)) ${OBJO} ${STLIB}
	${CC} -shared -o ${APP}.so ${OBJO} ${AUX_OBJO}
	mv ${APP}.so ..
	${AR} rc ${APP}.a ${OBJO} ${AUX_OBJO}

clean clear:
	${RM} *.so *.a ${OBJO} ${APPO}


######################################################################
#   Install Section
######################################################################
#install:
#	${MAKE} clean
#	cp mymodule.* ../SOURCE
#	dp *.h ../include