#/usr/bin/env python3

import pnd, sys, os

sys.setswitchinterval(0.001)

project_xml = "PND.xml"
menu_lock = False

#---------------------------------------------------------------------------------

def get_alarm_type(type):
    if type == 0x00:
        return "PNIO_ALARM_UNKNOWN"
    elif type == 0x01:
        return "PNIO_ALARM_DIAGNOSTIC"
    elif type == 0x02:
        return "PNIO_ALARM_PROCESS"
    elif type == 0x03:
        return "PNIO_ALARM_PULL"
    elif type == 0x04:
        return "PNIO_ALARM_PLUG"
    elif type == 0x05:
        return "PNIO_ALARM_STATUS"
    elif type == 0x06:
        return "PNIO_ALARM_UPDATE"
    elif type == 0x07:
        return "PNIO_ALARM_REDUNDANCY"
    elif type == 0x08:
        return "PNIO_ALARM_CONTROLLED_BY_SUPERVISOR"
    elif type == 0x09:
        return "PNIO_ALARM_RELEASED_BY_SUPERVISOR"
    elif type == 0x0A:
        return "PNIO_ALARM_PLUG_WRONG"
    elif type == 0x0B:
        return "PNIO_ALARM_RETURN_OF_SUBMODULE"
    elif type == 0x0C:
        return "PNIO_ALARM_DIAGNOSTIC_DISAPPEARS"
    elif type == 0x0D:
        return "PNIO_ALARM_MCR_MISMATCH"
    elif type == 0x0E:
        return "PNIO_ALARM_PORT_DATA_CHANGED"
    elif type == 0x0F:
        return "PNIO_ALARM_SYNC_DATA_CHANGED"
    elif type == 0x10:
        return "PNIO_ALARM_ISOCHRONE_MODE_PROBLEM"
    elif type == 0x11:
        return "PNIO_ALARM_NETWORK_COMPONENT_PROBLEM"
    elif type == 0x12:
        return "PNIO_ALARM_TIME_DATA_CHANGED"
    elif type == 0x13:
        return "PNIO_ALARM_DFP_PROBLEM"
    elif type == 0x14:
        return "PNIO_ALARM_MRPD_PROBLEM"
    elif type == 0x16:
        return "PNIO_ALARM_MULTIPLE_INTERFACE"
    elif type == 0x1E:
        return "PNIO_ALARM_UPLOAD_AND_STORAGE"
    elif type == 0x1F:
        return "PNIO_ALARM_PULL_MODULE"
    elif type == 0x00010000:
        return "PNIO_ALARM_DEV_FAILURE"
    elif type == 0x00010001:
        return "PNIO_ALARM_DEV_RETURN"

#---------------------------------------------------------------------------------

