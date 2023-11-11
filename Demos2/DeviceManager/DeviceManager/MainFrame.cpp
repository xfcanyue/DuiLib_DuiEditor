#include "StdAfx.h"
#include "MainFrame.h"

#include "DEVPKEY.h"

CM_GET_DEVNODE_PROPERTY FunCM_Get_DevNode_Property = NULL;
CM_GET_CLASS_PROPERTY FunCM_Get_Class_Property = NULL;

static tagConfigRet g_configRet[] = {
	{	_T("CR_SUCCESS"),                  (0x00000000)	},
	{	_T("CR_DEFAULT"),                  (0x00000001)	},
	{	_T("CR_OUT_OF_MEMORY"),            (0x00000002)	},
	{	_T("CR_INVALID_POINTER"),          (0x00000003)	},
	{	_T("CR_INVALID_FLAG"),             (0x00000004)	},
	{	_T("CR_INVALID_DEVNODE"),          (0x00000005)	},
	{	_T("CR_INVALID_DEVINST"),          CR_INVALID_DEVNODE	},
	{	_T("CR_INVALID_RES_DES"),          (0x00000006)	},
	{	_T("CR_INVALID_LOG_CONF"),         (0x00000007)	},
	{	_T("CR_INVALID_ARBITRATOR"),       (0x00000008)	},
	{	_T("CR_INVALID_NODELIST"),         (0x00000009)	},
	{	_T("CR_DEVNODE_HAS_REQS"),         (0x0000000A)	},
	{	_T("CR_DEVINST_HAS_REQS"),         CR_DEVNODE_HAS_REQS	},
	{	_T("CR_INVALID_RESOURCEID"),       (0x0000000B)	},
	{	_T("CR_DLVXD_NOT_FOUND"),          (0x0000000C)	},   // WIN 95 ONLY
	{	_T("CR_NO_SUCH_DEVNODE"),          (0x0000000D)	},
	{	_T("CR_NO_SUCH_DEVINST"),          CR_NO_SUCH_DEVNODE	},
	{	_T("CR_NO_MORE_LOG_CONF"),         (0x0000000E)	},
	{	_T("CR_NO_MORE_RES_DES"),          (0x0000000F)	},
	{	_T("CR_ALREADY_SUCH_DEVNODE"),     (0x00000010)	},
	{	_T("CR_ALREADY_SUCH_DEVINST"),     CR_ALREADY_SUCH_DEVNODE	},
	{	_T("CR_INVALID_RANGE_LIST"),       (0x00000011)	},
	{	_T("CR_INVALID_RANGE"),            (0x00000012)	},
	{	_T("CR_FAILURE"),                  (0x00000013)	},
	{	_T("CR_NO_SUCH_LOGICAL_DEV"),      (0x00000014)	},
	{	_T("CR_CREATE_BLOCKED"),           (0x00000015)	},
	{	_T("CR_NOT_SYSTEM_VM"),            (0x00000016)	},   // WIN 95 ONLY
	{	_T("CR_REMOVE_VETOED"),            (0x00000017)	},
	{	_T("CR_APM_VETOED"),               (0x00000018)	},
	{	_T("CR_INVALID_LOAD_TYPE"),        (0x00000019)	},
	{	_T("CR_BUFFER_SMALL"),             (0x0000001A)	},
	{	_T("CR_NO_ARBITRATOR"),            (0x0000001B)	},
	{	_T("CR_NO_REGISTRY_HANDLE"),       (0x0000001C)	},
	{	_T("CR_REGISTRY_ERROR"),           (0x0000001D)	},
	{	_T("CR_INVALID_DEVICE_ID"),        (0x0000001E)	},
	{	_T("CR_INVALID_DATA"),             (0x0000001F)	},
	{	_T("CR_INVALID_API"),              (0x00000020)	},
	{	_T("CR_DEVLOADER_NOT_READY"),      (0x00000021)	},
	{	_T("CR_NEED_RESTART"),             (0x00000022)	},
	{	_T("CR_NO_MORE_HW_PROFILES"),      (0x00000023)	},
	{	_T("CR_DEVICE_NOT_THERE"),         (0x00000024)	},
	{	_T("CR_NO_SUCH_VALUE"),            (0x00000025)	},
	{	_T("CR_WRONG_TYPE"),               (0x00000026)	},
	{	_T("CR_INVALID_PRIORITY"),         (0x00000027)	},
	{	_T("CR_NOT_DISABLEABLE"),          (0x00000028)	},
	{	_T("CR_FREE_RESOURCES"),           (0x00000029)	},
	{	_T("CR_QUERY_VETOED"),             (0x0000002A)	},
	{	_T("CR_CANT_SHARE_IRQ"),           (0x0000002B)	},
	{	_T("CR_NO_DEPENDENT"),             (0x0000002C)	},
	{	_T("CR_SAME_RESOURCES"),           (0x0000002D)	},
	{	_T("CR_NO_SUCH_REGISTRY_KEY"),     (0x0000002E)	},
	{	_T("CR_INVALID_MACHINENAME"),      (0x0000002F)	},   // NT ONLY
	{	_T("CR_REMOTE_COMM_FAILURE"),      (0x00000030)	},   // NT ONLY
	{	_T("CR_MACHINE_UNAVAILABLE"),      (0x00000031)	},   // NT ONLY
	{	_T("CR_NO_CM_SERVICES"),           (0x00000032)	},   // NT ONLY
	{	_T("CR_ACCESS_DENIED"),            (0x00000033)	},   // NT ONLY
	{	_T("CR_CALL_NOT_IMPLEMENTED"),     (0x00000034)	},
	{	_T("CR_INVALID_PROPERTY"),         (0x00000035)	},
	{	_T("CR_DEVICE_INTERFACE_ACTIVE"),  (0x00000036)	},
	{	_T("CR_NO_SUCH_DEVICE_INTERFACE"), (0x00000037)	},
	{	_T("CR_INVALID_REFERENCE_STRING"), (0x00000038)	},
	{	_T("CR_INVALID_CONFLICT_LIST"),    (0x00000039)	},
	{	_T("CR_INVALID_INDEX"),            (0x0000003A)	},
	{	_T("CR_INVALID_STRUCTURE_SIZE"),   (0x0000003B)	},
	{	_T("NUM_CR_RESULTS"),              (0x0000003C)	}
};

