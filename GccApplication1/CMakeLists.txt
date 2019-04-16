cmake_minimum_required(VERSION 3.0)

set(PROG_TYPE avrisp)
set(USBPORT /dev/tty.usbmodem411)
# extra arguments to avrdude: baud rate, chip type, -F flag, etc.
set(PROG_ARGS -b 19200 -P ${USBPORT})

# Variables regarding the AVR chip
set(MCU   atmega4808)
set(AVR_DEV_LIB_NAME m4808)
set(F_CPU 3333333)
set(CMAKE_CXX_STANDARD 20)
add_definitions(-DF_CPU=${F_CPU})

# program names
set(AVRCPP   avr-g++)
set(AVRC     avr-gcc)
set(AVRSTRIP avr-strip)
set(OBJCOPY  avr-objcopy)
set(OBJDUMP  avr-objdump)
set(AVRSIZE  avr-size)
set(AVRDUDE  avrdude)

# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${AVRCPP})
set(CMAKE_C_COMPILER   ${AVRC})
set(CMAKE_ASM_COMPILER   ${AVRC})

project (avrProject)

# Important project paths
set(BASE_PATH    "${${PROJECT_NAME}_SOURCE_DIR}")
set(INC_PATH     "${BASE_PATH}/inc")
set(LIB_DIR_PATH "${BASE_PATH}/lib")
set(SRC_PATH     "${BASE_PATH}/src")
set(PARSER_PATH "${BASE_PATH}/parser")

set(DEVICEFILE_4808 "${PARSER_PATH}/ATmega4808.atdf")
set(DEVICEFILE_4809 "${PARSER_PATH}/ATmega4809.atdf")
set(HWFOLDER_4808 "${INC_PATH}/mega4808/hal")
set(HWFOLDER_4809 "${INC_PATH}/mega4809/hal")

# Files to be compiled
file(GLOB SRC_FILES "${SRC_PATH}/*.cpp"
                    "${SRC_PATH}/*.cc"
                    "${SRC_PATH}/*.c"
                    "${SRC_PATH}/*.cxx"
                    "${SRC_PATH}/*.S"
                    "${SRC_PATH}/*.s"
                    "${SRC_PATH}/*.sx"
                    "${SRC_PATH}/*.asm")

set(LIB_INC_PATH "${BASE_PATH}/inc/std ")
file(GLOB LIBRARIES "${LIB_DIR_PATH}/*")
foreach(subdir ${LIBRARIES})
    file(GLOB lib_files "${subdir}/*.cpp"
                        "${subdir}/*.cc"
                        "${subdir}/*.c"
                        "${subdir}/*.cxx"
                        "${subdir}/*.S"
                        "${subdir}/*.s"
                        "${subdir}/*.sx"
                        "${subdir}/*.asm")
    if(IS_DIRECTORY ${subdir})
        list(APPEND LIB_INC_PATH  "${subdir}")
    endif()
    list(APPEND LIB_SRC_FILES "${lib_files}")
endforeach()

# Compiler flags
set(CSTANDARD "-std=gnu99")
set(CDEBUG    "-gstabs -g -ggdb")
set(CWARN     "-Wall -Wstrict-prototypes -Wl,--gc-sections -Wl,--relax")
set(CTUNING   "-funsigned-char -fno-jump-tables -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${MCU}")
set(CXXOPTS   "-std=c++2a -fconcepts")

set(CFLAGS   "${CMCU} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${COPT} ${CTUNING} ${CXXOPTS} -D_${AVR_DEV_LIB_NAME}")

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS} ${CXXOPTS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS}")
# Project setup
include_directories(${INC_PATH} ${LIB_INC_PATH})
add_executable(${PROJECT_NAME} ${SRC_FILES} ${LIB_SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

add_custom_target(atmega4809 COMMAND untitled ${DEVICEFILE_4809} ${HWFOLDER_4809}
        WORKING_DIRECTORY ${PARSER_PATH} DEPENDS hwfilesexe)


add_custom_target(atmega4808 COMMAND untitled "${DEVICEFILE_4808}" "${HWFOLDER_4808}"
        WORKING_DIRECTORY ${PARSER_PATH} DEPENDS hwfilesexe)


add_custom_target(hwfiles DEPENDS atmega4808 atmega4809 )
add_custom_target(hwfilesexe COMMAND make all
        WORKING_DIRECTORY ${PARSER_PATH}
        DEPENDS hwfilescmake
        )
add_custom_target(hwfilescmake COMMAND cmake -G "MinGW Makefiles" ./
        WORKING_DIRECTORY ${PARSER_PATH}
        )

#add_custom_target(hwfiles ALL   ${PARSER_PATH}/make ${PARSER_PATH})
add_dependencies(${PROJECT_NAME} hwfiles)
# Compiling targets
add_custom_target(strip ALL     ${AVRSTRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)
add_custom_target(disassemble   ${OBJDUMP} -S "${PROJECT_NAME}.elf" > "${PROJECT_NAME}.lst" DEPENDS strip)
# Flashing targets
add_custom_target(flash         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(flash_usbtiny ${AVRDUDE} -c usbtiny -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(flash_usbasp  ${AVRDUDE} -c usbasp -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(flash_ardisp  ${AVRDUDE} -c avrisp -p ${MCU} -b 19200 -P ${USBPORT} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(flash_109     ${AVRDUDE} -c avr109 -p ${MCU} -b 9600 -P ${USBPORT} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(flash_eeprom  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U eeprom:w:${PROJECT_NAME}.hex DEPENDS eeprom)
# Fuses (For ATMega328P-PU, Calculated using http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p)
add_custom_target(reset         ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -e)
add_custom_target(fuses_1mhz    ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0x62:m)
add_custom_target(fuses_8mhz    ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xE2:m)
add_custom_target(fuses_16mhz   ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xFF:m)
add_custom_target(fuses_uno     ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U lfuse:w:0xFF:m -U hfuse:w:0xDE:m -U efuse:w:0x05:m)
add_custom_target(set_eeprom_save_fuse   ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U hfuse:w:0xD1:m)
add_custom_target(clear_eeprom_save_fuse ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -U hfuse:w:0xD9:m)
# Utilities targets
add_custom_target(avr_terminal  ${AVRDUDE} -c ${PROG_TYPE} -p ${MCU} ${PROG_ARGS} -nt)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")

# Config logging
message("* ")
message("${DEVICEFILE_4808}")
message("${HWFOLDER_4808}")
message("* Project Name:\t ${PROJECT_NAME}")
message("* Project Source:\t ${SRC_PATH}")
message("* Project Include:\t ${INC_PATH}")
message("* Library Include:\t ${LIB_INC_PATH}")
message("* ")
message("* Project Source Files:\t ${SRC_FILES}")
message("* Library Source Files:\t ${LIB_SRC_FILES}")
message("* ")
message("* C Flags:\t ${CMAKE_C_FLAGS}")
message("* ")
message("* CXX Flags:\t ${CMAKE_CXX_FLAGS}")
message("* ")