def callback(args):
    global menu_lock
    while menu_lock == True:
        pass
    menu_lock = True
    if args[0] == 1:
        print("<CALLBACK> Mode change indication received, new mode is", args[1])
    elif args[0] == 2:
        print("<CALLBACK> Alarm indication received")
        print("          AlarmType       :", get_alarm_type(args[1]))
        print("          DeviceNum       :", args[3])
        print("          SlotNum         :", args[4])
        print("          SubslotNum      :", args[5])
        print("          LADDR           :", args[6])
    elif args[0] == 8:
        if args[1] == 2:
            print("<CALLBACK> Interface NoS Confirmation indication received")
            print("          New NoS : ", args[2])
        elif args[1] == 1:
            print("<CALLBACK> Interface Set IP Address Confirmation indication received")
            print("          New IP      : ", args[2])
            print("          New Netmask : ", args[3])
            print("          New Gateway : ", args[4])
        elif args[1] == 3:
            print("<CALLBACK> Interface Set IP Address/NoS Confirmation indication received")
            print("          New NoS     : ", args[2])
            print("          New IP      : ", args[3])
            print("          New Netmask : ", args[4])
            print("          New Gateway : ", args[5])
    elif args[0] == 14:
        print("<CALLBACK> Interface Application Ready indication received")
        if args[1] != None:
            print("          ModuleDiffBlock present as below:")
            for api in args[1]:
                print("          API                       :", api[0])
                for modules in api[1]:
                    print("            Slot Number             :", modules[0])
                    print("            Module Ident Number     :", modules[1])
                    print("            Module State            :", modules[2])
                    for submodules in modules[3]:
                        print("              Subslot Number        :", submodules[0])
                        print("              Submodule Ident Number:", submodules[1])
                        print("              Submodule State       :", submodules[2])
                        if (submodules[2] & 0x0007) == 0x0001:
                            print("               -AddInfo             : Takeover is not allowed")
                        else:
                            print("               -AddInfo             : No additional info")
                        if (submodules[2] & 0x0008) == 0x0008:
                            print("               -Advice              : Advice information available")
                        else:
                            print("               -Advice              : No Advice information available")
                        if (submodules[2] & 0x0010) == 0x0010:
                            print("               -MaintenanceRequired : MaintenanceRequired information available")
                        else:
                            print("               -MaintenanceRequired : No MaintenanceRequired information available")
                        if (submodules[2] & 0x0020) == 0x0020:
                            print("               -MaintenanceDemanded : MaintenanceDemanded information available")
                        else:
                            print("               -MaintenanceDemanded : No MaintenanceDemanded information available")
                        if (submodules[2] & 0x0040) == 0x0040:
                            print("               -Fault               : Fault information available")
                        else:
                            print("               -Fault               : No Fault information available")
                        if (submodules[2] & 0x8000) == 0x8000:
                            if (submodules[2] & 0x0780) == 0:
                                print("               -ARInfo              : Own")
                            elif (submodules[2] & 0x0780) == 0x0080:
                                print("               -ARInfo              : Application Ready Pending")
                            elif (submodules[2] & 0x0780) == 0x0100:
                                print("               -ARInfo              : Superordinated Locked")
                            elif (submodules[2] & 0x0780) == 0x0180:
                                print("               -ARInfo              : Locked By IO Controller")
                            elif (submodules[2] & 0x0780) == 0x0200:
                                print("               -ARInfo              : Locked By IO Supervisor")
                            else:
                                print("               -ARInfo              : Reserved")
                            if (submodules[2] & 0x7800) == 0:
                                print("               -IdentInfo           : OK")
                            elif (submodules[2] & 0x7800) == 0x0800:
                                print("               -IdentInfo           : Substitute")
                            elif (submodules[2] & 0x7800) == 0x1000:
                                print("               -IdentInfo           : Wrong")
                            elif (submodules[2] & 0x7800) == 0x1800:
                                print("               -IdentInfo           : No Submodule")
                            else:
                                print("               -IdentInfo           : Reserved")
                        print("")
    else:
        print("<CALLBACK>", args)
    menu_lock = False

#---------------------------------------------------------------------------------

def input_number(message):
    while True:
        try:
            userInput = int(input(message))
        except ValueError:
            print("Not an integer! Try again\n")
            continue
        else:
            return userInput

#---------------------------------------------------------------------------------

def pnd_test_startup():
	pnd.serv_cp_init()
	a = pnd.serv_cp_get_network_adapters()
	print("Available interfaces:")
	print("------------------------------------------");
	for index, interface in enumerate(a):
		print("{:>2d} - {} - {}".format(index + 1, interface[0], interface[1]))
	print("------------------------------------------");
	while True:
		selection = input_number("Selection?\n")
		print("")
		if selection > len(a) or selection == 0:
			print("Invalid selection!\n")
		else:
			break
	pnd.pnio_interface_register_cbf(14, callback)
	pnd.serv_cp_startup(a[selection - 1], project_xml, 'CONTROLLER')

#---------------------------------------------------------------------------------

def pnd_test_rec_read_req():
    laddr = input_number("Logical address : ")
    recindex = input_number("Record index    : ")
    pnd.pnio_rec_read_req(laddr, recindex)

#---------------------------------------------------------------------------------

def pnd_test_interface_rec_read_req():
    laddr = input_number("Logical address : ")
    recindex = input_number("Record index    : ")
    pnd.pnio_interface_rec_read_req(laddr, recindex)

#---------------------------------------------------------------------------------

def pnd_test_interface_set_ip_or_nos():
	global menu_lock
	while menu_lock == True:
		pass
	menu_lock = True;
	print("---------------------------");
	print("1...IPv4 (IPv4 Suite)      ");
	print("2...NoS  (Name Of Station) ");
	print("---------------------------");
	print("0...Quit This              ");
	print("---------------------------");
	menu_lock = False
	while True:
		selection = input_number("Selection?\n")
		print("")
		if selection > 2:
			print("Invalid selection!\n")
		elif selection == 0:
			break
		elif selection == 1:
			ip = input("Ip: ")
			netmask = input("Netmask: ")
			gateway = input("Gateway: ")
			rema = input_number("Remanent (0-NO/1-YES): ")
			pnd.pnio_interface_set_ip_and_nos(1, (),(ip, netmask, gateway, rema))
		elif selection == 2:
			nos = input("Input the NoS: ")
			rema = input_number("Remanent (0-NO/1-YES): ")
			pnd.pnio_interface_set_ip_and_nos(2, (nos, rema), ())

#---------------------------------------------------------------------------------

