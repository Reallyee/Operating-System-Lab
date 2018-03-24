#include <stdio.h>
#include <signal.h>
#include <stddef.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/termios.h>

int main()
{
	int cpid;/* �����ӽ��̵�id�� */
	int ppid;/* ���游���̵�id�� */
	char buf[256];
  
	ppid=getpid();//�õ����̺�
	cpid=fork();
  
	if (cpid<0)
		exit(-1);
    
	if (!cpid)
	{
		fprintf(stdout,"ID(child)=%d\n",getpid());

		/* ʹ�ӽ������ڵĽ������Ϊǰ̨�����飬Ȼ��ִ��vi */
		//setpgid(0,0);
		//tcsetpgrp(0,getpid());
		execvp("/bin/vi","vi",NULL);
		exit(-1);
	}
   
	fprintf(stdout,"ID(parent)=%d\n",ppid);
	//setpgid(cpid,cpid);/* ���ý����� */
	///tcsetpgrp(0,cpid);/* ���ÿ����ն�Ϊ�ӽ���ӵ�� */
	waitpid(cpid,NULL,0);/* �����̵ȴ��ӽ���ִ����ϣ����ڽ������Ϊǰ̨������ */
	//tcsetpgrp(0,ppid);

	//�����̵ȴ��ն����룬Ȼ�����?
	while(1)
	{
		memset(buf, 0, 256);
		fgets(buf, 256, stdin);
		puts("ECHO: ");
		puts(buf);
		puts("\n");
	}

	return 0;
}