CDuiString GetConfigErrorText(CONFIGRET cfgRet)
{
	int size = sizeof(g_configRet) / sizeof(tagConfigRet);
	for (int i=0; i<size; i++)
	{
		if(g_configRet[i].value == cfgRet)
			return g_configRet[i].name;
	}
	return _T("");
}

#define Add_DevPropKey(devPropKey, sFriendlyName) Add_DevPropKey_List(devPropKey, L#devPropKey, sFriendlyName)

CMainFrame::CMainFrame(void)
{
	//////////////////////////////////////////////////////////////////////////
	//Add_DevPropKey(DEVPKEY_NAME, _T("")); //	0xb725f130, 0x47ef, 0x101a, 0xa5, 0xf1, 0x02, 0x60, 0x8c, 0x9e, 0xeb, 0xac, 10);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DeviceDesc, _T("设备描述"));		//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 2);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_HardwareIds, _T("硬件 ID"));		//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 3);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_CompatibleIds, _T("兼容 ID"));	//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 4);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_Service, _T("服务"));			//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 6);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_Class, _T("类"));			//,                  0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 9);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_ClassGuid, _T("类 Guid"));		//,              0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 10);    // DEVPROP_TYPE_GUID
	Add_DevPropKey(DEVPKEY_Device_Driver, _T("驱动程序关键字"));			//,                 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 11);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_ConfigFlags, _T("配置标识"));		//,            0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 12);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_Manufacturer, _T("提供商"));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 13);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_FriendlyName, _T("友好名称"));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 14);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_LocationInfo, _T("位置信息"));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 15);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_PDOName, _T("PDO Name"));			//,                0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 16);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_Capabilities, _T("兼容(功能)"));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 17);    // DEVPROP_TYPE_UNINT32
	Add_DevPropKey(DEVPKEY_Device_UINumber, _T(""));		//,               0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 18);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_UpperFilters, _T(""));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 19);    // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_LowerFilters, _T(""));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 20);    // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_BusTypeGuid, _T("总线类型 Guid"));		//,            0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 21);    // DEVPROP_TYPE_GUID
	Add_DevPropKey(DEVPKEY_Device_LegacyBusType, _T("过时总线类型"));	//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 22);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_BusNumber, _T("总线数"));		//,              0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 23);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_EnumeratorName, _T("枚举"));	//,         0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 24);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_Security, _T(""));		//,               0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 25);    // DEVPROP_TYPE_SECURITY_DESCRIPTOR
	Add_DevPropKey(DEVPKEY_Device_SecuritySDS, _T(""));		//,            0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 26);    // DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING
	Add_DevPropKey(DEVPKEY_Device_DevType, _T(""));			//,                0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 27);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_Exclusive, _T(""));		//,              0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 28);    // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_Device_Characteristics, _T(""));	//,        0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 29);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_Address, _T("地址"));			//,                0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 30);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_UINumberDescFormat, _T(""));//,     0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 31);    // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_PowerData, _T("电源数据"));		//,              0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 32);    // DEVPROP_TYPE_BINARY
	Add_DevPropKey(DEVPKEY_Device_RemovalPolicy, _T("删除策略"));	//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 33);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_RemovalPolicyDefault, _T("删除策略默认值"));//,   0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 34);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_RemovalPolicyOverride, _T(""));//,  0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 35);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_InstallState, _T("安装状态"));	//,           0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 36);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_LocationPaths, _T("位置路径"));	//,          0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 37);    // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_BaseContainerId, _T("基本容器 ID"));	//,        0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 38);    // DEVPROP_TYPE_GUID

	//
	// Device properties
	// These DEVPKEYs correspond to a device's status and problem code.
	//
	Add_DevPropKey(DEVPKEY_Device_DevNodeStatus, _T("状态"));	//,          0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 2);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_ProblemCode, _T("问题代码"));	//,            0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 3);     // DEVPROP_TYPE_UINT32

	//
	// Device properties
	// These DEVPKEYs correspond to a device's relations.
	//
	Add_DevPropKey(DEVPKEY_Device_EjectionRelations, _T(""));	//,      0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 4);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_RemovalRelations, _T(""));	//,       0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 5);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_PowerRelations, _T(""));	//,         0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 6);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_BusRelations, _T("总线关系"));	//,           0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 7);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_Parent, _T("父系"));	//,                 0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 8);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_Children, _T("子代"));	//,               0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 9);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_Siblings, _T("同级"));	//,               0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 10);    // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_TransportRelations, _T(""));	//,     0x4340a6c5, 0x93fa, 0x4706, 0x97, 0x2c, 0x7b, 0x64, 0x80, 0x08, 0xa5, 0xa7, 11);    // DEVPROP_TYPE_STRING_LIST

	//
	// Other Device properties
	// These DEVPKEYs are set for the corresponding types of root-enumerated devices.     ;comment
	//
	Add_DevPropKey(DEVPKEY_Device_Reported, _T(""));	//,               0x80497100, 0x8c73, 0x48b9, 0xaa, 0xd9, 0xce, 0x38, 0x7e, 0x19, 0xc5, 0x6e, 2);     // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_Device_Legacy, _T(""));	//,                 0x80497100, 0x8c73, 0x48b9, 0xaa, 0xd9, 0xce, 0x38, 0x7e, 0x19, 0xc5, 0x6e, 3);     // DEVPROP_TYPE_BOOLEAN

	//
	// Device Instance Id
	//
	Add_DevPropKey(DEVPKEY_Device_InstanceId, _T("设备实例路径"));	//,             0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 256);   // DEVPROP_TYPE_STRING

	//
	// Device Container Id
	//
	Add_DevPropKey(DEVPKEY_Device_ContainerId, _T("容器 ID"));	//,            0x8c7ed206, 0x3f8a, 0x4827, 0xb3, 0xab, 0xae, 0x9e, 0x1f, 0xae, 0xfc, 0x6c, 2);     // DEVPROP_TYPE_GUID

	//
	// Device Experience related Keys
	//
	Add_DevPropKey(DEVPKEY_Device_ModelId, _T(""));	//,                0x80d81ea6, 0x7473, 0x4b0c, 0x82, 0x16, 0xef, 0xc1, 0x1a, 0x2c, 0x4c, 0x8b, 2);     // DEVPROP_TYPE_GUID
	Add_DevPropKey(DEVPKEY_Device_FriendlyNameAttributes, _T(""));	//, 0x80d81ea6, 0x7473, 0x4b0c, 0x82, 0x16, 0xef, 0xc1, 0x1a, 0x2c, 0x4c, 0x8b, 3);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_ManufacturerAttributes, _T(""));	//, 0x80d81ea6, 0x7473, 0x4b0c, 0x82, 0x16, 0xef, 0xc1, 0x1a, 0x2c, 0x4c, 0x8b, 4);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_PresenceNotForDevice, _T(""));	//,   0x80d81ea6, 0x7473, 0x4b0c, 0x82, 0x16, 0xef, 0xc1, 0x1a, 0x2c, 0x4c, 0x8b, 5);     // DEVPROP_TYPE_BOOLEAN

	//
	// Other Device properties
	//
	Add_DevPropKey(DEVPKEY_Numa_Proximity_Domain, _T(""));	//,         0x540b947e, 0x8b40, 0x45bc, 0xa8, 0xa2, 0x6a, 0x0b, 0x89, 0x4c, 0xbd, 0xa2, 1);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_DHP_Rebalance_Policy, _T(""));	//,   0x540b947e, 0x8b40, 0x45bc, 0xa8, 0xa2, 0x6a, 0x0b, 0x89, 0x4c, 0xbd, 0xa2, 2);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_Numa_Node, _T(""));	//,              0x540b947e, 0x8b40, 0x45bc, 0xa8, 0xa2, 0x6a, 0x0b, 0x89, 0x4c, 0xbd, 0xa2, 3);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_BusReportedDeviceDesc, _T("总线已报告设备描述"));	//,  0x540b947e, 0x8b40, 0x45bc, 0xa8, 0xa2, 0x6a, 0x0b, 0x89, 0x4c, 0xbd, 0xa2, 4);     // DEVPROP_TYPE_STRING


	//
	// Device Session Id
	//
	Add_DevPropKey(DEVPKEY_Device_SessionId, _T(""));	//,              0x83da6326, 0x97a6, 0x4088, 0x94, 0x53, 0xa1, 0x92, 0x3f, 0x57, 0x3b, 0x29, 6);     // DEVPROP_TYPE_UINT32

	//
	// Device activity timestamp properties
	//
	Add_DevPropKey(DEVPKEY_Device_InstallDate, _T("安装日期"));	//,            0x83da6326, 0x97a6, 0x4088, 0x94, 0x53, 0xa1, 0x92, 0x3f, 0x57, 0x3b, 0x29, 100);   // DEVPROP_TYPE_FILETIME
	Add_DevPropKey(DEVPKEY_Device_FirstInstallDate, _T("首次安装日期"));	//,       0x83da6326, 0x97a6, 0x4088, 0x94, 0x53, 0xa1, 0x92, 0x3f, 0x57, 0x3b, 0x29, 101);   // DEVPROP_TYPE_FILETIME

	//
	// Device driver properties
	//
	Add_DevPropKey(DEVPKEY_Device_DriverDate, _T("驱动程序日期"));	//,             0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 2);      // DEVPROP_TYPE_FILETIME
	Add_DevPropKey(DEVPKEY_Device_DriverVersion, _T("驱动程序版本"));	//,          0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 3);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverDesc, _T("驱动程序说明"));	//,             0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 4);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverInfPath, _T("INF 名称"));	//,          0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 5);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverInfSection, _T("INF 部分"));	//,       0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 6);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverInfSectionExt, _T(""));	//,    0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 7);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_MatchingDeviceId, _T("匹配设备 ID"));	//,       0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 8);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverProvider, _T(""));	//,         0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 9);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverPropPageProvider, _T(""));	//, 0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 10);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverCoInstallers, _T(""));	//,     0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 11);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_Device_ResourcePickerTags, _T(""));	//,     0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 12);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_ResourcePickerExceptions, _T(""));	//, 0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 13);   // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_Device_DriverRank, _T("驱动程序等级"));	//,             0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 14);     // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_Device_DriverLogoLevel, _T(""));	//,        0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 15);     // DEVPROP_TYPE_UINT32

	//
	// Device properties
	// These DEVPKEYs may be set by the driver package installed for a device.
	//
	Add_DevPropKey(DEVPKEY_Device_NoConnectSound, _T(""));	//,         0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 17);     // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_Device_GenericDriverInstalled, _T(""));	//, 0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 18);     // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_Device_AdditionalSoftwareRequested, _T(""));	//, 0xa8b865dd, 0x2e3d, 0x4094, 0xad, 0x97, 0xe5, 0x93, 0xa7, 0xc, 0x75, 0xd6, 19); //DEVPROP_TYPE_BOOLEAN

	//
	// Device safe-removal properties
	//
	Add_DevPropKey(DEVPKEY_Device_SafeRemovalRequired, _T("需要安全删除"));	//,    0xafd97640,  0x86a3, 0x4210, 0xb6, 0x7c, 0x28, 0x9c, 0x41, 0xaa, 0xbe, 0x55, 2);    // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_Device_SafeRemovalRequiredOverride, _T(""));	//, 0xafd97640,  0x86a3, 0x4210, 0xb6, 0x7c, 0x28, 0x9c, 0x41, 0xaa, 0xbe, 0x55, 3); // DEVPROP_TYPE_BOOLEAN

	//
	// Device properties
	// These DEVPKEYs may be set by the driver package installed for a device.
	//
	Add_DevPropKey(DEVPKEY_DrvPkg_Model, _T(""));	//,                  0xcf73bb51, 0x3abf, 0x44a2, 0x85, 0xe0, 0x9a, 0x3d, 0xc7, 0xa1, 0x21, 0x32, 2);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DrvPkg_VendorWebSite, _T(""));	//,          0xcf73bb51, 0x3abf, 0x44a2, 0x85, 0xe0, 0x9a, 0x3d, 0xc7, 0xa1, 0x21, 0x32, 3);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DrvPkg_DetailedDescription, _T(""));	//,    0xcf73bb51, 0x3abf, 0x44a2, 0x85, 0xe0, 0x9a, 0x3d, 0xc7, 0xa1, 0x21, 0x32, 4);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DrvPkg_DocumentationLink, _T(""));	//,      0xcf73bb51, 0x3abf, 0x44a2, 0x85, 0xe0, 0x9a, 0x3d, 0xc7, 0xa1, 0x21, 0x32, 5);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DrvPkg_Icon, _T(""));	//,                   0xcf73bb51, 0x3abf, 0x44a2, 0x85, 0xe0, 0x9a, 0x3d, 0xc7, 0xa1, 0x21, 0x32, 6);     // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_DrvPkg_BrandingIcon, _T(""));	//,           0xcf73bb51, 0x3abf, 0x44a2, 0x85, 0xe0, 0x9a, 0x3d, 0xc7, 0xa1, 0x21, 0x32, 7);     // DEVPROP_TYPE_STRING_LIST


	//
	// Device setup class properties
	// These DEVPKEYs correspond to the SetupAPI SPCRP_XXX setup class properties.
	//
	Add_DevPropKey(DEVPKEY_DeviceClass_UpperFilters, _T("类高层筛选程序"));	//,      0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 19);    // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_DeviceClass_LowerFilters, _T(""));	//,      0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 20);    // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_DeviceClass_Security, _T(""));	//,          0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 25);    // DEVPROP_TYPE_SECURITY_DESCRIPTOR
	Add_DevPropKey(DEVPKEY_DeviceClass_SecuritySDS, _T(""));	//,       0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 26);    // DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_DevType, _T(""));	//,           0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 27);    // DEVPROP_TYPE_UINT32
	Add_DevPropKey(DEVPKEY_DeviceClass_Exclusive, _T(""));	//,         0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 28);    // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceClass_Characteristics, _T(""));	//,   0x4321918b, 0xf69e, 0x470d, 0xa5, 0xde, 0x4d, 0x88, 0xc7, 0x5a, 0xd2, 0x4b, 29);    // DEVPROP_TYPE_UINT32

	//
	// Device setup class properties
	//
	Add_DevPropKey(DEVPKEY_DeviceClass_Name, _T("类名"));	//,              0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 2);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_ClassName, _T("类描述"));	//,         0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 3);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_Icon, _T(""));	//,              0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 4);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_ClassInstaller, _T(""));	//,    0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 5);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_PropPageProvider, _T(""));	//,  0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 6);      // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_NoInstallClass, _T("无安装类"));	//,    0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 7);      // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceClass_NoDisplayClass, _T(""));	//,    0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 8);      // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceClass_SilentInstall, _T(""));	//,     0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 9);      // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceClass_NoUseClass, _T(""));	//,        0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 10);     // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceClass_DefaultService, _T(""));	//,    0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 11);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceClass_IconPath, _T("类图标路径"));	//,          0x259abffc, 0x50a7, 0x47ce, 0xaf, 0x8, 0x68, 0xc9, 0xa7, 0xd7, 0x33, 0x66, 12);     // DEVPROP_TYPE_STRING_LIST

	Add_DevPropKey(DEVPKEY_DeviceClass_DHPRebalanceOptOut, _T(""));	//, 0xd14d3ef3, 0x66cf, 0x4ba2, 0x9d, 0x38, 0x0d, 0xdb, 0x37, 0xab, 0x47, 0x01, 2);    // DEVPROP_TYPE_BOOLEAN

	//
	// Other Device setup class properties
	//
	Add_DevPropKey(DEVPKEY_DeviceClass_ClassCoInstallers, _T(""));	//, 0x713d1703, 0xa2e2, 0x49f5, 0x92, 0x14, 0x56, 0x47, 0x2e, 0xf3, 0xda, 0x5c, 2);     // DEVPROP_TYPE_STRING_LIST


	//
	// Device interface properties
	//
	Add_DevPropKey(DEVPKEY_DeviceInterface_FriendlyName, _T(""));	//,  0x026e516e, 0xb814, 0x414b, 0x83, 0xcd, 0x85, 0x6d, 0x6f, 0xef, 0x48, 0x22, 2);     // DEVPROP_TYPE_STRING
	Add_DevPropKey(DEVPKEY_DeviceInterface_Enabled, _T(""));	//,       0x026e516e, 0xb814, 0x414b, 0x83, 0xcd, 0x85, 0x6d, 0x6f, 0xef, 0x48, 0x22, 3);     // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceInterface_ClassGuid, _T(""));	//,     0x026e516e, 0xb814, 0x414b, 0x83, 0xcd, 0x85, 0x6d, 0x6f, 0xef, 0x48, 0x22, 4);     // DEVPROP_TYPE_GUID


	//
	// Device interface class properties
	//
	Add_DevPropKey(DEVPKEY_DeviceInterfaceClass_DefaultInterface, _T(""));	//,  0x14c83a99, 0x0b3f, 0x44b7, 0xbe, 0x4c, 0xa1, 0x78, 0xd3, 0x99, 0x05, 0x64, 2); // DEVPROP_TYPE_STRING

	//
	// DeviceDisplay properties that can be set on a devnode
	//
	Add_DevPropKey(DEVPKEY_DeviceDisplay_IsShowInDisconnectedState, _T(""));	//, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x44); // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceDisplay_IsNotInterestingForDisplay, _T(""));	//, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x4a); // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceDisplay_Category, _T("设备类别"));						//, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x5a); // DEVPROP_TYPE_STRING_LIST
	Add_DevPropKey(DEVPKEY_DeviceDisplay_UnpairUninstall, _T(""));				//, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x62); // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceDisplay_RequiresUninstallElevation, _T(""));	//, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x63); // DEVPROP_TYPE_BOOLEAN
	Add_DevPropKey(DEVPKEY_DeviceDisplay_AlwaysShowDeviceAsConnected, _T(""));	//, 0x78c34fc8, 0x104a, 0x4aca, 0x9e, 0xa4, 0x52, 0x4d, 0x52, 0x99, 0x6e, 0x57, 0x65); // DEVPROP_TYPE_BOOLEAN
}


