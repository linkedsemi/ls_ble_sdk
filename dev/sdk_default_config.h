#ifndef SDK_DEFAULT_CONFIG_H_
#define SDK_DEFAULT_CONFIG_H_

#ifndef SDK_BLE_STORAGE_PEER_MAX
#define SDK_BLE_STORAGE_PEER_MAX 3
#endif

#ifndef SDK_USER_TINYFS_NODE_MAX
#define SDK_USER_TINYFS_NODE_MAX 5
#endif

#ifndef SDK_MAX_USER_TASK_NUM
#define SDK_MAX_USER_TASK_NUM 0
#endif

#ifndef SDK_MAX_CONN_NUM
#define SDK_MAX_CONN_NUM 1
#endif

#ifndef SDK_MAX_PROFILE_NUM
#define SDK_MAX_PROFILE_NUM 1
#endif

#ifndef SDK_MAX_RAL_NUM
#define SDK_MAX_RAL_NUM 1
#endif

#define SDK_MAX_ACT_NUM    (SDK_MAX_CONN_NUM + 1)

#endif

