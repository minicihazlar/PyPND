#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "servusrx.h"
#include "pniousrx.h"
#include "pniobase.h"

#define SERV_CP_MAX_NR_OF_CP	50

static PNIO_UINT32 apphandle = 0xFFFF;
static PNIO_UINT32 ifchandle = 0xFFFF;
static PNIO_UINT32 rolestore = 1;
static PyObject* callbackfunc[21] = { NULL }; // Yes you are right, total of callback functions should be 20
       			                								  // but as the PNIO_CBE_TYPE values start from 1, instead of
											                        // decrementing 1 in each array indexing, having 1 more in the
											                        // array is a much better comprimise, isn't it?
/*--------------------------------------------------------------------*/

void set_error_string(PNIO_UINT32 error_code)
{
	char const *msg;

	switch (error_code)
	{
		case PNIO_WARN_IRT_INCONSISTENT:
		{
			msg = "IRT Data may be inconsistent";
			break;
		}
		case PNIO_WARN_NO_SUBMODULES:
		{
			msg = "No submodules to be updated";
			break;
		}
		case PNIO_WARN_LOCAL_STATE_BAD:
		{
			msg = "Data was written with local state PNIO_S_BAD, because not all components of splitted module have local state PNIO_S_GOOD";
			break;
		}
		case PNIO_WARN_NO_DECENTRALIOSYSTEM:
		{
			msg = "No decentral IO system is found in the configuration";
			break;
		}
		case PNIO_WARN_VERSION_MISMATCH:
		{
			msg = "The version in hardware configuration is different from the version that is running";
			break;
		}
		case PNIO_ERR_PRM_HND:
		{
			msg = "Parameter handle is illegal";
			break;
		}
		case PNIO_ERR_PRM_BUF:
		{
			msg = "Parameter buffer is NULL pointer";
			break;
		}
		case PNIO_ERR_PRM_LEN:
		{
			msg = "Parameter length is wrong";
			break;
		}
		case PNIO_ERR_PRM_ADD:
		{
			msg = "Parameter address is wrong";
			break;
		}
		case PNIO_ERR_PRM_RSTATE:
		{
			msg = "Parameter remote state is NULL-Ptr";
			break;
		}
		case PNIO_ERR_PRM_CALLBACK:
		{
			msg = "Parameter callback function is illegal";
			break;
		}
		case PNIO_ERR_PRM_TYPE:
		{
			msg = "Parameter type has no valid value";
			break;
		}
		case PNIO_ERR_PRM_EXT_PAR:
		{
			msg = "Parameter ExtPar has no valid value";
			break;
		}
		case PNIO_ERR_PRM_IO_TYPE:
		{
			msg = "Parameter IODataType is wrong";
			break;
		}
		case PNIO_ERR_PRM_CP_ID:
		{
			msg = "Parameter CpIndex is wrong, propably driver is not loaded";
			break;
		}
		case PNIO_ERR_PRM_LOC_STATE:
		{
			 msg = "Parameter IO local state has no valid value";
			 break;
		}
		case PNIO_ERR_PRM_REC_INDEX:
		{
			msg = "Parameter record index has no valid value";
			break;
		}
		case PNIO_ERR_PRM_TIMEOUT:
		{
			msg = "Parameter timeout has no valid value";
			break;
		}
		case PNIO_ERR_PRM_DEV_ANNOTATION:
		{
			msg = "Parameter annotation has no valid value";
			break;
		}
		case PNIO_ERR_PRM_DEV_STATE:
		{
			msg = "Parameter state has no valid value";
			break;
		}
		case PNIO_ERR_PRM_PCBF:
		{
			msg = "Parameter callback function pointer  has no valid value";
			break;
		}
		case PNIO_ERR_PRM_MAX_AR_VALUE:
		{
			msg = "Parameter maximum AR has no valid value";
			break;
		}
		case PNIO_ERR_PRM_ACCESS_TYPE:
		{
			msg = "Parameter access type has no valid value";
			break;
		}
		case PNIO_ERR_PRM_POINTER:
		{
			msg = "Qn invalid pointer was passed";
			break;
		}
		case PNIO_ERR_PRM_INVALIDARG:
		{
			msg = "An invalid argument was passed";
			break;
		}
		case PNIO_ERR_PRM_MEASURE_NUMBER:
		{
			msg = "Wrong Measure No in cycle statistics, must be -1 (actual measure) up to 49";
			break;
		}
		case PNIO_ERR_PRM_CYCLE_OFFSET:
		{
			msg = "Wrong offset for cycle info buffer (must be 0 to 19)";
			break;
		}
		case PNIO_ERR_PRM_ROUTER_ADD:
		{
			msg = "Address used by io router";
			break;
		}
		case PNIO_ERR_PRM_IP:
		{
			msg = "Parameter IP has no valid value";
			break;
		}
		case PNIO_ERR_PRM_NOS:
		{
			msg = "Parameter NoS has no valid value";
			break;
		}
		case PNIO_ERR_PRM_NOS_LEN:
		{
			msg = "Parameter length is wrong";
			break;
		}
		case PNIO_ERR_PRM_CONSISTENCY:
		{
			msg = "The parameterization is not consistent";
			break;
		}
		case PNIO_ERR_WRONG_HND:
		{
			msg = "Unknown handle";
			break;
		}
		case PNIO_ERR_MAX_REACHED:
		{
			msg = "Maximal number of opens reached; close unused applications";
			break;
		}
		case PNIO_ERR_CREATE_INSTANCE:
		{
			msg = "Fatal error, reboot your system";
			break;
		}
		case PNIO_ERR_MODE_VALUE:
		{
			msg = "Parameter mode has no valid value";
			break;
		}
		case PNIO_ERR_OPFAULT_NOT_REG:
		{
			msg = "Register OPFAULT callback before register STARTOP callback";
			break;
		}
		case PNIO_ERR_NEWCYCLE_SEQUENCE_REG:
		{
			msg = "Register NEWCYCLE callback before register STARTOP callback";
			break;
		}
		case PNIO_ERR_NETWORK_PROT_NOT_AVAILABLE:
		{
			msg = "Network protocol not available, check card configuration";
			break;
		}
		case PNIO_ERR_RETRY:
		{
			msg = "PNIO stack not available, try again later";
			break;
		}
		case PNIO_ERR_NO_CONNECTION:
		{
			msg = "Device data not available, because device is not connected to controller";
			break;
		}
		case PNIO_ERR_OS_RES:
		{
			msg = "Fatal error, no more operation system resources available";
			break;
		}
		case PNIO_ERR_ALREADY_DONE:
		{
			msg = "Action was already performed";
			break;
		}
		case PNIO_ERR_NO_CONFIG:
		{
			msg = "No configuration for this index available";
			break;
		}
		case PNIO_ERR_SET_MODE_NOT_ALLOWED:
		{
			msg = "PNIO_set_mode not allowed, use PNIO_CEP_MODE_CTRL by PNIO_controller_open";
			break;
		}
		case PNIO_ERR_DEV_ACT_NOT_ALLOWED:
		{
			msg = "PNIO_device_activate not allowed, use PNIO_CEP_MODE_CTRL by PNIO_controller_open";
			break;
		}
		case PNIO_ERR_NO_LIC_SERVER:
		{
			msg = "Licence server not running, check your installation";
			break;
		}
		case PNIO_ERR_VALUE_LEN:
		{
			msg = "Wrong length value";
			break;
		}
		case PNIO_ERR_SEQUENCE:
		{
			msg = "Wrong calling sequence";
			break;
		}
		case PNIO_ERR_INVALID_CONFIG:
		{
			msg = "Invalid configuration, check your configuration";
			break;
		}
		case PNIO_ERR_UNKNOWN_ADDR:
		{
			msg = "Address unknown in configuration, check your configuration";
			break;
		}
		case PNIO_ERR_NO_RESOURCE:
		{
			msg = "No resouce too many requests been processed";
			break;
		}
		case PNIO_ERR_CONFIG_IN_UPDATE:
		{
			msg = "Configuration update is in progress or CP is in STOP state, try again later";
			break;
		}
		case PNIO_ERR_NO_FW_COMMUNICATION:
		{
			msg = "No communication with firmware, reset CP or try again later";
			break;
		}
		case PNIO_ERR_STARTOP_NOT_REGISTERED:
		{
			msg = "No synchonous function allowed, use PNIO_CEP_SYNC_MODE by PNIO_controller_open or PNIO_device_open";
			break;
		}
		case PNIO_ERR_OWNED:
		{
			msg = "Interface-submodule cannot be removed because it is owned by an AR";
			break;
		}
		case PNIO_ERR_START_THREAD_FAILED:
		{
			msg = "Failed to start thread, propably by lack of pthread resources";
			break;
		}
		case PNIO_ERR_START_RT_THREAD_FAILED:
		{
			msg = "Failed to start realtime thread, propably you need root capability to do it";
			break;
		}
		case PNIO_ERR_DRIVER_IOCTL_FAILED:
		{
			msg = "Failed in ioctl driver, propably API version mismatch";
			break;
		}
		case PNIO_ERR_AFTER_EXCEPTION:
		{
			msg = "Exception ocurred, save exception info (see manual) and reset CP";
			break;
		}
		case PNIO_ERR_NO_CYCLE_INFO_DATA:
		{
			msg = "No cycle data available";
			break;
		}
		case PNIO_ERR_SESSION:
		{
			msg = "Request belongs to an old session";
			break;
		}
		case PNIO_ERR_ALARM_DATA_FORMAT:
		{
			msg = "Wrong format of alarm data";
			break;
		}
		case PNIO_ERR_ABORT:
		{
			msg = "Operation was aborted";
			break;
		}
		case PNIO_ERR_CORRUPTED_DATA:
		{
			msg = "Data is corrupted or have wrong format";
			break;
		}
		case PNIO_ERR_FLASH_ACCESS:
		{
			msg = "Error during flash operations";
			break;
		}
		case PNIO_ERR_WRONG_RQB_LEN:
		{
			msg = "Wrong length of request block at firmware interface, firmware not compatible to host SW";
			break;
		}
		case PNIO_ERR_NO_RESET_VERIFICATION:
		{
			msg = "Reset request was sent to firmware, but firmware rut up can't be verified";
			break;
		}
		case PNIO_ERR_SET_IP_NOS_NOT_ALLOWED:
		{
			msg = "Setting IP and/or NoS is not allowed";
			break;
		}
		case PNIO_ERR_INVALID_REMA:
		{
			msg = "REMA data is not valid";
			break;
		}
		case PNIO_ERR_NOT_REENTERABLE:
		{
			msg = "The function is not reenterable";
			break;
		}
		case PNIO_ERR_INVALID_STATION:
		{
			msg = "Station does not exist or is configured as optional";
			break;
		}
		case PNIO_ERR_INVALID_PORT:
		{
			msg = "Port not configured as programmable peer";
			break;
		}
		case PNIO_ERR_NO_ADAPTER_FOUND:
		{
			msg = "No ethernet adapter found";
			break;
		}
		case PNIO_ERR_ACCESS_DENIED:
		{
			msg = "Access denied";
			break;
		}
		case PNIO_ERR_VARIANT_MISMATCH:
		{
			msg = "The variant in hardware configuration is different from the variant that is running";
			break;
		}
		case PNIO_ERR_MEMORY_OPERATION:
		{
			msg = "Memory allocation or deallocation error";
			break;
		}
		case PNIO_ERR_UNEXPECTED_CM_ERROR:
		{
			msg = "Cannot map CM error code to PNIO error code";
			break;
		}
		case PNIO_ERR_EDD_STATUS:
		{
			msg = "ISO application sync error caused by EDD status";
			break;
		}
		case PNIO_ERR_INTERNAL:
		{
			msg = "Fatal error, contact devkits support at profinet.devkits.industry@siemens.com";
			break;
		}
		default:
		{
			msg = "Unknown error occured";
		}
	}
	char err[255];
	sprintf(err, "%s! Error code is %ld", msg, error_code);
	PyErr_SetString(PyExc_RuntimeError, err);
}

