#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <unistd.h>

char ssh_path[] = "/bin/ssh";
char sshd_path[] = "/bin/sshd";

void run_sshd()
{
    char *sshd_args[] = {sshd_path, "-ddd", NULL};
    if (execv("/bin/sshd", sshd_args) == -1) {
        perror("execv failed for sshd");
        exit(EXIT_FAILURE);
    }
}

void run_ssh_client()
{
    char *ssh_args[] = {ssh_path,
                        "-o",
                        "KexAlgorithms=curve25519-sha256@libssh.org",
                        "-o",
                        "Ciphers=aes128-ctr",
                        "-o",
                        "MACs=hmac-sha2-256",
                        "-vvv",
                        "-p",
                        "22",
                        "-i",
                        "/root/.ssh/id_ed25519",
                        "localhost",
                        NULL};
    if (execv("/bin/ssh", ssh_args) == -1) {
        perror("execv failed for ssh");
        exit(EXIT_FAILURE);
    }
}

void ssh_linux()
{
    char *ssh_args[] = {ssh_path,
                        "-o",
                        "KexAlgorithms=curve25519-sha256@libssh.org",
                        "-o",
                        "Ciphers=aes128-ctr",
                        "-o",
                        "MACs=hmac-sha2-256",
                        "-q",
                        "-p",
                        "22",
                        "-i",
                        "/root/.ssh/id_ed25519",
                        "stone@198.18.0.1",
                        NULL};
    if (execv("/bin/ssh", ssh_args) == -1) {
        perror("execv failed for ssh");
        exit(EXIT_FAILURE);
    }
}

void generate_client_key()
{
    const char *private_key = "/root/.ssh/id_ed25519";
    const char *public_key = "/root/.ssh/id_ed25519.pub";
    unlink(private_key);
    unlink(public_key);
    char *args[] = {"ssh-keygen", "-t", "ed25519", "-f", "/root/.ssh/id_ed25519", // 明确指定路径
                    "-N",         "",                                             // 空密码
                    NULL};
    char *ssh_keygen_path = "/bin/ssh-keygen";
    execv(ssh_keygen_path, args);
}

void generate_host_key()
{
    char *ssh_keygen_path = "/bin/ssh-keygen";
    char *args[] = {"ssh-keygen", "-A", NULL};
    execv(ssh_keygen_path, args);
}

int main(int argc, char **argv, char **envp)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        generate_host_key();
        exit(EXIT_SUCCESS);
    } else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("generate_host_key process exited with status: %d\n", WEXITSTATUS(status));
            run_sshd();
        } else {
            printf("Child process did not exit normally\n");
        }
    }

    return 0;
}