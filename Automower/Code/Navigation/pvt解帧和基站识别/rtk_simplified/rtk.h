#ifndef __RTK_H
#define __RTK_H
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define DEBUG_MSG

#define USER_CONF         1   //1=user config file,0=system config file

#define SEC_PER_WEEK      604800
#define SEC_PER_HALF_WEEK 302400
#define SEC_PER_DAY       86400

#define PI          3.1415926535897932  /* pi */
#define D2R         (PI/180.0)          /* deg to rad */
#define R2D         (180.0/PI)          /* rad to deg */
#define CLIGHT      299792458.0         /* speed of light (m/s) */
#define SC2RAD      3.1415926535898     /* semi-circle to radian (IS-GPS) */
#define MUY			3.986005E14
#define OMGE        7.2921151467E-5     /* earth angular velocity (IS-GPS) (rad/s) */
//#define F 			-4.442807633e-10

#define RE_WGS84    6378137.0           /* earth semimajor axis (WGS84) (m) */
#define FE_WGS84    (1.0/298.257223563) /* earth flattening (WGS84) */

#define HION        350000.0            /* ionosphere height (m) */

#define P2_5        0.03125             /* 2^-5 */
#define P2_6        0.015625            /* 2^-6 */
#define P2_11       4.882812500000000E-04 /* 2^-11 */
#define P2_15       3.051757812500000E-05 /* 2^-15 */
#define P2_17       7.629394531250000E-06 /* 2^-17 */
#define P2_19       1.907348632812500E-06 /* 2^-19 */
#define P2_20       9.536743164062500E-07 /* 2^-20 */
#define P2_21       4.768371582031250E-07 /* 2^-21 */
#define P2_23       1.192092895507810E-07 /* 2^-23 */
#define P2_24       5.960464477539063E-08 /* 2^-24 */
#define P2_27       7.450580596923828E-09 /* 2^-27 */
#define P2_29       1.862645149230957E-09 /* 2^-29 */
#define P2_30       9.313225746154785E-10 /* 2^-30 */
#define P2_31       4.656612873077393E-10 /* 2^-31 */
#define P2_32       2.328306436538696E-10 /* 2^-32 */
#define P2_33       1.164153218269348E-10 /* 2^-33 */
#define P2_35       2.910383045673370E-11 /* 2^-35 */
#define P2_38       3.637978807091710E-12 /* 2^-38 */
#define P2_39       1.818989403545856E-12 /* 2^-39 */
#define P2_40       9.094947017729280E-13 /* 2^-40 */
#define P2_43       1.136868377216160E-13 /* 2^-43 */
#define P2_48       3.552713678800501E-15 /* 2^-48 */
#define P2_50       8.881784197001252E-16 /* 2^-50 */
#define P2_55       2.775557561562891E-17 /* 2^-55 */
#define P211        2048    /* 2^11*/
#define P212        4096    /* 2^12*/
#define P214        16384   /* 2^14*/
#define P216        65536   /* 2^16*/
#define MAX_SAT      (NSATGPS+NSATGLO+NSATSBS)
//#define MAX_SAT      (NSATGPS+NSATSBS)                             /* max satellite number (1 to MAXSAT) */
#define MAX_RAW_LEN   4096
#define MAX_ERRMSG	  500
//#define MAX_OBS 24//max observation per epoch (rover+base)  //??????????????
#define MAX_OBS       60//max observation per epoch (rover+base)  //??????????????
#define MAXDTOE       7200.0
#define MAX_SOL_BUF   5
#define FREQ1         1.57542E9           /* L1/E1  frequency (Hz) */
#define FREQ1_GLO   1.60200E9           /* GLONASS G1 base frequency (Hz) */
#define DFRQ1_GLO   0.56250E6           /* GLONASS G1 bias frequency (Hz/n) */

#define LAM1	      CLIGHT/FREQ1
#define BASE_AVE_TIME 10
/* solution */
//#define ENCODER
#define TIME_MEASURE
#define SOL_MSG_LEN   160
//#ifdef ENCODER
//	#define MSG_LEN 149//last byte is '\n', no '\r'
//#elif define TIME_MEASURE
//	#define MSG_LEN 146
//#else
//	#define MSG_LEN 141
//#endif

