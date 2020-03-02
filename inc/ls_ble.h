#ifndef LS_BLE_H_
#define LS_BLE_H_
#include <stdint.h>
#include <stdbool.h>

#define INVALID_CON_IDX (0xff)
#define INVALID_PEER_ID (0xff)

enum dev_evt_type
{
	STACK_INIT,
	STACK_READY,
	PROFILE_ADDED,
	SERVICE_ADDED,
	ADV_STOPPED,
	SCAN_STOPPED,
};

struct profile_added_evt
{
	uint16_t start_hdl;
	uint8_t status;
};

struct service_added_evt
{
	uint16_t start_hdl;
	uint8_t status;
};

struct adv_stopped_evt
{
	
	
};

struct scan_stopped_evt
{
	
	
};

union dev_evt_u
{
	struct profile_added_evt profile_added;
	struct service_added_evt service_added;
	struct adv_stopped_evt adv_stopped;
	struct scan_stopped_evt scan_stopped;
};

struct ble_stack_cfg
{
	bool private_addr;
	bool controller_privacy;
};

enum uuid_length
{
	UUID_LEN_16BIT = 0,
	UUID_LEN_32BIT,
	UUID_LEN_128BIT,
};

struct char_properties
{
	uint8_t broadcast:1,
			rd_en:1,
			wr_cmd:1,
			wr_req:1,
			ntf_en:1,
			ind_en:1,			
			wr_signed:1,
			ext_prop:1;
};

struct char_permissions
{
	uint8_t rd_perm:2,
			wr_perm:2,
			ind_perm:2,
			ntf_perm:2;
};

struct att_decl
{
	uint8_t *uuid;
	struct char_permissions char_perm;
	struct char_properties char_prop; 
	enum uuid_length uuid_len;
};

struct svc_decl
{
	uint8_t *uuid;
	struct att_decl *att;
	uint8_t nb_att;
	uint8_t	sec_lvl:2,
			uuid_len:2,
			secondary:1;
};

enum gap_evt_type
{
	CONNECTED,
	DISCONNECTED,
	MASTER_PAIR_REQ,
	SLAVE_SECURITY_REQ,
	PAIR_DONE,
	ENCRYPT_DONE,
	GET_LEGACY_OOB,
	DISPLAY_PASSWORD,
	INPUT_PASSWORD,
};

struct pair_feature
{
	uint8_t iocap;
	uint8_t oob;
	uint8_t auth;
	uint8_t key_size;
	uint8_t ikey_dist;
	uint8_t rkey_dist;
};

struct gap_connected
{
	uint8_t peer_id;
};

struct gap_disconnected
{
	uint8_t reason;
};

struct gap_master_pair_req
{
	uint8_t auth;
};

struct gap_slave_security_req
{
	uint8_t auth;
};

struct gap_pair_done
{
	bool succeed;
	union{
		uint8_t auth;
		uint8_t fail_reason;
	}u;
};

struct gap_encrypt_done
{
	uint8_t auth;
};

struct gap_display_password
{
	uint32_t password;
};

union gap_evt_u
{
	struct gap_connected connected;
	struct gap_disconnected disconnected;
	struct gap_master_pair_req master_pair_req;
	struct gap_slave_security_req slave_security_req;
	struct gap_pair_done pair_done;
	struct gap_encrypt_done encrypt_done;
	struct gap_display_password display_pwd;
};

void dev_manager_init(struct ble_stack_cfg *cfg,void (*cb)(enum dev_evt_type,union dev_evt_u *));

void gap_manager_init(void (*evt_cb)(enum gap_evt_type,union gap_evt_u *,uint8_t));

void gap_manager_pairing_response_send(uint8_t con_idx,uint8_t accept,struct pair_feature *feat);

#endif
