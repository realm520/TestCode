#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

using namespace std;

#define MY_LOGO "ztzhang"

const int PORT = 8888;
const int BACKLOG = 5;

int lsnSock = -1;
union
{
	struct cmsghdr cm;
	char control[CMSG_SPACE(sizeof(int))];
} control_un;

bool initListenSock();
void childProcess();
void parentProcess();
bool acceptSock();
int sendFd(int fd, int fdToSend);
int recvFd(int fd, int *fdToRecv);
int connectToServer();

int x_sock_set_block(int sock, int on)
{
	int             val;
	int             rv;

	val = fcntl(sock, F_GETFL, 0);
	if (on)
	{
		rv = fcntl(sock, F_SETFL, ~O_NONBLOCK&val);
	}
	else
	{
		rv = fcntl(sock, F_SETFL, O_NONBLOCK|val);
	}

	if (rv)
	{
		return errno;
	}

	return 0;
}

int main()
{
	pid_t   pid;
	int     sockpair[2];
	int     rv;
	char    fname[256];
	int     fd;

	rv = socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair);
	if (rv < 0)
	{
		printf("Call socketpair error, errno is %d\n", errno);
		return errno;
	}

	pid = fork();
	if (pid == 0)
	{
		/* in child */
		close(sockpair[1]);

		rv = x_sock_set_block(sockpair[0], 1);
		if (rv != 0)
		{
			printf("[CHILD]: x_sock_set_block error, errno is %d\n", rv);
			return 0;
		}

		rv = recvFd(sockpair[0], &fd);
		if (rv < 0)
		{
			printf("[CHILD]: recv_fd error, errno is %d\n", rv);
			return 0;
		}

		if (fd < 0)
		{
			printf("[CHILD]: child process exit normally!\n");
			return 0;
		}

		cout << "received fd: " << fd << endl;
		rv = write(fd, MY_LOGO, strlen(MY_LOGO));
		if (rv < 0)
		{
			printf("[CHILD]: write error, errno is %d\n", rv);
		}
		else
		{
			printf("[CHILD]: append logo successfully\n");
		}
        sleep(10);
		close(fd);

		exit(0);
	}

	/* in parent */
/*
	memset(fname, 0, sizeof(fname));
	printf("[PARENT]: please enter filename:\n");
	scanf("%s", fname);

	if (strcmp(fname, "exit") == 0)
	{
		rv = sendFd(sockpair[1], -1);
		if (rv < 0)
		{
			printf("[PARENT]: send_fd error, errno is %d\n", rv);
		}
		return 0;
	}

	fd = open(fname, O_RDWR | O_APPEND);
	if (fd < 0)
	{
		if (errno == ENOENT)
		{
			printf("[PARENT]: can't find file '%s'\n", fname);
			return 0;
		}
		printf("[PARENT]: open file error, errno is %d\n", errno);
	}
*/
    int cfd = connectToServer();
        printf("[PARENT]: connect_fd success, fd is %d\n", cfd);
        fgetc(stdin);
	rv = sendFd(sockpair[1], cfd);
	if (rv != 0) {
        printf("[PARENT]: send_fd error, errno is %d\n", rv);
    }
    else {
        sleep(1);
		rv = write(cfd, MY_LOGO, strlen(MY_LOGO));
		if (rv < 0)
		{
			printf("[PARENT]: write error, errno is %d\n", rv);
		}
		else
		{
			printf("[PARENT]: append logo successfully\n");
		}
        fgetc(stdin);
        close(cfd);
        printf("[PARENT]: send_fd success, close fd\n");
        fgetc(stdin);
    }

    wait(NULL);
    return 0;
}


bool initListenSock()
{
	lsnSock = socket(PF_INET, SOCK_STREAM, 0);
	if (-1 == lsnSock)
	{
		cerr << "create socket failed." << endl;
		return false;
	}

	struct sockaddr_in lsnAddr;
	lsnAddr.sin_family = AF_INET;
	lsnAddr.sin_port = htons(PORT);
	lsnAddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(lsnAddr.sin_zero), '\0', 8);

	if (bind(lsnSock, (struct sockaddr *)&lsnAddr,
				sizeof(struct sockaddr)) == -1)
	{
		cerr << "bind socket failed." << endl;
		return false;
	}

	if (listen(lsnSock, BACKLOG) == -1)
	{
		cerr << "listen socket failed." << endl;
		return false;
	}
}