#define SOLF_ENU 
//#define SOLF_ECEF 
//#define SOLF_LLH
#define SQR(x)   ((x)*(x))
#define sos2(x)  (x[0]*x[0]+x[1]*x[1])
#define sos3(x)  (x[0]*x[0]+x[1]*x[1]+x[2]*x[2])
#define sos4(x)  (x[0]*x[0]+x[1]*x[1]+x[2]*x[2]+x[3]*x[3])
#define norm2(x) sqrt(x[0]*x[0]+x[1]*x[1])
#define norm3(x) sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2])

#define PMODE_SINGLE       0
#define PMODE_KINEMA       2            /* positioning mode: kinematic */
#define PMODE_STATIC       3            /* positioning mode: static */
#define PMODE_STATIC_START 4            /* positioning mode: static start*/
#define PMODE_FIXED        6            /* positioning mode: fixed */
#define SOL_TYPE_BACKWARD 0
#define SOL_TYPE_FORWARD  1
#define SOL_TYPE_COMBINED 2

#define ION_BRDC		1
#define ION_SBAS		2

#define SOLQ_NONE   0                   /* solution status: no solution */
#define SOLQ_FIX    1                   /* solution status: fix */
#define SOLQ_FLOAT  2                   /* solution status: float */
#define SOLQ_SBAS   3                   /* solution status: SBAS */
//#define SOLQ_DGPS   4                   /* solution status: DGPS/DGNSS */
#define SOLQ_SINGLE 5                   /* solution status: single */
//#define SOLQ_PPP    6                   /* solution status: PPP */
//#define SOLQ_DR     7                   /* solution status: dead reconing */ 

#define EFACT_GPS   1.0                 /* error factor: GPS */
#define EFACT_GLO   1.5                 /* error factor: GLONASS */
#define EFACT_SBS   3.0                 /* error factor: SBAS */

#define SYS_NONE    0x00                /* navigation system: none */
#define SYS_GPS     0x01                /* navigation system: GPS */
#define SYS_SBS     0x02                /* navigation system: SBAS */
#define SYS_GLO     0x04                /* navigation system: GLONASS */
#define SYS_GAL     0x08                /* navigation system: Galileo */
#define SYS_QZS     0x10                /* navigation system: QZSS */
#define SYS_CMP     0x20                /* navigation system: BeiDou */
#define SYS_LEO     0x40                /* navigation system: LEO */
#define SYS_ALL     0xFF                /* navigation system: all */
#ifndef NFREQ
#define NFREQ       1                   /* number of carrier frequencies */
#endif

#define IONOOPT_OFF  0                   /* ionosphere option: correction off */
#define IONOOPT_BRDC 1                   /* ionosphere option: broadcast model */
#define IONOOPT_SBAS 2                   /* ionosphere option: SBAS model */
#define IONOOPT_IFLC 3                 /* ionosphere option: L1/L2 or L1/L5 iono-free LC */
#define IONOOPT_EST  4                   /* ionosphere option: estimation */
#define IONOOPT_TEC  5                   /* ionosphere option: IONEX TEC model */
#define IONOOPT_QZS  6                   /* ionosphere option: QZSS broadcast model */
#define IONOOPT_LEX  7                   /* ionosphere option: QZSS LEX ionospehre */
#define IONOOPT_STEC 8                   /* ionosphere option: SLANT TEC model */

#define TROPOPT_OFF  0                   /* troposphere option: correction off */
#define TROPOPT_SAAS 1                   /* troposphere option: Saastamoinen model */
#define TROPOPT_SBAS 2                   /* troposphere option: SBAS model */
#define TROPOPT_EST  3                   /* troposphere option: ZTD estimation */
#define TROPOPT_ESTG 4                   /* troposphere option: ZTD+grad estimation */
#define TROPOPT_COR  5                   /* troposphere option: ZTD correction */
#define TROPOPT_CORG 6                   /* troposphere option: ZTD+grad correction */