CMainFrame::~CMainFrame(void)
{

}

void CMainFrame::Add_DevPropKey_List(DEVPROPKEY devPropKey, LPCTSTR sName, LPCTSTR sFirendlyName)
{
	tag_DevPropKey obj;
	obj.devPropKey = devPropKey;
	obj.sName = sName;
	obj.sFirendlyName = sFirendlyName;
	m_arrDevPropKey.push_back(obj);
}

void CMainFrame::InitWindow()
{
	UI_BINDCONTROL(CTreeDeviceUI, m_tree, _T("tree_device_info"));
	UI_BINDCONTROL(CGridUI, m_grid, _T("grid_info"));

#ifdef _UNICODE
	FunCM_Get_DevNode_Property = (CM_GET_DEVNODE_PROPERTY)GetProcAddress(uiApp.m_hLibCfgmgr32, "CM_Get_DevNode_PropertyW");
	FunCM_Get_Class_Property = (CM_GET_CLASS_PROPERTY)GetProcAddress(uiApp.m_hLibCfgmgr32, "CM_Get_Class_PropertyW");
#else
#endif

	InitTree();
}

bool CMainFrame::OnCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_DEVICECHANGE)
	{
		InitTree();
	}
	else if(uMsg == WM_CLOSE)
	{
		m_tree->DeleteAllNode();
	}
	return false;
}

