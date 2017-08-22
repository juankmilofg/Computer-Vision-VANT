// Copyright (c) 2008-2011, Guoshen Yu <yu@cmap.polytechnique.fr>
// Copyright (c) 2008-2011, Jean-Michel Morel <morel@cmla.ens-cachan.fr>
//
// WARNING: 
// This file implements an algorithm possibly linked to the patent
//
// Jean-Michel Morel and Guoshen Yu, Method and device for the invariant 
// affine recognition recognition of shapes (WO/2009/150361), patent pending. 
//
// This file is made available for the exclusive aim of serving as
// scientific tool to verify of the soundness and
// completeness of the algorithm description. Compilation,
// execution and redistribution of this file may violate exclusive
// patents rights in certain countries.
// The situation being different for every country and changing
// over time, it is your responsibility to determine which patent
// rights restrictions apply to you before you compile, use,
// modify, or redistribute this file. A patent lawyer is qualified
// to make this determination.
// If and only if they don't conflict with any patent terms, you
// can benefit from the following license terms attached to this
// file.
//
// This program is provided for scientific and educational only:
// you can use and/or modify it for these purposes, but you are
// not allowed to redistribute this work or derivative works in
// source or executable form. A license must be obtained from the
// patent right holders for any other use.
//
// 
//*----------------------------- demo_ASIFT  --------------------------------*/
// Detect corresponding points in two images with the ASIFT method. 

