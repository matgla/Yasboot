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
Boot standalone non relocatable PICO SDK binary 
  Prepare Machine 

  Wait For Line On Uart   This is first example that just prints!   timeout=1
  Wait For Line On Uart   This is first example that just prints!   timeout=2

