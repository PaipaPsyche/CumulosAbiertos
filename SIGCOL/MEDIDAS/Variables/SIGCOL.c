/***********************************************************************************/
/*                                                                                 */
/* Program:                                                                        */
/* ========                                                                        */
/*                                                                                 */
/* sigcol.c                                                                        */
/*                                                                                 */
/*                                                                                 */
/* Version:                                                                        */
/* ========                                                                        */
/*                                                                                 */
/* First version: January 1996                                                     */
/* Latest update: 2003-02-15                                                       */
/*                                                                                 */
/*                                                                                 */
/* Author:                                                                         */
/* =======                                                                         */
/*                                                                                 */
/* Program by: Stefan Spaennare                                                    */
/* Email:      stefans@astro.lu.se                                                 */
/* Web-page:   http://www.astro.lu.se/~stefans/aprog.html                          */
/*                                                                                 */
/*                                                                                 */
/* Purpose:                                                                        */
/* ========                                                                        */
/*                                                                                 */
/* The program calculates the usual mean and standard deviation together with a    */
/* more robust estimation of the mean and standard deviation from a column in a    */
/* text (ascii) table with numbers. The minimum and maximum values of the column   */
/* are also calculated.                                                            */
/*                                                                                 */
/*                                                                                 */
/* Algorithms:                                                                     */
/* ===========                                                                     */
/*                                                                                 */
/* The robust estimation of the mean and standard deviation is less sensitive to   */
/* askew distributions and outliers than the usual method. The robust estimation   */
/* also has the nice property to give the same result as the usual method if the   */
/* distribution is Normal (Gaussian). The calculation of the robust mean is based  */
/* on the median value (sextile 3) and the robust standard deviation is based on   */
/* the difference between sextile 5 and 1.                                         */
/*                                                                                 */
/*                                                                                 */
/* References:                                                                     */
/* ===========                                                                     */
/*                                                                                 */
/* Lennart Lindegren (Lund Observatory) private communication.                     */
/*                                                                                 */
/*                                                                                 */
/* Compiling:                                                                      */
/* ==========                                                                      */
/*                                                                                 */
/* &gt;gcc -O3 -o sigcol sigcol.c -lm                                                 */
/*                                                                                 */
/*                                                                                 */
/* Input data:                                                                     */
/* ===========                                                                     */
/*                                                                                 */
/* &gt;Usage: sigcol datafile maxcoli sigmacoli                                       */
/*                                                                                 */
/* datafile: Text (ascii) file table containing the column with numbers.           */
/* maxcoli: Maximum number of columns in the table.                                */
/* sigmacoli: Column number to calculate statistics from.                          */
/*                                                                                 */
/* Input variables ending with "i" are integers.                                   */
/* The column "sigmacoli" in the table must only contain numbers. Other columns    */
/* can also contain text (letters). The table must have the same number of         */
/* columns from the beginning to the end.                                          */
/*                                                                                 */
/*                                                                                 */
/*                                                                                 */
/***********************************************************************************/

/***********************************************************************************/
/*                                                                                 */
/* Notice:                                                                         */
/* =======                                                                         */
/*                                                                                 */
/* I make no warranties that this program is (1) free of error, (2) con-           */
/* sistent with any standard merchantability, or (3) meeting the require-          */
/* ments of a particular application.  This software shall not, partly or          */
/* as a whole, participate in a process, whose outcome can result in injury        */
/* to a person or loss of property. It is solely designed for analytical           */
/* work.  Permission to use, copy, and distribute is hereby granted without        */
/* fee, providing that the header above including this notice appears in           */
/* all copies.                                                                     */
/*                                                                                 */
/*                                                         Stefan Spaennare        */
/*                                                                                 */
/***********************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

double timetic=(double)(CLOCKS_PER_SEC);

double pi=3.14159265358979323;

void qqsort(d,m,n)
double *d;
int m;
int n;
{
  int l,r,s,i,j;

  double d2,temp;

  int sr[65];
  int sl[65];

  s=1;
  sl[s]=m;
  sr[s]=n;

  do {
     l=sl[s];
     r=sr[s];
     s--;
    do {
     i=l;
     j=r;
     d2=d[(l+r)/2];
     do {
        while (d[i] < d2) {
           i++;
        } /* while */
        while (d[j] > d2) {
           j--;
        } /* while */
        if (i <= j) {
           temp=d[i];
           d[i]=d[j];
           d[j]=temp;
           i++;
           j--;
        } /* if */
     } while (i <= j);
     if (i < r) {
        s++;
        sl[s]=i;
        sr[s]=r;
     } /* if */
     r=j;
    } while (l < r);
  } while (s > 0);

} /* qqsort */