#define ARMODE_OFF     0                 /* AR mode: off */
#define ARMODE_CONT    1                 /* AR mode: continuous */
#define ARMODE_INST    2                 /* AR mode: instantaneous */
#define ARMODE_FIXHOLD 3                 /* AR mode: fix and hold */
#define ARMODE_WLNL    6                 /* AR mode: wide lane/narrow lane */
#define ARMODE_TCAR    7                 /* AR mode: triple carrier ar */

#define GLO_ARMODE_OFF     0             /* GLO AR mode: off */
#define GLO_ARMODE_ON      1             /* GLO AR mode: on */
#define GLO_ARMODE_AUTOCAL 2             /* GLO AR mode: autocal */
#define GLO_ARMODE_FIXHOLD 3             /* GLO AR mode: fix and hold */
//#define BASE_POS_TYPE_LLH
#define BASE_POS_TYPE_XYZ_ECEF

#define MINPRNGPS   1                       /* min satellite PRN number of GPS */
#define MAXPRNGPS   32                      /* max satellite PRN number of GPS */
#define NSATGPS     (MAXPRNGPS-MINPRNGPS+1) /* number of GPS satellites */
#define NSYSGPS     1
#define MAXDTOE_GLO 1800.0					/* max time difference to GLONASS Toe (s) */
#define MAXDTOE_SBS 360.0					/* max time difference to SBAS Toe (s) */
#define MINPRNGLO   1						/* min satellite slot number of GLONASS */
#define MAXPRNGLO   24						/* max satellite slot number of GLONASS */
#define NSATGLO     (MAXPRNGLO-MINPRNGLO+1) /* number of GLONASS satellites */
#define NSYSGLO     1
#define MAXSBSMSG   32						/* max number of SBAS msg in RTK server */
#define MAXLEAPS    64                      /* max number of leap seconds table */

#ifdef ENALEO
#define MINPRNLEO   1                   /* min satellite sat number of LEO */
#define MAXPRNLEO   10                  /* max satellite sat number of LEO */
#define NSATLEO     (MAXPRNLEO-MINPRNLEO+1) /* number of LEO satellites */
#define NSYSLEO     1
#else
#define MINPRNLEO   0
#define MAXPRNLEO   0
#define NSATLEO     0
#define NSYSLEO     0
#endif
#define NSYS        (NSYSGPS+NSYSGLO+NSYSGAL+NSYSQZS+NSYSCMP+NSYSLEO) /* number of systems */

#define MINPRNSBS   120						/* min satellite PRN number of SBAS */
#define MAXPRNSBS   142						/* max satellite PRN number of SBAS */
#define NSATSBS     (MAXPRNSBS-MINPRNSBS+1) /* number of SBAS satellites */
#define DTTOL       0.005					/* tolerance of time difference (s) */

#ifdef WIN_DLL
#define EXPORT __declspec(dllexport) /* for Windows DLL */
#else
#define EXPORT
#endif



typedef struct{
	time_t time;	//seconds since 00:00:00 January 1st 1970
	double frac;	//fraction of second under 1s
}gtime_t;

typedef enum{
	LEN_ERROR=0,
	CHECKSUM_ERROR=1,
	INCOMPLETE=2,
	TIMING_ERROR=3,
	ID_ERROR=4,
	IODE_ERROR=5,
	TEST_ERROR=6,	
	MAT_ERR=7,
	ZERO_MAT_ERR=8,
	UNCHANGE=9,
	OBS_ERROR=0x0A,
	EPH_ERROR=0x0B,	
	TYPE_ERROR=0x0C,
	GLO_ID_ERROR=0x0D,
	NO_ERROR1=0x0E,
	NO_ERROR2=0x0F,
	NO_ERROR3=0x10,
	EPHEMERIS=0x11,
	OBS=0x12,
	SOLUTION=0x13,
	ION_UTC=0x14,
	SBAS=0x15,
}Error;

typedef struct {        /* SNR mask type */
    int ena[2];         /* enable flag {rover,base} */
    float mask[9]; /* mask (dBHz) at 5,15,25,35,45,55,65,75,85 deg */
} snrmask_t;

