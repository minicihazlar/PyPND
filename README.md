# PyPND

This is a Python module implementation for PROFINET based on SIEMENS' PN Driver product. In this way all the supported variants of PN Driver can be used in python. Even IRT mode can be supported if one of the CP1625 based variant is used.

As PN Driver is a 32 bit application, in order to compile the module a 32 bit Python needs to be used. Although it is only tested wiht Python v3.4 and above, it should work without or with mior changes even in Python v2.x.

It is not strictly required (if placed in another folder the paths in the win32 project and linux compile file needs to be adapted) but the repo should be placed under a folder (for example named as pypnd) under the PN Driver examples folder. 

Also an example application which mimics the PN Driver test_app is also provided to test the created module. 