/*--------------------------------------------------------------------*/

PyObject* populate_cbf_args(PNIO_CBE_PRM* pCbfPrm)
{
	PyObject *args = Py_None;

	switch (pCbfPrm->CbeType)
	{
	case PNIO_CBE_MODE_IND:
	{
		switch (pCbfPrm->u.ModeInd.Mode)
		{
		case PNIO_MODE_OFFLINE:
			args = PyTuple_Pack(1, Py_BuildValue("(is)", pCbfPrm->CbeType, "OFFLINE"));
			break;

		case PNIO_MODE_CLEAR:
			args = PyTuple_Pack(1, Py_BuildValue("(is)", pCbfPrm->CbeType, "CLEAR"));
			break;

		case PNIO_MODE_OPERATE:
			args = PyTuple_Pack(1, Py_BuildValue("(is)", pCbfPrm->CbeType, "OPERATE"));
			break;

		}
		break;
	}
	case PNIO_CBE_ALARM_IND:
	case PNIO_CBE_IFC_ALARM_IND:
	{
		args = PyTuple_Pack(1, Py_BuildValue("(iiiHHHII(bHHH)(IHIIH(y#)))",
			pCbfPrm->CbeType,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmType,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmPriority,
			pCbfPrm->u.AlarmInd.pAlarmData->DeviceNum,
			pCbfPrm->u.AlarmInd.pAlarmData->SlotNum,
			pCbfPrm->u.AlarmInd.pAlarmData->SubslotNum,
			pCbfPrm->u.AlarmInd.pAlarmData->LADDR,
			pCbfPrm->u.AlarmInd.pAlarmData->PnioCompatModtype,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmTinfo.DeviceFlag,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmTinfo.PnioVendorIdent,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmTinfo.PnioDevIdent,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmTinfo.PnioDevInstance,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.API,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.AlarmSpecifier,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.ModIdent,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.SubIdent,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.UserStrucIdent,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.UAData.UserAlarmData,
			pCbfPrm->u.AlarmInd.pAlarmData->AlarmAinfo.UserAlarmDataLen));
		break;
	}
	case PNIO_CBE_REC_READ_CONF:
	case PNIO_CBE_IFC_REC_READ_CONF:
	{
		PyObject *addr = NULL;

		if (pCbfPrm->u.RecReadConf.pAddr->AddrType == PNIO_ADDR_LOG)
		{
			addr = Py_BuildValue("III", pCbfPrm->u.RecReadConf.pAddr->AddrType,
				pCbfPrm->u.RecReadConf.pAddr->IODataType,
				pCbfPrm->u.RecReadConf.pAddr->u.Addr);
		}
		else
		{
			addr = Py_BuildValue("IIIHH", pCbfPrm->u.RecReadConf.pAddr->AddrType,
				pCbfPrm->u.RecReadConf.pAddr->IODataType,
				pCbfPrm->u.RecReadConf.pAddr->u.Geo.API,
				pCbfPrm->u.RecReadConf.pAddr->u.Geo.Slot,
				pCbfPrm->u.RecReadConf.pAddr->u.Geo.Subslot);
		}
		args = PyTuple_Pack(1, Py_BuildValue("(i(O)III(BBBBHH)(y#)HH)",
			pCbfPrm->CbeType,
			addr,
			pCbfPrm->u.RecReadConf.RecordIndex,
			pCbfPrm->u.RecReadConf.ReqRef,
			pCbfPrm->u.RecReadConf.Response,
			pCbfPrm->u.RecReadConf.Err.ErrCode,
			pCbfPrm->u.RecReadConf.Err.ErrDecode,
			pCbfPrm->u.RecReadConf.Err.ErrCode1,
			pCbfPrm->u.RecReadConf.Err.ErrCode2,
			pCbfPrm->u.RecReadConf.Err.AddValue1,
			pCbfPrm->u.RecReadConf.Err.AddValue2,
			pCbfPrm->u.RecReadConf.pBuffer,
			pCbfPrm->u.RecReadConf.Length,
			pCbfPrm->u.RecReadConf.VendorID,
			pCbfPrm->u.RecReadConf.DeviceID));
		break;
	}
	case PNIO_CBE_REC_WRITE_CONF:
	case PNIO_CBE_IFC_REC_WRITE_CONF:
	{
		PyObject *addr = NULL;

		if (pCbfPrm->u.RecWriteConf.pAddr->AddrType == PNIO_ADDR_LOG)
		{
			addr = Py_BuildValue("III", pCbfPrm->u.RecWriteConf.pAddr->AddrType,
				pCbfPrm->u.RecWriteConf.pAddr->IODataType,
				pCbfPrm->u.RecWriteConf.pAddr->u.Addr);
		}
		else
		{
			addr = Py_BuildValue("IIIHH", pCbfPrm->u.RecWriteConf.pAddr->AddrType,
				pCbfPrm->u.RecWriteConf.pAddr->IODataType,
				pCbfPrm->u.RecWriteConf.pAddr->u.Geo.API,
				pCbfPrm->u.RecWriteConf.pAddr->u.Geo.Slot,
				pCbfPrm->u.RecWriteConf.pAddr->u.Geo.Subslot);
		}
		args = PyTuple_Pack(1, Py_BuildValue("(i(O)II(y#)IB(BBBBHH))",
			pCbfPrm->CbeType,
			addr,
			pCbfPrm->u.RecWriteConf.RecordIndex,
			pCbfPrm->u.RecWriteConf.ReqRef,
			pCbfPrm->u.RecWriteConf.pBuffer,
			pCbfPrm->u.RecWriteConf.Length,
			pCbfPrm->u.RecWriteConf.Response,
			pCbfPrm->u.RecWriteConf.RecReset,
			pCbfPrm->u.RecWriteConf.Err.ErrCode,
			pCbfPrm->u.RecWriteConf.Err.ErrDecode,
			pCbfPrm->u.RecWriteConf.Err.ErrCode1,
			pCbfPrm->u.RecWriteConf.Err.ErrCode2,
			pCbfPrm->u.RecWriteConf.Err.AddValue1,
			pCbfPrm->u.RecWriteConf.Err.AddValue2));
		break;
	}
	case PNIO_CBE_DEV_ACT_CONF:
	{
		PyObject *addr;

		if (pCbfPrm->u.DevActConf.pAddr->AddrType == PNIO_ADDR_LOG)
		{
			addr = Py_BuildValue("III", pCbfPrm->u.DevActConf.pAddr->AddrType,
				pCbfPrm->u.DevActConf.pAddr->IODataType,
				pCbfPrm->u.DevActConf.pAddr->u.Addr);
		}
		else
		{
			addr = Py_BuildValue("IIIHH", pCbfPrm->u.DevActConf.pAddr->AddrType,
				pCbfPrm->u.DevActConf.pAddr->IODataType,
				pCbfPrm->u.DevActConf.pAddr->u.Geo.API,
				pCbfPrm->u.DevActConf.pAddr->u.Geo.Slot,
				pCbfPrm->u.DevActConf.pAddr->u.Geo.Subslot);
		}
		args = PyTuple_Pack(1, Py_BuildValue("(i(O)II))",
			pCbfPrm->CbeType,
			addr,
			pCbfPrm->u.DevActConf.Mode,
			pCbfPrm->u.DevActConf.Result));
		break;
	}
	case PNIO_CBE_CTRL_DIAG_CONF:
	{
		PyObject *addr = Py_None;
		PyObject *data = Py_None;

		switch (pCbfPrm->u.CtrlDiagConf.pDiagData->DiagService)
		{
			case PNIO_CTRL_DIAG_CONFIG_SUBMODULE_LIST:
			{
				PNIO_CTRL_DIAG_CONFIG_SUBMODULE *pDiagConfSub;
				PNIO_UINT32 submodule_cnt = 0;


				pDiagConfSub = (PNIO_CTRL_DIAG_CONFIG_SUBMODULE *)pCbfPrm->u.CtrlDiagConf.pDiagDataBuffer;
				submodule_cnt = (pCbfPrm->u.CtrlDiagConf.DiagDataBufferLen) / sizeof(PNIO_CTRL_DIAG_CONFIG_SUBMODULE);
				data = PyTuple_New(submodule_cnt);
				for (PNIO_UINT32 i = 0; i < submodule_cnt; i++)
				{
					PyObject *addr = NULL;

					if (pDiagConfSub[i].Address.AddrType == PNIO_ADDR_LOG)
					{
						addr = Py_BuildValue("III", pDiagConfSub[i].Address.AddrType,
							                        pDiagConfSub[i].Address.IODataType,
							                        pDiagConfSub[i].Address.u.Addr);
					}
					else
					{
						addr = Py_BuildValue("IIIHH", pDiagConfSub[i].Address.AddrType,
							                          pDiagConfSub[i].Address.IODataType,
							                          pDiagConfSub[i].Address.u.Geo.API,
							                          pDiagConfSub[i].Address.u.Geo.Slot,
							                          pDiagConfSub[i].Address.u.Geo.Subslot);
					}
					PyTuple_SetItem(data, i, Py_BuildValue("(OIIIiiIIIIIBIIIIII)", addr,
																	               pDiagConfSub[i].DataLength,
 																				   pDiagConfSub[i].InDataLength,
																				   pDiagConfSub[i].OutDataLength,
																				   pDiagConfSub[i].DataType,
																				   pDiagConfSub[i].ComType,
																				   pDiagConfSub[i].API,
																				   pDiagConfSub[i].ReductionRatio,
																			 	   pDiagConfSub[i].Phase,
																				   pDiagConfSub[i].CycleTime,
																				   pDiagConfSub[i].HwIdentifier,
																				   pDiagConfSub[i].AddressValid,
																				   pDiagConfSub[i].StatNo,
																			 	   pDiagConfSub[i].ParentDeviceLADDR,
																				   pDiagConfSub[i].Slot,
																				   pDiagConfSub[i].Subslot,
																				   pDiagConfSub[i].ModIdent,
																				   pDiagConfSub[i].SubModIdent));
 				}
				break;
			}
			case PNIO_CTRL_DIAG_DEVICE_DIAGNOSTIC:
			{
				PNIO_CTRL_DIAG_DEVICE_DIAGNOSTIC_DATA *pDiagDeviceState;

				pDiagDeviceState = (PNIO_CTRL_DIAG_DEVICE_DIAGNOSTIC_DATA *)pCbfPrm->u.CtrlDiagConf.pDiagDataBuffer;
				pDiagDeviceState->ErrorCause = (pDiagDeviceState->ErrorCause >> 8) | (pDiagDeviceState->ErrorCause << 8);

				data = Py_BuildValue("(iHB(y#))", pDiagDeviceState->Mode,
					                            pDiagDeviceState->ErrorCause,
					                            pDiagDeviceState->ReasonCode,
					                            pDiagDeviceState->AdditionalInfo, 10);
				break;
			}
			case PNIO_CTRL_DIAG_CONFIG_NAME_ADDR_INFO:
			{
				PNIO_CTRL_DIAG_CONFIG_NAME_ADDR_INFO_DATA *pDiagConfigNameAddrInfo;

				pDiagConfigNameAddrInfo = (PNIO_CTRL_DIAG_CONFIG_NAME_ADDR_INFO_DATA *)pCbfPrm->u.CtrlDiagConf.pDiagDataBuffer;
				char ip[32];
				char netmask[32] = {};
				char gateway[32] = {};

				sprintf(ip, "%hhu.%hhu.%hhu.%hhu", pDiagConfigNameAddrInfo->ip_addr[0],
									               pDiagConfigNameAddrInfo->ip_addr[1],
										           pDiagConfigNameAddrInfo->ip_addr[2],
										           pDiagConfigNameAddrInfo->ip_addr[3]);
				sprintf(netmask, "%hhu.%hhu.%hhu.%hhu", pDiagConfigNameAddrInfo->ip_mask[0],
				 						                pDiagConfigNameAddrInfo->ip_mask[1],
											            pDiagConfigNameAddrInfo->ip_mask[2],
											            pDiagConfigNameAddrInfo->ip_mask[3]);
				sprintf(gateway, "%hhu.%hhu.%hhu.%hhu", pDiagConfigNameAddrInfo->default_router[0],
										                pDiagConfigNameAddrInfo->default_router[1],
											            pDiagConfigNameAddrInfo->default_router[2],
											            pDiagConfigNameAddrInfo->default_router[3]);
				data = Py_BuildValue("(sssss)", pDiagConfigNameAddrInfo->name, pDiagConfigNameAddrInfo->TypeOfStation, ip, netmask, gateway);
				break;
			}
            default:
            {
                data = Py_None;
            }
		}
		if (pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.AddrType == PNIO_ADDR_LOG)
		{
			addr = Py_BuildValue("III", pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.AddrType,
				pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.IODataType,
				pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.u.Addr);
		}
		else
		{
			addr = Py_BuildValue("IIIHH", pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.AddrType,
				pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.IODataType,
				pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.u.Geo.API,
				pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.u.Geo.Slot,
				pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr.u.Geo.Subslot);
		}
		args = PyTuple_Pack(1, Py_BuildValue("(iIOIII(HBBBHIHH)BOI)",
			pCbfPrm->CbeType,
			pCbfPrm->u.CtrlDiagConf.pDiagData->DiagService,
			addr,
			pCbfPrm->u.CtrlDiagConf.pDiagData->ReqRef,
			pCbfPrm->u.CtrlDiagConf.pDiagData->DiagMode,
			pCbfPrm->u.CtrlDiagConf.pDiagData->DiagType,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ErrorTypeName,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.Maintenance_req,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.Maintenance_dem,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.Accumulative,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ExtChannelErrorType,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ExtChannelAddValue,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.InfoTag,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ChannelNumber,
			pCbfPrm->u.CtrlDiagConf.pDiagData->SendAlarm,
			data,
			pCbfPrm->u.CtrlDiagConf.ErrorCode));
		break;
	}
	case PNIO_CBE_IFC_SET_ADDR_CONF:
	{
		if ((pCbfPrm->u.SetAddrConf.Mode & PNIO_SET_IP_MODE) == pCbfPrm->u.SetAddrConf.Mode)
		{
			char ip[32];
			char netmask[32];
			char gateway[32];

			sprintf(ip, "%hhu.%hhu.%hhu.%hhu", pCbfPrm->u.SetAddrConf.LocalIPAddress[0], pCbfPrm->u.SetAddrConf.LocalIPAddress[1], pCbfPrm->u.SetAddrConf.LocalIPAddress[2], pCbfPrm->u.SetAddrConf.LocalIPAddress[3]);
			sprintf(netmask, "%hhu.%hhu.%hhu.%hhu", pCbfPrm->u.SetAddrConf.LocalSubnetMask[0], pCbfPrm->u.SetAddrConf.LocalSubnetMask[1], pCbfPrm->u.SetAddrConf.LocalSubnetMask[2], pCbfPrm->u.SetAddrConf.LocalSubnetMask[3]);
			sprintf(gateway, "%hhu.%hhu.%hhu.%hhu", pCbfPrm->u.SetAddrConf.DefaultRouterAddr[0], pCbfPrm->u.SetAddrConf.DefaultRouterAddr[1], pCbfPrm->u.SetAddrConf.DefaultRouterAddr[2], pCbfPrm->u.SetAddrConf.DefaultRouterAddr[3]);
			args = PyTuple_Pack(1, Py_BuildValue("(iIs#s#s#II)",
				pCbfPrm->CbeType,
				pCbfPrm->u.SetAddrConf.Mode,
				ip, strlen(ip),
				netmask, strlen(netmask),
				gateway, strlen(gateway),
				pCbfPrm->u.SetAddrConf.Options,
				pCbfPrm->u.SetAddrConf.Err));
		}
		else if ((pCbfPrm->u.SetAddrConf.Mode & PNIO_SET_NOS_MODE) == pCbfPrm->u.SetAddrConf.Mode)
		{
			args = PyTuple_Pack(1, Py_BuildValue("(iIs#II)",
				pCbfPrm->CbeType,
				pCbfPrm->u.SetAddrConf.Mode,
				pCbfPrm->u.SetAddrConf.pStationName,
				pCbfPrm->u.SetAddrConf.StationNameLen,
				pCbfPrm->u.SetAddrConf.Options,
				pCbfPrm->u.SetAddrConf.Err));
		}
		else
		{
			char ip[32];
			char netmask[32];
			char gateway[32];

			sprintf(ip, "%hhu.%hhu.%hhu.%hhu", pCbfPrm->u.SetAddrConf.LocalIPAddress[0], pCbfPrm->u.SetAddrConf.LocalIPAddress[1], pCbfPrm->u.SetAddrConf.LocalIPAddress[2], pCbfPrm->u.SetAddrConf.LocalIPAddress[3]);
			sprintf(netmask, "%hhu.%hhu.%hhu.%hhu", pCbfPrm->u.SetAddrConf.LocalSubnetMask[0], pCbfPrm->u.SetAddrConf.LocalSubnetMask[1], pCbfPrm->u.SetAddrConf.LocalSubnetMask[2], pCbfPrm->u.SetAddrConf.LocalSubnetMask[3]);
			sprintf(gateway, "%hhu.%hhu.%hhu.%hhu", pCbfPrm->u.SetAddrConf.DefaultRouterAddr[0], pCbfPrm->u.SetAddrConf.DefaultRouterAddr[1], pCbfPrm->u.SetAddrConf.DefaultRouterAddr[2], pCbfPrm->u.SetAddrConf.DefaultRouterAddr[3]);
			args = PyTuple_Pack(1, Py_BuildValue("(iIs#s#s#s#II)",
				pCbfPrm->CbeType,
				pCbfPrm->u.SetAddrConf.Mode,
				pCbfPrm->u.SetAddrConf.pStationName,
				pCbfPrm->u.SetAddrConf.StationNameLen,
				ip, strlen(ip),
				netmask, strlen(netmask),
				gateway, strlen(gateway),
				pCbfPrm->u.SetAddrConf.Options,
				pCbfPrm->u.SetAddrConf.Err));
		}
		break;
	}
	case PNIO_CBE_REMA_READ_CONF:
	{
		args = PyTuple_Pack(1, Py_BuildValue("(i(y#)I)",
			pCbfPrm->CbeType,
			pCbfPrm->u.RemaReadConf.RemaXMLBuffer,
			pCbfPrm->u.RemaReadConf.RemaXMLBufferLength,
			pCbfPrm->u.RemaReadConf.Err));
		break;
	}
	case PNIO_CBE_IOSYSTEM_RECONFIG:
	{
		PyObject *addr = NULL;

		if (pCbfPrm->u.IoSystemReconf.pAddr->AddrType == PNIO_ADDR_LOG)
		{
			addr = Py_BuildValue("III", pCbfPrm->u.IoSystemReconf.pAddr->AddrType,
				pCbfPrm->u.IoSystemReconf.pAddr->IODataType,
				pCbfPrm->u.IoSystemReconf.pAddr->u.Addr);
		}
		else
		{
			addr = Py_BuildValue("IIIHH", pCbfPrm->u.IoSystemReconf.pAddr->AddrType,
				pCbfPrm->u.IoSystemReconf.pAddr->IODataType,
				pCbfPrm->u.IoSystemReconf.pAddr->u.Geo.API,
				pCbfPrm->u.IoSystemReconf.pAddr->u.Geo.Slot,
				pCbfPrm->u.IoSystemReconf.pAddr->u.Geo.Subslot);
		}
		args = PyTuple_Pack(1, Py_BuildValue("(iOII)",
			pCbfPrm->CbeType,
			addr,
			pCbfPrm->u.IoSystemReconf.Mode,
			pCbfPrm->u.IoSystemReconf.Status));
		break;
	}
	case PNIO_CBE_IFC_APPL_READY:
	{
		if (pCbfPrm->u.ApplReadyInd.pApplReadyData->ModDiffBlock.ModDiffBlockLength != 0)
		{
			uint8_t *mdb = (uint8_t *)(pCbfPrm->u.ApplReadyInd.pApplReadyData->ModDiffBlock.pModDiffBlock);
			uint16_t nofapis = (mdb[6] << 8) | mdb[7];
			mdb += 8;
			PyObject *apis = PyTuple_New(nofapis);
			for (int i = 0; i < nofapis; ++i)
			{
				uint32_t api = (mdb[0] << 24) | (mdb[1] << 16) | (mdb[2] << 8) | mdb[3];
				mdb += 4;
				uint16_t nofmodules = (mdb[0] << 8) | mdb[1];
				mdb += 2;
				PyObject *modules = PyTuple_New(nofmodules);
				for (int j = 0; j < nofmodules; ++j)
				{
					uint16_t slotn = (mdb[0] << 8) | mdb[1];
					mdb += 2;
					uint32_t modidn = (mdb[0] << 24) | (mdb[1] << 16) | (mdb[2] << 8) | mdb[3];
					mdb += 4;
					char modst[32];

					switch ((mdb[0] << 8) | mdb[1])
					{
					case 0:
						strcpy(modst, "No Module");
						break;

					case 1:
						strcpy(modst, "Wrong Module");
						break;

					case 2:
						strcpy(modst, "Proper Module");
						break;

					case 3:
						strcpy(modst, "Substitute Module");
						break;

					default:
						strcpy(modst, "Unknown State");
						break;
					}
					mdb += 2;
					uint16_t nofsubmodules = (mdb[0] << 8) | mdb[1];
					mdb += 2;
					PyObject *submodules = PyTuple_New(nofsubmodules);
					for (int k = 0; k < nofsubmodules; ++k)
					{
						uint16_t subslotn = (mdb[0] << 8) | mdb[1];
						mdb += 2;
						uint32_t submodidn = (mdb[0] << 24) | (mdb[1] << 16) | (mdb[2] << 8) | mdb[3];
						mdb += 4;
						uint16_t submodstate = (mdb[0] << 8) | mdb[1];
						mdb += 2;
						PyTuple_SetItem(submodules, k, Py_BuildValue("HIH", subslotn, submodidn, submodstate));
					}
					PyTuple_SetItem(modules, j, Py_BuildValue("HIsO", slotn, modidn, modst, submodules));
				}
				PyTuple_SetItem(apis, i, Py_BuildValue("IO", api, modules));
			}
			args = PyTuple_Pack(1, Py_BuildValue("iO", pCbfPrm->CbeType, apis));
		}
		else
		{
			args = PyTuple_Pack(1, Py_BuildValue("iO", pCbfPrm->CbeType, Py_None));
		}
		break;
	}
	case PNIO_CBE_DEV_DIAG_CONF:
	{
		PyObject *diag = Py_BuildValue("(I(y#)III(HBBBHIHH)B)",
			pCbfPrm->u.CtrlDiagConf.pDiagData->DiagService,
			pCbfPrm->u.CtrlDiagConf.pDiagData->u.Addr, 8,
			pCbfPrm->u.CtrlDiagConf.pDiagData->ReqRef,
			pCbfPrm->u.CtrlDiagConf.pDiagData->DiagMode,
			pCbfPrm->u.CtrlDiagConf.pDiagData->DiagType,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ErrorTypeName,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.Maintenance_req,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.Maintenance_dem,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.Accumulative,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ExtChannelErrorType,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ExtChannelAddValue,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.InfoTag,
			pCbfPrm->u.CtrlDiagConf.pDiagData->Advanced.ChannelNumber,
			pCbfPrm->u.CtrlDiagConf.pDiagData->SendAlarm);
		args = PyTuple_Pack(1, Py_BuildValue("(iO(y#)I)",
			pCbfPrm->CbeType,
			diag,
			pCbfPrm->u.DevDiagConf.pDiagDataBuffer,
			pCbfPrm->u.DevDiagConf.DiagDataBufferLen,
			pCbfPrm->u.DevDiagConf.ErrorCode));
		break;
	}
	case PNIO_CBE_DEV_CONNECT_IND:
	{
		args = PyTuple_Pack(1, Py_BuildValue("(iIIHHHH)",
			pCbfPrm->CbeType,
			pCbfPrm->u.ConnectInd.HostIp,
			pCbfPrm->u.ConnectInd.ArNum,
			pCbfPrm->u.ConnectInd.ArSessionKey,
			pCbfPrm->u.ConnectInd.ReductionRatioOut,
			pCbfPrm->u.ConnectInd.ReductionRatioIn,
			pCbfPrm->u.ConnectInd.SendClock));
		break;
	}
	case PNIO_CBE_DEV_DISCONNECT_IND:
	{
		args = PyTuple_Pack(1, Py_BuildValue("(iIIH)",
			pCbfPrm->CbeType,
			pCbfPrm->u.DisconnectInd.ArNum,
			pCbfPrm->u.DisconnectInd.ReasonCode,
			pCbfPrm->u.DisconnectInd.SessionKey));
		break;
	}
	case PNIO_CBE_DEV_OWNERSHIP_IND:
	{
		args = PyTuple_Pack(1, Py_BuildValue("(i(IIIIIHBIH)IH)",
			pCbfPrm->CbeType,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->ApiNum,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->SlotNum,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->SubSlotNum,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->ModIdent,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->SubIdent,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->OwnSessionKey,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->IsWrongSubmod,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->SubProperties,
			pCbfPrm->u.OwnershipInd.pExpSubmodules->DataLength,
			pCbfPrm->u.OwnershipInd.ArNum,
			pCbfPrm->u.OwnershipInd.NrOfExpSubmodule));
		break;
	}
	case PNIO_CBE_DEV_PRMEND_IND:
	{
		PyObject *addr = NULL;

		if (pCbfPrm->u.PrmendInd.pAddr->AddrType == PNIO_ADDR_LOG)
		{
			addr = Py_BuildValue("III", pCbfPrm->u.PrmendInd.pAddr->AddrType,
				pCbfPrm->u.PrmendInd.pAddr->IODataType,
				pCbfPrm->u.PrmendInd.pAddr->u.Addr);
		}
		else
		{
			addr = Py_BuildValue("IIIHH", pCbfPrm->u.PrmendInd.pAddr->AddrType,
				pCbfPrm->u.PrmendInd.pAddr->IODataType,
				pCbfPrm->u.PrmendInd.pAddr->u.Geo.API,
				pCbfPrm->u.PrmendInd.pAddr->u.Geo.Slot,
				pCbfPrm->u.PrmendInd.pAddr->u.Geo.Subslot);
		}
		args = PyTuple_Pack(1, Py_BuildValue("(iOHHH)",
			pCbfPrm->CbeType,
			addr,
			pCbfPrm->u.PrmendInd.NrOfElem,
			pCbfPrm->u.PrmendInd.ArNum,
			pCbfPrm->u.PrmendInd.SessionKey));
		break;
	}
	case PNIO_CBE_DEV_INDATA_IND:
	{
		args = PyTuple_Pack(1, Py_BuildValue("(iIH)",
			pCbfPrm->CbeType,
			pCbfPrm->u.IndataInd.ArNum,
			pCbfPrm->u.IndataInd.SessionKey));
		break;
	}
	case PNIO_CBE_CP_STOP_REQ:
	{
		break;
	}
	}
	return args;
}