typedef struct{
	uint8_t sat; /* satellite number*/
	int iode, iodc;
	int sva;	/* SV accuracy (URA index) */
	int svh;	/* SV health (0: ok)*/
	int week;	/* gps week*/
	gtime_t toe, toc, ttr;	//Toe:time of ephemeris,toc:time of clock, ttr: transmit time
	double A, e, i0, OMG0, omg, M0, deln, OMgd, idot; //delta_n:[rad],M0:[rad],OMG0[rad],idot:rate of inclination
	double crc, crs, cuc, cus, cic, cis;  //crs:sin correction to radius,crc:cosine correction to radius
	//cic:cosine correction to inclination cis:sin correction to inclination
	double toes;/*Toe(s) in week*/
	double fit;/*fit interval[h]*/ 
	double f0, f1, f2;/*SV clock params*/
	double tgd[4];	/*group delay params*/
}eph_t;

typedef struct {        /* GLONASS broadcast ephemeris type */
    int sat;            /* satellite number */
    int iode;           /* IODE (0-6 bit of tb field) */
    int frq;            /* satellite frequency number */
    int svh,sva,age;    /* satellite health, accuracy, age of operation */
    gtime_t toe;        /* epoch of epherides (gpst) */
    gtime_t tof;        /* message frame time (gpst) */
    double pos[3];      /* satellite position (ecef) (m) */
    double vel[3];      /* satellite velocity (ecef) (m/s) */
    double acc[3];      /* satellite acceleration (ecef) (m/s^2) */
    double taun,gamn;   /* SV clock bias (s)/relative freq bias */
    double dtaun;       /* delay between L1 and L2 (s) */
} geph_t;


typedef struct {        /* SBAS ephemeris type */
    int sat;            /* satellite number */
    gtime_t t0;         /* reference epoch time (GPST) */
    gtime_t tof;        /* time of message frame (GPST) */
    int sva;            /* SV accuracy (URA index) */
    int svh;            /* SV health (0:ok) */
    double pos[3];      /* satellite position (m) (ecef) */
    double vel[3];      /* satellite velocity (m/s) (ecef) */
    double acc[3];      /* satellite acceleration (m/s^2) (ecef) */
    double af0,af1;     /* satellite clock-offset/drift (s,s/s) */
} seph_t;

typedef struct {        /* SBAS message type */
    int week,tow;       /* receiption time */
    int prn;            /* SBAS satellite PRN number */
    unsigned char msg[29]; /* SBAS message (226bit) padded by 0 */
}sbsmsg_t;

typedef struct {        // SBAS ionospheric correction type 
    gtime_t t0;         // correction time 
    short lat,lon;      // latitude/longitude (deg) 
    short give;         // GIVI+1 
    float delay;        // vertical delay estimate (m) 
}sbsigp_t;

typedef struct {        /* IGP band type */
    short x;            /* longitude/latitude (deg) */
    const short *y;     /* latitudes/longitudes (deg) */
    unsigned char bits; /* IGP mask start bit */
    unsigned char bite; /* IGP mask end bit */
} sbsigpband_t;

typedef struct {        /* SBAS ionospheric corrections type */
    int iodi;           /* IODI (issue of date ionos corr) */
    int nigp;           /* number of igps */
    sbsigp_t igp[201]; /* ionospheric correction */
} sbsion_t;

typedef struct {        /* SBAS fast correction type */
    gtime_t t0;         /* time of applicability (TOF) */
    double prc;         /* pseudorange correction (PRC) (m) */
    double rrc;         /* range-rate correction (RRC) (m/s) */
    double dt;          /* range-rate correction delta-time (s) */
    int iodf;           /* IODF (issue of date fast corr) */
    short udre;         /* UDRE+1 */
    short ai;           /* degradation factor indicator */
} sbsfcorr_t;

typedef struct {        /* SBAS long term satellite error correction type */
    gtime_t t0;         /* correction time */
    int iode;           /* IODE (issue of date ephemeris) */
    double dpos[3];     /* delta position (m) (ecef) */
    double dvel[3];     /* delta velocity (m/s) (ecef) */
    double daf0,daf1;   /* delta clock-offset/drift (s,s/s) */
} sbslcorr_t;

