#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/xfeatures2d.hpp>
#include "opencv2/highgui/highgui.hpp"

#include <time.h>
#include <sys/time.h>

#include <omp.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include "ASIFT_ANGULOS_VANT.h" 

# define IM_X 800
# define IM_Y 800
# define PI 3.14159265
# define ABS(x)    (((x) > 0) ? (x) : (-(x)))

using namespace std;
using namespace cv;


ASIFT_ANGULOS_VANT::ASIFT_ANGULOS_VANT(Mat *img1,Mat *img2, int theta, int phi)
{
    this->imagem1 = *img1;
    this->imagem2 = *img2;
    
    int AngulothetaEntrada;  
    int AngulophiEntrada; 
    int KeyPoint_total1 =0;
    
    struct timeval start, end;
    long seconds, useconds; 
    
    switch (theta)
	{
        case 1:
            AngulothetaEntrada=0;         
        break;
        case 2:
            AngulothetaEntrada=16;         
        break;
        case 3:
            AngulothetaEntrada=32;         
        break;
        case 4:
            AngulothetaEntrada=48;         
        break;
        case 5:
            AngulothetaEntrada=64;         
        break;
        case 6:
            AngulothetaEntrada=72;         
        break;
       }
       
    switch (phi)
	{
        case 1:
            AngulophiEntrada=0;         
        break;
        case 2:
            AngulophiEntrada=100;         
        break;
        case 3:
            AngulophiEntrada=150;         
        break;
       }
    
    //cout<<"theta ad "<<AngulothetaEntrada<<" phi ad "<<AngulophiEntrada<<endl;
    //imshow("imagen1",imagem1);
    //imshow("imagen2",imagem2);
     
    vector<int> Angulotheta(6);
    Angulotheta[0]=0;   Angulotheta[1]=45;  Angulotheta[2]=60;  
    Angulotheta[3]=69;  Angulotheta[4]=75;  Angulotheta[5]=80;
    vector< vector <int> > Angulophi(6);
    Angulophi[0]=vector<int>(1);
    Angulophi[0][1]=0;
    Angulophi[1]=vector<int>(4);
    Angulophi[1][0]=0;   Angulophi[1][1]=45;   
    Angulophi[1][2]=90;  Angulophi[1][3]=135;
    Angulophi[2]=vector<int>(5);
    Angulophi[2][0]=0;    Angulophi[2][1]=36;   Angulophi[2][2]=72; 
    Angulophi[2][3]=108;  Angulophi[2][4]=144; 
    Angulophi[3]=vector<int>(7);
    Angulophi[3][0]=0;    Angulophi[3][1]=26;   Angulophi[3][2]=51;   Angulophi[3][3]=77;
    Angulophi[3][4]=102;  Angulophi[3][5]=128;  Angulophi[3][6]=154;
    Angulophi[4]=vector<int>(10);
    Angulophi[4][0]=0;    Angulophi[4][1]=18;   Angulophi[4][2]=36;   Angulophi[4][3]=54;    Angulophi[4][4]=72;
    Angulophi[4][5]=90;   Angulophi[4][6]=108;  Angulophi[4][7]=126;  Angulophi[4][8]=144;   Angulophi[4][9]=162;
    Angulophi[5]=vector<int>(14);
    Angulophi[5][0]=0;    Angulophi[5][1]=13;   Angulophi[5][2]=26;   Angulophi[5][3]=39;    Angulophi[5][4]=51;
    Angulophi[5][5]=64;   Angulophi[5][6]=77;   Angulophi[5][7]=90;   Angulophi[5][8]=103;   Angulophi[5][9]=116;
    Angulophi[5][10]=129; Angulophi[5][11]=141; Angulophi[5][12]=154; Angulophi[5][13]=167;  

    vector<int> AngulothetaIntervalo(2);
    vector<vector<int> >AngulophiIntervalo(2);
    AngulophiIntervalo[0]=vector<int>(2);
    AngulophiIntervalo[1]=vector<int>(2);
    AngulothetaIntervalo[0]=0;      AngulothetaIntervalo[1]=1;
    AngulophiIntervalo[0][0]=0;     AngulophiIntervalo[0][1]=0;
    AngulophiIntervalo[1][0]=0;     AngulophiIntervalo[1][1]=0;

    vector< vector< vector<KeyPoint > > >keys_alla1;
    vector< vector< Mat > > descriptors_alla1;
    keys_alla1 = vector< vector<  vector< KeyPoint > > >(2);	
    descriptors_alla1 = vector< vector< Mat > >(2);
    keys_alla1[0] = vector<  vector< KeyPoint > >(2);	
    descriptors_alla1[0] = vector< Mat >(2);
    keys_alla1[1] = vector<  vector< KeyPoint > >(2);	
    descriptors_alla1[1] = vector< Mat >(2);

    //angulos theta
    for( int k = 0; k < Angulotheta.size(); k++ )
    { 
        AngulothetaIntervalo[1]=k;
        if (AngulothetaEntrada < 32)
            break;
        if (AngulothetaEntrada <= Angulotheta[k])
            break;
        AngulothetaIntervalo[0]=k;
    }

    //angulos phi->thetamin
    for( int k = 0; k < Angulophi[AngulothetaIntervalo[0]].size(); k++ )
    { 
        AngulophiIntervalo[0][1]=k;
        if (AngulophiEntrada <= Angulophi[AngulothetaIntervalo[0]][k])
            break;
        AngulophiIntervalo[0][0]=k;
    }

    //angulos phi->thetamax
    for( int k = 0; k < Angulophi[AngulothetaIntervalo[1]].size(); k++ )
    { 
        AngulophiIntervalo[1][1]=k;
        if (AngulophiEntrada < Angulophi[AngulothetaIntervalo[1]][k])
            break;
        AngulophiIntervalo[1][0]=k;
    }

    Mat transformada1 ,transformada2; 

    //Redimensionar imagens
    //cout << "WARNING: As imagens de entrada são redimensionadas " << IM_X << "x" << IM_Y << " para ASIFT. " << endl 
    //<< " mas los resiltados seran normalizados ao tamano original" << endl << endl;

    int wS = IM_X;
    int hS = IM_Y;

    CvSize size_dim = cvSize(wS,hS);
    resize(imagem2,imagem2,size_dim,CV_INTER_CUBIC);

    /*  GaussianBlur( img_1,img_1, Size( 3,3 ), 1.6, 1.6);
    GaussianBlur( img_2,img_2, Size( 3,3 ), 1.6, 1.6);*/

    //int num_of_tilts1=1; //inclinação primeira imagem
    //cout << "tilts1: "<<num_of_tilts1 << endl;

    //(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma)
    Ptr<Feature2D> sift = xfeatures2d::SIFT::create(20000, 5, 0.05, 10, 3);

    // Homografica
    Size s = imagem2.size();
    int h1 = s.height;
    int w1 = s.width;

    vector<KeyPoint> keypoints2;
    Mat descriptors2;
    gettimeofday(&start, NULL);
    #ifdef _OPENMP
    omp_set_nested(1);
    #endif
    #pragma omp parallel for private(k)
    for( int k = 0; k < 2; k++ )
    { 
        if ((k==0)||(Angulotheta[AngulothetaIntervalo[k]]!= Angulotheta[AngulothetaIntervalo[0]]))
        {
            int j;
            #pragma omp parallel for private(j)
            for( j = 0; j < 2; j++ )
            { 
                if ((j==0)||(Angulophi[AngulothetaIntervalo[k]][AngulophiIntervalo[k][j]]!=Angulophi[AngulothetaIntervalo[k]]                               [AngulophiIntervalo[k][0]]))
                {
                    
                    //cout<<"angulo phi"<<Angulophi[AngulothetaIntervalo[k]][AngulophiIntervalo[k][j]]<<"angulo theta"<<Angulotheta[AngulothetaIntervalo[k]]<< endl;

                    float ThetaEntrada = Angulotheta[AngulothetaIntervalo[k]]*PI/180;
                    float PhiEntrada= Angulophi[AngulothetaIntervalo[k]][AngulophiIntervalo[k][j]]*PI/180;

                    float theta, t3, t;
                    theta = PhiEntrada;
                    t3=(cos(ThetaEntrada));
                    t=1/t3;

                    //cout<<"angulo theta"<<PhiEntrada<< endl;
                    //cout<<"angulo phi"<<ThetaEntrada<< endl;

                    //transformada homografica com theta e phi determinados
                    double Homografica[3][3];
                    Homografica[0][0] = cos(theta);
                    Homografica[0][1] = -sin(theta);
                    Homografica[0][2] = h1*sin(theta);
                    if ( w1*cos(theta) < 0 )
                        Homografica[0][2] = h1*sin(theta) + ABS (w1*cos(theta));
                    Homografica[1][0] = sin(theta);
                    Homografica[1][1] = cos(theta);
                    Homografica[1][2] = 0;
                    if ( h1*cos(theta) < 0 )
                        Homografica[1][2]  = ABS (h1*cos(theta));
                    Homografica[2][0] = 0;
                    Homografica[2][1] = 0;
                    Homografica[2][2] = 1;

                    Mat homography_matrix( 2, 3, CV_64F , Homografica);

                    CvSize size_ut = cvSize(round( ABS (w1 * cos(theta)) + ABS (h1 * sin(theta))),round(  ABS(h1 * cos(theta)) + ABS (w1 * sin(theta))));
                    Mat transformada;
                    warpAffine(imagem2,transformada,homography_matrix ,size_ut ,CV_INTER_CUBIC );

                    int gw = round( ABS (w1 * cos(theta)) + ABS (h1 * sin(theta)))/w1;
                    int gh = round(  ABS(h1 * cos(theta)) + ABS (w1 * sin(theta)))/h1;
                    int dgw = round( ABS (w1 * cos(theta)) + ABS (h1 * sin(theta)));
                    int dgh = round(  ABS(h1 * cos(theta)) + ABS (w1 * sin(theta)));

                    //t3
                    CvSize sicet3 =cvSize(dgw,dgh*t3);
                    resize(transformada,transformada,sicet3,CV_INTER_CUBIC);
                    GaussianBlur( transformada,transformada, Size( 3,3 ), 1.6*t/2, 1.6*t/2 );

                    //-- Step 1: Detect the keypoints using SIFT Detector
                    vector<KeyPoint> keypoints1;
                    sift->detect( transformada, keypoints1  );

                    //-- Step 2: Calculate descriptors (feature vectors)
                    Mat descriptors1;
                    sift->compute( transformada, keypoints1 , descriptors1 ); 

                    //drawKeypoints(transformada,keypoints1,transformada,Scalar(255, 255, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                    //imshow("imagen con todos puntossin transformar 1", transformada );

                    //transformada inversa
                    theta=-theta;
                    double Homografica1[3][3];
                    Homografica1[0][0] = cos(theta);
                    Homografica1[0][1] = -sin(theta);
                    Homografica1[0][2] = -h1*cos(-theta)*sin(-theta);
                    if ( cos(theta) < -0.001 )
                        Homografica1[0][2] = ABS(w1*cos(-theta)*cos(PI+theta));
                    Homografica1[1][0] = sin(theta);
                    Homografica1[1][1] = cos(theta);
                    Homografica1[1][2] = h1 - h1*cos(-theta)*cos(-theta) ;//+w1*cos(-theta)*sin(-theta);//400;
                    if ( cos(theta) < -0.001 )
                        Homografica1[1][2]  = h1 + ABS(w1*cos(-theta)*sin(PI+theta));
                    Homografica1[2][0] = 0;
                    Homografica1[2][1] = 0;
                    Homografica1[2][2] = 1;

                    //transportar puntos
                    for(unsigned int i = 0; i < keypoints1.size(); i++)
                        {
                        double x = keypoints1[i].pt.x/gw;
                        double y = keypoints1[i].pt.y/(gh*t3);
                        keypoints1[i].pt.x = Homografica1[0][0] *x + Homografica1[0][1] * y + Homografica1[0][2];
                        keypoints1[i].pt.y = Homografica1[1][0] *x + Homografica1[1][1] * y + Homografica1[1][2];
                        }
                    //drawKeypoints(img_2,keypoints1,transformada1,Scalar(255, 255, 255),DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
                    //imshow("imagen con todos puntos 1", transformada1 );
                    //waitKey(0);
                    keys_alla1[k][j] = keypoints1;
                    descriptors_alla1[k][j] = descriptors1;
                }
            } 
        }
    }



    for( int k = 0; k < 2; k++ )
    { 
        if ((k==1)&&(Angulotheta[AngulothetaIntervalo[k]]== Angulotheta[AngulothetaIntervalo[0]]))
            break;
        for( int j = 0; j < 2; j++ )
        { 
            if ((j==1)&&(Angulophi[AngulothetaIntervalo[k]][AngulophiIntervalo[k][j]]==Angulophi[AngulothetaIntervalo[k]]                       [AngulophiIntervalo[k][0]])){
                break;}
            keypoints2.insert(keypoints2.end(), keys_alla1[k][j].begin(), keys_alla1[k][j].end());
            descriptors2.push_back(descriptors_alla1[k][j]);             
        }
    }

    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo2 = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    pontos2 = keypoints2.size();
    //cout << "Imagem Mosaico:" << mtime << " milliseconds." << endl;

    
    //simulaciones primeira imagem
    gettimeofday(&start, NULL);
    vector<KeyPoint> keypoints1;
    sift->detect( imagem1, keypoints1  );
    Mat descriptors1;
    sift->compute( imagem1, keypoints1 , descriptors1 ); 
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo1 = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    pontos1=keypoints1.size();
    //cout << "Primera imagem:" << mtime << " milliseconds." << endl;

    //-- Step 3: Matching descriptor vectors with a brute force matcher

    
    //casamento
        /** Econtra a correspondencia mais proxima entre os keypoints das 2 imagens */
    //fmatcher.match(descritores1, descritores2, matches);
                    
    gettimeofday(&start, NULL);
    BFMatcher matcher(NORM_L2);
    //vector< DMatch > matches;
    matcher.match( descriptors1, descriptors2, matches );
    //cout << "Keypoints matches: "<<matches.size() << endl;

    //-- Draw matches
    /*Mat img_matches;
    drawMatches( imagem1, keypoints1, imagem2, keypoints2, matches, img_matches );
    imshow("MatchesSIFTsinRANSAc.png", img_matches );
    waitKey(0);
    imwrite("MatchesSIFTsinRANSAc.png", img_matches );
    cout<<keypoints1.size()<<endl;
    cout<<keypoints2.size()<<endl;
    cout<<matches.size()<<endl;*/
    
    
    vector<KeyPoint> keypointsfil1, keypointsfil2;
    Mat descriptorsfil1, descriptorsfil2;
    vector<Point2f> point1;
    vector<Point2f> point2;
    vector<unsigned char> match_mask;
    
    
    for( size_t i = 0; i < matches.size(); i++ )
        {
    //-- Get the keypoints from the good matches
    keypointsfil1.insert(keypointsfil1.end(), keypoints1[ matches[i].queryIdx ]);
    descriptorsfil1.push_back(descriptors1.row(matches[i].queryIdx));
    keypointsfil2.insert(keypointsfil2.end(), keypoints2[ matches[i].trainIdx ]);
    descriptorsfil2.push_back(descriptors2.row(matches[i].trainIdx));
    point1.push_back(keypoints1.at(matches.at(i).queryIdx).pt);
    point2.push_back(keypoints2.at(matches.at(i).trainIdx).pt);
        }
    
    matcher.match( descriptorsfil1, descriptorsfil2, matches );
    
    if((keypointsfil1.size()>4)&&(keypointsfil2.size()>4))
    {
        //Filtrado de lineas
        /*drawMatches(  img_1, keypointsfil1,img_2, keypointsfil2, matches, img_matches );
        imshow("Matches filtrado .png", img_matches );
        */    
        pontos_matches=matches.size();
        Mat H;
        H = findHomography(point1,point2, CV_RANSAC,3,match_mask);
        //cout<<H<<endl;
        
        if(match_mask.size())
        {
            for( size_t i = 0; i < point2.size(); i++ )
                {
                if (match_mask[i]) //> Outlier
                    {
                    keypointsfil11.insert(keypointsfil11.end(), keypointsfil1[ i ]);
                    descriptorsfil11.push_back(descriptorsfil1.row(i));
                    keypointsfil22.insert(keypointsfil22.end(), keypointsfil2[ i ]);
                    descriptorsfil22.push_back(descriptorsfil2.row(i));
                    }
                }
            matcher.match( descriptorsfil11, descriptorsfil22, matches );
            /*drawMatches(  imagem1 , keypointsfil11,imagem2, keypointsfil22, matches, img_matches );
            imshow("Matches filtrado RANSAC.png", img_matches ); 
            waitKey(0);
            imwrite("Matches_SIFT_VANT_RANSAC5.png", img_matches ); 
            cout<<matches.size()<<endl;*/
            pontos_matches_total=matches.size();
        }
        else
        {   
            pontos_matches_total=0;
        }
        
    }
    else{
    
        pontos_matches=0; 
        pontos_matches_total=0;
    }
    
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo_casamento = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    
    
              
}

long ASIFT_ANGULOS_VANT::tempo_1( )
{
  return tempo1;
}

long ASIFT_ANGULOS_VANT::tempo_2( )
{
  return tempo2;
}

long ASIFT_ANGULOS_VANT::tempo__casamento( )
{
  return tempo_casamento;
}

int ASIFT_ANGULOS_VANT::pontos_1( )
{
  return pontos1;
}

int ASIFT_ANGULOS_VANT::pontos_2( )
{
  return pontos2;
}

int ASIFT_ANGULOS_VANT::pontos_casamento( )
{
  return pontos_matches; 
}

int ASIFT_ANGULOS_VANT::pontos_casamento_total( )
{
  return pontos_matches_total;
}

vector<KeyPoint> ASIFT_ANGULOS_VANT::key1( )
{
  return keypointsfil11;
}

vector<KeyPoint> ASIFT_ANGULOS_VANT::key2( )
{
  return keypointsfil22;
}

Mat ASIFT_ANGULOS_VANT::desc1( )
{
  return descriptorsfil11;
}

Mat ASIFT_ANGULOS_VANT::desc2( )
{
  return descriptorsfil22;
}

vector< DMatch > ASIFT_ANGULOS_VANT::keymatches( )
{
  return matches;
}


