# How to Run sshd on RuxOS

1. Download the RuxOS source code:

   ```shell
   git clone --branch dev https://github.com/syswonder/ruxos.git
   cd ruxos
   ```

2. Import the sshd App directory:

   ```shell
   git clone https://github.com/syswonder/rux-sshd.git ./apps/c/openssh
   ```

3. Configure passwordless login. On the SSH client, run:

   ```shell
   ssh-keygen -t ed25519 -f ~/.ssh/id_ed25519
   ```

   Copy the content of the generated `~/.ssh/id_ed25519.pub` file to `apps/c/openssh/rootfs/root/.ssh/authorized_keys`:

   ```shell
   cat ~/.ssh/id_ed25519.pub >> apps/c/openssh/rootfs/root/.ssh/authorized_keys
   ```

4. Run sshd:

   ```shell
   make run ARCH=aarch64 A=apps/c/openssh V9P=y MUSL=y NET=y V=2
   ```

5. Use the client to connect:
   ```shell
   ssh -vvv -p 5555 syswonder@localhost -i ~/.ssh/id_ed25519
   ```