bool CMainFrame::OnMenuCommand(const MenuCmd *cmd)
{
	return false;
}

bool CMainFrame::OnMenuUpdateCommandUI(CMenuCmdUI *cmdUI)
{
	return false;
}

void CMainFrame::OnNotifyClick(TNotifyUI& msg)
{

}

void CMainFrame::OnNotifySelectChanged(TNotifyUI& msg)
{
	if(msg.pSender == m_tree)
	{
		TNodeData *pNodeSelected = m_tree->GetSelectNode();
		if(pNodeSelected)
		{
			InitGrid(pNodeSelected);
		}
		return;
	}
}

CDuiString CMainFrame::GetTreeNodeName(DEVINST devInst) //获取插入树中的名字
{
	ULONG uLength = 1024;

	CDuiString sFriendlyName;
	CM_Get_DevInst_Registry_Property(devInst, CM_DRP_FRIENDLYNAME, NULL, sFriendlyName.GetBuffer(1024), &uLength, 0);
	if(!sFriendlyName.IsEmpty())
		return sFriendlyName;

	CDuiString sDesc;
	uLength = 1024;
	CM_Get_DevInst_Registry_Property(devInst, CM_DRP_DEVICEDESC, NULL, sDesc.GetBuffer(1024), &uLength, 0);
	if(!sDesc.IsEmpty())
		return sDesc;

	CDuiString sEnumName;
	uLength = 1024;
	CM_Get_DevInst_Registry_Property(devInst, CM_DRP_ENUMERATOR_NAME, NULL, sEnumName.GetBuffer(1024), &uLength, 0);
	if(!sEnumName.IsEmpty())
		return sEnumName;

	CDuiString szDeviceID;
	uLength = 1024;
	CM_Get_Device_ID(devInst, szDeviceID.GetBuffer(1024), uLength, 0);
	if(!szDeviceID.IsEmpty())
		return szDeviceID;

	return _T("");
}

