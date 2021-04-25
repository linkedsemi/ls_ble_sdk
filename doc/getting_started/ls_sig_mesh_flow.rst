.. _header-n28:

SIG MESH工作流程  
==================

.. _header-n30:

1. sig mesh 初始化
-------------------------
..  mermaid::

   sequenceDiagram

   Participant A as App_Layer

   Participant B as Stack_Layer

   A->>A :sys_init_app

   A->>A :user init code

   A->>A :tinyfs_mkdir  <br> (please refer to modulde tinyfs)

   A->>A :check unbind for node <br> (fast up and down power 5 times to unbind the node)

   

   rect rgb(150,210,249)

   autonumber

   A->>B:ble_init()

   A->>B:dev_manager_init(dev_manager_callback)

   A->>B:gap_manager_init(gap_manager_callback)

   A->>B:gatt_manager_init(gatt_manager_callback)

   end

   loop ble_loop

   B-->>+A: stack_init

   A->>-B: ble_stack_cfg <br> ( Privacy configuration mac_addr/controller) <br> (user define mac addr)

   B-->>A: stack_ready

   Note right of A: get mac address

   rect rgb(222,232,222)

   opt mesh application

   activate A

   A->>B:dev_manager_prf_ls_sig_mesh_add() <br>(Add feature for sig mesh)

   deactivate A

   activate B

   B-->>A:profile mesh added

   deactivate B

   A->>B:prf_ls_sig_mesh_callback_init(mesh_manager_callback)

   A->>B:ls_sig_mesh_init(&model_env)

   note over A,B:resgister all models

   end

   end

   end

   

   

   

   


.. _header-n32:

2. Register  Model
---------------------

..  mermaid::

   sequenceDiagram

   Participant A as App_Layer

   Participant B as Mesh_Stack_Layer

   A->>B:ls_sig_mesh_init(&model_env)

   note over A,B:All models have been registered  

   rect rgb(222,232,222)

   opt mesh_auto_prov(unprov)

   B-->>A:MESH_ACTIVE_AUTO_PROV

   

   note left of A: App_defined:<br>unicast_address<br>group_address<br>app_key<br>net_key 

   note left of A: App_defined:<br> Server Model <br>Client Model 

   A->>B:ls_sig_mesh_auto_prov_handler

   end

   end

   B-->>A:MESH_ACTIVE_STORAGE_LOAD

   note left of A: Node_Get_Proved_State:<br>UNPROVISIONED_KO <br> PROVISIONING <br> PROVISIONED_OK

   B-->>A:MESH_ACTIVE_ENABLE

   rect rgb(218,234,130)

   par clear power up num

   A->>A: Clear number of the power up  after 3 Seconds

   and proving_param_req

   B-->>A:MESH_GET_PROV_INFO

   note left of A:App_Set_Prov_Param:<br> devuuid/UriHash/OobInfo/PubKeyOob<br>StaticOob/OutOobSize/InOobSize<br>OutOobAction/InOobAction/Info

   A->>B:set_prov_param

   note over A,B:The device waits to be provisioned

   end

   end

   


.. _header-n34:

3. Provisoning
-------------------

..  mermaid::

   sequenceDiagram

   Participant A as App_Layer

   Participant B as Mesh_Stack_Layer

   note over A,B: The device is provisioning

   rect rgb(222,232,222)

   opt Static OOB

   B-->>A:MESH_GET_PROV_AUTH_INFO

   note left of A:App_defined:Auth_data

   A->>B:set_prov_auth_info

   end

   end

   note right of B:PROV_STARTED<br>PROV_SUCCEED<br>PROV_FAILED

   B-->>A:MESH_REPOPT_PROV_RESULT

   note left of A:app_store:<br>Provisoning result

   rect rgb(251,209,176)

   alt Prov_Successed

   note right of B: all models local index<br>app key local index 

   B-->>A:MESH_ACTIVE_REGISTER_MODEL

   note left of A: app_store:<br>all models local index <br> app key local index 

   else Prov_Failed

   note over A,B: The device is provisioning

   end

   end
