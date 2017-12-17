/*
 * (C) Radim Kolar 1997-2004
 * This is free software, see GNU Public License version 2 for
 * details.
 *
 * Simple forking WWW Server benchmark:
 *
 * Usage:
 *   webbench --help
 *
 * Return codes:
 *    0 - sucess
 *    1 - benchmark failed (server is not on-line)
 *    2 - bad param
 *    3 - internal error, fork failed
 * 
 */ 
#include "socket.c"
#include <unistd.h>
#include <sys/param.h>
#include <rpc/types.h>
#include <getopt.h>
#include <strings.h>
#include <time.h>
#include <signal.h>

/* values */
//zzy:timerexpired用作是否超时的标志0表示没有超时,1表示超时
volatile int timerexpired=0;
//zzy:speed,failed,bytes 为了方便benchcore的使用，因此设置成全局变量
int speed=0;
int failed=0;
int bytes=0;
/* globals */
int http10=1; /* 0 - http/0.9, 1 - http/1.0, 2 - http/1.1 */
/* Allow: GET, HEAD, OPTIONS, TRACE */
#define METHOD_GET 0
#define METHOD_HEAD 1
#define METHOD_OPTIONS 2
#define METHOD_TRACE 3
#define PROGRAM_VERSION "1.5"
int method=METHOD_GET;
int clients=1;
//zzy:force和force_load为0表示需要接收到http的响应包,1表示不需要接收
int force=0;
int force_reload=0;

int proxyport=80;
char *proxyhost=NULL;
//zzy:默认benchtime
int benchtime=30;
/* internal */
int mypipe[2];
//zzy:MAXHOSTNAMELENDE的值我没有查到明确的答案
char host[MAXHOSTNAMELEN];
#define REQUEST_SIZE 2048
char request[REQUEST_SIZE];

//zzy:struct option解释:http://blog.csdn.net/zhuziyu1157817544/article/details/78822247
static const struct option long_options[]=
{
    //zzy:请注意，凡是不是0的第三个参数,都指向第四个参数,也就是赋值
    {"force",no_argument,&force,1},
    {"reload",no_argument,&force_reload,1},
    {"time",required_argument,NULL,'t'},
    {"help",no_argument,NULL,'?'},
    {"http09",no_argument,NULL,'9'},
    {"http10",no_argument,NULL,'1'},
    {"http11",no_argument,NULL,'2'},
    {"get",no_argument,&method,METHOD_GET},
    {"head",no_argument,&method,METHOD_HEAD},
    {"options",no_argument,&method,METHOD_OPTIONS},
    {"trace",no_argument,&method,METHOD_TRACE},
    {"version",no_argument,NULL,'V'},
    {"proxy",required_argument,NULL,'p'},
    {"clients",required_argument,NULL,'c'},
    {NULL,0,NULL,0}
};

/* prototypes */
static void benchcore(const char* host,const int port, const char *request);
static int bench(void);
static void build_request(const char *url);

//zzy:SIGALRM信号处理函数
static void alarm_handler(int signal)
{
   timerexpired=1;
}	
//zzy:用户使用说明函数
static void usage(void)
{
   fprintf(stderr,
	"webbench [option]... URL\n"
	"  -f|--force               Don't wait for reply from server.\n"
	"  -r|--reload              Send reload request - Pragma: no-cache.\n"
	"  -t|--time <sec>          Run benchmark for <sec> seconds. Default 30.\n"
	"  -p|--proxy <server:port> Use proxy server for request.\n"
	"  -c|--clients <n>         Run <n> HTTP clients at once. Default one.\n"
	"  -9|--http09              Use HTTP/0.9 style requests.\n"
	"  -1|--http10              Use HTTP/1.0 protocol.\n"
	"  -2|--http11              Use HTTP/1.1 protocol.\n"
	"  --get                    Use GET request method.\n"
	"  --head                   Use HEAD request method.\n"
	"  --options                Use OPTIONS request method.\n"
	"  --trace                  Use TRACE request method.\n"
	"  -?|-h|--help             This information.\n"
	"  -V|--version             Display program version.\n"
	);
};