/*--------------------------------------------------------------------*/

void cbf(PNIO_CBE_PRM* pCbfPrm)
{
	PyGILState_STATE gstate;
	gstate = PyGILState_Ensure();
	PyObject *args = populate_cbf_args(pCbfPrm);
	PyObject *result = PyObject_CallObject(callbackfunc[pCbfPrm->CbeType], args);
	if (result == NULL)
	{
		PyErr_Print();
	}
	Py_CLEAR(result);
	Py_DECREF(args);
	PyGILState_Release(gstate);
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* serv_cp_init(PyObject* self)
{
	PNIO_UINT32 ret;

	// INFO: Tracing for now.
	ret = SERV_CP_init(NULL);
	if(ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	PyErr_SetString(PyExc_RuntimeError, "Already initialized!");
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* serv_cp_undo_init(PyObject* self)
{
	PNIO_UINT32 ret;

	ret = SERV_CP_undo_init();
	if(ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	PyErr_SetString(PyExc_RuntimeError, "Already uninitialized!");
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* serv_cp_get_network_adapters(PyObject* self)
{
	PNIO_CP_ID_PTR_TYPE cplist;
	PNIO_UINT8 nrofcp;
	PNIO_UINT32 ret;

	cplist = (PNIO_CP_ID_PTR_TYPE)calloc(1, sizeof(PNIO_CP_ID_TYPE) * SERV_CP_MAX_NR_OF_CP);
	ret = SERV_CP_get_network_adapters(cplist, &nrofcp);
	if((ret != PNIO_OK) || (nrofcp != 0))
	{
		PyObject *adapters = PyList_New(nrofcp);

		for(PNIO_UINT8 i = 0; i < nrofcp; i++)
		{
			char mac_or_pci[32];

			if(PNIO_CP_SELECT_WITH_MAC_ADDRESS == cplist[i].CpSelection)
			{

				sprintf(mac_or_pci, "%02X:%02X:%02X:%02X:%02X:%02X", cplist[i].CpMacAddr[0],
															         cplist[i].CpMacAddr[1],
															         cplist[i].CpMacAddr[2],
															         cplist[i].CpMacAddr[3],
															         cplist[i].CpMacAddr[4],
															         cplist[i].CpMacAddr[5]);
			}
			else
			{
				sprintf(mac_or_pci, "%02X.%02X.%X", cplist[i].CpPciLocation.BusNr,
					                                cplist[i].CpPciLocation.DeviceNr,
					                                cplist[i].CpPciLocation.FunctionNr);
			}
			PyList_SetItem(adapters, i, Py_BuildValue("s#s#", mac_or_pci,
				                                              strlen(mac_or_pci),
				                                              (char *)cplist[i].Description,
				                                              strlen((char *)cplist[i].Description)));
		}
		free(cplist);
		return adapters;
	}
	free(cplist);
	PyErr_SetString(PyExc_RuntimeError, "No adapters found!");
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* serv_cp_startup(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	char *xmlfilename;
	char *rolename;
	PNIO_ROLE_TYPE role;
	FILE* xmlfile;
	PNIO_UINT8* xmlbuffer;
	PNIO_UINT32 xmlbuffersize;
	PNIO_SYSTEM_DESCR sd;
	PNIO_CP_ID_PTR_TYPE cplist;
	static PNIO_CP_ID_TYPE cp;
	PNIO_UINT8 nrofcp;
	char *macpci;
	char *desc;
	PNIO_UINT8 i;

	if(!PyArg_ParseTuple(args, "(ss)ss", &macpci, &desc, &xmlfilename, &rolename))
	{
		return NULL;
	}
	if (strcmp("CONTROLLER", rolename) && strcmp("DEVICE", rolename))
	{
		PyErr_SetString(PyExc_RuntimeError, "Unknown role, should be \'CONTROLLER\' or \'DEVICE\'!");
		return NULL;
	}
	if (strcmp("CONTROLLER", rolename) == 0)
	{
		role = PNIO_CONTROLLER_ROLE;
	}
	else
	{
		role = PNIO_DEVICE_ROLE;
	}
	cplist = (PNIO_CP_ID_PTR_TYPE)calloc(1, sizeof(PNIO_CP_ID_TYPE) * SERV_CP_MAX_NR_OF_CP);
	ret = SERV_CP_get_network_adapters(cplist, &nrofcp);
	for(i = 0; i < nrofcp; ++i)
	{
		char mac_or_pci[32];

		if (PNIO_CP_SELECT_WITH_MAC_ADDRESS == cplist[i].CpSelection)
		{
			sprintf(mac_or_pci, "%02X:%02X:%02X:%02X:%02X:%02X", cplist[i].CpMacAddr[0],
				cplist[i].CpMacAddr[1],
				cplist[i].CpMacAddr[2],
				cplist[i].CpMacAddr[3],
				cplist[i].CpMacAddr[4],
				cplist[i].CpMacAddr[5]);
		}
		else
		{
			sprintf(mac_or_pci, "%02X.%02X.%X", cplist[i].CpPciLocation.BusNr, cplist[i].CpPciLocation.DeviceNr, cplist[i].CpPciLocation.FunctionNr);
		}
		if(!strcmp(macpci, mac_or_pci))
		{
			memcpy((void *)&cp, &cplist[i], sizeof(PNIO_CP_ID_TYPE));
			break;
		}
	}
	if(i == nrofcp)
	{
		free(cplist);
		PyErr_SetString(PyExc_RuntimeError, "CP not found!");
		return NULL;
	}
	xmlfile = fopen(xmlfilename, "rb");
	if(xmlfile == NULL)
	{
		free(cplist);
		PyErr_Format(PyExc_RuntimeError, "XML file \"%s\" could not be opened!", xmlfilename);
		return NULL;
	}
	fseek(xmlfile, 0, SEEK_END);
	xmlbuffersize = ftell(xmlfile);
	fseek(xmlfile, 0, SEEK_SET);
	xmlbuffer = (PNIO_UINT8*)malloc(xmlbuffersize+1);
	fread(xmlbuffer, 1, xmlbuffersize, (FILE*)xmlfile);
	fclose(xmlfile);
	xmlbuffer[xmlbuffersize] = 0;
	// TODO: For now the below values are hardcoded, should be in central configuration header or maybe user can provide it from python
	strncpy((char*)(sd.Vendor), "Siemens AG", sizeof(sd.Vendor));
	strncpy((char*)(sd.ProductFamily), "PN DRIVER", sizeof(sd.ProductFamily));
	strncpy((char*)(sd.IM_DeviceType), "PN_DRIVER", sizeof(sd.IM_DeviceType));
	strncpy((char*)(sd.IM_OrderId), "6ES7195-3AA00-0YA1", sizeof(sd.IM_OrderId));
	sd.IM_HwRevision = 0;
	sd.IM_SwVersion.revision_prefix = 'V';
	sd.IM_SwVersion.functional_enhancement = 2;
	sd.IM_SwVersion.bug_fix = 3;
	sd.IM_SwVersion.internal_change = 0;
	strncpy((char*)(sd.ProductSerialNr), "S0000000", sizeof(sd.ProductSerialNr));
	Py_BEGIN_ALLOW_THREADS
	ret = SERV_CP_startup(&cp, 1, xmlbuffer, xmlbuffersize, NULL, 0, &sd, role);
	Py_END_ALLOW_THREADS
	if (PNIO_OK == ret)
	{
		free(cplist);
		free(xmlbuffer);
		Py_RETURN_NONE;
	}
	free(cplist);
	free(xmlbuffer);
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* serv_cp_shutdown(PyObject* self)
{
	PNIO_UINT32 ret;

	Py_BEGIN_ALLOW_THREADS
	ret = SERV_CP_shutdown();
	Py_END_ALLOW_THREADS
	if(ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_controller_open(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PyObject* _rcbf;
	PyObject* _wcbf;
	PyObject* _acbf;

	if(!PyArg_ParseTuple(args, "OOO", &_rcbf, &_wcbf, &_acbf))
	{
		return NULL;
	}
	if(!PyCallable_Check(_rcbf) || !PyCallable_Check(_wcbf) || !PyCallable_Check(_acbf)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		return NULL;
	}
	Py_XINCREF(_rcbf);
	Py_XINCREF(_wcbf);
	Py_XINCREF(_acbf);
	Py_XDECREF(callbackfunc[PNIO_CBE_REC_READ_CONF]);
	Py_XDECREF(callbackfunc[PNIO_CBE_REC_WRITE_CONF]);
	Py_XDECREF(callbackfunc[PNIO_CBE_ALARM_IND]);
	callbackfunc[PNIO_CBE_REC_READ_CONF] = _rcbf;
	callbackfunc[PNIO_CBE_REC_WRITE_CONF] = _wcbf;
	callbackfunc[PNIO_CBE_ALARM_IND] = _acbf;
	ret = PNIO_controller_open(1, PNIO_CEP_MODE_CTRL, cbf, cbf, cbf, &apphandle);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_controller_close(PyObject* self)
{
	PNIO_UINT32 ret;

	ret = PNIO_controller_close(apphandle);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_device_open(PyObject* self)
{
	PNIO_UINT32 ret;

	ret = PNIO_device_open(1, &apphandle);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_device_close(PyObject* self)
{
	PNIO_UINT32 ret;

	ret = PNIO_device_close(apphandle);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_set_mode(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	char *bufmode;
	PNIO_MODE_TYPE mode;

	if(!PyArg_ParseTuple(args, "s", &bufmode))
	{
		return NULL;
	}
	if(strcmp(bufmode, "OFFLINE") &&
	   strcmp(bufmode, "CLEAR") &&
	   strcmp(bufmode, "OPERATE"))
	{
		char err[255];
		sprintf(err, "Unknown mode \'%s\', should be \'OFFLINE\', \'CLEAR\' or \'OPERATE\'!", bufmode);
		PyErr_SetString(PyExc_RuntimeError, err);
		return NULL;
	}
	else
	{
		if(strcmp(bufmode, "OFFLINE") == 0)
		{
			mode = PNIO_MODE_OFFLINE;
		}
		else if(strcmp(bufmode, "CLEAR") == 0)
		{
			mode = PNIO_MODE_CLEAR;
		}
		else
		{
			mode = PNIO_MODE_OPERATE;
		}
	}
	ret = PNIO_set_mode(apphandle, mode);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_device_activate(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_UINT32 api;
	PNIO_UINT16 slot;
	PNIO_UINT16 subslot;
	PNIO_UINT32 mode;
	PNIO_ADDR   addr = {};

	if(!PyArg_ParseTuple(args, "IHHI", &api, &slot, &subslot, &mode))
	{
		return NULL;
	}
	addr.AddrType = PNIO_ADDR_GEO;
	addr.u.Geo.API = api;
	addr.u.Geo.Slot = slot;
	addr.u.Geo.Subslot = subslot;
	ret = PNIO_device_activate(apphandle, &addr, (PNIO_DEV_ACT_TYPE)mode);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_interface_open(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PyObject* _ircbf;
	PyObject* _iwcbf;
	PyObject* _iacbf;

	if(!PyArg_ParseTuple(args, "OOO", &_ircbf, &_iwcbf, &_iacbf))
	{
		return NULL;
	}
	if(!PyCallable_Check(_ircbf) || !PyCallable_Check(_iwcbf) || !PyCallable_Check(_iacbf)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		return NULL;
	}
	Py_XINCREF(_ircbf);
	Py_XINCREF(_iwcbf);
	Py_XINCREF(_iacbf);
	Py_XDECREF(callbackfunc[PNIO_CBE_IFC_REC_READ_CONF]);
	Py_XDECREF(callbackfunc[PNIO_CBE_IFC_REC_WRITE_CONF]);
	Py_XDECREF(callbackfunc[PNIO_CBE_IFC_ALARM_IND]);
	callbackfunc[PNIO_CBE_IFC_REC_READ_CONF] = _ircbf;
	callbackfunc[PNIO_CBE_IFC_REC_WRITE_CONF] = _iwcbf;
	callbackfunc[PNIO_CBE_IFC_ALARM_IND] = _iacbf;
	ret = PNIO_interface_open(1, cbf, cbf, cbf, &ifchandle);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_interface_close(PyObject* self)
{
	PNIO_UINT32 ret;

	ret = PNIO_interface_close(ifchandle);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_register_cbf(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PyObject* _cbf;
	PNIO_UINT32 cbetype;

	if(!PyArg_ParseTuple(args, "iO", &cbetype, &_cbf))
	{
		return NULL;
	}
	if(!PyCallable_Check(_cbf)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		return NULL;
	}
	Py_XINCREF(_cbf);
	Py_XDECREF(callbackfunc[cbetype]);
	callbackfunc[cbetype] = _cbf;
	ret = PNIO_register_cbf(apphandle, (PNIO_CBE_TYPE)cbetype, &cbf);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_interface_register_cbf(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PyObject* _cbf;
	PNIO_UINT32 cbetype;

	if(!PyArg_ParseTuple(args, "iO", &cbetype, &_cbf))
	{
		return NULL;
	}
	if(!PyCallable_Check(_cbf)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be callable");
		return NULL;
	}
	Py_XINCREF(_cbf);
	Py_XDECREF(callbackfunc[cbetype]);
	callbackfunc[cbetype] = _cbf;
	ret = PNIO_interface_register_cbf(ifchandle, (PNIO_CBE_TYPE)cbetype, &cbf);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_interface_set_ip_and_nos(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_UINT8 mode;
	PNIO_IPv4 ip = {};
	PNIO_NOS nos = {};
	PyObject *nparam = Py_None;
	PyObject *iparam = Py_None;
	char *nameparam;
	char *ipparam;
	char *netmaskparam;
	char *gatewayparam;
	int rema;

	if(!PyArg_ParseTuple(args, "iOO", &mode, &nparam, &iparam))
	{
		return NULL;
	}
	if(mode < 0 || mode > 3)
	{
		PyErr_SetString(PyExc_RuntimeError, "Unknown mode!");
		return NULL;
	}
	if(mode & PNIO_SET_NOS_MODE)
	{
		if(!PyArg_ParseTuple(nparam, "si", &nameparam, &rema))
		{
			return NULL;
		}
		strcpy((char *)nos.Nos, nameparam);
		nos.Length = (PNIO_UINT16)strlen(nameparam);
		nos.Remanent = rema;
	}
	if(mode & PNIO_SET_IP_MODE)
	{
		if(!PyArg_ParseTuple(iparam, "sssi", &ipparam, &netmaskparam, &gatewayparam, &rema))
		{
			return NULL;
		}
		sscanf(ipparam, "%hhu.%hhu.%hhu.%hhu", &ip.IpAddress[0], &ip.IpAddress[1], &ip.IpAddress[2], &ip.IpAddress[3]);
		sscanf(netmaskparam, "%hhu.%hhu.%hhu.%hhu", &ip.NetMask[0], &ip.NetMask[1], &ip.NetMask[2], &ip.NetMask[3]);
		sscanf(gatewayparam, "%hhu.%hhu.%hhu.%hhu", &ip.Gateway[0], &ip.Gateway[1], &ip.Gateway[2], &ip.Gateway[3]);
		ip.Remanent = rema;
	}
	ret = PNIO_interface_set_ip_and_nos(ifchandle, (PNIO_SET_IP_NOS_MODE_TYPE)mode, ip, nos);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_rec_read_req(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_ADDR addr;
	PNIO_UINT32 logaddr;
	PNIO_UINT32 recordindex;

	if (!PyArg_ParseTuple(args, "II", &logaddr, &recordindex))
	{
		return NULL;
	}
	addr.AddrType = PNIO_ADDR_LOG;
	addr.IODataType = PNIO_IO_IN;
	addr.u.Addr = logaddr;
	ret = PNIO_rec_read_req(apphandle, &addr, recordindex, 1, 32*1024);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_rec_write_req(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_ADDR addr;
	PyObject *bufferparam;
	PNIO_UINT32 logaddr;
	PNIO_UINT32 recordindex;
	PNIO_UINT32 buffersize;
	PNIO_UINT8 *buffer;

	if (!PyArg_ParseTuple(args, "IIIY", &logaddr, &recordindex, &buffersize, &bufferparam))
	{
		return NULL;
	}
	buffer = (PNIO_UINT8 *)calloc(1, buffersize);

	memcpy(buffer, (PNIO_UINT8 *)PyByteArray_AsString(bufferparam), buffersize);

	addr.AddrType = PNIO_ADDR_LOG;
	addr.IODataType = PNIO_IO_IN;
	addr.u.Addr = logaddr;
	ret = PNIO_rec_write_req(apphandle, &addr, recordindex, 1, buffersize, buffer);
	if (ret == PNIO_OK)
	{
		free(buffer);
		Py_RETURN_NONE;
	}
	free(buffer);
	set_error_string(ret);
	return NULL;
}


/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_interface_rec_read_req(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_ADDR addr;
	PNIO_UINT32 logaddr;
	PNIO_UINT32 recordindex;

	if (!PyArg_ParseTuple(args, "II", &logaddr, &recordindex))
	{
		return NULL;
	}
	addr.AddrType = PNIO_ADDR_LOG;
	addr.IODataType = PNIO_IO_IN;
	addr.u.Addr = logaddr;
	ret = PNIO_interface_rec_read_req(ifchandle, &addr, recordindex, 0, 32*1024);
	if (ret == PNIO_OK)
	{
		Py_RETURN_NONE;
	}
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_interface_rec_write_req(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_ADDR addr;
	PyObject *bufferparam;
	PNIO_UINT32 logaddr;
	PNIO_UINT32 recordindex;
	PNIO_UINT32 buffersize;
	PNIO_UINT8 *buffer;

	if (!PyArg_ParseTuple(args, "IIIO", &logaddr, &recordindex, &buffersize, &bufferparam))
	{
		return NULL;
	}
	buffer = (PNIO_UINT8 *)calloc(1, buffersize);

	memcpy(buffer, (PNIO_UINT8 *)PyByteArray_AsString(bufferparam), buffersize);

	addr.AddrType = PNIO_ADDR_LOG;
	addr.IODataType = PNIO_IO_IN;
	addr.u.Addr = logaddr;
	ret = PNIO_interface_rec_write_req(apphandle, &addr, recordindex, 1, buffersize, buffer);
	if (ret == PNIO_OK)
	{
		free(buffer);
		Py_RETURN_NONE;
	}
	free(buffer);
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_data_read(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_ADDR addr;
	PNIO_UINT32 locstate;
	PNIO_UINT32 remstate;
	PNIO_UINT32 length;
	PNIO_UINT32 readlength = 0;
	PNIO_UINT8 *buffer;

	if (rolestore == 1)
	{
		PNIO_UINT32 logaddr;

		if (!PyArg_ParseTuple(args, "III", &logaddr, &length, &locstate))
		{
			return NULL;
		}
		addr.AddrType = PNIO_ADDR_LOG;
		addr.IODataType = PNIO_IO_IN;
		addr.u.Addr = logaddr;
	}
	else
	{
		PNIO_UINT32 slot;
		PNIO_UINT32 subslot;

		if (!PyArg_ParseTuple(args, "IIII", &slot, &subslot, &length, &locstate))
		{
			return NULL;
		}
		addr.AddrType = PNIO_ADDR_GEO;
		addr.IODataType = PNIO_IO_IN;
		addr.u.Geo.API = 0;
		addr.u.Geo.Slot = slot;
		addr.u.Geo.Subslot = subslot;
	}
	buffer = (PNIO_UINT8 *)calloc(1, length);
	ret = PNIO_data_read(apphandle, &addr, length, &readlength, buffer, (PNIO_IOXS *)&locstate, (PNIO_IOXS *)&remstate);
	if (ret == PNIO_OK)
	{
		free(buffer);
		return Py_BuildValue("(y#)I", buffer, readlength, remstate);
	}
	free(buffer);
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_data_write(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_ADDR addr;
	PyObject *bufferparam;
	PNIO_UINT32 locstate;
	PNIO_UINT32 remstate;
	PNIO_UINT32 length;
	PNIO_UINT8 *buffer;

	if (rolestore == 1)
	{
		PNIO_UINT32 logaddr;

		if (!PyArg_ParseTuple(args, "IIYI", &logaddr, &length, &bufferparam, &locstate))
		{
			return NULL;
		}
		addr.AddrType = PNIO_ADDR_LOG;
		addr.IODataType = PNIO_IO_OUT;
		addr.u.Addr = logaddr;
	}
	else
	{
		PNIO_UINT32 slot;
		PNIO_UINT32 subslot;

		if (!PyArg_ParseTuple(args, "IIIOI", &slot, &subslot, &length, &bufferparam, &locstate))
		{
			return NULL;
		}
		addr.AddrType = PNIO_ADDR_GEO;
		addr.IODataType = PNIO_IO_OUT;
		addr.u.Geo.API = 0;
		addr.u.Geo.Slot = slot;
		addr.u.Geo.Subslot = subslot;
	}
	buffer = (PNIO_UINT8 *)calloc(1, length);

	memcpy(buffer, (PNIO_UINT8 *)PyByteArray_AsString(bufferparam), length);
	
	ret = PNIO_data_write(apphandle, &addr, length, buffer, (PNIO_IOXS *)&locstate, (PNIO_IOXS *)&remstate);
	if (ret == PNIO_OK)
	{
		free(buffer);
		return Py_BuildValue("I", remstate);
	}
	free(buffer);
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_iosystem_reconfig(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	PNIO_UINT32 mode;
	PyObject *deviceparam;
	PyObject *portparam;
	PNIO_UINT32 devicecount;
	PNIO_UINT32 portcount;
	PNIO_ADDR *devicelist;
	PNIO_ADDR *portlist;

	if (!PyArg_ParseTuple(args, "IIOIO", &mode, &devicecount, &deviceparam, &portcount, &portparam))
	{
		return NULL;
	}
	if (mode == 0 || mode > 2)
	{
		PyErr_SetString(PyExc_TypeError, "wrong mode provided");
		return NULL;
	}
	devicelist = (PNIO_ADDR *)calloc(1, devicecount * sizeof(PNIO_ADDR));
	portlist = (PNIO_ADDR *)calloc(1, portcount * sizeof(PNIO_ADDR));
	for (PNIO_UINT32 i = 0; i < devicecount; i++)
	{
		PNIO_UINT32 addr;

		if (i == devicecount - 1)
		{
			if (!PyArg_ParseTuple(deviceparam, "I", &addr))
			{
				free(devicelist);
				free(portlist);
				return NULL;
			}
		}
		else
		{
			if (!PyArg_ParseTuple(deviceparam, "IO", &addr, &deviceparam))
			{
				free(devicelist);
				free(portlist);
				return NULL;
			}
		}
		devicelist[i].AddrType = PNIO_ADDR_LOG;
		devicelist[i].IODataType = PNIO_IO_IN;
		devicelist[i].u.Addr = addr;
	}
	for (PNIO_UINT32 i = 0; i < portcount; i++)
	{
		PNIO_UINT32 addr;

		if (i == portcount - 1)
		{
			if (!PyArg_ParseTuple(portparam, "I", &addr))
			{
				free(devicelist);
				free(portlist);
				return NULL;
			}
		}
		else
		{
			if (!PyArg_ParseTuple(portparam, "IO", &addr, &portparam))
			{
				free(devicelist);
				free(portlist);
				return NULL;
			}
		}
		devicelist[i].AddrType = PNIO_ADDR_LOG;
		devicelist[i].IODataType = PNIO_IO_IN;
		devicelist[i].u.Addr = addr;
	}
	ret = PNIO_iosystem_reconfig(apphandle,(PNIO_IOS_RECONFIG_MODE) mode, devicecount, devicelist, portcount, portlist);
	if (ret == PNIO_OK)
	{
		free(devicelist);
		free(portlist);
		Py_RETURN_NONE;
	}
	free(devicelist);
	free(portlist);
	set_error_string(ret);
	return NULL;


}

/*--------------------------------------------------------------------*/

extern "C"
PyObject* pnio_ctrl_diag_req(PyObject* self, PyObject* args)
{
	PNIO_UINT32 ret;
	int mode;

	if (!PyArg_ParseTuple(args, "i", &mode))
	{
		return NULL;
	}
	PNIO_CTRL_DIAG * pDiagReq = (PNIO_CTRL_DIAG *)(calloc(1, sizeof(PNIO_CTRL_DIAG)));
	pDiagReq->ReqRef = 0;
	switch (mode)
	{
		case PNIO_CTRL_DIAG_CONFIG_SUBMODULE_LIST:
			pDiagReq->DiagService = PNIO_CTRL_DIAG_CONFIG_SUBMODULE_LIST;
			break;

		case PNIO_CTRL_DIAG_CONFIG_NAME_ADDR_INFO:
			pDiagReq->DiagService = PNIO_CTRL_DIAG_CONFIG_NAME_ADDR_INFO;
			break;

		case PNIO_CTRL_DIAG_DEVICE_DIAGNOSTIC:
			pDiagReq->DiagService = PNIO_CTRL_DIAG_DEVICE_DIAGNOSTIC;
			break;

		default:
			free(pDiagReq);
			PyErr_SetString(PyExc_RuntimeError, "Unknown mode!");
			return NULL;
	}
	ret = PNIO_ctrl_diag_req(apphandle, pDiagReq);
	if (ret == PNIO_OK)
	{
		free(pDiagReq);
		Py_RETURN_NONE;
	}
	free(pDiagReq);
	set_error_string(ret);
	return NULL;
}

/*--------------------------------------------------------------------*/

static PyMethodDef pndMethods[] = {
	/* SERV_CP */
	{"serv_cp_init", (PyCFunction)serv_cp_init, METH_NOARGS, "Initializes CP services"},
	{"serv_cp_undo_init", (PyCFunction)serv_cp_undo_init, METH_NOARGS, "Undoes CP services initialization"},
	{"serv_cp_get_network_adapters", (PyCFunction)serv_cp_get_network_adapters, METH_NOARGS, "Returns a list of all detected network adapters"},
	{"serv_cp_startup",	(PyCFunction)serv_cp_startup, METH_VARARGS,	"Starts all internal tasks and configures the PNIO stack according to the configuration file provided"},
	{"serv_cp_shutdown", (PyCFunction)serv_cp_shutdown, METH_NOARGS, "Shuts down CP services, all internal threads are exited and the entire local memory is released"},

	/* PNIO */
	{"pnio_controller_open", (PyCFunction)pnio_controller_open, METH_VARARGS, "Allocate resources and open controller"},
	{"pnio_controller_close", (PyCFunction)pnio_controller_close, METH_NOARGS, "Deallocate resources and close controller"},
	{"pnio_device_open", (PyCFunction)pnio_device_open,	METH_NOARGS, "Initialize and open device"},
	{"pnio_device_close", (PyCFunction)pnio_device_close, METH_NOARGS, "Deallocate resources and close device"},
	{"pnio_set_mode", (PyCFunction)pnio_set_mode, METH_VARARGS, "Change operation mode"},
	{"pnio_device_activate", (PyCFunction)pnio_device_activate,	METH_VARARGS, "Activate device"},
	{"pnio_interface_open", (PyCFunction)pnio_interface_open, METH_VARARGS, "Open interface channel"},
	{"pnio_interface_close", (PyCFunction)pnio_interface_close,	METH_VARARGS, "Close interface channel"},
	{"pnio_register_cbf", (PyCFunction)pnio_register_cbf, METH_VARARGS, "Register a callback for a specific event"},
	{"pnio_rec_read_req", (PyCFunction)pnio_rec_read_req, METH_VARARGS, "Record read function, response is through callback"},
	{"pnio_rec_write_req", (PyCFunction)pnio_rec_write_req, METH_VARARGS, "Record write function, response is through callback"},
	{"pnio_interface_register_cbf", (PyCFunction)pnio_interface_register_cbf, METH_VARARGS, "Register a callback on interface for a specific event"},
	{"pnio_interface_set_ip_and_nos", (PyCFunction)pnio_interface_set_ip_and_nos, METH_VARARGS, "Set NoS (name of station) and/or IP suite"},
	{"pnio_interface_rec_read_req", (PyCFunction)pnio_interface_rec_read_req, METH_VARARGS, "Record read function for interface, response is through callback"},
	{"pnio_interface_rec_write_req", (PyCFunction)pnio_interface_rec_write_req,	METH_VARARGS, "Record write function for interface, response is through callback"},
	{"pnio_ctrl_diag_req", (PyCFunction)pnio_ctrl_diag_req, METH_VARARGS, "Request diagnostic information"},
	{"pnio_data_read", (PyCFunction)pnio_data_read, METH_VARARGS, "Read IO data function"},
	{"pnio_data_write", (PyCFunction)pnio_data_write, METH_VARARGS, "Write IO data function"},
	{"pnio_iosystem_reconfig", (PyCFunction)pnio_iosystem_reconfig, METH_VARARGS, "Reconfigure IO System"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef pnd = {
	PyModuleDef_HEAD_INIT,
	"pnd",
	R""""(
	NAME
		pnd

	DESCRIPTION
		PN Driver as a Python Module
		============================

		This is a Python extension module which is wrapping PN Driver IOBase User API and providing access to it from Python.
		All callbacks are implemented and arguments are parsed and structured data is provided to Python code. In this way
		arguments are more python friendly and Python code needs not much to work with bits & bytes.

		As all the real-time actions are inside PN Driver library, performance is not much impacted.
		Only concern is in callbacks. User should return from callbacks as soon as possible.

	)"""",
	-1,
	pndMethods
};

/*--------------------------------------------------------------------*/

PyMODINIT_FUNC PyInit_pnd(void)
{
	if(!Py_IsInitialized())
	{
		Py_Initialize();
	}
	return PyModule_Create(&pnd);
}
