/*
Federal University of Minas Gerais - Brazil LITC
Computational Intelligence and Technology Laboratory. 
http://litc.cpdee.ufmg.br. 
Code developed by: 
Juan Camilo Fonseca Galindo - juankmilofg@ufmg.br
*/

#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include <opencv2/xfeatures2d.hpp>
#include "opencv2/highgui/highgui.hpp"

#include <time.h>
#include <sys/time.h>

#include <cstdlib>
#include <iostream>
#include <string>
#include "SIFT_VANT.h" 

using namespace std;
using namespace cv;


SIFT_VANT::SIFT_VANT(Mat *img1,Mat *img2)
{
    this->imagem1 = *img1;
    this->imagem2 = *img2;
    
    //imshow("imagen1",imagem1);
    //imshow("imagen2",imagem2);

    int num_keys1=0, num_keys2=0;
    struct timeval start, end;
    long seconds, useconds; 
    
    //(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma)
    Ptr<Feature2D> sift = xfeatures2d::SIFT::create(20000, 5, 0.05, 10, 3);
                    
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    
    gettimeofday(&start, NULL);
    sift->detect( imagem1, keypoints1  );
    sift->compute( imagem1, keypoints1 , descriptors1 ); 
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo1 = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    pontos1=keypoints1.size();
    
    //Imagem busqueda
    gettimeofday(&start, NULL);
    sift->detect(imagem2, keypoints2  );
    sift->compute( imagem2, keypoints2 , descriptors2  ); 
    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo2 = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    pontos2=keypoints2.size();

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

long SIFT_VANT::tempo_1( )
{
  return tempo1;
}

long SIFT_VANT::tempo_2( )
{
  return tempo2;
}

long SIFT_VANT::tempo__casamento( )
{
  return tempo_casamento;
}

int SIFT_VANT::pontos_1( )
{
  return pontos1;
}

int SIFT_VANT::pontos_2( )
{
  return pontos2;
}

int SIFT_VANT::pontos_casamento( )
{
  return pontos_matches; 
}

int SIFT_VANT::pontos_casamento_total( )
{
  return pontos_matches_total;
}

vector<KeyPoint> SIFT_VANT::key1( )
{
  return keypointsfil11;
}

vector<KeyPoint> SIFT_VANT::key2( )
{
  return keypointsfil22;
}

Mat SIFT_VANT::desc1( )
{
  return descriptorsfil11;
}

Mat SIFT_VANT::desc2( )
{
  return descriptorsfil22;
}

vector< DMatch > SIFT_VANT::keymatches( )
{
  return matches;
}
