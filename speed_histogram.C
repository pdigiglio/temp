#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX		3.
#define MIN		- MAX
#define NBIN	100

using namespace std;

double maxwell ( double *x, double *p ) {
	unsigned int n = 128;
	double kt = 2. * p[0] / ( 3. * n );

	double tmp = 1. / sqrt( 2. * TMath::Pi() * p[0] );

	return TMath::Exp( - x[0] * x[0] / ( 2. * p[0] )) * tmp; 
}

void speed_histogram ( TString input = "histo.dat" ) {
	/* dichiaro l'istogramma */
	TH1D *histo = new TH1D( "Dati", "Speed distribution (x axis)", NBIN, MIN, MAX);
	
	/* riempio l'istogramma */
	ifstream inFile ( input, ios::in );
	double data;
	while ( !inFile.eof() ) {
		inFile >> data;
		(*histo).Fill(data);
	}
	inFile.close();

	/* normalizzo l'istogramma */
//	(*histo).ComputeIntegral();
//	double integral = (*histo).Integral();
//	printf( "Integrale: %g\n", integral );

//	(*histo).Scale( 1. / (*histo).Integral() );

	printf( "Integrale: %g\n", (*histo).Integral() );

	(*gROOT).SetStyle("Plain");
	(*gStyle).SetPalette();
	(*gStyle).SetOptStat();
	(*gStyle).SetOptFit();
	
	/*INTERPOLAZIONE */
	TCanvas *c_1 = new TCanvas();
	/* funzione gaussiana (signal) */
	TF1 *fit = new TF1("maxwell", maxwell, MIN, MAX, 1 );
	(*fit).SetParameter( 0, 0.); //, 3000 );
	(*fit).SetParName( 0, "E_{c}" );
//	(*fit).SetParName( 1, "Norm." );

	/* 7 = ciano */
//	(*fit).SetLineColor(7);
	/* interpolo tra 3 e 7 */
//	(*histo).Fit("maxwell");
//	
//	/* funzione parabolica */
//	TF1 *back_func = new TF1("back_func", background, xMin, xMax, 3);
//	(*back_func).SetParameters(-1., 5., 4000.);
//	/* 6 = rosa */
//	(*back_func).SetLineColor(6);
//
//	/*
//	** l'argomento "+" serve
//	** per sovrascrivere i due fit e non
//	** creare due grafici diversi
//	*/
//	(*histo).Fit("back_func", "+");

//	(*histo).DrawNormalized();
	(*histo).Draw();
	(*c_1).Print( "histo.gif", "gif");
	(*c_1).Print( "histo.pdf", "pdf");
	
//	TCanvas *c_2 = new TCanvas();
//	/* array dei parametri della funzione somma */
//	double sumPar[6];
//	for (int i = 0; i < 6; i += 1) {
//		if ( i < 3 ) 
//			sumPar[i] = (*back_func).GetParameter(i);
//		else
//			/*
//			** in questo caso, devo prendere i 
//			** parametri 1, 2, 3; che sono i
//			** parametri i-3
//			*/
//			sumPar[i] = (*signal_func).GetParameter(i-3);
//	}
//
//	/* interpolo con la funzione somma */
//	TF1 * sum_func = new TF1("sum_func", sum, xMin, xMax, 6);
//	(*sum_func).SetParameters(sumPar);
//	/* 8 = verdone */
//	(*sum_func).SetLineColor(8);
//
//	(*histo).Fit("sum_func");
//	(*histo).Draw();
//	(*c_2).Print("fit_SumFunc_2.gif", "gif");
//
//	/* disegno la parabola di fondo */
//	/* setto i parametri */
//	double back_par[3];
//	for (int e = 0; e < 3; e += 1)
//		back_par[e] = (*sum_func).GetParameter(e);
//
//	(*back_func).SetParameters(back_par);
//	/* 9 = blu scuro */
//	(*back_func).SetLineColor(9);
//	(*back_func).Draw("same");
//	
//	/* CALCOLO CONTEGGI */
//
//	/* setto i parametri */
//	for (int d = 0; d < 3; d += 1)
//		(*signal_func).SetParameter(d,(*sum_func).GetParameter(d + 3));
//	
//	cout << "Numero conteggi: " <<
//		(*signal_func).Integral(xMin, xMax)	<< endl;
//

	exit(EXIT_SUCCESS);
}
