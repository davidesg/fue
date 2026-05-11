/***************************************************************************
 *   Copyright (C) 2009 by David E. Guerrero & Arthur B. Treadway          *
 *   warriord@rocketmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "fue.h"            /* Header file (prototype declarations)        */
#include "gnuplot_graphics.h"
#include <stdlib.h>
#include "gnuplot_i.h"        /* gnuplot interface           */
#include "nlatools.h"            /* Header file (prototype declarations)        */
#include <math.h>

extern FILE *outputv;         /* Output file (global: declared in DRV.C)     */

double Acf_Pacf_Max ( struct Tseries *ser,  int lags )

{
int i;
double cmax, *corr;
corr = vector( 1, lags );

Acf( ser, lags, corr );

 cmax=0.40;

 for ( i = 1; i <= lags; i++ )
      if ( fabs(corr[i]) >= cmax )
          {
          cmax = fabs(corr[i]);
          }

//Pacf_New (ser->data ,   ser->nobs,  corr,  lags);
Pacf( lags, corr );

 for ( i = 1; i <= lags; i++ )
      if ( fabs(corr[i]) >= cmax )
          {
          cmax = fabs(corr[i]);
          }


 if ((cmax > 0.40) & (cmax <= 0.60))
      cmax = 0.60;
 if ((cmax > 0.60) & (cmax <= 0.80))
      cmax = 0.80;
 if ((cmax > 0.80) & (cmax <= 1))
      cmax=1;

return (cmax);

}

double SeriesMax ( double *a, int n )

{
int i;
double AbsMax;

AbsMax = 4;
for ( i = 1; i <= n; i++ ){
	if ( fabs(a[i]) >= AbsMax ) AbsMax = fabs(a[i]);         
	}

if ((AbsMax > 4) & (AbsMax <= 6)) AbsMax = 6;
   	else if ((AbsMax > 6) & (AbsMax <= 8)) AbsMax=  8;
   	else if ((AbsMax > 8) & (AbsMax <= 10)) AbsMax=  10;      	
	else if (AbsMax > 10) AbsMax=  12;
return (AbsMax);

}

int SeriesSize  ( int nobs, int freq )

{
int size;
        if ( freq > 4 ) size = 2;
	else if ( nobs > 200 ) size = 2;
	else size = 1;

return ( size );
}

void gnuplot_File_PlotSer ( struct Tseries *ser, int tsnobs, int tmornsop, int tsby, int nrdiff, int nadiff,  char *x11out, char *seriesname  )