void childProcess()
{
	cout << "child process is running ..." << endl;
	acceptSock();
	cout << "accept socket by child ..." << endl;
	sleep(10);
}

void parentProcess()
{
	cout << "parent process is running ..." << endl;
	acceptSock();
	cout << "accept socket by parent ..." << endl;
	sleep(10);
}

bool acceptSock()
{
	socklen_t sinSize = sizeof(struct sockaddr_in);
    struct sockaddr_in remoteAddr;
	int newSock = accept(lsnSock, (struct sockaddr *)&remoteAddr, &sinSize);
	if (-1 == newSock)
	{
		cerr << "accept error" << endl;
		return false;
	}

	return true;
}

int sendFd(int fd, int fdToSend)
{
	struct iovec iov[1];
	struct msghdr msg;
	struct cmsghdr *hdr;
	char buf[1];

	if (fdToSend >= 0)
	{
		msg.msg_control = control_un.control;
		msg.msg_controllen = sizeof(control_un.control);
	}
	else
	{
		msg.msg_control         = (caddr_t)NULL;
		msg.msg_controllen      = 0;
	}

	hdr = CMSG_FIRSTHDR(&msg);
	hdr->cmsg_len = CMSG_LEN(sizeof(int));
	hdr->cmsg_level = SOL_SOCKET;
	hdr->cmsg_type = SCM_RIGHTS;
	*(int *)CMSG_DATA(hdr) = fdToSend;

	msg.msg_name    = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = buf;
	iov[0].iov_len  = 1;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;

	if(sendmsg(fd, &msg, 0) < 0) {
		printf("sendmsg error, errno is %d\n", errno);
		return errno;
	} else {
		printf("sendmsg success\n");
    }

	return 0;
}

int recvFd(int fd, int *fdToRecv)
{
	struct cmsghdr *hdr;
	struct iovec    iov[1];
	struct msghdr   msg;
	char            buf[1];

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);

	msg.msg_name    = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = buf;
	iov[0].iov_len  = 1;
	msg.msg_iov     = iov;
	msg.msg_iovlen  = 1;

	if (recvmsg(fd, &msg, 0) < 0)
	{
		return errno;
	}

	hdr = CMSG_FIRSTHDR(&msg);
	*fdToRecv = *(int *)CMSG_DATA(hdr);
	cout << *fdToRecv << endl;

	return 0;
}

int connectToServer() {
    int cfd;
    int recbytes;
    int sin_size;
    char buffer[1024]={0};   
    struct sockaddr_in s_add,c_add;
    unsigned short portnum=8989; 
    printf("Hello,welcome to client !\r\n");

    cfd = socket(AF_INET, SOCK_STREAM, 0);
    if(-1 == cfd) {
        printf("Create socket fail ! \r\n");
        return -1;
    }
    printf("Create socket ok !\r\n");

    bzero(&s_add, sizeof(struct sockaddr_in));
    s_add.sin_family = AF_INET;
    s_add.sin_addr.s_addr = inet_addr("192.168.69.181");
    s_add.sin_port = htons(portnum);
    printf("s_addr = %#x ,port : %#x\r\n", s_add.sin_addr.s_addr,s_add.sin_port);

    if(-1 == connect(cfd,(struct sockaddr *)(&s_add), sizeof(struct sockaddr)))
    {
        printf("connect fail !\r\n");
        return -1;
    }
    printf("connect ok !\r\n");
/*
    if(-1 == (recbytes = read(cfd,buffer,1024)))
    {
        printf("read data fail !\r\n");
        return -1;
    }
    printf("read ok\r\nREC:\r\n");
    buffer[recbytes]='\0';
    printf("%s\r\n",buffer);
    getchar();
    close(cfd);
*/
    return cfd;
}