void CMainFrame::InitTree()
{
	CONFIGRET cfgRet = CR_SUCCESS;
	ULONG uLength = 1024;
	BYTE buffer[1024] = { 0 };

	for (ULONG  ulClassIndex=0; ; ulClassIndex++)
	{
		GUID guid;
		cfgRet = CM_Enumerate_Classes(ulClassIndex, &guid, 0);
		if(cfgRet != CR_SUCCESS) break;

		uLength = 1024;
		CDuiString sClassName;
		CM_Get_Class_Name(&guid, sClassName.GetBuffer(1024), &uLength, 0);

		uLength = 1024;
		CDuiString sClassFriendlyName;
		CM_Get_Class_Registry_Property(&guid, CM_DRP_FRIENDLYNAME, NULL, sClassFriendlyName.GetBuffer(1024), &uLength, 0, NULL);

		TNodeData *pNode = m_tree->InsertNode(sClassFriendlyName.IsEmpty() ? sClassName : sClassFriendlyName);
		tagDeviceNodeTag *pTag = new tagDeviceNodeTag;
		pTag->type = 0;
		pTag->classguid = FormatGuid(&guid);
		m_tree->SetNodeTag(pNode, (UINT_PTR)pTag);
		pNode->Expand(true);

		InitTree_SetupApi_Class(&guid, pNode);
	}
}

void CMainFrame::InitTree_SetupApi_Class(LPGUID pGuid, TNodeData *pParentNode)
{
	CONFIGRET cfgRet = CR_SUCCESS;
	ULONG uLength = 1024;
	DWORD err = 0;

	//获取设备信息集
	HDEVINFO hDevInfo = SetupDiGetClassDevs(pGuid, NULL, NULL, 0);
	if (hDevInfo == INVALID_HANDLE_VALUE)
	{
		err = GetLastError();
		return;
	}

	//遍历设备
	for (DWORD dwMemberIndex = 0; ; dwMemberIndex++)
	{
		SP_DEVINFO_DATA spDevInfoData;
		memset(&spDevInfoData, 0, sizeof(SP_DEVINFO_DATA));
		spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
		if (!SetupDiEnumDeviceInfo(hDevInfo, dwMemberIndex, &spDevInfoData))
		{
			err = GetLastError();
			break;
		}

		CDuiString sNodeName = GetTreeNodeName(spDevInfoData.DevInst);
		TNodeData *pNode = m_tree->InsertNode(sNodeName, pParentNode);
		tagDeviceNodeTag *pTag = new tagDeviceNodeTag;
		pTag->type = 1;
		pTag->devInst = spDevInfoData.DevInst;
		SetupDiLoadDeviceIcon(hDevInfo, &spDevInfoData, 16, 16, 0, &pTag->hIcon);
		tagDeviceNodeTag *pParentTag = (tagDeviceNodeTag *)pParentNode->GetTag();
		if(pParentTag->hIcon == NULL) 
			pParentTag->hIcon = pTag->hIcon;
		m_tree->SetNodeTag(pNode, (UINT_PTR)pTag);
	}

	SetupDiDestroyDeviceInfoList(hDevInfo);
}

