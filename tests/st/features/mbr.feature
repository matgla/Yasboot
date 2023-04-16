Feature: Process MBR inside image 
  Scenario: Read correct drive image 
    Given we have yasboot executable
    When we execute with
      """
      DISK=data/mbr_and_two_partitions.img
      """
    Then stdout contains
      """
      Parsing MBR of primary drive 
      """
    Then stdout contains 
      """
      Drive 0 contains valid MBR 
      """

  Scenario: Read invalid MBR drive reports error
    Given we have yasboot executable
    When we execute with
      """
      DISK=data/invalid_mbr.img
      """
    Then stdout contains 
      """
      Drive 0 contains invalid \(0x12ef\) MBR
      """

    Scenario: Get bootable partition 
      Given we have yasboot executable 
      When we execute with 
        """
        DISK=data/mbr_with_partition_at_0x4000.img
        """
      Then stdout contains 
        """
        Found bootable partition at address: 0x4000 
        """