{
   int  i , itmp1, itmp2, n, f, size;
   real  rtmp1, rtmp2, rtmp3, rtmp4, cmax, AbsMax;
   real  *a;

   gnuplot_ctrl*h;
   h=gnuplot_init();

/* Maximum value to plot (if < 2.0, then force 3.0):                         */

   n     = ser->nobs;
   f     = ser->freq;
   itmp1 = ser->max;
   itmp2 = ser->min;
   rtmp1 = ser->data[itmp1];
   rtmp2 = ser->data[itmp2];
   rtmp3 = ser->mean;
   rtmp4 = sqrt( ser->var );




/* Load standardized series:        */
        a = vector( 1, n  );                     

 /* Allocate Time series data:       */
	for ( i = 1; i <= n; i++ ) a[i]= (ser->data[i] - rtmp3) / rtmp4;

/* Find Abs Maximun               */

   AbsMax =  SeriesMax (  a,   n );

/* Find the size */

   size = SeriesSize  (  n ,  f  );


	if ( size == 2 )  gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 16") ;
	else gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 15") ;

	gnuplot_cmd(h, "set output \"%s.eps\"",   x11out);


	if ( size == 2 ) gnuplot_cmd(h,"set size 2,1");	
	else gnuplot_cmd(h,"set size 1.3,.87");

	gnuplot_cmd(h,"set origin 0.02,0");

	gnuplot_setstyle(h,"linespoints");
       
	if ( size == 2 )  gnuplot_cmd(h,"set pointsize .90");
       	else gnuplot_cmd(h,"set pointsize .80");

	if ( size == 2  ) gnuplot_cmd(h,"set title '%s' 40.65,3.75 font 'bold,24'", seriesname);
	else gnuplot_cmd (h,"set title '%s' 26,0.95 font 'bold,22'", seriesname);

       gnuplot_cmd(h,"set border 3 lw 1.6");
       gnuplot_cmd(h,"set xtics  1");
       gnuplot_cmd(h,"set format x \" \"");
       gnuplot_cmd(h,"set xtics nomirror 1");
       gnuplot_cmd(h,"set mxtics 1");
       gnuplot_cmd(h,"set tics out");
       gnuplot_cmd(h,"set ytics nomirror 2");
       gnuplot_cmd(h,"set ticscale 1 .8");
       gnuplot_cmd(h,"set ticslevel  .4");
       gnuplot_cmd(h,"set mytics 2");
           
       gnuplot_cmd(h,"set style line 3 lt 2 lw 1.5"); 
       gnuplot_cmd(h,"set style line 4 lt 1 lw 2.5");

       if( size == 2 ){
         gnuplot_cmd(h,"set xlabel '@^{/E=30  -}_{}{/Times-Roman=24 w} ( @^^{/Symbol=19 \331}_{}{/Symbol=24 s}_{@^{/E=24  -}_{}{/Times-Roman=18 w}} ) = %2.2f{/Arial %} (%2.2f{/Arial %})&{def}&{def}&{def}&{def}&{def} @^^{/Symbol=19 \331}_{}{/Symbol=24 s}_{/Times-Roman=18 w} =  %2.2f{/Arial %} ' 3.4,-2.6 font 'Arial,19'",
                     ((ser->mean)*100),  ((Stdev( ser->data, ser->nobs) / sqrt( ser->nobs ))*100), ((Stdev( ser->data, ser->nobs))*100));
       }
       else{
	 gnuplot_cmd(h,"set xlabel '@^{/E=24  -}_{}{/Times-Roman=21 w} ( @^^{/Symbol=18 \331}_{}{/Symbol=22 s}_{@^{/E=22  -}_{}{/Times-Roman=18 w}} ) = %2.2f{/Arial %} (%2.2f{/Arial %})&{def}&{def}&{def}&{def}&{def}@^^{/Symbol=18 \331}_{}{/Symbol=22 s}_{/Times-Roman=18 w} =  %2.2f{/Arial %} ' -2,-3 font 'Arial,17'",
                     ((ser->mean)*100), ((Stdev( ser->data, ser->nobs) / sqrt( ser->nobs ))*100), ((Stdev( ser->data, ser->nobs))*100));
	}


      /* Set vertical lines */

       if (f>1) {
	 for (i = 1; i <= ((tsnobs-1)/(2*f)); i++ ) 
		gnuplot_cmd(h,"set arrow %d from %d,-%.2f to %d, %.2f nohead lt 1",  i, (-tmornsop+2*f*i), (AbsMax+0.08), (-tmornsop+2*f*i), AbsMax);
	 	}
       else {
	 for (i = 1; i <= ((tsnobs+tmornsop-1)/(2*f*10)); i++ )
              gnuplot_cmd(h,"set arrow %d from %d,-%.2f to %d, %.2f nohead lt 1", 
			  i, (-tmornsop+2*f*i*10), (AbsMax+0.08), (-tmornsop+2*f*i*10), AbsMax) ;
	 	}


double  tics_size;

        /* Find the tics size depending size of the plot */

        if (( size == 2 ) && (AbsMax > 4 )) tics_size = AbsMax + 0.75;
	else if (( size == 2 ) && (AbsMax < 4 )) tics_size = AbsMax + 0.65;
	else if (AbsMax > 4 ) tics_size = AbsMax + 0.72;
	else tics_size = AbsMax + 0.62;

       if (f>1) {
	   for (i = 0; i <= ((tsnobs-1)/(2*f)); i++ )
		   gnuplot_cmd(h,"set label %d '%d' at %d, -%.2f center font 'Arial,17.2'",
			  i+1 , tsby+2*i,(-tmornsop+2*f*i), tics_size );
	 }

       else {
	   for (i = 0; i <= ((tsnobs+tmornsop-1)/(2*f*10)); i++ )
		   gnuplot_cmd(h,"set label %d '%d' at %d, -%.2f center font 'Arial,17.2'",
			       i+1 , tsby+2*i*10-1, (-tmornsop+2*f*i*10), tics_size );
	     }

       gnuplot_plot_ser(h, a, n, tmornsop, AbsMax, "");
       free_vector (a, 1, n);


    gnuplot_close(h) ;

}


