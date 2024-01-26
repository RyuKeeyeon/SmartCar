/*********************************************************************
    hal_type.h (Rev 1.00)

    HAL Pinmap

    Copyright(C) Memorylab Ltd.

    Programmed by  Stellar Respree
*********************************************************************/

// -------------------------------------------------------------------
//	Basic Type Definition
// -------------------------------------------------------------------
#define PIN_1_3V
#define PIN_3_SDA
#define PIN_5_SCL
#define PIN_7					4
#define PIN_9_GND
#define PIN_11					17
#define PIN_13					27
#define PIN_15					22
#define PIN_17_3V
#define PIN_19_SPI_MOSI
#define PIN_19_GPIO				10
#define PIN_21_SPI_MISO
#define PIN_23_SPI_CLK
#define PIN_23_GPIO				11
#define PIN_25_GND
#define PIN_27_ID_SD
#define PIN_29					5
#define PIN_31					6
#define PIN_33					13
#define PIN_35					19
#define PIN_37					26
#define PIN_39_GND

#define PIN_2_5V
#define PIN_4_5V
#define PIN_6_GND
#define PIN_8_TXD0
#define PIN_10_RXD0
#define PIN_12					18
#define PIN_14_GND
#define PIN_16					23
#define PIN_18					24
#define PIN_20_GND
#define PIN_22					25
#define PIN_24_SPI_CS0
#define PIN_26_SPI_CS1
#define PIN_26					7
#define PIN_28_ID_SC

#define PIN_30_GND
#define PIN_32					12
#define PIN_34_GND
#define PIN_36					16
#define PIN_38					20
#define PIN_40					21


// -------------------------------------------------------------------
//	Application Mapping for SmartFarm
// -------------------------------------------------------------------
#define PORT_ONEWIRE			PIN_7

#define DO_AC_RELAY1			PIN_11
#define DO_AC_RELAY2			PIN_13
#define DO_AC_RELAY3			PIN_15
#define DO_AC_RELAY4			PIN_29
#define DO_AC_RELAY5			PIN_31
#define DO_AC_RELAY6			PIN_33
#define DO_AC_RELAY7			PIN_35
#define DO_AC_RELAY8			PIN_37

#define DO_DC_RELAY1			PIN_12
#define DO_DC_RELAY2			PIN_16
#define DO_DC_RELAY3			PIN_18
#define DO_DC_RELAY4			PIN_22
#define DO_DC_RELAY5			PIN_32
#define DO_DC_RELAY6			PIN_36
#define DO_DC_RELAY7			PIN_38
#define DO_DC_RELAY8			PIN_40

/*
#define I2C_SCL					PIN_5_SCL
#define I2C_SDA					PIN_3_SDA

#define SPI_CE0_N				PIN_24_SPI_CS0
#define SPI_CE1_N				PIN_26_SPI_CS1
#define SPI_SCLK				PIN_23_SPI_CLK
#define SPI_MISO				PIN_21_SPI_MISO
#define SPI_MOSI				PIN_19_SPI_MOSI
*/


// -------------------------------------------------------------------
//	Application Mapping for SmartCar
// -------------------------------------------------------------------
#define CAR_SW0					PIN_18
#define CAR_SW1					PIN_22
#define CAR_SW2					PIN_37
#define CAR_SW3					PIN_13

#define CAR_MOTOR_PM			PIN_33
#define CAR_MOTOR_SM			PIN_32
#define CAR_BLDC_DIR			PIN_38
#define CAR_BLDC_BRAKE			PIN_40

#define CAR_IR0_SENSE			PIN_36
#define CAR_IR1_SENSE			PIN_11
#define CAR_IR2_SENSE			PIN_12
#define CAR_IR3_SENSE			PIN_35

#define CAR_US_TRIG			    PIN_31
#define CAR_US_FF_ECHO			PIN_29
#define CAR_US_RR_ECHO			PIN_26

