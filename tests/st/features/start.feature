Feature: Start Yasboot
  Scenario: Just start Yasboot
    Given we have yasboot executable
    When we execute 
    Then stdout contains
      """
      Hello from Yasboot
      """