typedef struct {        /* SBAS satellite correction type */
    int sat;            /* satellite number */
    sbsfcorr_t fcorr;   /* fast correction */
    sbslcorr_t lcorr;   /* long term correction */
} sbssatp_t;

typedef struct {        /* SBAS satellite corrections type */
    int iodp;           /* IODP (issue of date mask) */
    int nsat;           /* number of satellites */
    int tlat;           /* system latency (s) */
    sbssatp_t sat[MAX_SAT]; /* satellite correction */
} sbssat_t;


typedef struct{			//observation data record, single band
	gtime_t time;		//receiver sampling time
	uint8_t sat, rcv;	//satellite/receiver number
	uint8_t SNR;
	uint8_t LLI;		//lost of clock indicator
	double L;			/* observation data carrier-phase (cycle) */
	double P;			/* observation data pseudorange (m) */
	float D;			/* observation data doppler frequency (Hz) */	
}obsd_t;

typedef struct{
	int n, nmax;		 //number of observation data/allocated
	obsd_t data[MAX_OBS];//~280 bytes
}obs_t;

typedef struct {        /* processing options type */
    int mode;           /* positioning mode (PMODE_???) */
    int soltype;        /* solution type (0:forward,1:backward,2:combined) */
    int nf;             /* number of frequencies (1:L1,2:L1+L2,3:L1+L2+L5) */  //nf==1
    int navsys;         /* navigation system */
    float elmin;       /* elevation mask angle (rad) */
    snrmask_t snrmask;  /* SNR mask */
    int sateph;         /* satellite ephemeris/clock (EPHOPT_:ephemeris option) */
    int maxout;         /* obs outage count to reset bias */
    int minlock;        /* min lock count to fix ambiguity */
    int minfix;         /* min fix count to hold ambiguity */
    int ionoopt;        /* ionosphere option (IONOOPT_???) */
    int tropopt;        /* troposphere option (TROPOPT_???) */
    int dynamics;       /* dynamics model (0:none,1:velociy,2:accel) */
    int tidecorr;       /* earth tide correction (0:off,1:solid,2:solid+otl+pole) */
    int niter;          /* number of filter iteration */

    int codesmooth;     /* code smoothing window size (0:none) */
    int intpref;        /* interpolate reference obs (for post mission) */
    int sbascorr;       /* SBAS correction options */
    int sbassatsel;     /* SBAS satellite selection (0:all) */
    int rovpos;         /* rover position for fixed mode */
    int refpos;         /* base position for relative mode */
                        /* (0:pos in prcopt,  1:average of single pos, */
                        /*  2:read from file, 3:rinex header, 4:rtcm pos) */
    double eratio; /* code/phase error ratio */
    double err[5];      /* measurement error factor */
                        /* [0]:reserved */
                        /* [1-3]:error factor a/b/c of phase (m) */
                        /* [4]:doppler frequency (hz) */
    double std[3];      /* initial-state std [0]bias,[1]iono [2]trop */
    double prn[5];      /* process-noise std [0]bias,[1]iono [2]trop [3]acch [4]accv */
    double sclkstab;    /* satellite clock stability (sec/sec) */
    double thresar[4];  /* AR validation threshold */
    double elmaskar;    /* elevation mask of AR for rising satellite (deg) */
    double elmaskhold;  /* elevation mask to hold ambiguity (deg) */
    double thresslip;   /* slip threshold of geometry-free phase (m) */
    double maxtdiff;    /* max difference of time (sec) */
    double maxinno;     /* reject threshold of innovation (m) */
    float maxgdop;     /* reject threshold of gdop */
    double baseline[2]; /* baseline length constraint {const,sigma} (m) */
    double ru[3];       /* rover position for fixed mode {x,y,z} (ecef) (m) */
    double rb[3];       /* base position for relative mode {x,y,z} (ecef) (m) */
    int minfixsats;     /* min sats to fix integer ambiguities */
    int minholdsats;    /* min sats to hold integer ambiguities */
    int mindropsats;    /* min sats to drop sats in AR */
	int arfilter;       /* AR filtering to reject bad sats (0:off,1:on) */
	int modear;         /* AR mode (0:off,1:continuous,2:instantaneous,3:fix and hold,4:ppp-ar) */
	int glomodear;      /* GLONASS AR mode (0:off,1:on,2:auto cal,3:ext cal) */
    int gpsmodear;      /* GPS AR mode (0:off,1:on) */
} prcopt_t;

