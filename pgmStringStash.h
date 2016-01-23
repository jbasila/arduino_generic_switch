
PGM_STRING(STR_AT, "AT")
PGM_STRING(STR_ATBAUD, "AT+BAUD")
PGM_STRING(STR_ATNAME, "AT+NAME")
PGM_STRING(STR_ATPIN, "AT+PIN")

PGM_STRING(STR_BT_DEFAULT_NAME, "Tabbouleh BT")
PGM_STRING(STR_BT_DEFAULT_PIN, "0000")

PGM_STRING(STR_FATAL_BT, "FATAL: bluetooth device not initialized")
PGM_STRING(STR_READY, "\n\nREADY")
PGM_STRING(STR_CFG_UPDATE, "Config Update")
PGM_STRING(STR_CFG_RESTORED, "restored")
PGM_STRING(STR_CFG_BAUD_RATE, "Setting the baud rate to: ")

PGM_STRING(STR_CMD_HELP, "help")
PGM_STRING(STR_CMD_LIST, "list")
PGM_STRING(STR_CMD_SET, "set")
PGM_STRING(STR_CMD_CLEAR, "clear")
PGM_STRING(STR_CMD_SAVE, "save")
PGM_STRING(STR_CMD_ACTION, "action")
PGM_STRING(STR_CMD_BT, "bt")
PGM_STRING(STR_CMD_FREE, "free")

PGM_STRING(STR_COMMAND_HELP,
           "list   - List current devices\n"
           "set    - Set device on a slot\n"
           "clear  - Clear device from slot\n"
           "save   - Save configurmation in permanent memory\n"
           "action - Send an action to a configured device slot")
PGM_STRING(STR_COMMAND_HELP_USAGE, "usage: help [set|action]")

PGM_STRING(STR_COMMAND_HELP_SET,
           "  <index>      - The slot number to use (see list command)\n"
           "  <deviceName> - Available devices are:\n"
           "                     on_off  - On/Off switch\n"
           "                     trigger - Indicate when an input pin has a\n"
           "                               rising edge or falling edge\n"
           "                     sensor  - Indicate when an input pin drops\n"
           "                               lower than a low threshold or rises\n"
           "                               above a high threshold\n"
           "  <param1>     - a number to be passed as the first argument\n"
           "  <param1>     - a number to be passed as the second argument\n"
           "\n"
           "  Valid parameters for devices:\n"
           "    on_off\n"
           "      - param1   - The output pin for the switch\n"
           "      - param2   - N/A\n"
           "    trigger\n"
           "      - param1   - Input pin to check for events\n"
           "      - param2   - 1 rising edge, 0 falling edge\n"
           "    sensor\n"
           "      - param1   - Input pin to check\n"
           "      - param2   - N/A")
PGM_STRING(STR_COMMAND_HELP_ACTION,
           "  <index>  - The slot number\n"
           "  <action> - the action to issue\n"
           "  <param>  - a parameter specifc\n"
           "\n"
           "  Valid action and param for devices:\n"
           "    the format of the possible action/param:\n"
           "    <device>\n"
           "       action: what it does\n"
           "         param: what does it indicate\n"
           "\n"
           "    on_off\n"
           "      0: fetch status\n"
           "        N/A\n"
           "      1: set switch state\n"
           "        0: off\n"
           "        1: on\n"
           "    trigger\n"
           "      0: get trigger active state notification\n"
           "        N/A\n"
           "      1: set trigger active state notification\n"
           "        0: inactive\n"
           "        1: active\n"
           "      2: set trigger edge\n"
           "        0: falling\n"
           "        1: rising\n"
           "      3: input debounce\n"
           "        0: off\n"
           "        1: on\n"
           "    sensor\n"
           "      0: get sensor value\n"
           "        N/A\n"
           "      1: display thresholds\n"
           "        N/A\n"
           "      2: set active threshold notification\n"
           "        0: off\n"
           "        1: on\n"
           "      10 ... 13: set lower threshold\n"
           "        0 ... 255: lower threshold: (action % 10) * 256 + param\n"
           "      20 ... 23: set high threshold\n"
           "        0 ... 255: high threshold: (action % 10) * 256 + param")

PGM_STRING(STR_COMMAND_LIST_FORMAT_HEADER, "ID |Device  |P1 |P2 |PD")
PGM_STRING(STR_COMMAND_LIST_FORMAT_STRING, "%2d |%-7s |%-2d |%-2d |%02X %02X %02X %02X")
PGM_STRING(STR_COMMAND_LIST_FORMAT_LINE, "---|--------|---|---|-----------")