// Please report bugs and/or send comments to Guoshen Yu yu@cmap.polytechnique.fr
// 
// Reference: J.M. Morel and G.Yu, ASIFT: A New Framework for Fully Affine Invariant Image 
//            Comparison, SIAM Journal on Imaging Sciences, vol. 2, issue 2, pp. 438-469, 2009. 
// Reference: ASIFT online demo (You can try ASIFT with your own images online.) 
//			  http://www.ipol.im/pub/algo/my_affine_sift/
/*---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <cstring>

#include <time.h>
#include <sys/time.h>
#include <math.h> 
#include <vector>
using namespace std;

#ifdef _OPENMP
#include <omp.h>
#endif

#include "demo_lib_sift.h"
#include "io_png/io_png.h"

#include "library.h"
#include "frot.h"
#include "fproj.h"
#include "compute_asift_keypoints.h"
#include "compute_asift_matches.h"

# define IM_X 700
# define IM_Y 700

int main(int argc, char **argv)
{			
	//printf("argc %d \n",argc);
	//std::cerr << "argv[1] " << argv[1] << std::endl;

	
    if ((argc != 8) && (argc != 9) && (argc != 10)) {
        std::cerr << " ******************************************************************************* " << std::endl
				  << " ***************************  ASIFT image matching  **************************** " << std::endl
				  << " ******************************************************************************* " << std::endl
				  << "Usage: " << argv[0] << " imgIn1.png imgIn2.png imgOutVert.png imgOutHori.png " << std::endl
										  << "           matchings.txt keys1.txt keys2.txt [Resize option: 0/1] " << std::endl
									      << "- imgIn1.png, imgIn2.png: input images (in PNG format). " << std::endl
										  << "- imgOutVert.png, imgOutHori.png: output images (vertical/horizontal concatenated, " << std::endl
				                          << "  in PNG format.) The detected matchings are connected by write lines." << std::endl
										  << "- matchings.txt: coordinates of matched points (col1, row1, col2, row2). " << std::endl
										  << "- keys1.txt keys2.txt: ASIFT keypoints of the two images." << std::endl
										  << "- [optional 0/1]. 1: input images resize to 800x600 (default). 0: no resize. " << std::endl 
   				  << " ******************************************************************************* " << std::endl
				  << " *********************  Jean-Michel Morel, Guoshen Yu, 2010 ******************** " << std::endl
				  << " ******************************************************************************* " << std::endl;
        return 1;
    }
	
	//////////////////////////////////////////////// Input

	int pruebapersp,pruebaimagem,metodos,blur=0, teste_mosaico=9;
for (teste_mosaico=5;teste_mosaico<6;teste_mosaico++) //10
{ 
  	std::string  mosaico_tex = "";
        switch (teste_mosaico)
	{
         case 0:
            	mosaico_tex += "1";
		break;
         case 1:
                mosaico_tex += "2";
            break;
	 case 2:
                mosaico_tex += "google_0";
            break;
	 case 3:
            	mosaico_tex += "google_1";
            break;
	 case 4:
            	mosaico_tex += "google_2";
            break;
	 case 5:
            	mosaico_tex += "casas";
            break;
	 case 6:
            	mosaico_tex += "mato1";
            break;
	 case 7:
            	mosaico_tex += "mato2";
            break;
	case 8:
            	mosaico_tex += "rio";
            break;
	case 9:
            	mosaico_tex += "rodovia";
            break;
	}
	
	std::string mosaic = "";
	mosaic += "transformadas/test_mosaico_" + mosaico_tex + ".txt";

        char *mosaico_texto = new char[mosaic.length() + 1];
	std::strcpy(mosaico_texto, mosaic.c_str());
        //printf("%s \n",mosaico_texto);
	std::ofstream file_keyteste(mosaico_texto);
	
		
for (blur=0;blur<1;blur++) // 0:blur 1:Normal  //2
{
  //for(pruebapersp=18;pruebapersp<19;pruebapersp++)  //21
  //{
  pruebapersp=0;
      for(pruebaimagem=14;pruebaimagem<16;pruebaimagem++) //16
	  {
	
	std::string mosaico1texto1 ("imagenes_entrada/transformadas/mosaicos/mosaico_");
        mosaico1texto1 += mosaico_tex + ".png";
        char *mosaico1texto = new char[mosaico1texto1.length() + 1]; // or
        std::strcpy(mosaico1texto, mosaico1texto1.c_str());
        
	// Read image1
	float * iarr1;
        size_t w1, h1;
        //mosaico1texto="imagenes_entrada/prueba1territorio.png";
	//printf("ca asdca sdad");
	//printf("%c",mosaico1texto);
        if (NULL == (iarr1 = read_png_f32_gray(mosaico1texto, &w1, &h1))) {
        //if (NULL == (iarr1 = read_png_f32_gray("imagenes_entrada/prueba1territorio.png", &w1, &h1))) {
        std::cerr << "Unable to load image file " << argv[1] << std::endl;
        return 1;
        }
        std::vector<float> ipixels1(iarr1, iarr1 + w1 * h1);
    
	free(mosaico1texto);
	free(iarr1); /*memcheck*/
	mosaic.clear();
        mosaico1texto1.clear();
	

	    
	std::string imagen2texto1 ("imagenes_entrada/transformadas/data_");
	imagen2texto1 += mosaico_tex + "/total_imagens/";
	std::string numero1 = "";
	numero1 = static_cast<std::ostringstream*>(&(std::ostringstream() << pruebapersp))->str();
	std::string numero2 = "";
	numero2 = static_cast<std::ostringstream*>(&(std::ostringstream() << pruebaimagem))->str();
        if(blur == 0)
	   imagen2texto1 += "blur_prova_" + numero1+ "_" + numero2+ ".png"; 
	else
	   imagen2texto1 += "prova_" + numero1+ "_" + numero2+ ".png"; 
	      
	char *imagem2 = new char[imagen2texto1.length() + 1]; // or
        std::strcpy(imagem2, imagen2texto1.c_str());
	           
        printf("imagen: %s \n",imagem2);
    
        float * iarr2;
        size_t w2, h2;
	
        if (NULL == (iarr2 = read_png_f32_gray(imagem2, &w2, &h2))) {
	//if (NULL == (iarr2 = read_png_f32_gray("imagenes_entrada/prueba1territorio.png", &w2, &h2))) {
            std::cerr << "Unable to load image file " << argv[2] << std::endl;
        return 1;
        }
        
        std::vector<float> ipixels2(iarr2, iarr2 + w2 * h2);
     
	free(iarr2); /*memcheck*/	
	imagen2texto1.clear();
	numero1.clear();
	numero2.clear();

	      	      
	for(metodos=0;metodos<2;metodos++) // 0:sift-orsa  1:asift-orsa  2:sift-Ransac  3::asift-Ransac
	 {

	
	///// Resize the images to area wS*hW in remaining the apsect-ratio	
	///// Resize if the resize flag is not set or if the flag is set unequal to 0
	float wS = IM_X;
	float hS = IM_Y;
	
	float zoom1=0, zoom2=0;	
	int wS1=0, hS1=0, wS2=0, hS2=0;
	vector<float> ipixels1_zoom, ipixels2_zoom;	
		
	int flag_resize = 1;
	if (argc == 9)
	{	
		flag_resize = atoi(argv[8]);
	}
	
	if ((argc == 8) || (flag_resize != 0))
	{
		cout << "WARNING: The input images are resized to " << wS << "x" << hS << " for ASIFT. " << endl 
		<< "         But the results will be normalized to the original image size." << endl << endl;
		
		float InitSigma_aa = 1.6;
		float fproj_p, fproj_bg;
		char fproj_i;
		float *fproj_x4, *fproj_y4;
		int fproj_o;

		fproj_o = 3;
		fproj_p = 0;
		fproj_i = 0;
		fproj_bg = 0;
		fproj_x4 = 0;
		fproj_y4 = 0;
				
		float areaS = wS * hS;
		
		// Resize image 1 
		float area1 = w1 * h1;
		zoom1 = sqrt(area1/areaS);
		
		wS1 = (int) (w1 / zoom1);
		hS1 = (int) (h1 / zoom1);
				
		int fproj_sx = wS1;
		int fproj_sy = hS1;     
		
		printf("areaS: %f wS: %f hS: %f \n \n" ,areaS,wS, hS);
		printf("area1: %f w1: %d h1: %d \n" ,area1,w1, h1);
		printf("zoom1: %f \n" ,zoom1);

		float fproj_x1 = 0;
		float fproj_y1 = 0;
		float fproj_x2 = wS1;
		float fproj_y2 = 0;
		float fproj_x3 = 0;	     
		float fproj_y3 = hS1;
			
		/* Anti-aliasing filtering along vertical direction */
		if ( zoom1 > 1 )
		{
			float sigma_aa = InitSigma_aa * zoom1 / 2;
			GaussianBlur1D(ipixels1,w1,h1,sigma_aa,1);
			GaussianBlur1D(ipixels1,w1,h1,sigma_aa,0);
		}
			
		// simulate a tilt: subsample the image along the vertical axis by a factor of t.
		ipixels1_zoom.resize(wS1*hS1);
		fproj (ipixels1, ipixels1_zoom, w1, h1, &fproj_sx, &fproj_sy, &fproj_bg, &fproj_o, &fproj_p, 
			   &fproj_i , fproj_x1 , fproj_y1 , fproj_x2 , fproj_y2 , fproj_x3 , fproj_y3, fproj_x4, fproj_y4); 
		
			
		// Resize image 2 
		float area2 = w2 * h2;
		zoom2 = sqrt(area2/areaS);
				
		wS2 = (int) (w2 / zoom2);
		hS2 = (int) (h2 / zoom2);
		
		fproj_sx = wS2;
		fproj_sy = hS2;     
		
		fproj_x2 = wS2;
		fproj_y3 = hS2;
		
		printf("area2: %f w2: %d h2: %d \n" ,area2,w2, h2);
		printf("zoom2: %f \n \n" ,zoom2);
		
		/* Anti-aliasing filtering along vertical direction */
		if ( zoom1 > 1 )
		{
			float sigma_aa = InitSigma_aa * zoom2 / 2;
			GaussianBlur1D(ipixels2,w2,h2,sigma_aa,1);   //vertica
			GaussianBlur1D(ipixels2,w2,h2,sigma_aa,0);   //horizontal
		}
			
		// simulate a tilt: subsample the image along the vertical axis by a factor of t.
		ipixels2_zoom.resize(wS2*hS2);
		fproj (ipixels2, ipixels2_zoom, w2, h2, &fproj_sx, &fproj_sy, &fproj_bg, &fproj_o, &fproj_p, 
			   &fproj_i , fproj_x1 , fproj_y1 , fproj_x2 , fproj_y2 , fproj_x3 , fproj_y3, fproj_x4, fproj_y4); 
		
		//InitSigma_aa='\O';  
                //fproj_p, fproj_bg='\O'; fproj_i='\O';
                // *fproj_x4='\O'; *fproj_y4='\O'; fproj_o='\O';
                //areaS='\O'; area1='\O';
	}
	else 
	{
		ipixels1_zoom.resize(w1*h1);	
		ipixels1_zoom = ipixels1;
		wS1 = w1;
		hS1 = h1;
		zoom1 = 1;
		
		ipixels2_zoom.resize(w2*h2);	
		ipixels2_zoom = ipixels2;
		wS2 = w2;
		hS2 = h2;
		zoom2 = 1;
	}
	
	int wSS = wS;
	int hSS = hS;

	float *img_zoom1 = new float[wSS*hSS];
	float *img_zomm2 = new float[wSS*hSS];

	for(int j = 0; j < (int) hSS; j++)
		for(int i = 0; i < (int) wSS; i++)  img_zoom1[j*i] = ipixels1_zoom[j*i];
		
	for(int j = 0; j < (int) hSS; j++)
		for(int i = 0; i < (int) wSS; i++)  img_zomm2[j*i] = ipixels2_zoom[j*i];
	  
	write_png_f32(argv[8], img_zoom1, wS, hS, 1);
	write_png_f32(argv[9], img_zomm2, wS, hS, 1);
	
	///// Compute ASIFT keypoints
	// number N of tilts to simulate t = 1, \sqrt{2}, (\sqrt{2})^2, ..., {\sqrt{2}}^(N-1)
	int num_of_tilts1;
	int num_of_tilts2;
	int filtro;
	switch ( metodos )
      {
         case 0:
            	num_of_tilts1 = 1;
	        num_of_tilts2 = 1;
		filtro = 2;
            break;
         case 1:
                num_of_tilts1 = 3;
	        num_of_tilts2 = 6;
		filtro = 2;
            break;
	 case 2:
                num_of_tilts1 = 1;
	        num_of_tilts2 = 1;
		filtro = 1;
            break;
	 case 3:
            	num_of_tilts1 = 3;
	        num_of_tilts2 = 6;
		filtro = 1;
            break;
         default:
                num_of_tilts1 = 3;
	        num_of_tilts2 = 6;
      }

//	int num_of_tilts1 = 1;
//	int num_of_tilts2 = 1;
	int verb = 0;
	printf("tilts1: %d \n" ,num_of_tilts1);
	printf("tilts2: %d \n \n" ,num_of_tilts2);
	
	// Define the SIFT parameters
	siftPar siftparameters;	
	default_sift_parameters(siftparameters);

	vector< vector< keypointslist > > keys1;		
	vector< vector< keypointslist > > keys2;	
	
	int num_keys1=0, num_keys2=0;
	struct timeval start, end;
	long mtime, seconds, useconds; 
	
//	cout << "Computing keypoints on the two images..." << endl;
		
	cout << "Computing keypoints on the image 1..." << endl;
	gettimeofday(&start, NULL);
	num_keys1 = compute_asift_keypoints(ipixels1_zoom, wS1, hS1, num_of_tilts1, verb, keys1, siftparameters);
	//num_keys1 = compute_sift_keypoints(ipixels1_zoom,keys1, wS1, hS1,siftparameters);
	gettimeofday(&end, NULL);
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
        cout << "Keypoints image 1 computation accomplished in " << mtime << " milliseconds. \n" << endl;
        std::ofstream file_keyteste(mosaico_texto,ios::app);
	file_keyteste <<"Imagem1;"<<  num_keys1 << ";"<< mtime << ";";          

	cout << "Computing keypoints on the image 2..." << endl;
	gettimeofday(&start, NULL);
	num_keys2 = compute_asift_keypoints(ipixels2_zoom, wS2, hS2, num_of_tilts2, verb, keys2, siftparameters);
	//num_keys2 = compute_sift_keypoints(ipixels2_zoom,keys2,wS2, hS2,siftparameters);
	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	cout << "Keypoints image 2 computation accomplished in " << mtime << " milliseconds. \n" << endl;
	file_keyteste <<"Imagem2;"<<  num_keys2 << ";"<< mtime << ";";         
//	cout << "Keypoints image 2 computation accomplished in " << difftime(tend, tstart) << " seconds." << endl;
	
	//// Match ASIFT keypoints
	int num_matchings;
	matchingslist matchings;	
	cout << "Matching the keypoints..." << endl;
	gettimeofday(&start, NULL);
	num_matchings = compute_asift_matches(num_of_tilts1, num_of_tilts2, wS1, hS1, wS2, 
										  hS2, filtro, keys1, keys2, matchings, siftparameters);
	gettimeofday(&end, NULL);
	seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
	cout << "\nKeypoints matching accomplished in " <<  mtime << " milliseconds. \n" << endl;
	file_keyteste <<"matches;"<<  num_matchings << ";"<< mtime << ";";          
	
	//accuracy dos pontos casados
	int x_0,y_0;
	double sizeancho,sizealtura;
	int xcasado,ycasado,difx,dify,tot;
	double Homografica[2][3] ;
	
	
	 switch (pruebapersp)
      {
         case 0: // rotacao=0, lomgitude=0, latitude=0, traslacao=0, 0
	        //size_ut = cvSize(800,800);
	        sizeancho=  1;//800/800 ;
	        sizealtura= 1;//800/800 ;
            	Homografica[0][0] = 1;
		Homografica[0][1] = 0;
		Homografica[0][2] = 0;
		Homografica[1][0] = 0;
		Homografica[1][1] = 1;
		Homografica[1][2] = 0;
		
		switch (pruebaimagem)
                    {
		  case 0:
		      x_0=0; 
		      y_0=0;
		      break;
		  case 1:
		      x_0=0; 
		      y_0=199;
		      break;
		  case 2:
		      x_0=0; 
		      y_0=399;
		      break;
		  case 3:
		      x_0=0; 
		      y_0=599;
		      break;
		  case 4:
		      x_0=199; 
		      y_0=0;
		      break;
		  case 5:
		      x_0=199; 
		      y_0=199;
		      break;
		  case 6:
		      x_0=199; 
		      y_0=399;
		      break;
		  case 7:
		      x_0=199; 
		      y_0=599;
		      break;
		  case 8:
		      x_0=399; 
		      y_0=0;
		      break;
		  case 9:
		      x_0=399; 
		      y_0=199;
		      break;
		  case 10:
		      x_0=399; 
		      y_0=399;
		      break;
		  case 11:
		      x_0=399; 
		      y_0=599;
		      break;
		  case 12:
		      x_0=599; 
		      y_0=0;
		      break;
		  case 13:
		      x_0=599; 
		      y_0=199;
		      break;
		  case 14:
		      x_0=599; 
		      y_0=399;
		      break;
		  case 15:
		      x_0=0; 
		      y_0=599;
		      break;
		    }    
		      
	break;
         case 1:  // rotacao=15, lomgitude=5, latitude=0, traslacao=50, 0
		//size_ut = cvSize(1037,1028);
		sizeancho= 0.771456;//800/1037;
	        sizealtura=0.778210;//800/1028;
                Homografica[0][0] = 0.9396926;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3420201;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;

		switch (pruebaimagem)
                    {
		  case 0:
		      x_0=194; 
		      y_0=74;  
		      break;
		  case 1:
		      x_0=148; 
		      y_0=192; 
		      break;
		  case 2:
		      x_0=273; 
		      y_0=192;  
		      break;
		  case 3:
		      x_0=389; 
		      y_0=192;  
		      break;
		  case 4:
		      x_0=522; 
		      y_0=192;  
		      break;
		  case 5:
		      x_0=102; 
		      y_0=314;  
		      break;
		  case 6:
		      x_0=229; 
		      y_0=314;  
		      break;
		  case 7:
		      x_0=356; 
		      y_0=314; 
		      break;
		  case 8:
		      x_0=483; 
		      y_0=314; 
		      break;
		  case 9:
		      x_0=399; 
		      y_0=199;
		      break;
		  case 10:
		      x_0=399; 
		      y_0=399; 
		      break;
		  case 11:
		      x_0=406; 
		      y_0=524; 
		      break;
		  case 12:
		      x_0=70;  
		      y_0=405; 
		      break;
		  case 13:
		      x_0=197; 
		      y_0=405; 
		      break;
		  case 14:
		      x_0=324; 
		      y_0=405; 
		      break;
		  case 15:
		      x_0=451; 
		      y_0=405;
		      break;
		    }    
		
		break;
		
	 /*case 2:  // rotacao=15, lomgitude=5, latitude=4, traslacao=50, 0
		size_ut = cvSize(1039,1029);
		sizeancho= 0.771456;//800/1037;
	        sizealtura=0.778210;//800/1028;
                Homografica[0][0] = 0.9419873;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3428553;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=192; x_1=148; x_2=273;  x_3=389;  x_4=522;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=406; x_12=70;  x_13=197; x_14=324; x_15=451;
                y_0=68;  y_1=192; y_2=192;  y_3=192;  y_4=192;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=524; y_12=405; y_13=405; y_14=405; y_15=405;
            break;
	 case 3:  // rotacao=3, lomgitude=3, latitude=8, traslacao=50, 0
		size_ut = cvSize(1041,1031);
                Homografica[0][0] = 0.9489275;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3453814;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=192; x_1=148; x_2=273;  x_3=389;  x_4=522;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=406; x_12=70;  x_13=197; x_14=324; x_15=451;
                y_0=68;  y_1=192; y_2=192;  y_3=192;  y_4=192;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=524; y_12=405; y_13=405; y_14=405; y_15=405;
             break;
	    case 4:  // rotacao=3, lomgitude=3, latitude=12, traslacao=50, 0
		size_ut = cvSize(1049,1036);
                Homografica[0][0] = 0.9606859;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3496611;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=190; x_1=148; x_2=273;  x_3=389;  x_4=522;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=406; x_12=70;  x_13=197; x_14=324; x_15=451;
                y_0=69;  y_1=192; y_2=192;  y_3=192;  y_4=192;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=524; y_12=405; y_13=405; y_14=405; y_15=405;
            break;*/
	 
	       case 5:  // rotacao=3, lomgitude=3, latitude=16, traslacao=50, 0
		//size_ut = cvSize(1062,1040);
		sizeancho= 0.753295;//800/1062;
	        sizealtura=0.768491;//800/1041;
                Homografica[0][0] = 0.9775617;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3558034;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		
		switch (pruebaimagem)
                    {
		  case 0:
		      x_0=190; 
		      y_0=69;   
		      break;
		  case 1:
		      x_0=148; 
		      y_0=192;  
		      break;
		  case 2:
		      x_0=273;  
		      y_0=192;   
		      break;
		  case 3:
		      x_0=389;  
		      y_0=192;   
		      break;
		  case 4:
		      x_0=522;   
		      y_0=192;  
		      break;
		  case 5:
		      x_0=102;  
		      y_0=314;   
		      break;
		  case 6:
		      x_0=229;  
		      y_0=314;     
		      break;
		  case 7:
		      x_0=356;  
		      y_0=314;  
		      break;
		  case 8:
		      x_0=483; 
		      y_0=314; 
		      break;
		  case 9:
		      x_0=399; 
		      y_0=199; 
		      break;
		  case 10:
		      x_0=399; 
		      y_0=399; 
		      break;
		  case 11:
		      x_0=406; 
		      y_0=524; 
		      break;
		  case 12:
		      x_0=70; 
		      y_0=405; 
		      break;
		  case 13:
		      x_0=197;  
		      y_0=405; 
		      break;
		  case 14:
		      x_0=324; 
		      y_0=405; 
		      break;
		  case 15:
		      x_0=451;   
		      y_0=405; 
		      break;
		    }   
		
            break;
	    
	       /*case 6:  // rotacao=3, lomgitude=3, latitude=20, traslacao=50, 0
		size_ut = cvSize(1083,1047);
                Homografica[0][0] = 1;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3639702;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=184; x_1=148; x_2=273;  x_3=389;  x_4=522;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=406; x_12=70;  x_13=197; x_14=324; x_15=451;
                y_0=68;  y_1=192; y_2=192;  y_3=192;  y_4=192;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=524; y_12=400; y_13=405; y_14=405; y_15=405;
             break;
	       case 7:  // rotacao=3, lomgitude=3, latitude=24, traslacao=50, 0
		size_ut = cvSize(1105,1058);
                Homografica[0][0] = 1.0286216;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3743877;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=184; x_1=148; x_2=273;  x_3=389;  x_4=522;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=406; x_12=75;  x_13=197; x_14=324; x_15=451;
                y_0=68;  y_1=192; y_2=192;  y_3=192;  y_4=192;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=521; y_12=395; y_13=405; y_14=405; y_15=405;
             break;
	       case 8:  // rotacao=3, lomgitude=3, latitude=28, traslacao=50, 0
		size_ut = cvSize(1135,1066);
                Homografica[0][0] = 1.0642677;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.3873618;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=176; x_1=148; x_2=273;  x_3=389;  x_4=520;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=408; x_12=77;  x_13=197; x_14=324; x_15=451;
                y_0=68;  y_1=192; y_2=192;  y_3=192;  y_4=201;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=521; y_12=390; y_13=405; y_14=405; y_15=405;
             break;*/
	      
	       case 9:  // rotacao=3, lomgitude=3, latitude=32, traslacao=50, 0
		//size_ut = cvSize(1171,1079);
		sizeancho= 0.683176;//800/1171;
	        sizealtura= 0.741427;//800/1079;
                Homografica[0][0] = 1.1080652;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.4033028;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		
				switch (pruebaimagem)
                    {
		  case 0:
		      x_0=176; 
		      y_0=68;  
		      break;
		  case 1:
		      x_0=148; 
		      y_0=192; 
		      break;
		  case 2:
		      x_0=273;  
		      y_0=192;  
		      break;
		  case 3:
		      x_0=389;  
		      y_0=192; 
		      break;
		  case 4:
		      x_0=520;
		      y_0=201;  
		      break;
		  case 5:
		      x_0=102;  
		      y_0=314;  
		      break;
		  case 6:
		      x_0=229;  
		      y_0=314;
		      break;
		  case 7:
		      x_0=356; 
		      y_0=314; 
		      break;
		  case 8:
		      x_0=483; 
		      y_0=314; 
		      break;
		  case 9:
		      x_0=399; 
		      y_0=199; 
		      break;
		  case 10:
		      x_0=399;
		      y_0=399; 
		      break;
		  case 11:
		      x_0=408; 
		      y_0=517; 
		      break;
		  case 12:
		      x_0=77; 
		      y_0=387; 
		      break;
		  case 13:
		      x_0=197; 
		      y_0=405;
		      break;
		  case 14:
		      x_0=324; 
		      y_0=405; 
		      break;
		  case 15:
		      x_0=451; 
		      y_0=405;

		      break;
		    } 
                
                break;
	     /*  case 10:  // rotacao=3, lomgitude=3, latitude=36, traslacao=50, 0
		size_ut = cvSize(1214,1095);
                Homografica[0][0] = 1.1615240;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.4227601;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
		x_0=160; x_1=148; x_2=273;  x_3=389;  x_4=505;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=408; x_12=77;  x_13=197; x_14=324; x_15=451;
                y_0=75;  y_1=192; y_2=192;  y_3=192;  y_4=210;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=510; y_12=380; y_13=405; y_14=405; y_15=405;
             break;
	       case 11:  // rotacao=3, lomgitude=3, latitude=40, traslacao=50, 0
		size_ut = cvSize(1268,1113);
                Homografica[0][0] = 1.2266816;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.4464756;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=160; x_1=148; x_2=273;  x_3=389;  x_4=498;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=408; x_12=77;  x_13=197; x_14=324; x_15=451;
                y_0=75;  y_1=192; y_2=192;  y_3=192;  y_4=215;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=510; y_12=370; y_13=405; y_14=405; y_15=405;
             break;
	       case 12:  // rotacao=3, lomgitude=3, latitude=44, traslacao=50, 0
		size_ut = cvSize(1330,1140);
                Homografica[0][0] = 1.306326;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.475464;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=160; x_1=148; x_2=273;  x_3=389;  x_4=498;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=399; x_10=399; x_11=418; x_12=87;  x_13=197; x_14=324; x_15=451;
                y_0=75;  y_1=192; y_2=192;  y_3=192;  y_4=215;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=199; y_10=399; y_11=503; y_12=363; y_13=405; y_14=405; y_15=405;
             break;*/
	     
	       case 13:  // rotacao=3, lomgitude=3, latitude=48, traslacao=50, 0
		//size_ut = cvSize(1410,1165);
		sizeancho= 0.567375;//800/1410;
	        sizealtura= 0.686695;//800/1165;
                Homografica[0][0] = 1.4043486;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.5111411;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
                
                switch (pruebaimagem)
                    {
		  case 0:
		      x_0=145;  
		      y_0=80;   
		      break;
		  case 1:
		      x_0=148; 
		      y_0=192; 
		      break;
		  case 2:
		      x_0=273;   
		      y_0=192;   
		      break;
		  case 3:
		      x_0=389;    
		      y_0=192;    
		      break;
		  case 4:
		      x_0=470;  
		      y_0=225;    
		      break;
		  case 5:
		      x_0=102;   
		      y_0=314;    
		      break;
		  case 6:
		      x_0=229;   
		      y_0=314;  
		      break;
		  case 7:
		      x_0=356; 
		      y_0=314; 
		      break;
		  case 8:
		      x_0=483; 
		      y_0=314; 
		      break;
		  case 9:
		      x_0=399; 
		      y_0=199; 
		      break;
		  case 10:
		      x_0=399; 
		      y_0=399; 
		      break;
		  case 11:
		      x_0=418; 
		      y_0=503;  
		      break;
		  case 12:
		      x_0=87;   
		      y_0=353; 
		      break;
		  case 13:
		      x_0=197; 
		      y_0=400; 
		      break;
		  case 14:
		      x_0=324; 
		      y_0=405; 
		      break;
		  case 15:
		      x_0=451;  
		      y_0=405; 

		      break;
		    } 
            
	       break;
	     
	     /*  case 14:  // rotacao=3, lomgitude=3, latitude=52, traslacao=50, 0
		size_ut = cvSize(1510,1205);
                Homografica[0][0] = 1.5263138;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.5555328;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=132; x_1=148; x_2=273;  x_3=389;  x_4=465;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=390; x_10=399; x_11=418; x_12=87;  x_13=197; x_14=324; x_15=451;
                y_0=83;  y_1=192; y_2=192;  y_3=200;  y_4=235;  y_5=314;  y_6=314;  y_7=314; y_8=314; y_9=202; y_10=399; y_11=485; y_12=335; y_13=380; y_14=405; y_15=405;
             break;
	       case 15:  // rotacao=3, lomgitude=3, latitude=56, traslacao=50, 0
		size_ut = cvSize(1630,1248);
                Homografica[0][0] = 1.6804445;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.6116318;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=132; x_1=148; x_2=273;  x_3=370;  x_4=440;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=370; x_10=399; x_11=420; x_12=87;  x_13=197; x_14=324; x_15=451;
                y_0=83;  y_1=192; y_2=192;  y_3=207;  y_4=238;  y_5=320;  y_6=314;  y_7=314; y_8=314; y_9=210; y_10=399; y_11=480; y_12=320; y_13=375; y_14=405; y_15=405;
             break;
	       case 16:  // rotacao=3, lomgitude=3, latitude=60, traslacao=50, 0
		size_ut = cvSize(1790,1305);
                Homografica[0][0] = 1.8793852;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.6840403;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=100; x_1=148; x_2=273;  x_3=365;  x_4=440;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=370; x_10=399; x_11=420; x_12=97;  x_13=215; x_14=324; x_15=451;
                y_0=85;  y_1=192; y_2=192;  y_3=220;  y_4=257;  y_5=310;  y_6=314;  y_7=314; y_8=314; y_9=220; y_10=399; y_11=470; y_12=310; y_13=365; y_14=405; y_15=405;
             break;*/
	     
	       case 17:  // rotacao=3, lomgitude=3, latitude=64, traslacao=50, 0
		// = cvSize(2003,1380);
		sizeancho= 0.399400;//800/2003;
	        sizealtura= 0.579710;//800/1380;
                Homografica[0][0] = 2.1436005;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.7802068;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		
		switch (pruebaimagem)
                    {
		  case 0:
		      x_0=80; 
		      y_0=90;   
		      break;
		  case 1:
		      x_0=148;  
		      y_0=192; 
		      break;
		  case 2:
		      x_0=273;  
		      y_0=192;     
		      break;
		  case 3:
		      x_0=365;         
		      y_0=240;     
		      break;
		  case 4:
		      x_0=440;   
		      y_0=280;    
		      break;
		  case 5:
		      x_0=102;     
		      y_0=290;   
		      break;
		  case 6:
		      x_0=229;    
		      y_0=314;       
		      break;
		  case 7:
		      x_0=356; 
		      y_0=314;   
		      break;
		  case 8:
		      x_0=483;  
		      y_0=314;  
		      break;
		  case 9:
		      x_0=370;  
		      y_0=240; 
		      break;
		  case 10:
		      x_0=399; 
		      y_0=399; 
		      break;
		  case 11:
		      x_0=420; 
		      y_0=455; 
		      break;
		  case 12:
		      x_0=97;    
		      y_0=280;   
		      break;
		  case 13:
		      x_0=215;    
		      y_0=350; 
		      break;
		  case 14:
		      x_0=324; 
		      y_0=405;   
		      break;
		  case 15:
		      x_0=451; 
		      y_0=405;

		      break;
		    } 
		    
             break;
	     
	     /*  case 18:  // rotacao=3, lomgitude=3, latitude=68, traslacao=50, 0
		size_ut = cvSize(2290,1485);
                Homografica[0][0] = 2.5084786;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 0.9130115;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=70;  x_1=148; x_2=273;  x_3=365;  x_4=440;  x_5=102;  x_6=229;  x_7=356; x_8=483; x_9=370; x_10=399; x_11=420; x_12=97;  x_13=215; x_14=324; x_15=451;
                y_0=100; y_1=192; y_2=210;  y_3=260;  y_4=303;  y_5=263;  y_6=314;  y_7=314; y_8=330; y_9=265; y_10=399; y_11=442; y_12=260; y_13=325; y_14=385; y_15=405;
             break;
	       case 19:  // rotacao=3, lomgitude=3, latitude=72, traslacao=50, 0
		size_ut = cvSize(2720,1645);
                Homografica[0][0] = 3.040909;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 1.106800;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=57;  x_1=148; x_2=273;  x_3=365;  x_4=440;  x_5=108;  x_6=229;  x_7=356; x_8=483; x_9=370; x_10=399; x_11=420; x_12=97;  x_13=215; x_14=324; x_15=451;
                y_0=103; y_1=210; y_2=235;  y_3=300;  y_4=333;  y_5=230;  y_6=302;  y_7=314; y_8=360; y_9=292; y_10=399; y_11=420; y_12=225; y_13=295; y_14=360; y_15=405;
             break;*/
	     
	       case 20:  // rotacao=3, lomgitude=3, latitude=76, traslacao=50, 0
		//size_ut = cvSize(3400,1890);
		sizeancho= 0.235294;//800/3400;
	        sizealtura= 0.423280;//800/1890;
                Homografica[0][0] = 3.884281;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 1.413763;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
                
                               
		switch (pruebaimagem)
                    {
		  case 0:
		      x_0=42;   
		      y_0=114;   
		      break;
		  case 1:
		      x_0=148; 
		      y_0=210;  
		      break;
		  case 2:
		      x_0=273;  
		      y_0=265;     
		      break;
		  case 3:
		      x_0=365;           
		      y_0=325;  
		      break;
		  case 4:
		      x_0=440;    
		      y_0=373;          
		      break;
		  case 5:
		      x_0=115;       
		      y_0=193;   
		      break;
		  case 6:
		      x_0=275;   
		      y_0=295;          
		      break;
		  case 7:
		      x_0=356;  
		      y_0=320;   
		      break;
		  case 8:
		      x_0=450;    
		      y_0=390;   
		      break;
		  case 9:
		      x_0=355;  
		      y_0=317;  
		      break;
		  case 10:
		      x_0=420; 
		      y_0=399; 
		      break;
		  case 11:
		      x_0=430; 
		      y_0=400; 
		      break;
		  case 12:
		      x_0=115;      
		      y_0=200;     
		      break;
		  case 13:
		      x_0=230;      
		      y_0=270;  
		      break;
		  case 14:
		      x_0=354; 
		      y_0=330;  
		      break;
		  case 15:
		      x_0=451;  
		      y_0=405; 

		      break;
		    } 
		
		break;
	     
	     /*case 21:  // rotacao=3, lomgitude=3, latitude=80, traslacao=50, 0
		size_ut = cvSize(3400,1890);
                Homografica[0][0] = 5.411474;
		Homografica[0][1] = -0.3420201;
		Homografica[0][2] = 280;
		Homografica[1][0] = 1.969616;
		Homografica[1][1] = 0.9396926;
		Homografica[1][2] = 0;
		Homografica[2][0] = 0;
		Homografica[2][1] = 0;
		Homografica[2][2] = 1;
                x_0=42;  x_1=148; x_2=273;  x_3=365;  x_4=440;  x_5=115;  x_6=275;  x_7=356; x_8=450; x_9=355; x_10=420; x_11=430; x_12=115; x_13=230; x_14=354; x_15=451;
                y_0=114; y_1=210; y_2=265;  y_3=325;  y_4=373;  y_5=193;  y_6=295;  y_7=320; y_8=390; y_9=317; y_10=399; y_11=400; y_12=200; y_13=270; y_14=330; y_15=405;
             break;*/
      }
	int match_outlier=0;
	int match_inlier=0;
	int match_desv=0;
	matchingslist::iterator pttr = matchings.begin();
	//cout<<matchings.size()<<endl;
		for(int i=0; i < (int) matchings.size(); i++, pttr++)		
		{
		        xcasado = (zoom1*pttr->first.x * Homografica[0][0] + zoom1*pttr->first.y *Homografica[0][1] + Homografica[0][2])*sizeancho - x_0;
			ycasado = (zoom1*pttr->first.x * Homografica[1][0] + zoom1*pttr->first.y *Homografica[1][1] + Homografica[1][2])*sizealtura - y_0;
			difx = (zoom2*pttr->second.x - xcasado);
			dify = (zoom2*pttr->second.y - ycasado);
			tot=sqrt(difx*difx+dify*dify);
			if (tot>10)
	                {  
			  match_outlier+=1;
			}
			else
			{
			  match_inlier+=1;
			  match_desv+=tot;
			}
			//cout << "match fail  " <<  match_outlier << " match true  " <<  match_inlier <<" desv "<< tot<<"  Sum desv " << match_desv << endl;
	
		}
	cout << "Match fail:" <<  match_outlier << "\nMatch true:" <<  match_inlier <<"\nSum Erro:" << match_desv << endl;
	file_keyteste <<"MatchOutlier;" <<  match_outlier << ";MatchInlier;" <<  match_inlier <<";SumErro;" << match_desv << ";";
	

	
	
	///////////////// Output image containing line matches (the two images are concatenated one above the other)
	int band_w = 20; // insert a black band of width band_w between the two images for better visibility
	
	int wo =  MAX(w1,w2);
	int ho = h1+h2+band_w;
	
	float *opixelsASIFT = new float[wo*ho];
	
	for(int j = 0; j < (int) ho; j++)
		for(int i = 0; i < (int) wo; i++)  opixelsASIFT[j*wo+i] = 255;		
	
	/////////////////////////////////////////////////////////////////// Copy both images to output
	for(int j = 0; j < (int) h1; j++)
		for(int i = 0; i < (int) w1; i++)  opixelsASIFT[j*wo+i] = ipixels1[j*w1+i];				
	
	for(int j = 0; j < (int) h2; j++)
		for(int i = 0; i < (int) (int)w2; i++)  opixelsASIFT[(h1 + band_w + j)*wo + i] = ipixels2[j*w2 + i];	
	
	//////////////////////////////////////////////////////////////////// Draw matches
	matchingslist::iterator ptr = matchings.begin();
	for(int i=0; i < (int) matchings.size(); i++, ptr++)
	{		
		draw_line(opixelsASIFT, (int) (zoom1*ptr->first.x), (int) (zoom1*ptr->first.y), 
				  (int) (zoom2*ptr->second.x), (int) (zoom2*ptr->second.y) + h1 + band_w, 255.f, wo, ho);		
	}
			
	///////////////////////////////////////////////////////////////// Save imgOut	
	write_png_f32(argv[3], opixelsASIFT, wo, ho, 1);
	
	delete[] opixelsASIFT; /*memcheck*/
	
	/////////// Output image containing line matches (the two images are concatenated one aside the other)
	int woH =  w1+w2+band_w;
	int hoH = MAX(h1,h2);
	
	float *opixelsASIFT_H = new float[woH*hoH];
	
	for(int j = 0; j < (int) hoH; j++)
		for(int i = 0; i < (int) woH; i++)  opixelsASIFT_H[j*woH+i] = 255;
	
	/////////////////////////////////////////////////////////////////// Copy both images to output
	for(int j = 0; j < (int) h1; j++)
		for(int i = 0; i < (int) w1; i++)  opixelsASIFT_H[j*woH+i] = ipixels1[j*w1+i];				
	
	for(int j = 0; j < (int) h2; j++)
		for(int i = 0; i < (int) w2; i++)  opixelsASIFT_H[j*woH + w1 + band_w + i] = ipixels2[j*w2 + i];	

	
	//////////////////////////////////////////////////////////////////// Draw matches
	matchingslist::iterator ptrH = matchings.begin();
	for(int i=0; i < (int) matchings.size(); i++, ptrH++)
	{		
		draw_line(opixelsASIFT_H, (int) (zoom1*ptrH->first.x), (int) (zoom1*ptrH->first.y), 
				  (int) (zoom2*ptrH->second.x) + w1 + band_w, (int) (zoom2*ptrH->second.y), 255.0f, woH, hoH);		
	}
	
	///////////////////////////////////////////////////////////////// Save imgOut	
	write_png_f32(argv[4], opixelsASIFT_H, woH, hoH, 1);
	
	delete[] opixelsASIFT_H; /*memcheck*/
	
	////// Write the coordinates of the matched points (row1, col1, row2, col2) to the file argv[5]
	std::ofstream file_keyp1("transformadas/imagem1.txt");
	std::ofstream file_keyp2("transformadas/imagem2.txt");
	std::ofstream file(argv[5]);
	if (file_keyp2.is_open())
	{
	if (file_keyp1.is_open())
	{
	if (file.is_open())
	{		
		// Write the number of matchings in the first line
		file << num_matchings << std::endl;
		
		matchingslist::iterator ptr = matchings.begin();
		for(int i=0; i < (int) matchings.size(); i++, ptr++)		
		{
			file << zoom1*ptr->first.x << "  " << zoom1*ptr->first.y << "  " <<  zoom2*ptr->second.x << 
			"  " <<  zoom2*ptr->second.y << std::endl;
			file_keyp1 << zoom1*ptr->first.x << " " << zoom1*ptr->first.y  <<std::endl;
			file_keyp2 << zoom2*ptr->second.x <<" " <<  zoom2*ptr->second.y <<std::endl;
		}		
	}
	else 
	{
		std::cerr << "Unable to open the file matchings."; 
	}

	file.close();
        }
        }
	file_keyp2.close();
	file_keyp1.close();

	
	// Write all the keypoints (row, col, scale, orientation, desciptor (128 integers)) to 
	// the file argv[6] (so that the users can match the keypoints with their own matching algorithm if they wish to)
	// keypoints in the 1st image
	
	std::ofstream file_key1(argv[6]);

	if (file_key1.is_open())
	{
		// Follow the same convention of David Lowe: 
		// the first line contains the number of keypoints and the length of the desciptors (128)
		file_key1 << num_keys1 << "  " << VecLength << "  " << std::endl;
		for (int tt = 0; tt < (int) keys1.size(); tt++)
		{
			for (int rr = 0; rr < (int) keys1[tt].size(); rr++)
			{
				keypointslist::iterator ptr = keys1[tt][rr].begin();
				for(int i=0; i < (int) keys1[tt][rr].size(); i++, ptr++)	
				{
					file_key1 << zoom1*ptr->x << "  " << zoom1*ptr->y << "  " << zoom1*ptr->scale << "  " << ptr->angle;
										
					for (int ii = 0; ii < (int) VecLength; ii++)
					{
						file_key1 << "  " << ptr->vec[ii];
					}
					
					file_key1 << std::endl;
				}
			}	
		}
	}
	else 
	{
		std::cerr << "Unable to open the file keys1."; 
	}

	file_key1.close();
	
	////// keypoints in the 2nd image
	
	std::ofstream file_key2(argv[7]);
	
	if (file_key2.is_open())
	{
		// Follow the same convention of David Lowe: 
		// the first line contains the number of keypoints and the length of the desciptors (128)
		file_key2 << num_keys2 << "  " << VecLength << "  " << std::endl;
		for (int tt = 0; tt < (int) keys2.size(); tt++)
		{
			for (int rr = 0; rr < (int) keys2[tt].size(); rr++)
			{
				keypointslist::iterator ptr = keys2[tt][rr].begin();
				for(int i=0; i < (int) keys2[tt][rr].size(); i++, ptr++)	
				{
					file_key2 << zoom2*ptr->x << "  " << zoom2*ptr->y << "  " << zoom2*ptr->scale << "  " << ptr->angle;
										
					for (int ii = 0; ii < (int) VecLength; ii++)
					{
						file_key2 << "  " << ptr->vec[ii];
					}					
					file_key2 << std::endl;
				}
			}	
		}
	}
	else 
	{
		std::cerr << "Unable to open the file keys2."; 
	}
  
	
	
	


	
	
	
	
	
	
      	
        //ELIMINAR MEMORIA
        wS=0;  hS=0; zoom1=0; zoom2=0;
        wS1=0; hS1=0;wS2=0;hS2=0;
        wSS=0; hSS=0; 
        num_of_tilts1=0; num_of_tilts2=0; filtro=0;
        ipixels1_zoom.clear(); flag_resize=0;
        verb=0; num_keys1=0; num_keys2=0; mtime=0;
        seconds=0; useconds=0; //
        //free(timeval);// start='\O'; end='\O';
        num_matchings=0;
	}
	 std::ofstream file_keyteste(mosaico_texto,ios::app);
	 file_keyteste <<blur<<";"<<imagem2<<std::endl ;  
	}
	std::ofstream file_keyteste(mosaico_texto,ios::app);
	 file_keyteste <<std::endl ;  
	//}
}
mosaico_tex.clear();

	 }
    return 0;
}