extern const prcopt_t default_opt;
typedef struct {        /* almanac type */
    int sat;            /* satellite number */
    int svh;            /* sv health (0:ok) */
    int svconf;         /* as and sv config */
    int week;           /* GPS/QZS: gps week, GAL: galileo week */
    gtime_t toa;        /* Toa */
                        /* SV orbit parameters */
    double A,e,i0,OMG0,omg,M0,OMGd;
    double toas;        /* Toa (s) in week */
    double f0,f1;       /* SV clock parameters (af0,af1) */
} alm_t;

typedef struct{
	int n,nmax;//number of broadcast ephemeris
	int na, namax;//number of almanac data
	int ng,ngmax;       /* number of glonass ephemeris */
    int ns,nsmax;       /* number of sbas ephemeris */
	eph_t  *eph;
	geph_t *geph;       /* GLONASS ephemeris */
	seph_t *seph;       /* SBAS ephemeris */
	//erp_t erp;//earth rotation param
	alm_t *alm;         /* almanac data */
	double utc_gps[4];//GPS delta-UTC params {A0, A1, Tot, Wnt},A0&A1:constant and first order of polynomial, Tot:reference UTC,WNt:reference UTC week number
	double utc_glo[4];  /* GLONASS UTC GPS time parameters */
	double utc_sbs[4];  /* SBAS UTC parameters */
	double ion_gps[8];//GPS iono model params {a0-a3,b0-b3}
	int leaps;
	sbssat_t sbssat;    /* SBAS satellite corrections */
    sbsion_t sbsion[11]; /* SBAS ionosphere corrections */
	double lam[MAX_SAT]; /* carrier wave lengths (m) */   //?????????
//	double cbias[MAX_SAT][3];//code bias (0:p1-p2,1:p1-c1,2:p2-c2)
}nav_t;

typedef struct
{
	gtime_t time;         //message time
	gtime_t tobs;	      //observation data time	
	obs_t obs;	          //observation data	
//	obs_t obuf;	          //observation data buffer
	nav_t nav;
	int ephsat;	                   			  //sat number of update ephemeris (0: no satellite)
	uint8_t subfrm[MAX_SAT][150];  			  // ~5Kb
	double lockt[MAX_SAT][2];      			  //lock time, lockt[][1] is the internal flag
	//double icpp[MAX_SAT], off[MAX_SAT], icpc; //carrier parameters for ss2(novAtel superstar II);
	int nbyte;  							  //number of bytes in      
	int len;     							  //message length (bytes) 
	int iod;	 							  //issue of data
	int tod;	 							  //time of day (ms)
	int flag;
    uint8_t buff[MAX_RAW_LEN];		
	sbsmsg_t sbsmsg;    /* SBAS message */
}raw_t;

//solution type
typedef struct{
	gtime_t time;
	double rr[6];//pos/vel (m,m/s)
	float qr[6];//pos variance/covariance (m^2)
	                        /* {c_xx,c_yy,c_zz,c_xy,c_yz,c_zx} or */
                        /* {c_ee,c_nn,c_uu,c_en,c_nu,c_ue} */
	double dtr[2];  //receiver clock bias /* receiver clock bias to time systems (s) */
	uint8_t type;   //0: xyz-ecef, 1:enu-baseline
	uint8_t stat;   //solution status
	uint8_t ns;     //number of valid satellites
	float age;      //age of differential (s)
	float ratio;    // AR ratio factor for valiation 
	int processTime;
	int encoder;
	float prev_ratio1;   /* previous initial AR ratio factor for validation */
    float prev_ratio2;   /* previous final AR ratio factor for validation */
}sol_t;

