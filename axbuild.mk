app-objs := main.o
$(APP)/$(app-objs): change_mode 

V9P_PATH=${APP}/rootfs

_force:

change_mode: _force
	chmod 600 $(APP)/rootfs/root/.ssh/authorized_keys
	chmod 700 $(APP)/rootfs/root/.ssh

.PHONY: change_mode _force
