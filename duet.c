/*
 * Copyright (C) 2026 Ivan Gaydardzhiev
 * Licensed under the GPL-3.0-only
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

static int OO[8];
#define TC  OO[0]
#define TR  OO[1]
#define SEL OO[2]
#define TOP OO[3]
#define CNT OO[4]
#define SHH OO[5]
#define DRT OO[6]
#define RSZ OO[7]
#define _NE 1024
#define _NP 4096
static char _w[_NP];
static char _b[_NE][256];
static signed char _t[_NE];
static struct termios _o;
static int _x=0;
#define _e putchar(27)
#define _lb putchar(91)
#define _N(n) ((n)>=1000?(putchar((n)/1000%10+48),putchar((n)/100%10+48),putchar((n)/10%10+48),putchar((n)%10+48)):(n)>=100?(putchar((n)/100+48),putchar((n)/10%10+48),putchar((n)%10+48)):(n)>=10?(putchar((n)/10+48),putchar((n)%10+48)):putchar((n)+48))
#define _G(r,c) (_e,_lb,_N(r),putchar(59),_N(c),putchar(72))
#define _K (_e,_lb,putchar(50),putchar(75))
#define _S(x) (_e,_lb,_N(x),putchar(109))
#define _SB(x) (_e,_lb,putchar(49),putchar(59),_N(x),putchar(109))
#define _U(v) (putchar(0xe2),putchar(0x94),putchar(v))
#define _HD _U(0x80)
#define _VT _U(0x82)
#define _TL _U(0x8C)
#define _TR2 _U(0x90)
#define _BL _U(0x94)
#define _BR _U(0x98)
#define _ML _U(0x9C)
#define _MR _U(0xA4)
#define _TM _U(0xAC)
#define _BM _U(0xB4)
#define _XX _U(0xBC)
#define _REP(g,n) do{int _O=0;for(;_O<(n);_O++){g;}}while(0)
#define _ALT_ON do{_e;_lb;putchar(63);putchar(49);putchar(48);putchar(52);putchar(57);putchar(104);_e;_lb;putchar(63);putchar(50);putchar(53);putchar(108);}while(0)
#define _ALT_OFF do{_e;_lb;putchar(63);putchar(49);putchar(48);putchar(52);putchar(57);putchar(108);_e;_lb;putchar(63);putchar(50);putchar(53);putchar(104);}while(0)
#define _RV _S(7)
#define _RS _S(0)
#define _F(s,w) do{const char*_I=(s);int _l=(w),_O=0;for(;*_I&&_O<_l;_I++,_O++)putchar(*_I);for(;_O<_l;_O++)putchar(32);}while(0)
#define _HR(n) _REP(_HD,(n))
#define _V(f) static void f(void);
#define _VI(f,t) static int  f(t);
#define _VT_(f,t) static void f(t);
#define _VP(f,a,b) static void f(a,b);
#define _X(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_a,...) _V(_0)_VT_(_1,const char*)_V(_2)_V(_3)_V(_4)_V(_5)_VI(_6,const char*)_V(_7)_V(_8)_V(_9)_V(_a)
#define _PROTO _X(_unraw,_die,_size,_scan,_draw,_key,_elf,_enter,_up,_shell,_fzf)
_PROTO
_VP(_rdir,const char*,int)
_VP(_rfile,const char*,int)
static int _cmp(const void*,const void*);
#define _SIG static void _sig(int _){(void)_;RSZ=1;}
#define _TD typedef void(*_H)(void);
#define _KD(...) static const struct{unsigned char x;_H f;}_KT[]={__VA_ARGS__};
#define _Z (void(*)(void))NULL
_SIG _TD
_KD({0xC2,_up},{0xC6,_enter},{0xA9,_enter},{0xA3,_fzf},{0xD5,_fzf},{0xEB,_shell},{0x00,_Z})
#define _TF(_) _(c_iflag,&=~,(BRKINT|ICRNL|INPCK|ISTRIP|IXON)) _(c_oflag,&=~,(OPOST)) _(c_cflag,|=,(CS8)) _(c_lflag,&=~,(ECHO|ICANON|IEXTEN))
#define _AF(f,op,v) _q.f op v;
#define _X0a static void _raw(void){struct termios _q;if(tcgetattr(0,&_o)==-1)_die("tcgetattr");_q=_o;_TF(_AF)_q.c_cc[VMIN]=1;_q.c_cc[VTIME]=0;if(tcsetattr(0,TCSAFLUSH,&_q)==-1)_die("tcsetattr");}
#define _SUSP _x=1;_ALT_OFF;fflush(stdout);tcsetattr(0,TCSAFLUSH,&_o);
#define _RESU _raw();_x=0;_ALT_ON;fflush(stdout);
#define _MX(m,g) ((m)[0]==((g)[0]^0x5A)&&(m)[1]==((g)[1]^0x5A)&&(m)[2]==((g)[2]^0x5A)&&(m)[3]==((g)[3]^0x5A))
#define _DOTSKIP(_n) ((_n)[0]==46&&(!(_n)[1]||((_n)[1]==46&&!(_n)[2])||!SHH))
#define _TCLASS(_s) (S_ISDIR((_s).st_mode)?1:((_s).st_mode&(S_IXUSR|S_IXGRP|S_IXOTH)?2:0))
#define _CLAMP if(SEL>=CNT)SEL=CNT>0?CNT-1:0;if(TOP>SEL)TOP=SEL;
#define _PINIT int _rw=TC/2-2,_row=0,_mid=TC/2+2;
#define _PAD for(;_row<_cr;_row++){_G(4+_row,_mid);_VT;_F("",_rw);_VT;}
#define _PCELL _G(4+_row,_mid);_VT;putchar(32);
#define _DCOLOR(_d,_x) if(_d)_SB(34);else if(_x)_SB(32);else _RS;
#define _FOPEN_NB(_fd,_path) int _fd=open(_path,O_RDONLY|O_NONBLOCK);if(_fd<0){_G(4,_mid);_VT;_F("?",_rw);_VT;_row=1;_PAD return;}
#define _BYTE(_c) if((_c)==10){_ln[_lp]=0;_G(4+_row,_mid);_VT;putchar(32);_F(_ln,_rw-2);_VT;_row++;_lp=0;}else if((_c)==9){int _sp=4-(_lp%4);while(_sp--&&_lp<_rw-3)_ln[_lp++]=32;}else{if(_lp<_rw-3)_ln[_lp++]=((_c)>=32&&(_c)<127?(char)(_c):46);}
#define _FLUSHL if(_lp>0&&_row<_cr){_ln[_lp]=0;_G(4+_row,_mid);_VT;putchar(32);_F(_ln,_rw-2);_VT;_row++;}
#define _VPORT(_cr) if(SEL<TOP)TOP=SEL;if(SEL>=TOP+_cr)TOP=SEL-_cr+1;if(TOP<0)TOP=0;
#define _BORD_TOP(_lw,_rw) _G(1,1);_K;_TL;_HR(_lw+1);_TM;_HR(_rw+1);_TR2;
#define _HDR(_lw,_rw,_sp) _G(2,1);_K;_VT;putchar(32);_S(1);_F(_w,_lw-1);_RS;_VT;putchar(32);_F(_sp,_rw-1);_VT;
#define _DIV(_lw,_rw) _G(3,1);_K;_ML;_HR(_lw+1);_XX;_HR(_rw+1);_MR;
#define _LENTRY(_idx,_lw) do{int _sel=(_idx)==SEL,_ty=_t[_idx];_VT;if(_sel)_RV;if(_ty==1)_SB(34);else if(_ty==2)_SB(32);else _RS;if(_sel)_RV;putchar(32);_F(_b[_idx],_lw-2);putchar(_ty==1?47:32);_RS;_VT;}while(0)
#define _BORD_BOT(_lw,_rw,_cr) _G(3+_cr+1,1);_K;_BL;_HR(_lw+1);_BM;_HR(_rw+1);_BR;
#define _STATBAR do{char _sc[32]="";if(CNT>0)snprintf(_sc,32," %d/%d ",SEL+1,CNT);_RV;fputs(_sc,stdout);_RS;static const unsigned char _sb[]={0x55^32,0x55^104,0x55^106,0x55^107,0x55^108,0x55^47,0x55^97,0x55^114,0x55^114,0x55^111,0x55^119,0x55^115,0x55^58,0x55^110,0x55^97,0x55^118,0x55^32,0x55^32,0x55^46,0x55^58,0x55^104,0x55^105,0x55^100,0x55^100,0x55^101,0x55^110,0x55^32,0x55^32,0x55^84,0x55^65,0x55^66,0x55^58,0x55^102,0x55^122,0x55^102,0x55^32,0x55^32,0x55^33,0x55^58,0x55^115,0x55^104,0x55^101,0x55^108,0x55^108,0x55^32,0x55^32,0x55^113,0x55^58,0x55^113,0x55^117,0x55^105,0x55^116,0};const unsigned char*_I=_sb;for(;*_I;_I++)putchar(*_I^0x55);}while(0)
#define _GETED(_e) const char*_e=getenv("EDITOR");if(!_e||!*_e)_e=getenv("VISUAL");if(!_e||!*_e)_e="vi";
#define _MKCMD(_cmd,_cl,_e,_p) char _cmd[_NP+128];int _cl=0;const char*_ep=_e;while(*_ep)_cmd[_cl++]=*_ep++;_cmd[_cl++]=32;_cmd[_cl++]=39;const char*_pp=_p;while(*_pp)_cmd[_cl++]=*_pp++;_cmd[_cl++]=39;_cmd[_cl]=0;
#define _XDECODE(_dst,_src,_key) do{int _i=0;const unsigned char*_q=(_src);for(;*_q;_q++)(_dst)[_i++]=*_q^(_key);(_dst)[_i]=0;}while(0)
#define _FZFCMD static const unsigned char _fc[]={0x7E^102,0x7E^105,0x7E^110,0x7E^100,0x7E^32,0x7E^46,0x7E^32,0x7E^45,0x7E^109,0x7E^97,0x7E^120,0x7E^100,0x7E^101,0x7E^112,0x7E^116,0x7E^104,0x7E^32,0x7E^54,0x7E^32,0x7E^50,0x7E^62,0x7E^47,0x7E^100,0x7E^101,0x7E^118,0x7E^47,0x7E^110,0x7E^117,0x7E^108,0x7E^108,0x7E^32,0x7E^124,0x7E^32,0x7E^102,0x7E^122,0x7E^102,0x7E^32,0x7E^45,0x7E^45,0x7E^104,0x7E^101,0x7E^105,0x7E^103,0x7E^104,0x7E^116,0x7E^32,0x7E^52,0x7E^48,0x7E^37,0};
#define _ARR(_) _(65,if(SEL>0)SEL--) _(66,if(SEL<CNT-1)SEL++) _(67,_enter()) _(68,_up())
#define _ACASE(_byte,_act) case _byte:_act;return;
#define _SK(_) _(113,exit(0)) _(3,exit(0)) _(107,if(SEL>0)SEL--) _(106,if(SEL<CNT-1)SEL++) _(103,SEL=0;TOP=0) _(71,SEL=CNT>0?CNT-1:0) _(46,SHH=!SHH;DRT=1) _(33,_shell())
#define _SKIF(_byte,_act) if(_buf[0]==_byte){_act;return;}
#define _X0a static void _raw(void){struct termios _q;if(tcgetattr(0,&_o)==-1)_die("tcgetattr");_q=_o;_TF(_AF)_q.c_cc[VMIN]=1;_q.c_cc[VTIME]=0;if(tcsetattr(0,TCSAFLUSH,&_q)==-1)_die("tcsetattr");}
#define _X1b static void _unraw(void){if(_x)return;_x=1;_ALT_OFF;fflush(stdout);tcsetattr(0,TCSAFLUSH,&_o);putchar(99);putchar(100);putchar(32);putchar(39);fputs(_w,stdout);putchar(39);putchar(10);}
#define _X2c static void _die(const char*_s){if(!_x){_x=1;_ALT_OFF;fflush(stdout);tcsetattr(0,TCSAFLUSH,&_o);}perror(_s);exit(1);}
#define _X3d static void _size(void){struct winsize _q;if(ioctl(1,TIOCGWINSZ,&_q)==-1)_die("ioctl");TC=_q.ws_col,TR=_q.ws_row;}
#define _X4e static int _elf(const char*_p){unsigned char _m[4];int _f,_n;static const unsigned char _g[4]={0x25,0x1F,0x26,0x1C};if((_f=open(_p,O_RDONLY))<0)return 0;_n=read(_f,_m,4);close(_f);return _n==4&&_MX(_m,_g);}
#define _X5f static int _cmp(const void*_a,const void*_b){return strcasecmp((const char*)_a,(const char*)_b);}
#define _X6b static void _scan(void){DIR*_d;struct dirent*_de;int _i=0;char _p[_NP];struct stat _s;if(!(_d=opendir(_w))){char*_sl=strrchr(_w,47);if(_sl&&_sl!=_w)*_sl=0;else _w[1]=0;if(!(_d=opendir(_w))){CNT=0;return;}}while((_de=readdir(_d))&&_i<_NE-1){if(_DOTSKIP(_de->d_name))continue;strncpy(_b[_i],_de->d_name,255);_b[_i][255]=0;_i++;}closedir(_d);CNT=_i;qsort(_b,CNT,sizeof*_b,_cmp);for(_i=0;_i<CNT;_i++){snprintf(_p,_NP,"%s/%s",_w,_b[_i]);_t[_i]=stat(_p,&_s)==-1?0:_TCLASS(_s);}_CLAMP}
#define _X7c static void _rdir(const char*_path,int _cr){DIR*_d;struct dirent*_de;_PINIT char _fp[_NP];struct stat _s;if(!(_d=opendir(_path))){static const unsigned char _ce[]={99^7,97^7,110^7,110^7,111^7,116^7,32^7,111^7,112^7,101^7,110^7,0};int _r;for(_r=0;_r<_cr;_r++){_G(4+_r,_mid);_VT;putchar(32);if(!_r){const unsigned char*_I=_ce;for(;*_I;_I++)putchar(*_I^7);_F("",_rw-13);}else _F("",_rw-1);_VT;}return;}while((_de=readdir(_d))&&_row<_cr){if(_DOTSKIP(_de->d_name))continue;snprintf(_fp,_NP,"%s/%s",_path,_de->d_name);int _isd=stat(_fp,&_s)==0&&S_ISDIR(_s.st_mode);int _isx=!_isd&&(_s.st_mode&(S_IXUSR|S_IXGRP|S_IXOTH));_PCELL _DCOLOR(_isd,_isx)_F(_de->d_name,_rw-3);putchar(_isd?47:32);_RS;_VT;_row++;}closedir(_d);_PAD}
#define _X8d static void _rfile(const char*_path,int _cr){_PINIT if(_elf(_path)){struct stat _s;char _sz[32]="";if(!stat(_path,&_s)){long _B=_s.st_size;if(_B>=(1<<20))snprintf(_sz,32,"%.1fM",(double)_B/(1<<20));else if(_B>=(1<<10))snprintf(_sz,32,"%.1fK",(double)_B/(1<<10));else snprintf(_sz,32,"%ldB",_B);}static const unsigned char _ef[]={0x5B^0x1A,0x45^0x1A,0x4C^0x1A,0x46^0x1A,0x5D^0x1A,0};_G(4,_mid);_VT;putchar(32);_SB(32);const unsigned char*_I=_ef;for(;*_I;_I++)putchar(*_I^0x1A);putchar(32);_F(_sz,_rw-8);_RS;_VT;_row=1;_PAD return;}_FOPEN_NB(_fd,_path)unsigned char _raw[4096];int _got=(int)read(_fd,_raw,sizeof _raw);close(_fd);if(_got<=0){_PAD return;}char _ln[512];int _lp=0,_ri;for(_ri=0;_ri<_got&&_row<_cr;_ri++){unsigned char _c=_raw[_ri];_BYTE(_c)if(_lp>=_rw-3){_ln[_lp]=0;_G(4+_row,_mid);_VT;putchar(32);_F(_ln,_rw-2);_VT;_row++;_lp=0;}}_FLUSHL _PAD}
#define _X9e static void _draw(void){int _lw=TC/2-2,_rw=TC/2-2,_cr=TR-4,_mid=TC/2+1;char _sp[_NP]="";int _row,_idx;_VPORT(_cr)_BORD_TOP(_lw,_rw)if(CNT>0)snprintf(_sp,_NP,"%s/%s",_w,_b[SEL]);_HDR(_lw,_rw,_sp)_DIV(_lw,_rw)for(_row=0;_row<_cr;_row++){_idx=TOP+_row;_G(4+_row,1);_K;if(_idx<CNT){_LENTRY(_idx,_lw);}else{_VT;_F("",_lw+1);_VT;}}if(!CNT){static const unsigned char _em[]={40^15,101^15,109^15,112^15,116^15,121^15,41^15,0};for(_row=0;_row<_cr;_row++){_G(4+_row,_mid);_VT;if(!_row){putchar(32);const unsigned char*_I=_em;for(;*_I;_I++)putchar(*_I^15);_F("",_rw-8);}else _F("",_rw);_VT;}}else{struct stat _s;if(!stat(_sp,&_s)&&S_ISDIR(_s.st_mode))_rdir(_sp,_cr);else _rfile(_sp,_cr);}_BORD_BOT(_lw,_rw,_cr)_G(TR,1);_K;_STATBAR;fflush(stdout);}
#define _Xaf static void _up(void){char*_sl=strrchr(_w,47);if(!_sl||_sl==_w){_w[0]=47;_w[1]=0;}else *_sl=0;SEL=TOP=0;DRT=1;}
#define _Xb0 static void _enter(void){if(!CNT)return;char _p[_NP];struct stat _s;snprintf(_p,_NP,"%s/%s",_w,_b[SEL]);if(stat(_p,&_s)==-1)return;if(S_ISDIR(_s.st_mode)){strncpy(_w,_p,_NP-1);SEL=TOP=0;DRT=1;}else{_GETED(_ed)_SUSP _MKCMD(_cmd,_cl,_ed,_p)system(_cmd);_RESU}}
#define _Xc1 static void _shell(void){const char*_sh=getenv("SHELL");if(!_sh||!*_sh)_sh="/bin/sh";_SUSP system(_sh);_RESU DRT=1;}
#define _Xd2 static void _fzf(void){_SUSP _FZFCMD char _fc2[64];_XDECODE(_fc2,_fc,0x7E);FILE*_pp=popen(_fc2,"r");char _res[_NP]="";if(_pp){if(fgets(_res,_NP,_pp))_res[strcspn(_res,"\n")]=0;pclose(_pp);}_RESU if(!_res[0])return;char _ab[_NP];if(_res[0]==47)strncpy(_ab,_res,_NP-1);else snprintf(_ab,_NP,"%s/%s",_w,_res+2);struct stat _s;if(stat(_ab,&_s)==0){if(S_ISDIR(_s.st_mode)){strncpy(_w,_ab,_NP-1);SEL=TOP=0;DRT=1;}else{char*_sl=strrchr(_ab,47);if(_sl){*_sl=0;strncpy(_w,_ab,_NP-1);*_sl=47;DRT=1;_scan();DRT=0;const char*_nm=_sl+1;int _i;for(_i=0;_i<CNT;_i++)if(!strcmp(_b[_i],_nm)){SEL=_i;TOP=_i>3?_i-3:0;return;}}}}SEL=TOP=0;DRT=1;}
#define _Xe3 static void _key(void){unsigned char _buf[8]={0};int _n=read(0,_buf,8);if(_n<=0)return;if(_buf[0]==0x1B&&_buf[1]==0x5B){switch(_buf[2]){_ARR(_ACASE)}}_SK(_SKIF)if(_buf[0]==126){const char*_h=getenv("HOME");if(_h&&*_h){strncpy(_w,_h,_NP-1);SEL=TOP=0;DRT=1;}return;}int _i=0;for(;_KT[_i].x;_i++)if((_KT[_i].x^0xAA)==_buf[0]){if(_KT[_i].f)_KT[_i].f();return;}}
#define _Xf4 int main(void){signal(SIGWINCH,_sig);_size();if(TC<(6*8-8)||TR<(2*4))_die("too small");_raw();atexit(_unraw);getcwd(_w,_NP);SEL=TOP=0;DRT=1;_ALT_ON;fflush(stdout);for(;;RSZ&&(_size(),DRT=RSZ=1,0),DRT&&(_scan(),DRT=0,0),_draw(),_key());}
#define _Q _X0a _X1b _X2c _X3d _X4e _X5f _X6b _X7c _X8d _X9e _Xaf _Xb0 _Xc1 _Xd2 _Xe3 _Xf4
_Q