void CMainFrame::SetCellText(int row, int col, CDuiString &sCellText)
{
	int ct = 1;
	int pos = 0;
	pos = sCellText.Find(_T("\r\n"), pos);
	while (pos >= 0)
	{
		ct++; 
		pos += _tcslen(_T("\r\n"));
		pos = sCellText.Find(_T("\r\n"), pos);
	}
	int nHeight = m_grid->GetRowHeight(row);
	m_grid->SetRowHeight(row, nHeight*ct);

	m_grid->Cell(row,col).SetText(sCellText);
}

void CMainFrame::InitGrid(TNodeData *pNode)
{
	m_grid->ResetGridBody();

	tagDeviceNodeTag *pTag = (tagDeviceNodeTag *)pNode->GetTag();
	if(pTag == NULL) return;

	if(pTag->type == 0)
	{
		for (int i=0; i<m_arrDevPropKey.size(); i++)
		{
			GUID guid;
			IIDFromString(pTag->classguid, &guid);
			my_CM_Get_Class_Property(&guid, m_arrDevPropKey[i]);
		}
	}
	else if(pTag->type == 1)
	{
		for (int i=0; i<m_arrDevPropKey.size(); i++)
		{
			my_CM_Get_DevNode_Property(pTag->devInst, m_arrDevPropKey[i]);
		}
	}
}


void CMainFrame::my_CM_Get_DevNode_Property(DEVINST devInst, tag_DevPropKey &devKeys)
{
	if(FunCM_Get_DevNode_Property == NULL)
		return;

	CONFIGRET cfgRet = CR_SUCCESS;
	DEVPROPTYPE devPropType;
	BYTE PropertyBuffer[4096];
	ULONG uPropertyBufferSize = 4096;
	cfgRet = FunCM_Get_DevNode_Property(devInst, (const DEVPROPKEY *)&devKeys.devPropKey, &devPropType, PropertyBuffer, &uPropertyBufferSize, 0);
	if(cfgRet != CR_SUCCESS)  
	{
		if(cfgRet != CR_NO_SUCH_VALUE)
		{
			int row = m_grid->InsertRow();
			m_grid->Cell(row,1).SetText(devKeys.sFirendlyName.IsEmpty() ? devKeys.sName : devKeys.sFirendlyName);
			m_grid->Cell(row,2).SetText(GetConfigErrorText(cfgRet));
			m_grid->Refresh();
		}
		return;
	}

	int row = m_grid->InsertRow();
	m_grid->Cell(row,1).SetText(devKeys.sFirendlyName.IsEmpty() ? devKeys.sName : devKeys.sFirendlyName);
	CDuiString sCellText = GetDevKeyPropValue(row, devKeys, devPropType, PropertyBuffer, uPropertyBufferSize);
	SetCellText(row,2, sCellText);
	return;
}

void CMainFrame::my_CM_Get_Class_Property(LPGUID pClassGuid, tag_DevPropKey &devKeys)
{
	if(FunCM_Get_Class_Property == NULL)
		return;

	CONFIGRET cfgRet = CR_SUCCESS;
	DEVPROPTYPE devPropType;
	BYTE PropertyBuffer[4096];
	ULONG uPropertyBufferSize = 4096;
	cfgRet = FunCM_Get_Class_Property(pClassGuid, (const DEVPROPKEY *)&devKeys.devPropKey, &devPropType, PropertyBuffer, &uPropertyBufferSize, 0);
	if(cfgRet != CR_SUCCESS)  
	{
		if(cfgRet != CR_NO_SUCH_VALUE)
		{
			int row = m_grid->InsertRow();
			m_grid->Cell(row,1).SetText(devKeys.sFirendlyName.IsEmpty() ? devKeys.sName : devKeys.sFirendlyName);
			m_grid->Cell(row,2).SetText(GetConfigErrorText(cfgRet));
			m_grid->Refresh();
		}
		return;
	}

	int row = m_grid->InsertRow();
	m_grid->Cell(row,1).SetText(devKeys.sFirendlyName.IsEmpty() ? devKeys.sName : devKeys.sFirendlyName);
	CDuiString sCellText = GetDevKeyPropValue(row, devKeys, devPropType, PropertyBuffer, uPropertyBufferSize);
	SetCellText(row,2, sCellText);
	return;
}

BOOL CMainFrame::CompareDevPropKey(const DEVPROPKEY *key1, const DEVPROPKEY *key2)
{
	return key1->fmtid == key2->fmtid && key1->pid == key2->pid;
}

