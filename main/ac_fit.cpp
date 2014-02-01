/*
 * ==================================================================
 *
 *       Filename:  ac_fit.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  29/01/2014 18:13:03
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  P. Di Giglio (), p.digiglio91@gmail.com
 *   Organization:  
 *
 * ==================================================================
 */

#include <stdlib.h>
#include <math.h>

#include "round.h"

#include "TF1.h"
/* serve per settare il titolo degli assi */
#include "TH1.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int
main ( int argc, char *argv[] ) {
	if ( argc != 3 ) {
		fprintf( stderr, "Bad line arguments\n" );
		exit( EXIT_FAILURE );
	}

	/* input data plot */
	TGraph *gr1 = new TGraphErrors ( (TString) argv[1] + ".dat", "%lg\t%lg\t%lg\n" );

	(*gStyle).SetOptFit();

	/* 3 = verde */
	(*gr1).SetMarkerColor(3);
	/* titolo del grafico */
	(*gr1).SetTitle( argv[1] );
	(*gr1).SetMarkerStyle(20);
	(*gr1).SetMarkerSize(0.5);
	/* imposto i titoli degli assi */
	(*gr1).GetXaxis()->SetTitle("t");
	(*gr1).GetYaxis()->SetTitle("auto-correlatore");
	(*gr1).GetXaxis()->CenterTitle();
	(*gr1).GetYaxis()->CenterTitle();
	
	TCanvas *c1 = new TCanvas("c1", "");
	(*c1).SetGridx();
	(*c1).SetGridy();

	/*
	** mi aspetto che la (s)carica del condensatore
	** avvenga secondo una legge esponenziale, quindi
	** la interpolo con una funzione esponenziale
	*/
	TF1 *fit = new TF1( "f1", "exp( - x / [0] )", 1., 12. );
	/* diamo un "aiutino" a ROOT */
	(*fit).SetParameter( 0, 2. );
	/* 2 = rosso */
	(*fit).SetLineColor(2);
	/* interpolo */
	(*gr1).Fit("f1", "QR");
	
	/* output-file name */
	FILE *stream = stdout;

	fprintf( stream, "#B\ttau\terr\tchi/DOF\n" );
	fprintf( stream, "%g\t", (double) atof( argv[2] ) );
	round( (long double) (*fit).GetParameter( 0 ), (long double) (*fit).GetParError(0), stream );
	fprintf( stream, "\t%g\n", (*fit).GetChisquare() / (*fit).GetNDF() );

	/* Assi, Punti, (linea) Continua */
	(*gr1).Draw("AP");
	(*c1).Print( (TString) argv[1] + ".png", "png");

	return 0;
}				/* ----------  end of function main  ---------- */