void gnuplot_File_CorrSer ( struct Tseries *ser, int npar, int nrdiff, int nadiff, int lags, double cbands,  char *x11out, char *seriesname  )
/* copyright (2008) D. E. Guerrero */
{
   int  i , itmp1, itmp2, n, f;
   real  cmax, AbsMax ;
   real *corr;
   gnuplot_ctrl*h;
   h=gnuplot_init();

/* Maximum value to plot (if < 2.0, then force 3.0):                         */

   n     = ser->nobs;
   f     = ser->freq;

       if ( lags == 0 ){
       if ( ser->nobs < 3 * (ser->freq + 1) ) lags = ser->nobs - ser->freq / 2;
       else if (f==1) lags =9;
       else lags = 3 * (ser->freq + 1);
       }

	if (cbands>0) cmax = cbands;
	else cmax = Acf_Pacf_Max ( ser,  lags );

       corr = vector( 1, lags );
       Acf( ser, lags, corr );



	if (lags >= 30)  gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 16");
	else gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 15");

//	if (nadiff == 0) gnuplot_cmd(h, "set output \"acf_d%d%s.eps\"", nrdiff, x11out);
        gnuplot_cmd(h, "set output \"acf_%s.eps\"", x11out);
//		}

	if (lags >= 30 ) gnuplot_cmd(h,"set size .65,1");
	else if (( lags < 30 )&&( lags >= 15)) gnuplot_cmd(h,"set size 0.423,1");
	else gnuplot_cmd(h,"set size 0.283,1");


	gnuplot_cmd(h,"set origin 0,0");
	gnuplot_cmd(h,"set multiplot");
	gnuplot_setstyle(h,"linespoints");

       gnuplot_cmd(h,"set style line 3 lt 2 lw 1.5"); 
       gnuplot_cmd(h,"set style line 4 lt 1 lw 2.5");
    
  	if( lags >= 30 ) gnuplot_cmd(h,"set size 0.63,0.46");
  	else if (( lags < 30 )&&( lags >= 15)) gnuplot_cmd(h,"set size 0.423,0.41");
// 	else if (( f==1 ) && (n > 200)) gnuplot_cmd(h,"set size 0.65,0.46");	
  	else gnuplot_cmd(h,"set size 0.283,0.41");
  	  

  	if( lags >=  30 )	gnuplot_cmd(h,"set origin 0.02,0.476");
  	else if (( lags < 30 )&&( lags >= 15) )	gnuplot_cmd(h,"set origin 0.02,0.38");
//	else if  (( f==1 ) && (n > 200)) gnuplot_cmd(h,"set origin .02,0.476");
  	else gnuplot_cmd(h,"set origin 0.02,0.38");

       if ( lags >=  30 ) gnuplot_cmd(h,"set title 'acf' -2.2,-.21 font 'Arial,20'");
       else  gnuplot_cmd(h,"set title 'acf' -2.05,-.21 font 'Arial,18'");


/* Define the ripe of the acf  graph  */

  gnuplot_setstyle(h,"impulses");

/* Define the acf graphics border style */

  gnuplot_cmd(h,"set border 2 lw 1.6");


/*  Define the tics and the xgrid lines, no the default ones */

  gnuplot_cmd(h,"set noxtics");
  gnuplot_cmd(h,"set nolabel");


/* Plot the grid lines with arrows and labels*/

   if ((f==1) && ( lags > 9 ))
	{
       	int gap;
       	gap = iround(lags/3);
       	gnuplot_cmd(h,"set arrow 1 from %d,-%.2f to %d,%.2f nohead lt 1", gap , cmax, gap , cmax );
       	gnuplot_cmd(h,"set arrow 2 from %d,-%.2f to %d,%.2f nohead lt 1", 2*gap , cmax, 2*gap , cmax );
       	gnuplot_cmd(h,"set arrow 3 from %d,-%.2f to %d,%.2f nohead lt 1", 3*gap , cmax, 3*gap  , cmax );
 	gnuplot_cmd(h,"set label 1 '%d' at %d, -%.2f center ", gap, gap, cmax+0.08 );
      	gnuplot_cmd(h,"set label 2 '%d' at %d, -%.2f center ", 2*gap, 2*gap, cmax+0.08 );
      	gnuplot_cmd(h,"set label 3 '%d' at %d, -%.2f center ", 3*gap, 3*gap,cmax+0.08 );
	}
   else if ((f==1) && (lags <= 9))
     {
      gnuplot_cmd(h,"set arrow 1 from %d,-%.2f to %d,%.2f nohead lt 1", 3, cmax, 3, cmax );
      gnuplot_cmd(h,"set arrow 2 from %d,-%.2f to %d,%.2f nohead lt 1", 6, cmax, 6, cmax );
      if (lags == 9) gnuplot_cmd(h,"set arrow 3 from %d,-%.2f to %d,%.2f nohead lt 1", 9, cmax, 9, cmax );
      gnuplot_cmd(h,"set label 1 '%d' at %d, -%.2f center",3,3, cmax+0.08 );
      gnuplot_cmd(h,"set label 2 '%d' at %d, -%.2f center",6,6, cmax+0.08 );
      if (lags == 9) gnuplot_cmd(h,"set label 3 '%d' at %d, -%.2f center",9,9,cmax+0.08 );
	}
   else
     {
      gnuplot_cmd(h,"set arrow 1 from %d,-%.2f to %d,%.2f nohead lt 1", f, cmax, f, cmax );
     if (lags >= 2*f ) gnuplot_cmd(h,"set arrow 2 from %d,-%.2f to %d,%.2f nohead lt 1", f+f, cmax, f+f, cmax );
     if (lags >= 3*f ) gnuplot_cmd(h,"set arrow 3 from %d,-%.2f to %d,%.2f nohead lt 1", f+f+f, cmax, f+f+f, cmax );
      gnuplot_cmd(h,"set label 1 '%d' at %d, -%.2f center",f, f, cmax+0.08 );
     if (lags >= 2*f ) gnuplot_cmd(h,"set label 2 '%d' at %d, -%.2f center",f+f, f+f, cmax+0.08 );
     if (lags >= 3*f ) gnuplot_cmd(h,"set label 3 '%d' at %d, -%.2f center",f+f+f, f+f+f, cmax+0.08 );
     }


/* Allocate the ytics and labels */

 gnuplot_cmd(h,"set ytics nomirror %.2f", cmax/2 );
 gnuplot_cmd(h,"set mytics  1" );
 gnuplot_cmd(h,"set ytics out");

/* Allocate lines for grid and standard deviation lines */

  gnuplot_cmd(h,"set style line 1 lt 2 lw 1.2");
  gnuplot_cmd(h,"set style line 2 lt 1 lw 2.8"); 

/* Allocate style for impulses */

  if( lags >=  30 )
  gnuplot_cmd(h,"set style line 5 lt 1 lw 7"); 
  else
  gnuplot_cmd(h,"set style line 5 lt 1 lw 9"); 


/* Allocate the Ljung-Box statistic */

  if( (f>4) || (f==1) && (n>200)  )  
  gnuplot_cmd(h,"set xlabel 'Q( %d ) = %2.1f' -4.1,-2.1 font 'Arial,17'", lags - npar, ChiTest( corr, lags, n ));
  else
  gnuplot_cmd(h,"set xlabel 'Q( %d ) = %2.1f' -1.4,-1.1 font 'Arial,15'", lags - npar, ChiTest( corr, lags, n ));

    gnuplot_plot_acf(h, corr, lags, n, cmax, "");

/*******************************************************************************/

   Pacf( lags, corr );
//     Pacf_New (ser->data ,   ser->nobs,  corr,  lags);

  	if(lags >= 30 ) gnuplot_cmd(h,"set size 0.63,0.385");
  	else if (( lags < 30 )&&( lags >= 15)) gnuplot_cmd(h,"set size 0.423,0.37");
//      else if  ((f==1) && (n > 200)) gnuplot_cmd(h,"set size 0.65,0.385"); 
  	else  gnuplot_cmd(h,"set size 0.283,0.37");


  	if( lags >= 30 ) 
  	gnuplot_cmd(h,"set origin .02,.084");
  	else  if (( lags < 30 )&&( lags >= 15 )) gnuplot_cmd(h,"set origin 0.02,-.008");
//        else if  ((f==1) && (n > 200)) gnuplot_cmd(h,"set origin .02,.084");
  	else gnuplot_cmd(h,"set origin .02,-.008");

  if( lags >= 30 )
  gnuplot_cmd(h,"set title 'pacf'-2,-.21");
  else
  gnuplot_cmd(h,"set title 'pacf'-1.9,-.21");

  gnuplot_setstyle(h,"impulses");
  gnuplot_cmd(h,"set border 2 lw 1.6");

  gnuplot_cmd(h,"set xlabel '' 0,0");
    
    gnuplot_plot_acf(h, corr, lags, n, cmax, "");
    gnuplot_close(h) ;

    free_vector( corr, 1, lags );

}


