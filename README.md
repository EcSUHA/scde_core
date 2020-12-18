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
  
  
Programmers can provide commands and/or modules for use in Makers IoT-Devices
  * to read sensors, to distribute sensor values, ...
  
IoT Devices setup is stored in the maker.cfg file, IoT Devices state is stored in the state.cfg.

Embedded Commands (SCDE-Core) are
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

Embedded Commands
  * -/-
  
Embedded Modules (SCDE-Core) are
  * Global

Embedded Commands (in Plaform-Templates)
  * Reloadmodule (LINUX-Platform)
  * Reloadcommand (LINUX-Platform) 

Embedded Modules (in Plaform-Templates)
  * Telnet (maintained as Module-Template)(LINUX-Platform, ESP32-Platform)
  * ESP32_DeVICE (maintained as Module-Template) (ESP32-Platform)
    
Additional Commands (on request, according to the license)
  * Reloadmodule (LINUX-Platform)
  * Reloadcommand (LINUX-Platform) 

Additional Modules (on request, according to the license)
  * -/-

Additional EcSUHA Modules on request (for testing purposes only)
  * ESP32_BH1750       Experimental
  * ESP32_Control      To control the ESP32
  * ESP32_I2C_Master   Experimental
  * ESP32_S0           For S0 counting
  * ESP32_SwITCH       For switchig and PWM with internal ESP32 hardware
 
NOTE: ESP32-Platform: Modules and Commands are linked in build process by the maker. LINUX-Platform: Supports dynamic Modules and Commands loading.

---
