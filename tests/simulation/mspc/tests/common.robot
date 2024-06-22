*** Settings ***
Resource    ${RENODEKEYWORDS}


*** Keywords ***
Prepare Machine
  Execute Command   include @${CURDIR}/execute.resc 

  Create Terminal Tester  sysbus.uart0

  Start Emulation