def pnd_test_read_write_io_data():
	global menu_lock
	while menu_lock == True:
		pass
	menu_lock = True;
	print("---------------------------");
	print("1...Read IO Data           ");
	print("2...Write IO Data          ");
	print("---------------------------");
	print("0...Quit This              ");
	print("---------------------------");
	menu_lock = False
	while True:
		selection = input_number("Selection?\n")
		print("")
		if selection > 2:
			print("Invalid selection!\n")
		elif selection == 0:
			break
		elif selection == 1:
			iaddr = input("I Address: ")
			length = input("Length: ")
			locstate = input("Local State (0-GOOD/1-BAD): ")
			print(pnd.pnio_data_read(int(iaddr), int(length), int(locstate)))
		elif selection == 2:
			qaddr = input("Q Address: ")
			length = input("Length: ")
			buffer = input("Data: ")
			buffer = bytearray(int(x.strip(),16) for x in buffer.split(','))
			locstate = input("Local State (0-GOOD/1-BAD): ")
			print(pnd.pnio_data_write(int(qaddr), int(length, buffer), int(locstate)))

#---------------------------------------------------------------------------------

def print_menu():
	global menu_lock
	while menu_lock == True:
		pass
	menu_lock = True
	print("\nPROFINET Driver | Test Program");
	print("================================");
	print(" 1...Help (show menu)           ");
	print("================================");
	print(" 2...SERV_CP_Startup            ");
	print(" 4...SERV_CP_Shutdown           ");
	print("================================");
	print(" 5...Open Controller            ");
	print(" 6...Close Controller           ");
	print("================================");
	print(" 7...Set Mode PNIO_MODE_OFFLINE ");
	print(" 8...Set Mode PNIO_MODE_CLEAR   ");
	print(" 9...Set Mode PNIO_MODE_OPERATE ");
	print("================================");
	print("16...IO Data Read/Write         ");
	print("=============================== ");
	print("17...Record read                ");
	print("================================");
	print("18...List Modules and Submodules");
	print("================================");
	print("21...Open PDEV interface        ");
	print("22...Close PDEV interface       ");
	print("================================");
	print("23...Interface Record Read      ");
	print("================================");
	print("24...Interface Set IP or NoS    ");
	print("================================");
	print("0... QUIT \n");
	menu_lock = False

#---------------------------------------------------------------------------------

def testapp():
	while True:
		print_menu()
		selection = input_number("Selection?\n")
		print("")
		if selection == 1:
			pass

		elif selection == 2:
			pnd_test_startup()

		elif selection == 4:
			pnd.serv_cp_shutdown()
			pnd.serv_cp_undo_init()

		elif selection == 5:
			pnd.pnio_controller_open(callback, callback, callback)
			pnd.pnio_register_cbf(1,callback) # PNIO_CBE_MODE_IND
			pnd.pnio_register_cbf(5,callback) # PNIO_CBE_DEV_ACT_CONF
			pnd.pnio_register_cbf(7,callback) # PNIO_CBE_CTRL_DIAG_CONF
			pnd.pnio_register_cbf(10,callback) # PNIO_CBE_IOSYSTEM_RECONFIG

		elif selection == 6:
			pnd.pnio_controller_close()

		elif selection == 7:
			pnd.pnio_set_mode("OFFLINE")

		elif selection == 8:
			pnd.pnio_set_mode("CLEAR")

		elif selection == 9:
			pnd.pnio_set_mode("OPERATE")

		elif selection == 16:
			pnd_test_read_write_io_data()

		elif selection == 17:
			pnd_test_rec_read_req()

		elif selection == 18:
			pnd.pnio_ctrl_diag_req(1)

		elif selection == 21:
			pnd.pnio_interface_open(callback, callback, callback)
			pnd.pnio_interface_register_cbf(8, callback)
			pnd.pnio_interface_register_cbf(9, callback)

		elif selection == 22:
			pnd.pnio_interface_close()

		elif selection == 23:
			pnd_test_interface_rec_read_req()

		elif selection ==24:
			pnd_test_interface_set_ip_or_nos()

		elif selection == 0:
			sys.exit()

		else:
			print("Invalid selection!\n\n")

#---------------------------------------------------------------------------------

if __name__ == "__main__":
	if len(sys.argv) > 2:
		if sys.argv[1] == "-f":
			project_xml = sys.argv[2]
		elif sys.argv[1] == "-d":
			files = []
			index = 1
			print('Please select project XML file to be used:\n')
			for x in os.listdir(sys.argv[2]):
				if x.endswith('.xml'):
					files.append(x)
					print(str(index) + "-" + x)
					index += 1
			while True:
				selection = input_number("Selection?\n")
				if selection > len(files) or selection == 0:
					print("Invalid selection!\n")
				else:
					project_xml = sys.argv[2] + "/" + files[selection - 1]
					print(project_xml)	
					break
	testapp()