int main(int argc, char *argv[])
{
    int opt=0;
    int options_index=0;
    char *tmp=NULL;
    if(argc==1)
    {
        usage();
        return 2;
    } 
    //zzy:函数解释:http://blog.csdn.net/zhuziyu1157817544/article/details/78822247
    while((opt=getopt_long(argc,argv,"912Vfrt:p:c:?h",long_options,&options_index))!=EOF )
    {
        switch(opt){
        case  0 : break;
        case 'f': force=1;break;
        case 'r': force_reload=1;break; 
        case '9': http10=0;break;
        case '1': http10=1;break;
        case '2': http10=2;break;
        case 'V': printf(PROGRAM_VERSION"\n");exit(0);
        case 't': benchtime=atoi(optarg);break;	     
        case 'p': 
            //zzy:分解url为ip和port两部分
	        /*proxy server parsing server:port */
            //zzy:strchr函数http://blog.csdn.net/tommy_wxie/article/details/7554263
            tmp=strrchr(optarg,':');
	        proxyhost=optarg;
	        if(tmp==NULL){
		        break;
	        }
            //zzy:没有ip的情况
	        if(tmp==optarg){
		        fprintf(stderr,"Error in option --proxy %s: Missing hostname.\n",optarg);
		        return 2;
	        }
            //zzy:没有端口的情况
	        if(tmp==optarg+strlen(optarg)-1){
		        fprintf(stderr,"Error in option --proxy %s Port number is missing.\n",optarg);
		        return 2;
	        }
	        *tmp='\0';
	        proxyport=atoi(tmp+1);break;
        case ':':
        case 'h':
        case '?': usage();return 2;break;
        //zzy:确定client即确定子进程个数
        case 'c': clients=atoi(optarg);break;
        }
    }
 
    //zzy:optind==argc代表输入参数中没有要测试的URL
    //zzy:正常情况下optind = argc -1才对
    if(optind==argc) {
        fprintf(stderr,"webbench: Missing URL!\n");
	    usage();
	    return 2;
    }
    if(clients==0) clients=1;
    if(benchtime==0) benchtime=60;
    /*Copyright */
    fprintf(stderr,"Webbench - Simple Web Benchmark "PROGRAM_VERSION"\n"
	 "Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.\n"
	);
    //zzy:创建http请求包
    build_request(argv[optind]);

    /*print bench info */
    printf("\nBenchmarking: ");
    switch(method){
	 case METHOD_GET:
	 default:
		 printf("GET");break;
	 case METHOD_OPTIONS:
		 printf("OPTIONS");break;
	 case METHOD_HEAD:
		 printf("HEAD");break;
	 case METHOD_TRACE:
		 printf("TRACE");break;
    }
    printf(" %s",argv[optind]);
    switch(http10){
    case 0: printf(" (using HTTP/0.9)");break;
	case 2: printf(" (using HTTP/1.1)");break;
    }
    printf("\n");
    if(clients==1) printf("1 client");
    else
    printf("%d clients",clients);

    printf(", running %d sec", benchtime);
    if(force) printf(", early socket close");
    if(proxyhost!=NULL) printf(", via proxy server %s:%d",proxyhost,proxyport);
    if(force_reload) printf(", forcing reload");
    printf(".\n");
    //zzy:以上此等打印信息都是检查参数有没有被正确解析
    //zzy:下面的bench是开始进行压力测试
    return bench();
}