typedef struct{         //satellite status type
	unsigned char sys;  /* navigation system */
	uint8_t vs;//valid sat flag
	double azel[2];//azimuth, elevation angle (rad)
	double resp;//residuals of pseudorange (m)
	double resc;//residulas of carrier-phase (m)
	uint8_t vsat;//valid sat flag
	uint8_t snr;//signal strength (0.25dBHz)
	uint8_t fix;//1:fix,2:float,3:hold
	uint8_t slip;//cycle-slip flag
	int lock;//lock counter of phase    //first type it as unsigned int??????????????
	unsigned int outc;//obs outage counter of phase
	unsigned int slipc;//cycle-slip counter
	unsigned int rejc;//reject counter
	double phw;//phase windup
	gtime_t pt[2];//previous carrier-phase time
	double ph[2];//previous carrier-phase observable (cycle)
	unsigned char half; /* half-cycle valid flag */     
}ssat_t;

typedef struct{//ambiguity control type
	gtime_t epoch[4];//last epoch
	int fixcnt;//fix counter
	char flags[MAX_SAT];//fix flags
	double n[4];//number of epoch
	double LC[4];//linear combination average
	double LCv[4];//linear combination variance
}ambc_t;

typedef struct{
	sol_t sol;
	double rb[6];          //base position/velocity (ecef) (m,m/s)
	int nx, na;            //number of float states/fixed states
	double tt;             //time difference between current and previous
	double x[82], P[82*82];//float states and their cov float state:pos(3),vel(3),acc(3),sat(32)     //!!!!!!!!!!!!!!!!!!!!!!!
	double xa[3], Pa[9];  //fixed stated and their cov
	                       //fix state:pos(3),vel(3),acc(3)
	int nfix;              //number of continuous fixes of ambiguity
	ambc_t ambc[MAX_SAT];  // ambiguity control 
	ssat_t ssat[MAX_SAT];  // satellite status 
	int neb;//bytes in error msg buffer
	//char errbuf[MAX_ERRMSG];//error msg buffer
	//int errLen;
	prcopt_t opt;
	char holdamb;         // set if fix-and-hold has occurred at least once 
	int excsat;         /* index of next satellite to be excluded for partial ambiguity resolution */
}rtk_t;

/* RTK server type */
typedef struct{
	rtk_t rtk;
	int nb[2];         // number of bytes in input buffers {rov,base} 
	gtime_t ftime;     //download time
	obs_t obs[2];      //observation data (rover,base)
	nav_t nav;
	int format[3];      /* input format {rov,base,corr} */
	int nsbs;           /* number of sbas message */
	sbsmsg_t sbsmsg[MAXSBSMSG]; /* SBAS message buffer */
}rtksvr_t;


extern const double chisqr[100];
gtime_t epoch2time(const double* ep);
gtime_t utc2gpst(gtime_t t);
gtime_t gpst2utc(gtime_t t);
gtime_t gpst2time(int week, double sec);
double time2gpst(gtime_t t, int* week);

extern unsigned int getbitu(const uint8_t*buff,int word, int pos, int len);
extern int getbits(const uint8_t*buff,int word, int pos, int len);
extern unsigned int getbitu_ori(const unsigned char *buff, int pos, int len);
extern int getbits_ori(const unsigned char *buff, int pos, int len);
extern void setbit(uint8_t *buff,int word, int pos, int len, int32_t value);
extern void setbitu_ori(unsigned char *buff, int pos, int len, unsigned int data);

extern int test_glostr(const unsigned char *buff);
extern int decode_glostr(const unsigned char *buff, geph_t *geph);
void setbit(uint8_t*buff,int word, int pos, int len, int32_t value);
double timediff(gtime_t t1, gtime_t t2);
gtime_t timeadd(gtime_t t, double sec);
extern double *mat(int r, int c);
extern double *zeros(int r, int c);
extern void matcpy(double *A, const double *B, int n, int m);
extern int *imat(int n, int m);
extern void matmul(const char *tr, int n, int k, int m, double alpha,
                   const double *A, const double *B, double beta, double *C);