void gnuplot_File_PlotSer_CorrSer ( struct Tseries *ser, int npar, int tsnobs, int tmornsop, int tsby, double boxlam, int nrdiff, int nadiff, int lags, double cbands,  char *x11out, char *seriesname )
/* copyright (2008) David E. Guerrero & Arthur B. Treadway */
{
   int  i , itmp1, itmp2, n, f, size;
   real  rtmp1, rtmp2, rtmp3, rtmp4, cmax, AbsMax ;
   real *corr, *a;
   gnuplot_ctrl*h;
   h=gnuplot_init();

/* Maximum value to plot (if < 2.0, then force 3.0):                         */

   n     = ser->nobs;
   f     = ser->freq;
   itmp1 = ser->max;
   itmp2 = ser->min;
   rtmp1 = ser->data[itmp1];
   rtmp2 = ser->data[itmp2];
   rtmp3 = ser->mean;
   rtmp4 = sqrt( ser->var );




/* Load standardized series:        */
        a = vector( 1, n  );                      /* Allocate Time series data:       */
	for ( i = 1; i <= n; i++ ) a[i]= (ser->data[i] - rtmp3) / rtmp4;

/* Find Abs Maximun               */
AbsMax =  SeriesMax (  a,   n );


size = SeriesSize  (  n ,  f  );


	if ( size == 2 )  gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 16") ;
	else gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 15") ;


/* Allocate the output file name */
	gnuplot_cmd(h, "set output \"%s.eps\"", x11out);

	if ( size == 2 ) gnuplot_cmd(h,"set size 2,1");	
	else gnuplot_cmd(h,"set size 1.3,.87");

	gnuplot_cmd(h,"set origin 0.02,0");
	gnuplot_cmd(h,"set multiplot");
       
//	if( size == 2 ) gnuplot_cmd(h,"set size 1.32,.975");
    if( size == 2 ) gnuplot_cmd(h,"set size 1.32,.85");
	else gnuplot_cmd(h,"set size 0.852,.767");	


	if( size == 2  ) gnuplot_cmd(h,"set origin 0,0.03");
	else gnuplot_cmd(h,"set origin 0,0.028");

	gnuplot_setstyle(h,"linespoints");

	if ( size == 2  ) gnuplot_cmd(h,"set pointsize .90");
       	else gnuplot_cmd(h,"set pointsize .80");

//	if ( size == 2  ) gnuplot_cmd(h,"set title '%s' 40.65,3.75 font 'bold,24'", seriesname);
//	else gnuplot_cmd (h,"set title '%s' 26,0.95 font 'bold,22'", seriesname);

	if ( size == 2  ) gnuplot_cmd(h,"set title '%s' offset 40.65,3.75 font 'bold,24'", seriesname);
	else gnuplot_cmd (h,"set title '%s' offset 26,0.95 font 'bold,22'", seriesname);


       gnuplot_cmd(h,"set border 3 lw 1.6");
       gnuplot_cmd(h,"set xtics  1");
       gnuplot_cmd(h,"set format x \" \"");
       gnuplot_cmd(h,"set xtics nomirror 1");
       gnuplot_cmd(h,"set mxtics 1");
       gnuplot_cmd(h,"set tics out");
       gnuplot_cmd(h,"set ytics nomirror 2");
//       gnuplot_cmd(h,"set ticscale 1 .8");
       gnuplot_cmd(h,"set ticslevel  .4");
       gnuplot_cmd(h,"set mytics 2");
       gnuplot_cmd(h,"set style line 1 lt 1 lw 1 pt 31");   
       gnuplot_cmd(h,"set style line 3 lt 2 lw 1.5"); 
       gnuplot_cmd(h,"set style line 4 lt 1 lw 2.5");

       if( size == 2 ){
         gnuplot_cmd(h,"set xlabel '@^{/E=30-}_{}{/Times-Roman=24 w} ( @^^{/Symbol=19 \331}_{}{/Symbol=24 s}_{@^{/E=24-}_{}{/Times-Roman=18 w}} ) = %2.2f{/Arial %} (%2.2f{/Arial %})&{def}&{def}&{def}&{def}&{def} @^^{/Symbol=19 \331}_{}{/Symbol=24 s}_{/Times-Roman=18 w} =  %2.2f{/Arial %} ' offset 3.4,-2.6 font 'Arial,19'",
                     ((ser->mean)*100),  ((Stdev( ser->data, ser->nobs) / sqrt( ser->nobs ))*100), ((Stdev( ser->data, ser->nobs))*100));
       }
       else{
	 gnuplot_cmd(h,"set xlabel '@^{/E=24-}_{}{/Times-Roman=21 w} ( @^^{/Symbol=18 \331}_{}{/Symbol=22 s}_{@^{/E=22-}_{}{/Times-Roman=18 w}} ) = %2.2f{/Arial %} (%2.2f{/Arial %})&{def}&{def}&{def}&{def}&{def}@^^{/Symbol=18 \331}_{}{/Symbol=22 s}_{/Times-Roman=18 w} =  %2.2f{/Arial %} ' offset -2,-3 font 'Arial,17'",
                     ((ser->mean)*100), ((Stdev( ser->data, ser->nobs) / sqrt( ser->nobs ))*100), ((Stdev( ser->data, ser->nobs))*100));
	}

      /* Set vertical lines */

       if (f>1) {
	 for (i = 1; i <= ((tsnobs-1)/(2*f)); i++ ) 
		gnuplot_cmd(h,"set arrow %d from %d,-%.2f to %d, %.2f nohead lt 1",  i, (-tmornsop+2*f*i), (AbsMax+0.08), (-tmornsop+2*f*i), AbsMax);
	 	}
       else {
	 for (i = 1; i <= ((tsnobs+tmornsop-1)/(2*f*10)); i++ )
              gnuplot_cmd(h,"set arrow %d from %d,-%.2f to %d, %.2f nohead lt 1", 
			  i, (-tmornsop+2*f*i*10), (AbsMax+0.08), (-tmornsop+2*f*i*10), AbsMax) ;
	 	}


double  tics_size;

        /* Find the tics size depending size of the plot */

        if (( size == 2 ) && (AbsMax > 4 )) tics_size = AbsMax + 0.75;
	else if (( size == 2 ) && (AbsMax < 4 )) tics_size = AbsMax + 0.65;
	else if (AbsMax > 4 ) tics_size = AbsMax + 0.72;
	else tics_size = AbsMax + 0.62;

	if ( f == 1 ){
		   for (i = 0; i <= ((tsnobs+tmornsop-1)/(2*f*10)); i++ )
		   gnuplot_cmd(h,"set label %d '%d' at %d, -%.2f center font 'Arial,17.2'",
			       i+1 , tsby+2*i*10, (-tmornsop+2*f*i*10), tics_size );
	}

       else {
	   for (i = 0; i <= ((tsnobs-1)/(2*f)); i++ )
		   gnuplot_cmd(h,"set label %d '%d' at %d, -%.2f center font 'Arial,17.2'",
			  i+1 , tsby+2*i,(-tmornsop+2*f*i), tics_size );
	 }

       gnuplot_plot_ser(h, a, n, tmornsop, AbsMax, "");
       free_vector (a, 1, n);


/*****************************************************************/
/* Second part:  Add the acf/pacf graph                          */
/*****************************************************************/
gnuplot_cmd(h,"reset");
/* If lags variable is not defined, allocate de defaults values */

 if (lags == 0){
 if ( ser->nobs < 3 * (ser->freq + 1) ) lags = ser->nobs - ser->freq / 2;
      else if (f==1) lags =9;
      else  lags = 3 * (ser->freq + 1);
     }
 

if (cbands>0) cmax = cbands;
else cmax = Acf_Pacf_Max ( ser,  lags );

/* Allocate vector for the coeficients */

 corr = vector( 1, lags );

/* Calculate the acf values */

 Acf( ser, lags, corr );


/* calcule the size of acf, depend of the numbers of lags */    

  if( lags >= 30 ) gnuplot_cmd(h,"set size 0.63,0.46");
  else if ( ( lags < 30 )&&( lags >= 15) ) gnuplot_cmd(h,"set size 0.423,0.41");
  else gnuplot_cmd(h,"set size 0.383,0.41");  

/* Allocate the origin of acf graph, depends of the plots size */

  if( size == 2  ) gnuplot_cmd(h,"set origin 1.395,0.476");
  else gnuplot_cmd(h,"set origin 0.83,0.38");

/* Allocate acf title, the size depend of the global size of the figure */

  if ( lags >=  30 )  gnuplot_cmd(h,"set title 'acf' offset -2.2,-.21 font 'Arial,20'");
  else  gnuplot_cmd(h,"set title 'acf' offset -2.05,-.21 font 'Arial,18'");

/* Define the ripe of the acf  graph  */

  gnuplot_setstyle(h,"impulses");

/* Define the acf graphics border style */

  gnuplot_cmd(h,"set border 2 lw 1.6");


/*  Define the tics and the xgrid lines, no the default ones */

  gnuplot_cmd(h,"set noxtics");
  gnuplot_cmd(h,"set nolabel");


/* Plot the grid lines with arrows and labels*/

   if ((f==1) && ( lags > 9 ))
	{
       	int gap;
       	gap = iround(lags/3);
       	gnuplot_cmd(h,"set arrow 1 from %d,-%.2f to %d,%.2f nohead lt 1", gap , cmax, gap , cmax );
       	gnuplot_cmd(h,"set arrow 2 from %d,-%.2f to %d,%.2f nohead lt 1", 2*gap , cmax, 2*gap , cmax );
       	gnuplot_cmd(h,"set arrow 3 from %d,-%.2f to %d,%.2f nohead lt 1", 3*gap , cmax, 3*gap  , cmax );
 	gnuplot_cmd(h,"set label 1 '%d' at %d, -%.2f center ", gap, gap, cmax+0.08 );
      	gnuplot_cmd(h,"set label 2 '%d' at %d, -%.2f center ", 2*gap, 2*gap, cmax+0.08 );
      	gnuplot_cmd(h,"set label 3 '%d' at %d, -%.2f center ", 3*gap, 3*gap,cmax+0.08 );
	}
   else if ((f==1) && (lags <= 9))
     {
      gnuplot_cmd(h,"set arrow 1 from %d,-%.2f to %d,%.2f nohead lt 1", 3, cmax, 3, cmax );
      gnuplot_cmd(h,"set arrow 2 from %d,-%.2f to %d,%.2f nohead lt 1", 6, cmax, 6, cmax );
      if (lags == 9) gnuplot_cmd(h,"set arrow 3 from %d,-%.2f to %d,%.2f nohead lt 1", 9, cmax, 9, cmax );
      gnuplot_cmd(h,"set label 1 '%d' at %d, -%.2f center",3,3, cmax+0.08 );
      gnuplot_cmd(h,"set label 2 '%d' at %d, -%.2f center",6,6, cmax+0.08 );
      if (lags == 9) gnuplot_cmd(h,"set label 3 '%d' at %d, -%.2f center",9,9,cmax+0.08 );
	}
   else
     {
      gnuplot_cmd(h,"set arrow 1 from %d,-%.2f to %d,%.2f nohead lt 1", f, cmax, f, cmax );
      if (lags >= 2*f )gnuplot_cmd(h,"set arrow 2 from %d,-%.2f to %d,%.2f nohead lt 1", f+f, cmax, f+f, cmax );
      if (lags >= 3*f )gnuplot_cmd(h,"set arrow 3 from %d,-%.2f to %d,%.2f nohead lt 1", f+f+f, cmax, f+f+f, cmax );
      gnuplot_cmd(h,"set label 1 '%d' at %d, -%.2f center",f, f, cmax+0.08 );
      if (lags >= 2*f )gnuplot_cmd(h,"set label 2 '%d' at %d, -%.2f center",f+f, f+f, cmax+0.08 );
      if (lags >= 3*f )gnuplot_cmd(h,"set label 3 '%d' at %d, -%.2f center",f+f+f, f+f+f, cmax+0.08 );
     }


/* Allocate the ytics and labels */

 gnuplot_cmd(h,"set ytics nomirror %.2f", cmax/2 );
 gnuplot_cmd(h,"set mytics  1" );
 gnuplot_cmd(h,"set ytics out");

/* Allocate lines for grid and standard deviation lines */

  gnuplot_cmd(h,"set style line 1 lt 2 lw 1.2");
  gnuplot_cmd(h,"set style line 2 lt 1 lw 2.8"); 

/* Allocate style for impulses */

  if( lags >=  30 )
  gnuplot_cmd(h,"set style line 5 lt 1 lw 7"); 
  else
  gnuplot_cmd(h,"set style line 5 lt 1 lw 9"); 


/* Allocate the Ljung-Box statistic */


  if((f>4) || (f==1) && (n>200))  
  gnuplot_cmd(h,"set xlabel 'Q( %d ) = %2.1f' offset -4.1,-2.1 font 'Arial,17'", lags - npar, ChiTest( corr, lags, n ));
  else
  gnuplot_cmd(h,"set xlabel 'Q( %d ) = %2.1f' offset -1.4,-1.1 font 'Arial,15'", lags - npar, ChiTest( corr, lags, n ));

    gnuplot_plot_acf(h, corr, lags, n, cmax, "");

/*******************************************************************************/
  //   Pacf_New (ser->data ,   ser->nobs,  corr,  lags);
   Pacf( lags, corr );


  	if( lags >= 30 ) gnuplot_cmd(h,"set size 0.63,0.385");
  	else if (( lags < 30 )&&( lags >= 15)) gnuplot_cmd(h,"set size 0.423,0.37");
  	else gnuplot_cmd(h,"set size 0.383,0.37");


        if ( size == 2 ) gnuplot_cmd(h,"set origin 1.395,.084");
	else gnuplot_cmd(h,"set origin 0.83,-.008");


  if( lags >= 30 )
  gnuplot_cmd(h,"set title 'pacf' offset -2,-.21");
  else
  gnuplot_cmd(h,"set title 'pacf' offset -1.9,-.21");

  gnuplot_setstyle(h,"impulses");
  gnuplot_cmd(h,"set border 2 lw 1.6");


  gnuplot_cmd(h,"set xlabel ''  offset 0,0");
    
  gnuplot_plot_acf(h, corr, lags, n, cmax, "");

    gnuplot_close(h) ;

    free_vector( corr, 1, lags );

}


