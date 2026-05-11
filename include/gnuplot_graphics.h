/***************************************************************************
 *   Copyright (C) 2009 by David Guerrero   *
 *   warriord@rocketmail.com   *
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

double Acf_Pacf_Max ( struct Tseries *ser,  int lags );

double SeriesMax ( double *a, int n );

int SeriesSize  ( int nobs, int freq );

void gnuplot_File_PlotSer ( struct Tseries *ser, int tsnobs, int tmornsop, int tsby, int nrdiff, int nadiff,  char *x11out, char *seriesname  );

void gnuplot_File_PlotSer_CorrSer ( struct Tseries *ser, int npar, int tsnobs, int tmornsop, int tsby, double boxlam, int nrdiff, int nadiff, int lags, double cbands,  char *x11out, char *seriesname );

void gnuplot_File_CorrSer ( struct Tseries *ser, int npar, int nrdiff, int nadiff, int lags, double cbands,  char *x11out, char *seriesname  );


void meandv(double *y,double *dts, double *ms, int nog, int ng);
void graph_m_dt (double *y, int n, int nog, char *x11out, char *seriesname);
void histogram (struct Tseries *ser, int nrdiff, int nadiff, char *x11out, char *seriesname);