PGM_STRING(STR_COMMAND_SET_USAGE, "usage: set <index> <deviceName> <param1> <param2>")
PGM_STRING(STR_COMMAND_SET, "SET: ")

PGM_STRING(STR_COMMAND_CLEAR_NOT_NUMBER_OR_ALL, "clear: argument is not a number or 'all'")
PGM_STRING(STR_COMMAND_CLEAR_TOO_MANY_ARGUMENTS, "clear: too many arguments")
PGM_STRING(STR_COMMAND_CLEAR_USAGE, "usage: clear <index> | all")
PGM_STRING(STR_COMMAND_CLEAR, "CLEAR: ")

PGM_STRING(STR_COMMAND_ACTION_USAGE, "usage: action <index> <action> <param>")
PGM_STRING(STR_COMMAND_ACTION_INVALID_LOGIC, "action: invalid index logic")

PGM_STRING(STR_COMMAND_SAVE, "SAVE: ")

PGM_STRING(STR_COMMAND_BT_USAGE, "usage: bt BAUD NAME PIN")
PGM_STRING(STR_COMMAND_BT, "BT: ")
PGM_STRING(STR_COMMAND_BT_ERROR_INVALID_BAUD_RATE, "ERROR: Invalid baud rate: ")
PGM_STRING(STR_COMMAND_BT_ERROR_INVALID_PIN_CODE, "ERROR: Invalid PIN code: ")
PGM_STRING(STR_DBG_BT_FAILED_SET_BAUD_RATE, "ERROR: Failed to set BAUD RATE")
PGM_STRING(STR_DBG_BT_FAILED_SET_PIN, "ERROR: Failed to set PIN")
PGM_STRING(STR_DBG_BT_FAILED_SET_DEVICE_NAME, "ERROR: Failed to set DEVICE NAME")

PGM_STRING(STR_COMMAND_FREE, "Free memory: ")

PGM_STRING(STR_COMMAND_UNKNOWN, "Unknown Command")

PGM_STRING(STR_SETUP_DEBUG_RESTORING_CONFIGURATION, "\nRestoring Configuration: ")
PGM_STRING(STR_SETUP_DEBUG_DUMP_OF_CONFIGURATION, "\n\nDump of configuration:")
PGM_STRING(STR_SETUP_DEBUG_DUMP_OF_EEPROM, "\n\nDump of EEPROM:")
PGM_STRING(STR_SETUP_DEBUG_STORING, "\nStoring: ")

PGM_STRING(STR_SUCCESS, "success")
PGM_STRING(STR_FAILED, "failed")
PGM_STRING(STR_YES, "yes")
PGM_STRING(STR_NO, "no")
PGM_STRING(STR_ON, "on")
PGM_STRING(STR_OFF, "off")
PGM_STRING(STR_LOW, "low")
PGM_STRING(STR_HIGH, "high")
PGM_STRING(STR_ALL, "all")

PGM_STRING(STR_EVENT, "EVENT: ")
PGM_STRING(STR_ACTION, "ACTION: ")
PGM_STRING(STR_SEMICOLON_SPACE, "; ")

PGM_STRING(STR_INVALID_ACTION, "invalid action passed")

PGM_STRING(STR_ON_OFF, "on_off")
PGM_STRING(STR_ON_OFF__OK_SWITCH, "SET: ")
PGM_STRING(STR_ON_OFF__STATE, "STATE: ")

PGM_STRING(STR_TRIGGER, "trigger")
PGM_STRING(STR_TRIGGER__ACTIVE_STATE, "STATE: ")
PGM_STRING(STR_TRIGGER__SET_ACTIVE, "OK: active ")
PGM_STRING(STR_TRIGGER__SET_DEBOUNCE, "OK: debounce ")
PGM_STRING(STR_TRIGGER__SET_EDGE, "OK: edge ")
PGM_STRING(STR_TRIGGER__RISING, "rising")
PGM_STRING(STR_TRIGGER__FALLING, "falling")
PGM_STRING(STR_TRIGGER_EVENT, "TRIGGER SET")

PGM_STRING(STR_SENSOR, "sensor")
PGM_STRING(STR_SENSOR__STATE, "STATE: ")
PGM_STRING(STR_SENSOR__SET_ACTIVE_THRESHOLD, "SET: ACTIVE ")
PGM_STRING(STR_SENSOR__SET_THRESHOLD, "SET: THRESHOLD ")

PGM_STRING(STR_UNKNOWN, "unknown")