void histogram (struct Tseries *ser, int nrdiff, int nadiff, char *x11out, char *seriesname)

{

   int  itmp1, itmp2, Atip1, Atip2, NumCat, i, j, nphor, fmax,  *chk;
   real fmax1, rtmp1, rtmp2, xmax, num, ObsPerFil, *breakk, *freqs, *data, *prob ;
   STRING *shist, *aux ;
   gnuplot_ctrl*h;
   h=gnuplot_init();
/* Allocate workspace and initialize:                                        */
   prob  = vector( 1,  50 );
   data  = vector( 1,  50 );
   freqs  = vector( 1, 50 );
   chk    = ivector( 1, 50 );
   breakk = vector( 1, 50 );


   Atip1 = 0;
   Atip2 = 0;

/* Find maximum absolute value of standardized series:                       */

   itmp1 = ser->max;
   itmp2 = ser->min;
   rtmp1 = ser->mean;
   rtmp2 = sqrt( ser->var );

   ser->skew = Skew( ser->data, ser->nobs );
   ser->kurt = Kurt( ser->data, ser->nobs );
   ser->jarquebera = JarqueBera (ser->skew, ser->kurt, ser->nobs );


   xmax = fabs( (ser->data[itmp1]-rtmp1) / rtmp2 );
   if ( fabs( (ser->data[itmp2]-rtmp1) / rtmp2 ) > xmax )
      xmax = fabs( (ser->data[itmp2]-rtmp1) / rtmp2 );

/* Set maximum absolute value to either 4.0 or 8.0:                          */
/*
   if ( xmax > 8.0 )
      {
      fprintf( outputv, "Warning: at least one observation above 8 sigmas\n" );
      goto h1;
      }
*/
   xmax = ( xmax <= 4.0 ) ? 4.0 : 8.0;

/* Number of horizontal characters per category:                             */

   if ( xmax == 4.0 )
      {
      nphor = 4;
      }
   else
      {
      nphor = 2;
      }

/* Create vector of "breakpoints":                                           */

   for ( i = 1; i <= 50; i++ ) breakk[i] = 0.0;
   NumCat = 1;
   breakk[NumCat] = -xmax + 0.5;              /* Note that "bandwidth" = 0.5 */
   while ( breakk[NumCat] < xmax )
      {
      NumCat += 1;
      breakk[NumCat] = breakk[NumCat-1] + 0.5;
      }

/* Create vector of frequencies and set number of outliers:                  */

   for ( i = 1; i <= 50; i++ ) freqs[i] = 0;

   for ( i = 1; i <= ser->nobs; i++ )
       {
       num = (ser->data[i]-rtmp1) / rtmp2;
       if ( num <= breakk[1] )
          freqs[1] += 1;
       else
          for ( j = 2; j <= NumCat; j++ )
              if ( (num > breakk[j-1]) && (num <= breakk[j]) ) freqs[j] += 1;
       if ( fabs( num ) >= 2.0 )
          Atip2 += 1;
       if ( fabs( num ) >= 1.0 )
          Atip1 += 1;
       }

/* Maximum frequency = maximum to draw vertically (fills NumFil-1 rows):     */

   fmax = freqs[1];
   for ( i = 2; i <= NumCat; i++ )
       if ( freqs[i] > fmax ) fmax = freqs[i];

/* Number of observations represented by one row of dots:                    */

   fmax1     = fmax;
/* ObsPerFil = fmax1 / (NumFil - 1); (const int Numfil = 17)                 */
   ObsPerFil = fmax1 / 16.0;

/* do the histogram */

for ( i =  1; i <=  NumCat; i++ )
       {
        prob[i] =  100*2*freqs[i]/ser->nobs;
        data[i] =  breakk[i] -.25;
       }


/*
    for ( i = 1; i <= NumCat; i++ )
	{
         fprintf( outputv, "%7.3f   %7.3f\n", data[i], prob[i] );
        }

*/

    gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 18");

    gnuplot_cmd(h, "set output \"hist_%s.eps\"", x11out) ;
//    gnuplot_cmd(h, "set output \"hist_d%dD%d%s.eps\"", nrdiff, nadiff, x11out) ;
//    gnuplot_cmd(h,"set title '%s' font 'bold,22'", seriesname);
    gnuplot_cmd(h,"set size 0.75,.75");
    gnuplot_cmd(h,"set title '%s'   font 'bold,22'", seriesname);
/*
       if ( (nrdiff == 0) && (nadiff == 0)){
	 gnuplot_cmd(h,"set title '%s'   font 'bold,22'", seriesname);}
       if ( (nrdiff == 1) && (nadiff == 0)){
	 gnuplot_cmd(h,"set title '{/Symbol \321}%s' font 'bold,22'", seriesname);}
       if ( (nrdiff == 2) && (nadiff == 0)){
	 gnuplot_cmd(h,"set title '{/Symbol \321}^{2}%s' font 'bold,22'", seriesname);}
       if ( (nrdiff == 0) && (nadiff == 1)){
	 gnuplot_cmd(h,"set title '{/Symbol \321}_{%d}%s' font 'bold,22'", ser->freq,  seriesname);}
       if ( (nrdiff == 1) && (nadiff == 1)){
	 gnuplot_cmd(h,"set title '{/Symbol \321}{/Symbol \321}_{%d}%s' font 'bold,22'", ser->freq, seriesname);}
       if ( (nrdiff == 2) && (nadiff == 1)){
	 gnuplot_cmd(h,"set title '{/Symbol \321}^{2}{/Symbol \321}_{%d}%s' font 'bold,22'", ser->freq, seriesname);}
    */
    gnuplot_cmd(h,"set border 3 lw 1.3");
    gnuplot_cmd(h,"set size square");
    gnuplot_cmd(h,"set style fill solid .15 ");

//    gnuplot_cmd(h,"set xtics nomirror 1" );
    gnuplot_cmd(h,"set ytics nomirror 10" );
    gnuplot_cmd(h,"set mytics  2 " );

       gnuplot_cmd(h,"set xtics nomirror 2");
       gnuplot_cmd(h,"set tics scale 1");
//       gnuplot_cmd(h,"set ticslevel  .4");
       gnuplot_cmd(h,"set mxtics 2");
    gnuplot_cmd(h,"set ylabel '\%' 0,0 font 'Arial,24'");
    gnuplot_cmd(h,"set tics out");
    gnuplot_cmd(h,"set grid xtics ytics mxtics mytics");
    gnuplot_cmd(h,"set xrange [-%f:%f]", xmax, xmax );
    gnuplot_cmd(h,"set xzeroaxis");
    gnuplot_setstyle(h,"boxes");
    gnuplot_cmd(h,"set xlabel 'S = %2.1f       K = %2.1f        JB = %2.1f' 0,0 font 'Arial,20'",  ser->skew, ser->kurt, ser->jarquebera);

    gnuplot_cmd (h,"Gauss(x,mu,sigma) = 1./(sigma*sqrt(2*pi)) * exp( -(x-mu)**2 / (2*sigma**2) )");

    gnuplot_cmd (h,"d2(x) =   100*Gauss(x,  0,  1.)"   );

    gnuplot_plot_histo (h,   data   , prob,  NumCat, "" ) ;

//    gnuplot_plot_hist gnuplot_plot_histo(h,  data, NumCat , "histograma") ;



    gnuplot_close(h);


   free_vector( breakk, 1, 50 );
   free_ivector( chk, 1, 50 );
   free_vector( freqs, 1, 50 );
   free_vector( data, 1, 50 ); 
   free_vector( prob, 1, 50 );
}