extern int matinv(double *A, int n);
extern int solve(const char *tr, const double *A, const double *Y, int n,int m, double *X);
extern int lsq(const double *A,const double *y,int n, int m, double *x, double *Q);
extern double norm(const double *a, int n);
extern void dops(int ns, const double *azel, double elmin, double *dop);
extern void xyz2enu(const double *pos, double *E);
extern void ecef2enu(const double *pos, const double *r, double *e);
extern double satazel(const double *pos, const double *e, double *azel);
extern void ecef2pos(const double *r, double *pos);
extern void pos2ecef(const double *pos, double *r);
extern double geodist(const double *rs, const double *rr, double *e);
extern double tropmodel(gtime_t time, const double *pos, const double *azel,double humi);   //??????????
extern double ionmodel(gtime_t t, const double *ion, const double *pos,const double *azel); //?????????????
extern double dot(const double *a, const double *b, int n);
extern void time2epoch(gtime_t t, double *ep);
extern void covenu(const double *pos, const double *P, double *Q);
extern double tropmapf(gtime_t time, const double pos[], const double azel[],
                       double *mapfw);
extern int filter(double *x, double *P, const double *H, const double *v,
                  const double *R, int n, int m);
extern double *eye(int n);
extern int testsnr(int base, double el, double snr,
                   const snrmask_t *mask);
extern int satno(int sys, int prn);

//rcvraw.c
void init_raw(raw_t *raw);
extern Error decode_subfrm1(const uint8_t* buff,eph_t *eph);
extern Error decode_subfrm2(const uint8_t* buff,eph_t *eph);
extern Error decode_subfrm3(const uint8_t* buff,eph_t *eph);
extern Error decode_gps_subfrm4(const unsigned char *buff, alm_t *alm,double *ion, double *utc, int *leaps);
extern	Error decode_gps_subfrm5(const unsigned char *buff, alm_t *alm);

//rtksvr.c
extern void rtksvrstart(rtksvr_t* svr,raw_t* raw1,raw_t* raw2,FILE *fp);
extern void updatesvr(rtksvr_t* svr,raw_t* raw,Error Err,int index);

//rtkpos.c
extern void rtkinit(rtk_t *rtk,prcopt_t *opt);
extern int rtkpos(rtk_t *rtk,const obsd_t *obs, int n, const nav_t *nav);
//pntpos.c
extern int pntpos(const obsd_t *obs, int n, const nav_t *nav, sol_t *sol,double *azel, ssat_t *ssat, const prcopt_t *opt);
//ephemeris.c
extern void satposs(gtime_t teph,const obsd_t *obs,int n,const nav_t *nav,double *rs, double *dts, double *var, int *svh);
//solution.c
extern void outsol(const sol_t *sol, const double *rb,FILE *fp);
//ublox
extern Error input_ubx(raw_t* raw, uint8_t data);
//lambda.c
extern int lambda(int n, int m, const double *a, const double *Q, double *f,double *s);

//
extern void deg2dms(double deg, double *dms, int ndec);
extern int satsys(int sat, int *prn);
extern Error input_ubxf(raw_t *raw, FILE *fp);
extern Error decode_frame(const unsigned char *buff, eph_t *eph, alm_t *alm,double *ion, double *utc, int *leaps);
extern int sbsupdatecorr(const sbsmsg_t *msg, nav_t *nav);
extern int sbssatcorr(gtime_t time, int sat, const nav_t *nav, double *rs,double *dts, double *var);
extern int sbsdecodemsg(gtime_t time, int prn, const unsigned int *words,sbsmsg_t *sbsmsg);
extern int sortobs(obs_t *obs);
//extern unsigned int rtk_crc24q(const unsigned char *buff, int len);
//extern int sortobs(obs_t *obs);
extern double sbstropcorr(gtime_t time, const double *pos, const double *azel,
                          double *var);
extern double time2doy(gtime_t t);
extern void covecef(const double *pos, const double *Q, double *P);
extern double satwavelen(int sat, const nav_t *nav);
static void buff2sysopts(prcopt_t opt);
#endif 