void quantiles(x,n,q,nq)
double *x;
int n;
double *q;
int nq;
{
   int iq,i,i0;
   double p,x0,x1;

   if (n == 1) {
      for (iq=1; iq<=(nq-1); iq++) {
         q[iq]=x[1];
      } /* for iq */
   }
   else {

      qqsort(x,1,n);

      for (i=1; i<=(nq-1); i++) {

         p=(double)(n)*(double)(i)/(double)(nq);
         i0=(int)(p);

         if  (i0 == 0) {
            x0=x[1];
            x1=0.5*(x[1]+x[2]);
         }
         else if (i0 == (n-1)) {
            x0=0.5*(x[n-1]+x[n]);
            x1=x[n];
         }
         else {
            x0=0.5*(x[i0]+x[i0+1]);
            x1=0.5*(x[i0+1]+x[i0+2]);
         } /* if */

         q[i]=x0+(x1-x0)*(p-(double)(i0));

      } /* for i */

   } /* if */

} /* quantiles */


void rob_ave_std(x,n,ave,std)
double *x;
int n;
double *ave;
double *std;
{
   double q[7];

   if (n == 0) {

      *ave=0.0;
      *std=0.0;

   }
   else {

      quantiles(x,n,q,6);

      *ave=q[3];
      *std=(q[5]-q[1])/1.935;

   } /* if */

} /* rob_ave_std */

void ave_std(x,n,ave,std)
double *x;
int n;
double *ave;
double *std;
{

   int i;

   double mean,sigma,ssum,temp;

   if (n == 0) {

      *ave=0.0;
      *std=0.0;

   } /* if */

   if (n == 1) {

      *ave=x[1];
      *std=0.0;

   } /* if */

   if (n > 1) {

      mean=0.0;

      for (i=1; i<=n; i++) {
         mean=mean+x[i];
      } /* if */

      mean=mean/(double)(n);

      ssum=0.0;

      for (i=1; i<=n; i++) {
         temp=x[i]-mean;
         ssum=ssum+temp*temp;
      } /* if */

      sigma=sqrt(ssum/(double)(n-1));

      *ave=mean;
      *std=sigma;

   } /* if */

} /* ave_std */

int main(argc,argv)
int argc;
char *argv[];

{

   FILE *infile;

   int n,i,k,mcol,scol;

   double x,mean,sigma,max,min;
   double rmean,rsigma;

   double *v;

   char ss[257];

   clock_t time1,timediff1;

   if (argc != 4) {
      printf("Usage: %s datafile maxcoli sigmacoli\n",argv[0]);
      exit(0);
   } /* if */


   if ((infile = fopen(argv[1],"r")) == NULL) {
      printf("File %s not found.\n",argv[1]);
      exit(0);
   } /* if */

   mcol=atoi(argv[2]);
   scol=atoi(argv[3]);

   max=-1.0e30;
   min=1.0e30;
   k=0;
   i=1;

   while (fscanf(infile,"%s",ss) != EOF) {
      k++;
      if (k == scol) {
         x=atof(ss);
         if (x > max) {
             max=x;
         } /* if */
         if (x < min) {
             min=x;
         } /* if */
         i++;
      }
      if (k == mcol) {
         k=0;
      } /* if */
   } /* while */

   rewind(infile);

   n=i-1;

   v=(double *)calloc(n+1,sizeof(double));

   infile=fopen(argv[1],"r");

   k=0;
   i=1;

   while(fscanf(infile,"%s",ss) != EOF) {
      k++;
      if (k == scol) {
         x=atof(ss);
         v[i]=x;
         i++;
      } /* if */
      if (k == mcol) {
         k=0;
      } /* if */
   } /* while */

   time1=clock();

   ave_std(v,n,&mean,&sigma);

   rob_ave_std(v,n,&rmean,&rsigma);

   timediff1=clock()-time1;

   printf("ID               n           mean        sigma       rob_mean    rob_sigma");
   printf("            max            min\n");
   printf("%s %7d %14.3f %12.3f %14.3f %12.3f %14.3f %14.3f\n",
          argv[1],n,mean,sigma,rmean,rsigma,max,min);

   /* printf("\n%8.2f\n",(double)(timediff1)/timetic); */

} /* End */

