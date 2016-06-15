++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                             drivers/at_wini/at_wini.h
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
12000 #include "../drivers.h"
12001 #include "../libdriver/driver.h"
12002 #include "../libdriver/drvlib.h"
12003
12004 _PROTOTYPE(int main, (void));
12005
12006 #define VERBOSE 0 /* display identify messages during boot */
12007 #define ENABLE_ATAPI 0 /* add ATAPI cd-rom support to driver */