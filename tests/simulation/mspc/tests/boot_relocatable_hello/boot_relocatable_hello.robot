*** Settings ***
Resource        common.robot 

Suite Setup     Setup 
Suite Teardown  Teardown 
Test Teardown   Test Teardown 
Test Timeout    8 seconds 


*** Variables *** 
${TEST_DISK_IMAGE}            @renode_test_image@ 
${RP2040_RENODE_BOARD_DIR}    @renode_rp2040_board_dir@ 

*** Test Cases ***
Boot standalone relocatable PICO SDK binary 
  Prepare Machine 

  Wait For Line On Uart   Hello from standalone relocatable module 1!   timeout=1
  Wait For Line On Uart   Hello from standalone relocatable module 2!   timeout=2

