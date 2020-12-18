# scde_core
### is the core library used to start creating custom IoT Devices running the

# SCDE
## Smart Connected Device Engine - Generation 2

### The new tool for makers to create IoT Devices

Makers can quickly build a firmware for their IoT-Devices
based on the Smart Connected Device Engine core extended by 
  * included commands and modules
  * own commands and modules
  * third party commands and modules
  
  
  
| Tables        | Are           | Cool  |
| ------------- |:-------------:| -----:|
| col 3 is      | right-aligned | $1600 |
| col 2 is      | centered      |   $12 |
| zebra stripes | are neat      |    $1 |
  
  
Programmers can provide
  * commands and modules for use in Makers IoT-Devices
  
IoT Devices setup is stored in the maker.cfg, IoT Devices state is stored in the state.cfg

Embedded Commands (SCDE-Core)
  * Attr
  * Define
  * Delete
  * Deleteattr
  * Get
  * Help
  * Include
  * IOWrite
  * List
  * Rename
  * Rereadcfg
  * Save
  * Set
  * Setstate 
  * Shutdown

Embedded Modules (SCDE-Core)
  * Global

Embedded Commands (linked in build process, ESP32-Platform)
  * -/-

Embedded Modules (linked in build process, ESP32-Platform)
  * Telnet (maintained as Module-Template)
  * ESP32_DeVICE (maintained as Module-Template)
    
Additional Commands (loadable module, ESP32-Platform)
  * loadable modules not supported on platform

Additional Modules (loadable module, ESP32-Platform)
  * loadable modules not supported on platform

Additional EcSUHA Modules on request (for testing purposes only)
  * ESP32_BH1750       Experimental
  * ESP32_Control      To control the ESP32
  * ESP32_I2C_Master   Experimental
  * ESP32_S0           For S0 counting
  * ESP32_SwITCH       For switchig and PWM with internal ESP32 hardware
 
  
---