CDuiString CMainFrame::GetDevKeyPropValue(int row, const tag_DevPropKey &devKeys, DEVPROPTYPE devPropType, LPBYTE PropertyBuffer, ULONG uPropertyBufferSize)
{
	CDuiString sCellText;
	switch (devPropType)
	{
	case DEVPROP_TYPE_EMPTY:	//                      0x00000000  // nothing, no property data
		break;
	case DEVPROP_TYPE_NULL :	//                      0x00000001  // null property data
		break;
	case DEVPROP_TYPE_SBYTE:	//                      0x00000002  // 8-bit signed int (SBYTE)
		break;
	case DEVPROP_TYPE_BYTE :	//                      0x00000003  // 8-bit unsigned int (BYTE)
		break;
	case DEVPROP_TYPE_INT16 :	//                     0x00000004  // 16-bit signed int (SHORT)
		break;
	case DEVPROP_TYPE_UINT16:	//                     0x00000005  // 16-bit unsigned int (USHORT)
		break;
	case DEVPROP_TYPE_INT32 :	//                     0x00000006  // 32-bit signed int (LONG)
		break;
	case DEVPROP_TYPE_UINT32 :	//                    0x00000007  // 32-bit unsigned int (ULONG)
		{
			UINT *pInt = (UINT *)PropertyBuffer;
			if(CompareDevPropKey(&devKeys.devPropKey, &DEVPKEY_Device_DevNodeStatus))
			{
				sCellText = on_CM_Get_DevNode_Status(row, *pInt);
			}
			else if(CompareDevPropKey(&devKeys.devPropKey, &DEVPKEY_Device_Capabilities))
			{
				sCellText = On_CM_DRP_CAPABILITIES(row, *pInt);
			}
			else
			{
				sCellText.Format(_T("%08X"), *pInt);
			}
		}
		break;
	case DEVPROP_TYPE_INT64 :	//                     0x00000008  // 64-bit signed int (LONG64)
		break;
	case DEVPROP_TYPE_UINT64 :	//                    0x00000009  // 64-bit unsigned int (ULONG64)
		break;
	case DEVPROP_TYPE_FLOAT :	//                     0x0000000A  // 32-bit floating-point (FLOAT)
		break;
	case DEVPROP_TYPE_DOUBLE :	//                     0x0000000B  // 64-bit floating-point (DOUBLE)
		break;
	case DEVPROP_TYPE_DECIMAL :	//                    0x0000000C  // 128-bit data (DECIMAL)
		break;
	case DEVPROP_TYPE_GUID :		//                    0x0000000D  // 128-bit unique identifier (GUID)
		{
			GUID *guid = (GUID *)PropertyBuffer;
			sCellText = FormatGuid(guid);
		}
		break;
	case DEVPROP_TYPE_CURRENCY:	//                   0x0000000E  // 64 bit signed int currency value (CURRENCY)
		break;
	case DEVPROP_TYPE_DATE   :	//                    0x0000000F  // date (DATE)
		break;
	case DEVPROP_TYPE_FILETIME  :	//                 0x00000010  // file time (FILETIME)
		{
			FILETIME *pFileTime = (FILETIME *)PropertyBuffer;
			SYSTEMTIME st;
			FileTimeToSystemTime(pFileTime, &st);
			sCellText.Format(_T("%04d-%02d-%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		}
		break;
	case DEVPROP_TYPE_BOOLEAN   :	//                 0x00000011  // 8-bit boolean (DEVPROP_BOOLEAN)
		{
			DEVPROP_BOOLEAN *pBool = (DEVPROP_BOOLEAN *)PropertyBuffer;
			if(*pBool == 0)
				sCellText = _T("FALSE");
			else
				sCellText = _T("TRUE");
		}
		break;
	case DEVPROP_TYPE_STRING   :	//                  0x00000012  // null-terminated string
		sCellText = (LPCTSTR)PropertyBuffer;
		break;
	case DEVPROP_TYPE_STRING_LIST:	// (DEVPROP_TYPE_STRING|DEVPROP_TYPEMOD_LIST) // multi-sz string list
		{
			LPTSTR pszStringList = (LPTSTR)PropertyBuffer;
			int ct = 0;
			for (LPTSTR CurrentString = pszStringList; *CurrentString; CurrentString += _tcslen(CurrentString) + 1)
			{
				if(ct != 0)
					sCellText += _T("\r\n");
				sCellText += CurrentString;
				ct++;
			}
		}
		break;
	case DEVPROP_TYPE_SECURITY_DESCRIPTOR:	//        0x00000013  // self-relative binary SECURITY_DESCRIPTOR
		break;
	case DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING:	// 0x00000014  // security descriptor string (SDDL format)
		break;
	case DEVPROP_TYPE_DEVPROPKEY:	//                 0x00000015  // device property key (DEVPROPKEY)
		break;
	case DEVPROP_TYPE_DEVPROPTYPE:	//                0x00000016  // device property type (DEVPROPTYPE)
		break;
	case DEVPROP_TYPE_BINARY :	//     (DEVPROP_TYPE_BYTE|DEVPROP_TYPEMOD_ARRAY)  // custom binary data
		{
			CDuiString sText;

			if(CompareDevPropKey(&devKeys.devPropKey, &DEVPKEY_Device_PowerData))
			{
				CM_POWER_DATA *pPowerData = (CM_POWER_DATA *)PropertyBuffer;
				if(pPowerData->PD_Size == sizeof(CM_POWER_DATA))
				{
					sText = On_CM_DRP_DEVICE_POWER_DATA(row, pPowerData);
				}
			}
			else
			{
				sText += _T("二进制数据: ");
				for (int i=0; i<uPropertyBufferSize; i++)
				{
					sText.AppendFormat(_T("%02X "), PropertyBuffer[i]);
				}
			}
			sCellText = sText;
		}
		break;
	case DEVPROP_TYPE_ERROR:	//                      0x00000017  // 32-bit Win32 system error code
		break;
	case DEVPROP_TYPE_NTSTATUS:	//                   0x00000018  // 32-bit NTSTATUS code
		break;
	case DEVPROP_TYPE_STRING_INDIRECT:	//            0x00000019  // string resource (@[path\]<dllname>,-<strId>)
		break;
	}

	return sCellText;
}


CDuiString CMainFrame::on_CM_Get_DevNode_Status(int row, ULONG ulStatus)
{
	CDuiString sCellText;
	sCellText.AppendFormat(_T("%X \r\n"), ulStatus);
	if(DN_ROOT_ENUMERATED & ulStatus) sCellText += _T("DN_ROOT_ENUMERATED \r\n");
	if(DN_DRIVER_LOADED & ulStatus) sCellText += _T("DN_DRIVER_LOADED \r\n");
	if(DN_ENUM_LOADED & ulStatus) sCellText += _T("DN_ENUM_LOADED \r\n");
	if(DN_STARTED & ulStatus) sCellText += _T("DN_STARTED \r\n");
	if(DN_MANUAL & ulStatus) sCellText += _T("DN_MANUAL \r\n");
	if(DN_NEED_TO_ENUM & ulStatus) sCellText += _T("DN_NEED_TO_ENUM \r\n");
	if(DN_NOT_FIRST_TIME & ulStatus) sCellText += _T("DN_NOT_FIRST_TIME \r\n");
	if(DN_HARDWARE_ENUM & ulStatus) sCellText += _T("DN_HARDWARE_ENUM \r\n");
	if(DN_LIAR & ulStatus) sCellText += _T("DN_LIAR \r\n");
	if(DN_HAS_MARK & ulStatus) sCellText += _T("DN_HAS_MARK \r\n");
	if(DN_HAS_PROBLEM & ulStatus) sCellText += _T("DN_HAS_PROBLEM \r\n");
	if(DN_FILTERED & ulStatus) sCellText += _T("DN_FILTERED \r\n");
	if(DN_MOVED & ulStatus) sCellText += _T("DN_MOVED \r\n");
	if(DN_DISABLEABLE & ulStatus) sCellText += _T("DN_DISABLEABLE \r\n");
	if(DN_REMOVABLE & ulStatus) sCellText += _T("DN_REMOVABLE \r\n");
	if(DN_PRIVATE_PROBLEM & ulStatus) sCellText += _T("DN_PRIVATE_PROBLEM \r\n");
	if(DN_MF_PARENT & ulStatus) sCellText += _T("DN_MF_PARENT \r\n");
	if(DN_MF_CHILD & ulStatus) sCellText += _T("DN_MF_CHILD \r\n");
	if(DN_WILL_BE_REMOVED & ulStatus) sCellText += _T("DN_WILL_BE_REMOVED \r\n");
	if(DN_NOT_FIRST_TIMEE & ulStatus) sCellText += _T("DN_NOT_FIRST_TIMEE \r\n");
	if(DN_STOP_FREE_RES & ulStatus) sCellText += _T("DN_STOP_FREE_RES \r\n");
	if(DN_REBAL_CANDIDATE & ulStatus) sCellText += _T("DN_REBAL_CANDIDATE \r\n");
	if(DN_BAD_PARTIAL & ulStatus) sCellText += _T("DN_BAD_PARTIAL \r\n");
	if(DN_NT_ENUMERATOR & ulStatus) sCellText += _T("DN_NT_ENUMERATOR \r\n");
	if(DN_NT_DRIVER & ulStatus) sCellText += _T("DN_NT_DRIVER \r\n");
	if(DN_NEEDS_LOCKING & ulStatus) sCellText += _T("DN_NEEDS_LOCKING \r\n");
	if(DN_ARM_WAKEUP & ulStatus) sCellText += _T("DN_ARM_WAKEUP \r\n");
	if(DN_APM_ENUMERATOR & ulStatus) sCellText += _T("DN_APM_ENUMERATOR \r\n");
	if(DN_APM_DRIVER & ulStatus) sCellText += _T("DN_APM_DRIVER \r\n");
	if(DN_SILENT_INSTALL & ulStatus) sCellText += _T("DN_SILENT_INSTALL \r\n");
	if(DN_NO_SHOW_IN_DM & ulStatus) sCellText += _T("DN_NO_SHOW_IN_DM \r\n");
	if(DN_BOOT_LOG_PROB & ulStatus) sCellText += _T("DN_BOOT_LOG_PROB \r\n");
	if(DN_NEED_RESTART & ulStatus) sCellText += _T("DN_NEED_RESTART \r\n");
	if(DN_DRIVER_BLOCKED & ulStatus) sCellText += _T("DN_DRIVER_BLOCKED \r\n");
	if(DN_LEGACY_DRIVER & ulStatus) sCellText += _T("DN_LEGACY_DRIVER \r\n");
	if(DN_CHILD_WITH_INVALID_ID & ulStatus) sCellText += _T("DN_CHILD_WITH_INVALID_ID \r\n");

	return sCellText;
}

CDuiString CMainFrame::On_CM_DRP_DEVICE_POWER_DATA(int row, CM_POWER_DATA *pPowerData)
{
	CDuiString sCellText;

	sCellText += _T("当前电源状态:\r\n"); 

	if(pPowerData->PD_MostRecentPowerState == PowerDeviceUnspecified)
		sCellText.Append(_T("Unspecified"));
	else if(pPowerData->PD_MostRecentPowerState == PowerDeviceD0)
		sCellText.Append(_T("D0"));
	else if(pPowerData->PD_MostRecentPowerState == PowerDeviceD1)
		sCellText.Append(_T("D1"));
	else if(pPowerData->PD_MostRecentPowerState == PowerDeviceD2)
		sCellText.Append(_T("D2"));
	else if(pPowerData->PD_MostRecentPowerState == PowerDeviceD3)
		sCellText.Append(_T("D3"));
	else if(pPowerData->PD_MostRecentPowerState == PowerDeviceMaximum)
		sCellText.Append(_T("Maximum"));
	sCellText += _T("\r\n");


	sCellText += _T("电源功能:\r\n"); 
	sCellText.AppendFormat(_T("%08X\r\n"), pPowerData->PD_Capabilities);


	if(pPowerData->PD_Capabilities & PDCAP_D0_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_D0_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_D1_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_D1_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_D2_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_D2_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_D3_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_D3_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D0_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_WAKE_FROM_D0_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D1_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_WAKE_FROM_D1_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D2_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_WAKE_FROM_D2_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_WAKE_FROM_D3_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_WAKE_FROM_D3_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	if(pPowerData->PD_Capabilities & PDCAP_WARM_EJECT_SUPPORTED)
	{	
		sCellText.Append(_T("PDCAP_WARM_EJECT_SUPPORTED"));
		sCellText += _T("\r\n");

	}

	return sCellText;
}

CDuiString CMainFrame::On_CM_DRP_CAPABILITIES(int row, DWORD dwCaps)
{
	CDuiString sCellText;
	int nHeight = m_grid->GetRowHeight(row);

	sCellText.AppendFormat(_T("%08X\r\n"), dwCaps);
	if(dwCaps & CM_DEVCAP_LOCKSUPPORTED)
	{
		sCellText += _T("CM_DEVCAP_LOCKSUPPORTED");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_EJECTSUPPORTED)
	{
		sCellText += _T("CM_DEVCAP_EJECTSUPPORTED");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_REMOVABLE)
	{
		sCellText += _T("CM_DEVCAP_REMOVABLE");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_DOCKDEVICE)
	{
		sCellText += _T("CM_DEVCAP_DOCKDEVICE");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_UNIQUEID)
	{
		sCellText += _T("CM_DEVCAP_UNIQUEID");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_SILENTINSTALL)
	{
		sCellText += _T("CM_DEVCAP_SILENTINSTALL");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_RAWDEVICEOK)
	{
		sCellText += _T("CM_DEVCAP_RAWDEVICEOK");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_SURPRISEREMOVALOK)
	{
		sCellText += _T("CM_DEVCAP_SURPRISEREMOVALOK");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_HARDWAREDISABLED)
	{
		sCellText += _T("CM_DEVCAP_HARDWAREDISABLED");
		sCellText += _T("\r\n"); 
	}
	if(dwCaps & CM_DEVCAP_NONDYNAMIC)
	{
		sCellText += _T("CM_DEVCAP_NONDYNAMIC");
		sCellText += _T("\r\n"); 
	}

	return sCellText;
}