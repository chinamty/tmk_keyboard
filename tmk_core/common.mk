COMMON_DIR = common
SRC +=    $(COMMON_DIR)/host.c \
    $(COMMON_DIR)/keyboard.c \
    $(COMMON_DIR)/matrix.c \
    $(COMMON_DIR)/action.c \
    $(COMMON_DIR)/action_tapping.c \
    $(COMMON_DIR)/action_macro.c \
    $(COMMON_DIR)/action_layer.c \
    $(COMMON_DIR)/action_util.c \
    $(COMMON_DIR)/print.c \
    $(COMMON_DIR)/debug.c \
    $(COMMON_DIR)/util.c \
    $(COMMON_DIR)/hook.c \
    $(COMMON_DIR)/wait.c \
    $(COMMON_DIR)/avr/xprintf.S \
    $(COMMON_DIR)/avr/timer.c \
    $(COMMON_DIR)/avr/bootloader.c

# ATMEGA32A and/or VUSB has no suspend features for now
ifndef ATMEGA32
    SRC += $(COMMON_DIR)/avr/suspend.c
endif


# Option modules
ifeq (yes,$(strip $(UNIMAP_ENABLE)))
    SRC += $(COMMON_DIR)/unimap.c
    OPT_DEFS += -DUNIMAP_ENABLE
    OPT_DEFS += -DACTIONMAP_ENABLE
else
    ifeq (yes,$(strip $(ACTIONMAP_ENABLE)))
    SRC += $(COMMON_DIR)/actionmap.c
    OPT_DEFS += -DACTIONMAP_ENABLE
    else
    SRC += $(COMMON_DIR)/keymap.c
    endif
endif

ifeq (yes,$(strip $(BOOTMAGIC_ENABLE)))
    SRC += $(COMMON_DIR)/bootmagic.c
    SRC += $(COMMON_DIR)/avr/eeconfig.c
    OPT_DEFS += -DBOOTMAGIC_ENABLE
endif

ifeq (yes,$(strip $(MOUSEKEY_ENABLE)))
    SRC += $(COMMON_DIR)/mousekey.c
    OPT_DEFS += -DMOUSEKEY_ENABLE
    OPT_DEFS += -DMOUSE_ENABLE
endif

ifeq (yes,$(strip $(EXTRAKEY_ENABLE)))
    OPT_DEFS += -DEXTRAKEY_ENABLE
endif

ifeq (yes,$(strip $(CONSOLE_ENABLE)))
    OPT_DEFS += -DCONSOLE_ENABLE
else
    OPT_DEFS += -DNO_PRINT
    OPT_DEFS += -DNO_DEBUG
endif

ifeq (yes,$(strip $(LED_DATA_ENABLE)))
    OPT_DEFS += -DLED_DATA_ENABLE
endif

ifeq (yes,$(strip $(COMMAND_ENABLE)))
    SRC += $(COMMON_DIR)/command.c
    OPT_DEFS += -DCOMMAND_ENABLE
endif

ifeq (yes,$(strip $(NKRO_ENABLE)))
    OPT_DEFS += -DNKRO_ENABLE
endif

ifeq (yes,$(strip $(USB_6KRO_ENABLE)))
    OPT_DEFS += -DUSB_6KRO_ENABLE
endif

ifeq (yes, $(strip $(KEYBOARD_LOCK_ENABLE)))
    OPT_DEFS += -DKEYBOARD_LOCK_ENABLE
endif

ifeq (yes,$(strip $(SLEEP_LED_ENABLE)))
    SRC += $(COMMON_DIR)/avr/sleep_led.c
    OPT_DEFS += -DSLEEP_LED_ENABLE
    OPT_DEFS += -DNO_SUSPEND_POWER_DOWN
endif

ifeq (yes,$(strip $(SOFTPWM_LED_ENABLE)))
    SRC += $(COMMON_DIR)/avr/softpwm_led.c
    OPT_DEFS += -DSOFTPWM_LED_ENABLE
ifeq (yes,$(strip $(FADING_LED_ENABLE)))
    OPT_DEFS += -DFADING_LED_ENABLE
endif
ifeq (yes,$(strip $(BREATHING_LED_ENABLE)))
    OPT_DEFS += -DBREATHING_LED_ENABLE
endif
else
ifeq (yes,$(strip $(BREATHING_LED_ENABLE)))
    SRC += $(COMMON_DIR)/breathing_led.c
    OPT_DEFS += -DBREATHING_LED_ENABLE
endif
endif
ifeq (yes,$(strip $(BACKLIGHT_ENABLE)))
    SRC += $(COMMON_DIR)/avr/eeconfig.c
    SRC += $(COMMON_DIR)/backlight.c
    OPT_DEFS += -DBACKLIGHT_ENABLE
endif
ifeq (yes,$(strip $(RGBLIGHT_ENABLE)))
    OPT_DEFS += -DRGBLIGHT_ENABLE
endif
ifeq (yes,$(strip $(LEDMAP_ENABLE)))
    SRC += $(COMMON_DIR)/ledmap.c
    OPT_DEFS += -DLEDMAP_ENABLE
ifeq (yes,$(strip $(LEDMAP_IN_EEPROM_ENABLE)))
    SRC += $(COMMON_DIR)/ledmap_in_eeprom.c
    OPT_DEFS += -DLEDMAP_IN_EEPROM_ENABLE
endif
endif
ifeq (yes,$(strip $(KEYMAP_SECTION_ENABLE)))
    OPT_DEFS += -DKEYMAP_SECTION_ENABLE
    ifdef MCU_LDSCRIPT
        EXTRALDFLAGS = -Wl,-L$(TARGET_DIR)/ld,-T$(MCU_LDSCRIPT).x
    else ifeq ($(strip $(MCU)),atmega32u2)
        ifeq ($(strip $(BL_SIZE)),6K) 
        EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr35_bl6k.x
        else 
        EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr35.x
        endif
    else ifeq ($(strip $(MCU)),atmega32u4)
      ifeq ($(strip $(BL_SIZE)),1K) 
        EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5_bl1k.x
        else ifeq ($(strip $(BL_SIZE)),6K) 
        EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5_bl6k.x
        else
        EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5.x
        endif
    else ifeq ($(strip $(MCU)),at90usb1286)
      ifeq ($(strip $(BL_SIZE)),6K) 
        EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr51_bl6k.x
      else
    EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5.x
      endif
    else ifeq ($(strip $(MCU)),atmega32)
    EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5.x
    else ifeq ($(strip $(MCU)),atmega328p)
    EXTRALDFLAGS = -Wl,-L$(TMK_DIR),-Tldscript_keymap_avr5.x
    else
    EXTRALDFLAGS = $(error no ldscript for keymap section)
    endif
endif

# When bootmagic is disabled. This is needed for rgblight as it uses eeconfig.
ifeq (yes,$(strip $(RGBLIGHT_ENABLE)))
    SRC += $(COMMON_DIR)/avr/eeconfig.c
endif

# Version string
VERSION := $(shell (git describe --always --dirty || echo 'unknown') 2> /dev/null)
OPT_DEFS += -DVERSION=$(VERSION)


# Search Path
VPATH += $(TMK_DIR)/common