void graph_m_dt (double *y, int n, int nog, char *x11out, char *seriesname)
{
/* input  --> double *y = data vector */
/* input  --> int n = data number     */
/* input  --> int nog = number observation per group  */
int i, ng = n/nog; /* number  groups */
double AbsMax;
gnuplot_ctrl*h;
h=gnuplot_init();
double *ms  = vector( 1, ng  );
double *dts = vector( 1, ng  );

meandv(y,dts,ms,nog,ng);

AbsMax = 0.0;
for ( i = 1; i <= ng; i++ )
	{
        if ( fabs( ms[i]) >= AbsMax ) AbsMax =  fabs( ms[i]);
	if ( fabs( dts[i]) >= AbsMax ) AbsMax =  fabs( dts[i]);
	}
AbsMax += .1*AbsMax;

gnuplot_cmd(h, "set terminal postscript eps enhance 'Arial' 17");
gnuplot_cmd(h, "set output \"m_dt_%s.eps\"", x11out);
gnuplot_cmd(h,"set title '%s' font 'bold,22'", seriesname);
gnuplot_setstyle(h,"points");
gnuplot_cmd(h,"set pointsize .80");
gnuplot_cmd(h,"set xtics nomirror %.1f", AbsMax );
gnuplot_cmd(h,"set ytics nomirror %.1f", AbsMax );
gnuplot_cmd(h,"set tics out");
gnuplot_cmd(h,"set size square");
gnuplot_cmd(h,"set grid");
gnuplot_cmd(h,"set grid xtics lt 1");
gnuplot_cmd(h,"set grid ytics lt 1");
gnuplot_cmd(h,"set xlabel 'Mean' 0,0 font 'Arial,20'");
gnuplot_cmd(h,"set ylabel 'Standard-Deviation' 0,0 font 'Arial,20'");
/* Debug lines
for ( i = 1; i <= ng; i++ )
	{
fprintf( outputv, "%7.3f  %7.3f\n",  ms[i],  dts[i] );
        }
end debug lines */

gnuplot_plot_mdt(h, ms, dts, ng, "", AbsMax ) ;

gnuplot_close(h);
free_vector( ms, 1, ng );
free_vector( dts, 1, ng );
}