void build_request(const char *url)
{
    char tmp[10];
    int i;

    bzero(host,MAXHOSTNAMELEN);
    bzero(request,REQUEST_SIZE);

    /*zzy:针对强关不接受响应包的情况下
    * 当没有使用代理服务器并且用http0.9协议(默认就是0.9)　改成1.0协议使用
    */

    if(force_reload && proxyhost!=NULL && http10<1) http10=1;
    
    
    /*zzy:涉及参数有输入请求类型的时候
    * http请求类型介绍:https://www.cnblogs.com/liangxiaofeng/p/5798607.html
    * 此段是为了确定最终的请求包的http版本
    * */
    if(method==METHOD_HEAD && http10<1) http10=1;
    if(method==METHOD_OPTIONS && http10<2) http10=2;
    if(method==METHOD_TRACE && http10<2) http10=2;

    //zzy:构建http包的请求类型
    switch(method){
	  default:
	  case METHOD_GET: strcpy(request,"GET");break;
	  case METHOD_HEAD: strcpy(request,"HEAD");break;
	  case METHOD_OPTIONS: strcpy(request,"OPTIONS");break;
	  case METHOD_TRACE: strcpy(request,"TRACE");break;
    }
    //zzy:根据http包的格式,此处应有空格,然后才能追加url
    strcat(request," ");

    //以下两个if语句监测url是否规范
    if(NULL==strstr(url,"://"))
    {
	    fprintf(stderr, "\n%s: is not a valid URL.\n",url);
	    exit(2);
    }
    if(strlen(url)>1500)
    {
        fprintf(stderr,"URL is too long.\n");
	    exit(2);
    }

    if(proxyhost==NULL)
    //url头必须是http://开头
        if (0!=strncasecmp("http://",url,7)) 
	    { fprintf(stderr,"\nOnly HTTP protocol is directly supported, set --proxy for others.\n");
             exit(2);
        }
    /*protocol/host delimiter */
    //zzy:确定http://后面的第一个位置,比如http://www.baidu.com中的第一个w的位置
    i=strstr(url,"://")-url+3;
    /*printf("%d\n",i); */

    //zzy:检查url有没有一个字符/，要求必须至少一个/且不能在url+i的位置
    if(strchr(url+i,'/')==NULL) {
        fprintf(stderr,"\nInvalid URL syntax - hostname don't ends with '/'.\n");
        exit(2);
    }

    if(proxyhost==NULL)
    {
        /* get port from hostname */
        if(index(url+i,':')!=NULL &&index(url+i,':')<index(url+i,'/'))
        {
	        strncpy(host,url+i,strchr(url+i,':')-url-i);
	        bzero(tmp,10);
	        strncpy(tmp,index(url+i,':')+1,strchr(url+i,'/')-index(url+i,':')-1);
	        /* printf("tmp=%s\n",tmp); */
	        proxyport=atoi(tmp);
	        if(proxyport==0) proxyport=80;
        }
        else
        {
            strncpy(host,url+i,strcspn(url+i,"/"));
        }
        // printf("Host=%s\n",host);
        strcat(request+strlen(request),url+i+strcspn(url+i,"/"));
    }
    else
    {
        //printf("ProxyHost=%s\nProxyPort=%d\n",proxyhost,proxyport);
        strcat(request,url);
    }
    //zzy:执行到此出已经构建了http请求行的第二部分
    if(http10==1)
	    strcat(request," HTTP/1.0");
    else if (http10==2)
	    strcat(request," HTTP/1.1");
    strcat(request,"\r\n");
    //zzy:执行到此出时候,http请求行构建完成
    if(http10>0)  strcat(request,"User-Agent: WebBench "PROGRAM_VERSION"\r\n");
    //zzy:以下1234个if是为了构建http请求头
    //zzy:请求头介绍:http://www.cnblogs.com/zcy_soft/archive/2012/03/05/2380595.html
    if(proxyhost==NULL && http10>0)
    {
	    strcat(request,"Host: ");
	    strcat(request,host);
	    strcat(request,"\r\n");
    }
    if(force_reload && proxyhost!=NULL)
    {
	    strcat(request,"Pragma: no-cache\r\n");
    }
    if(http10>1)
	    strcat(request,"Connection: close\r\n");
        /* add empty line at end */
    if(http10>0) strcat(request,"\r\n"); 
    // printf("Req=%s\n",request);
}

