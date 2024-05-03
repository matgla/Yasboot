*** Settings ***
Resource    ${RENODEKEYWORDS}


*** Keywords ***
Prepare Machine
  Execute Command   path add @${RP2040_RENODE_BOARD_DIR}
  Execute Command   include @initialize_rp2040.resc 

  Execute Command   sysbus LoadBinary @${TEST_DISK_IMAGE} 0x10000000

  Execute Command   sysbus.cpu0 VectorTableOffset 0x10000200
  Execute Command   sysbus.cpu1 VectorTableOffset 0x10000200

  Create Terminal Tester  sysbus.uart0

  Start Emulation