void meandv(double *y,double *dts, double *ms, int nog, int ng)
/* input  --> double *y = data vector */
/* output <-- double *dts = standarized standard deviation vector */
/* output <-- double *ms  = standarized means vector */
/* input  --> int nog = number observation per group */
/* input  --> int ng = number groups         */
{
int i=1,j,k,h=1;  //define variables de tipo entero
double dta,mea; //define variables de tipo double

double *bb = vector( 1, nog ); //define vectores double
double *m  = vector( 1, ng  );
double *dt = vector( 1, ng  );

while (i <= ng )//para todos los grupos
        {
        for( j=1; j<=nog; j++ )//coja "nog" observaciones y las deposite en "bb"
                {
                bb[j]=y[h];
                h=h+1;
                }
        mea   = Mean (bb, nog); //calcule la media de "bb"
        dta   = Stdev (bb, nog); //calcule la desviación típica de "bb"
        m[i]  = mea;//deposite la media del grupo "i" en la posición "i" del vector "m"
        dt[i] = dta;//deposite la desviación típica del grupo "i" en la posición "i" del vector "dt"
        i=i+1;
        }


for ( i = 1; i <= ng; i++ )
	{
         ms[i] = (m[i] - Mean( m, ng ))/ Stdev ( m, ng );
         dts[i] =(dt[i] - Mean( dt, ng ))/ Stdev ( dt, ng );
	}

free_vector( bb, 1, nog );
free_vector( m, 1, ng );
free_vector( dt, 1, ng );
}