/* vraci system rc error kod */
static int bench(void)
{
    //zzy:个人觉得这个ijk不太好,ijk是为了父进程以后从文件流中读出子进程所给的speed,fail和bytes而设定的
    int i,j,k;	
    pid_t pid=0;
    FILE *f;

    /*check avaibility of target server */
    i=Socket(proxyhost==NULL?host:proxyhost,proxyport);
    if(i<0) { 
	    fprintf(stderr,"\nConnect to server failed. Aborting benchmark.\n");
        return 1;
    }
    close(i);
    /*create pipe */

    //zzy:通信使用,就是子进程发送本分支压力测试的结果给父进程，父进程接收读结果的一个通信
    if(pipe(mypipe))
    {
	  perror("pipe failed.");
	  return 3;
    }
    /* not needed, since we have alarm() in childrens */
    /* wait 4 next system clock tick */
    /*
    cas=time(NULL);
    while(time(NULL)==cas)sched_yield();
    */

    /* fork childs */
    for(i=0;i<clients;i++)
    {
        pid=fork();
	    if(pid <= (pid_t) 0)
	    {
		    /*child process or error*/
	        sleep(1); /* make childs faster */
		    break;
	    }
    }
    if( pid< (pid_t) 0)
    {
        fprintf(stderr,"problems forking worker no. %d\n",i);
	    perror("fork failed.");
	    return 3;
    }
    //zzy:子进程的任务如下
    if(pid== (pid_t) 0)
    {
        /* I am a child */
        //zzy:有代理测代理，没有代理测一般普通的那种url
        if(proxyhost==NULL)
            benchcore(host,proxyport,request);
        else
            benchcore(proxyhost,proxyport,request);
        /* write results to pipe */
	    f=fdopen(mypipe[1],"w");
	    if(f==NULL)
	    {
	        perror("open pipe for writing failed.");
		    return 3;
	    }
	    /* fprintf(stderr,"Child - %d %d\n",speed,failed); */
        //zzy:测试结果给父进程,然后本分支进程测试结束
        fprintf(f,"%d %d %d\n",speed,failed,bytes);
	    fclose(f);
	    return 0;
    } 
    else
    {
        f=fdopen(mypipe[0],"r");
	    if(f==NULL) 
	    {
		    perror("open pipe for reading failed.");
		    return 3;
	    }
	    setvbuf(f,NULL,_IONBF,0);
	    speed=0;
        failed=0;
        bytes=0;
	    while(1)
	    {
            /*zzy:我感觉是所有的写都写完了也就是fscanf会自动阻塞直到所有写的fp关闭它才会读，
            *要不然多进程对同一个流操作,不用锁,很容易数据错乱,
            *而且如果是实时读，那么当fscanf发现目前没有数据,
            *也就是本次数据没有到来,而并不意味这子进程结束,
            *也就是子进程可能还在recvbuf或者怎样的时候,
            *本次EOF fscanf意味着循环结束,
            *那么工作中的子进程没有办法给父进程传数据了,
            *但是我没有找到相关的信息.当然我的锅,我没有阅读过fscanf源码
            *同理,fprintf也自带阻塞,不然也会数据错乱
		    */
            pid=fscanf(f,"%d %d %d",&i,&j,&k);
		    //出错或者到头了就
            if(pid<2)
            {
                fprintf(stderr,"Some of our childrens died.\n");
                break;
            }
            //zzy:父进程作为主干，总和各分支的情况
		    speed+=i;
		    failed+=j;
		    bytes+=k;
		    /*fprintf(stderr,"*Knock* %d %d read=%d\n",speed,failed,pid); */
		    if(--clients==0) break;
	    }
	    fclose(f);
        //zzy:打印总和信息
        printf("\nSpeed=%d pages/min, %d bytes/sec.\nRequests: %d susceed, %d failed.\n",
		(int)((speed+failed)/(benchtime/60.0f)),
		(int)(bytes/(float)benchtime),
		speed,
		failed);
    }
  return i;
}

void benchcore(const char *host,const int port,const char *req)
{
    int rlen;
    char buf[1500];
    int s,i;
    struct sigaction sa;

    /*setup alarm signal handler */
    sa.sa_handler=alarm_handler;
    sa.sa_flags=0;
    if(sigaction(SIGALRM,&sa,NULL)) exit(3);
    //zzy:alarm解释:https://typecodes.com/linux/linuxalarmknockfunc.html
    alarm(benchtime);
    rlen=strlen(req);
nexttry:
    while(1)
    {
        //zzy:如果超时,就不给那个网站发数据了,也就是本分支的测试任务已经完成
        if(timerexpired)
        {
           if(failed>0)
            {
                /*fprintf(stderr,"Correcting failed by signal\n"); */
                failed--;
            }
            return;
        }
        s=Socket(host,port);                          
        
        //zzy:以下是各种发送出错情况
        if(s<0) { failed++;continue;}
        if(rlen!=write(s,req,rlen)) {failed++;close(s);continue;}
        if(http10==0) 
	        if(shutdown(s,1)) { failed++;close(s);continue;}
       
        //涉及到接收http响应包
        if(force==0) 
        {
            /*read all available data from socket */
	        while(1)
	        {
                if(timerexpired) break; 
	            i=read(s,buf,1500);
                /*fprintf(stderr,"%d\n",i); */
	            if(i<0) 
                { 
                    failed++;
                    close(s);
                    //zzy:读失败了无所谓,进行下一次压力发送即可
                    goto nexttry;
                }
	            else if(i==0) break;
                //zzy:这里的bytes代表本分支一共接收到的bytes数量
                else bytes+=i;

	        }
        }
        if(close(s)) {failed++;continue;}
        speed++;
    }
}
