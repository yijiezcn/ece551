#define MODESTR_LEN 10
/*4:*/
#line 55 "./mystat.w"

/*2:*/
#line 26 "./mystat.w"

#include <sys/types.h> 
#include <sys/stat.h> 
#include <time.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <pwd.h> 
#include <grp.h> 
#include <unistd.h> 

/*:2*//*6:*/
#line 91 "./mystat.w"

#include <assert.h>  
#include <string.h>  
#include <errno.h>   

/*:6*/
#line 56 "./mystat.w"

/*14:*/
#line 201 "./mystat.w"

char*get_modestr(char*buf,int mode);
/*:14*//*17:*/
#line 237 "./mystat.w"

void get_permissions(char*buf,int mode,int rmask,int wmask,int xmask);
/*:17*//*24:*/
#line 304 "./mystat.w"

void print_timeinfo(const char*what,const struct timespec*when);

/*:24*/
#line 57 "./mystat.w"

/*3:*/
#line 40 "./mystat.w"


char*time2str(const time_t*when,long ns){
char*ans= malloc(128*sizeof(*ans));
char temp1[64];
char temp2[32];
const struct tm*t= localtime(when);
strftime(temp1,512,"%Y-%m-%d %H:%M:%S",t);
strftime(temp2,32,"%z",t);
snprintf(ans,128,"%s.%09ld %s",temp1,ns,temp2);
return ans;
}

/*:3*//*7:*/
#line 110 "./mystat.w"

int printstat(const char*filename)
{
struct stat st;

/*8:*/
#line 126 "./mystat.w"

if(lstat(filename,&st)!=0){

fprintf(stderr,"stat: cannot stat '%s': %s\n",
filename,strerror(errno));
return errno;


}

#line 50 "./mystat.ch"
/*:8*/
#line 115 "./mystat.w"

/*9:*/
#line 56 "./mystat.ch"

if(S_ISLNK(st.st_mode)){
char buf[256];
ssize_t len;

len= readlink(filename,buf,256);
assert(len>=0);
if(len> 255){
buf[255]= '\0';
}else{
buf[len]= '\0';
}
printf("  File: '%s' -> '%s'\n",filename,buf);
}else{
printf("  File: '%s'\n",filename);
}
#line 140 "./mystat.w"

/*:9*//*10:*/
#line 143 "./mystat.w"

{
const char*filetype= NULL;

switch(st.st_mode&S_IFMT){
case S_IFBLK:filetype= "block special file";break;
case S_IFCHR:filetype= "character special file";break;
case S_IFDIR:filetype= "directory";break;
case S_IFIFO:filetype= "fifo";break;
case S_IFLNK:filetype= "symbolic link";break;
case S_IFREG:filetype= "regular file";break;
case S_IFSOCK:filetype= "socket";break;
}

assert(filetype!=NULL);


printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",
st.st_size,st.st_blocks,st.st_blksize,filetype);
}

#line 82 "./mystat.ch"
/*:10*//*11:*/
#line 88 "./mystat.ch"

printf("Device: %lxh/%lud\tInode: %-10lu",
st.st_dev,st.st_dev,st.st_ino);
if(S_ISCHR(st.st_mode)||S_ISBLK(st.st_mode)){

printf("  Links: %-5lu Device type: %d,%d\n",
st.st_nlink,major(st.st_rdev),minor(st.st_rdev));
}else{
printf("  Links: %lu\n",st.st_nlink);
}
#line 170 "./mystat.w"

/*:11*//*12:*/
#line 176 "./mystat.w"

/*13:*/
#line 190 "./mystat.w"

{
char modestr[MODESTR_LEN+1];

printf("Access: (%04o/%s)",st.st_mode&~S_IFMT,
get_modestr(modestr,st.st_mode));
}

/*:13*//*21:*/
#line 269 "./mystat.w"

{
struct passwd*pw;

pw= getpwuid(st.st_uid);
printf("  Uid: (%5d/%8s)",st.st_uid,pw->pw_name);
}

/*:21*//*22:*/
#line 280 "./mystat.w"

{
struct group*gr;

gr= getgrgid(st.st_gid);
printf("   Gid: (%5d/%8s)",st.st_gid,gr->gr_name);
}


/*:22*/
#line 177 "./mystat.w"

printf("\n");

/*:12*//*23:*/
#line 297 "./mystat.w"

print_timeinfo("Access",&st.st_atim);
print_timeinfo("Modify",&st.st_mtim);
print_timeinfo("Change",&st.st_ctim);
printf(" Birth: -\n");

/*:23*/
#line 116 "./mystat.w"

return 0;
}

/*:7*//*15:*/
#line 203 "./mystat.w"

char*get_modestr(char*buf,int mode)
{
/*16:*/
#line 218 "./mystat.w"

{
char c= 0;
switch(mode&S_IFMT){
case S_IFBLK:c= 'b';break;
case S_IFCHR:c= 'c';break;
case S_IFDIR:c= 'd';break;
case S_IFIFO:c= 'p';break;
case S_IFLNK:c= 'l';break;
case S_IFREG:c= '-';break;
case S_IFSOCK:c= 's';break;
}
assert(c!=0);
buf[0]= c;
}

/*:16*/
#line 206 "./mystat.w"

/*19:*/
#line 251 "./mystat.w"

get_permissions(buf+1,mode,S_IRUSR,S_IWUSR,S_IXUSR);
get_permissions(buf+4,mode,S_IRGRP,S_IWGRP,S_IXGRP);
get_permissions(buf+7,mode,S_IROTH,S_IWOTH,S_IXOTH);


/*:19*/
#line 207 "./mystat.w"

buf[MODESTR_LEN]= '\0';
return buf;
}

/*:15*//*18:*/
#line 239 "./mystat.w"

void get_permissions(char*buf,int mode,int rmask,int wmask,int xmask)
{
buf[0]= (mode&rmask)?'r':'-';
buf[1]= (mode&wmask)?'w':'-';
buf[2]= (mode&xmask)?'x':'-';
}

/*:18*//*25:*/
#line 309 "./mystat.w"

void print_timeinfo(const char*what,const struct timespec*when)
{
char*timestr;

timestr= time2str(&when->tv_sec,when->tv_nsec);
printf("%s: %s\n",what,timestr);
free(timestr);
}


/*:25*/
#line 58 "./mystat.w"

/*5:*/
#line 72 "./mystat.w"

#line 23 "./mystat.ch"
int main(int argc,char**argv)
{
int i;
int rc= EXIT_SUCCESS;
#line 75 "./mystat.w"
if(argc<2){

fprintf(stderr,
"stat: missing operand\n"
"Try 'stat --help' for more information.\n");
return EXIT_FAILURE;
}

#line 35 "./mystat.ch"
for(i= 1;i<argc;i++){
if(printstat(argv[i])!=0){
rc= EXIT_FAILURE;

}
}
return rc;
#line 87 "./mystat.w"
}

/*:5*/
#line 59 "./mystat.w"


/*:4*/
