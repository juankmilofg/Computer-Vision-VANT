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
#include "ASIFT_VANT.h" 

# define IM_X 500
# define IM_Y 500
# define PI 3.14159265
# define ABS(x)    (((x) > 0) ? (x) : (-(x)))

using namespace std;
using namespace cv;


ASIFT_VANT::ASIFT_VANT(Mat *img1,Mat *img2)
{
    this->imagem1 = *img1;
    this->imagem2 = *img2;
        
    //imshow("imagen1",imagem1);
    //imshow("imagen2",imagem2);
    
    int num_of_tilts1=6; //inclinação primeira imagem
    int num_of_tilts2=3; 

    struct timeval start, end;
    long seconds, useconds; 
    vector<KeyPoint> keypointsall1,keypointsall2;
    Mat descriptorsall1,descriptorsall2;
    float t_min, t_k;
    int num_tilt, tt, num_rot_t2, rr;

    num_rot_t2 = 10;
    t_min = 1;
    t_k = sqrt(2.);

    // Parametros SIFT
   
    //(nfeatures, nOctaveLayers, contrastThreshold, edgeThreshold, sigma)
    Ptr<Feature2D> sift = xfeatures2d::SIFT::create(20000, 5, 0.05, 10, 3);

    // Homografica
    Size s = imagem1.size();
    int h1 = s.height;
    int w1 = s.width;

    vector< vector< vector<KeyPoint > > >keys_all1;
    vector< vector< Mat > > descriptors_all1;
    vector< vector< vector< DMatch > > >matchesall;

    //simulaciones primeira imagem
    keys_all1 = vector< vector<  vector< KeyPoint > > >(num_of_tilts1);	
    descriptors_all1 = vector< vector< Mat > >(num_of_tilts1);
    matchesall =  vector< vector< vector< DMatch > > >(num_of_tilts1);	
    int KeyPoint_total=0;
    //criar lista keys_all1 e matchesall
    gettimeofday(&start, NULL);
    for (tt = 1; tt <= num_of_tilts1; tt++)
    {
        float t = t_min * pow(t_k, tt-1);

        if ( t == 1 )
        {
            keys_all1[tt-1] = vector<vector< KeyPoint > >(1);
            descriptors_all1[tt-1]=vector< Mat >(1);
            matchesall[tt-1]=vector<vector< DMatch > >(1);
        }
        else
        {
            int num_rot1 = round(num_rot_t2*t/2);        
            if ( num_rot1%2 == 1 )
            {
                num_rot1 = num_rot1 + 1;
            }
            num_rot1 = num_rot1 / 2;
            //printf(" num rotaciones= %d, counter simulaciones= %d. \n", num_rot1, counter_sim);
            keys_all1[tt-1] = vector<vector< KeyPoint > >(num_rot1);
            descriptors_all1[tt-1]=vector< Mat >(num_rot1);
            matchesall[tt-1]=vector< vector<DMatch > >(num_rot1);
        }         		
    }

    //simulações de ângulo theta e phi da primera imagem
    #ifdef _OPENMP
        omp_set_nested(1);
    #endif
    #pragma omp parallel for private(tt)
    for (tt = 1; tt <= num_of_tilts1; tt++)
    {
        float t3;
        float t = t_min * pow(t_k, tt-1);
        t3= 1/t;

        // If tilt t = 1, do not simulate rotation. 	
        if ( t == 1 )
        {	
            float theta;
            theta =0;        
            
            //-- Step 1: Detect the keypoints using SIFT Detector
            vector<KeyPoint> keypoints1;
            sift->detect( imagem1, keypoints1  );
            
            //-- Step 2: Calculate descriptors (feature vectors)
            Mat descriptors1;
            sift->compute( imagem1, keypoints1 , descriptors1 ); 
            
            //preencher a lista keys_all1 e descriptors_all1
            KeyPoint_total += keypoints1.size();
            keys_all1[tt-1][0] = keypoints1;
            descriptors_all1[tt-1][0] = descriptors1;
        }
        else
        {
            // The number of rotations to simulate under the current tilt.   
            int num_rot1 = round(num_rot_t2*t/2);        

            if ( num_rot1%2 == 1 )
            {
            num_rot1 = num_rot1 + 1;
            }
            num_rot1 = num_rot1 / 2;
            float delta_theta = PI/num_rot1;		

            // Loop on rotations.    
            #pragma omp parallel for private(rr)
            for ( int rr = 1; rr <= num_rot1; rr++ ) 
            {
                float theta;
                theta = delta_theta * (rr-1);

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
                
                //Aplicar transformada homografica
                Mat homography_matrix( 2, 3, CV_64F , Homografica);
                CvSize size_ut = cvSize(round( ABS (w1 * cos(theta)) + ABS (h1 * sin(theta))),round(  ABS(h1 * cos(theta)) + ABS (w1 * sin(theta))));
                Mat transformada;
                warpAffine(imagem1,transformada,homography_matrix ,size_ut ,CV_INTER_CUBIC );

                int gw = round( ABS (w1 * cos(theta)) + ABS (h1 * sin(theta)))/w1;
                int gh = round(  ABS(h1 * cos(theta)) + ABS (w1 * sin(theta)))/h1;
                int dgw = round( ABS (w1 * cos(theta)) + ABS (h1 * sin(theta)));
                int dgh = round(  ABS(h1 * cos(theta)) + ABS (w1 * sin(theta)));
                
                CvSize sicet3 =cvSize(dgw,dgh*t3);
                resize(transformada,transformada,sicet3,CV_INTER_CUBIC);
                GaussianBlur( transformada,transformada, Size( 3,3 ), 1.6*t/2, 1.6*t/2 );
                        
                //-- Step 1: Detect the keypoints using SIFT Detector
                vector<KeyPoint> keypoints1;
                sift->detect( transformada, keypoints1  );
                
                //-- Step 2: Calculate descriptors (feature vectors)
                Mat descriptors1;
                sift->compute( transformada, keypoints1 , descriptors1 ); 
                    
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
                
                //transportar pontos com a transformada inversa
                for(unsigned int i = 0; i < keypoints1.size(); i++)
                {
                    double x = keypoints1[i].pt.x/gw;
                    double y = keypoints1[i].pt.y/(gh*t3);
                    keypoints1[i].pt.x = Homografica1[0][0] *x + Homografica1[0][1] * y + Homografica1[0][2];
                    keypoints1[i].pt.y = Homografica1[1][0] *x + Homografica1[1][1] * y + Homografica1[1][2];
                }
                KeyPoint_total += keypoints1.size();
                keys_all1[tt-1][rr-1] = keypoints1;
                descriptors_all1[tt-1][rr-1] = descriptors1;                            
            }
        }		 
    } 

    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo1 = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    pontos1 = KeyPoint_total;
    
    // segunda imagen
    s = imagem2.size();
    h1 = s.height;
    w1 = s.width;
    KeyPoint_total=0;

    vector< vector< vector<KeyPoint > > >keys_all2;
    vector< vector< Mat > > descriptors_all2;
    keys_all2 = vector< vector<  vector< KeyPoint > > >(num_of_tilts2);	
    descriptors_all2 = vector< vector< Mat > >(num_of_tilts2);

    gettimeofday(&start, NULL);
    //criar lista keys_all1 e matchesall 
    for (tt = 1; tt <= num_of_tilts2; tt++)
    {
        float t = t_min * pow(t_k, tt-1);
            // cout<<"aqui voy"<<endl;
        if ( t == 1 )
        {
            keys_all2[tt-1] = vector<  vector< KeyPoint > >(1);
            descriptors_all2[tt-1]=vector< Mat >(1);
        }
        else
        {
            int num_rot1 = round(num_rot_t2*t/2);        
            if ( num_rot1%2 == 1 )
            {
                num_rot1 = num_rot1 + 1;
            }
            num_rot1 = num_rot1 / 2;
            //printf(" num rotaciones= %d, counter simulaciones= %d. \n", num_rot1, counter_sim);
            keys_all2[tt-1] = vector<  vector< KeyPoint > >(num_rot1);
            descriptors_all2[tt-1]=vector< Mat >(num_rot1);
        }         		
    }

    #pragma omp parallel for private(tt)
    for (tt = 1; tt <= num_of_tilts2; tt++)
    {
        float t3;//2,t3;
        float t = t_min * pow(t_k, tt-1);
        t3= 1/t;
        if ( t == 1 )
        {					
            float theta;
            theta =0;
            //-- Step 1: Detect the keypoints using SIFT Detector
            vector<KeyPoint> keypoints1;
            sift->detect( imagem2, keypoints1  );
   
            
            //-- Step 2: Calculate descriptors (feature vectors)
            Mat descriptors1;
            sift->compute( imagem2, keypoints1 , descriptors1 ); 
            KeyPoint_total += keypoints1.size();
            keys_all2[tt-1][0] = keypoints1;
            descriptors_all2[tt-1][0] = descriptors1;
        }
            else
        {
            // The number of rotations to simulate under the current tilt.   
            int num_rot1 = round(num_rot_t2*t/2);        

            if ( num_rot1%2 == 1 )
            {
            num_rot1 = num_rot1 + 1;
            }
            num_rot1 = num_rot1 / 2;
            float delta_theta = PI/num_rot1;		

            // Loop on rotations.    
            #pragma omp parallel for private(rr)
            for ( int rr = 1; rr <= num_rot1; rr++ ) 
            {
                float theta;
                theta = delta_theta * (rr-1);
                
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
                KeyPoint_total += keypoints1.size();
                keys_all2[tt-1][rr-1] = keypoints1;
                descriptors_all2[tt-1][rr-1] = descriptors1;
            }
        }		 
    }         

    gettimeofday(&end, NULL);
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    tempo2 = ((seconds) * 1000 + useconds/1000.0) + 0.5;
    pontos2 = KeyPoint_total;
    
    for (tt = 1; tt <= num_of_tilts2; tt++)
    {
        float t = t_min * pow(t_k, tt-1);

        if ( t == 1 )
        {
            keypointsall2.insert(keypointsall2.end(), keys_all2[tt-1][0].begin(), keys_all2[tt-1][0].end());
            descriptorsall2.push_back(descriptors_all2[tt-1][0]);
        }
        else
        {
            int num_rot1 = round(num_rot_t2*t/2);        

            if ( num_rot1%2 == 1 )
            {
            num_rot1 = num_rot1 + 1;
            }
            num_rot1 = num_rot1 / 2;
            float delta_theta = PI/num_rot1;		
            // Loop on rotations.    

            for ( int rr = 1; rr <= num_rot1; rr++ )  
            {
                keypointsall2.insert(keypointsall2.end(), keys_all2[tt-1][rr-1].begin(), keys_all2[tt-1][rr-1].end());
                descriptorsall2.push_back(descriptors_all2[tt-1][rr-1]);
            }
        }         		
    }

        //casamento paralelo (2)
    gettimeofday(&start, NULL);
    vector<KeyPoint> keypointsfila, keypointsfilb;
    Mat descriptorsfila, descriptorsfilb;
    vector<Point2f> pointa;
    vector<Point2f> pointb;
    #pragma omp parallel for private(tt)
    for (tt = 1; tt <= num_of_tilts1; tt++)
    {
        float t = t_min * pow(t_k, tt-1);

        if ( t == 1 )
        {
            vector<KeyPoint> keypointsaux;
            Mat descriptorsaux;
            keypointsaux.insert(keypointsaux.end(), keys_all1[tt-1][0].begin(), keys_all1[tt-1][0].end());
            descriptorsaux.push_back(descriptors_all1[tt-1][0]);
            BFMatcher matcher(NORM_L2);
            vector< DMatch > matches1;
            //cout << "detectores primera imagem: "<<descriptorsaux.size() << endl;
            //cout << "detectores segunda imagem: "<<descriptorsall2.size() << endl;
            matcher.match( descriptorsaux, descriptorsall2, matches1 );
            matchesall[tt-1][0]=matches1;
        }
        else
        {
            
            int num_rot1 = round(num_rot_t2*t/2);        
            if ( num_rot1%2 == 1 )
            {
                num_rot1 = num_rot1 + 1;
            }
            num_rot1 = num_rot1 / 2;
            float delta_theta = PI/num_rot1;		
            #pragma omp parallel for private(rr)
            for ( int rr = 1; rr <= num_rot1; rr++ )  
            {
                vector<KeyPoint> keypointsaux;
                Mat descriptorsaux;
                keypointsaux.insert(keypointsaux.end(), keys_all1[tt-1][rr-1].begin(), keys_all1[tt-1][rr-1].end());
                descriptorsaux.push_back(descriptors_all1[tt-1][rr-1]);
                BFMatcher matcher(NORM_L2);
                vector< DMatch > matches1;
                //cout << "detectores primera imagem: "<<descriptorsaux.size() << endl;
                //cout << "detectores segunda imagem: "<<descriptorsall2.size() << endl;
                matcher.match( descriptorsaux, descriptorsall2, matches1 );
                matchesall[tt-1][rr-1]=matches1;
            }
            
        }         		
    }
    
    

    for (tt = 1; tt <= num_of_tilts1; tt++)
    {
        float t = t_min * pow(t_k, tt-1);

        if ( t == 1 )
        {
            vector<KeyPoint> keypointsaux;
            Mat descriptorsaux;
            keypointsaux.insert(keypointsaux.end(), keys_all1[tt-1][0].begin(), keys_all1[tt-1][0].end());
            descriptorsaux.push_back(descriptors_all1[tt-1][0]);
            vector< DMatch > matches1;
            matches1=matchesall[tt-1][0];
            for( int i = 0; i < matches1.size(); i++ )
            { 
            keypointsfila.insert(keypointsfila.end(),keypointsaux[ matches1[i].queryIdx ]);
            descriptorsfila.push_back(descriptorsaux.row(matches1[i].queryIdx));
            keypointsfilb.insert(keypointsfilb.end(), keypointsall2[ matches1[i].trainIdx ]);
            descriptorsfilb.push_back(descriptorsall2.row(matches1[i].trainIdx));
            pointa.push_back(keypointsaux.at(matches1.at(i).queryIdx).pt);
            pointb.push_back(keypointsall2.at(matches1.at(i).trainIdx).pt);
            }
        }
        else
        {
            int num_rot1 = round(num_rot_t2*t/2);        
            if ( num_rot1%2 == 1 )
            {
            num_rot1 = num_rot1 + 1;
            }  
            num_rot1 = num_rot1 / 2;
            float delta_theta = PI/num_rot1;		 
            for ( int rr = 1; rr <= num_rot1; rr++ )  
            {
                vector<KeyPoint> keypointsaux;
                Mat descriptorsaux;
                keypointsaux.insert(keypointsaux.end(), keys_all1[tt-1][rr-1].begin(), keys_all1[tt-1][rr-1].end());
                descriptorsaux.push_back(descriptors_all1[tt-1][rr-1]);
                vector< DMatch > matches1;
                matches1=matchesall[tt-1][rr-1];
                for( int i = 0; i < matches1.size(); i++ )
                { 
                keypointsfila.insert(keypointsfila.end(),keypointsaux[ matches1[i].queryIdx ]);
                descriptorsfila.push_back(descriptorsaux.row(matches1[i].queryIdx));
                keypointsfilb.insert(keypointsfilb.end(), keypointsall2[ matches1[i].trainIdx ]);
                descriptorsfilb.push_back(descriptorsall2.row(matches1[i].trainIdx));
                pointa.push_back(keypointsaux.at(matches1.at(i).queryIdx).pt);
                pointb.push_back(keypointsall2.at(matches1.at(i).trainIdx).pt);
                }
            }
        }         		
    }
    

    if ((keypointsfila.size()>4)&&(keypointsfilb.size()>4))
    {
        //-- Draw matches
        pontos_matches=keypointsfila.size();
        Mat img_matches;
        BFMatcher matcher(NORM_L2);
        
        //-- Show detected matches
        vector<unsigned char> match_mask;
        Mat H;
        H = findHomography(pointa,pointb, CV_RANSAC,3,match_mask);      
        if(match_mask.size())
        {
            for( size_t i = 0; i < pointb.size(); i++ )
            {
                if (match_mask[i]) //> Outlier
                    {
                        keypointsfil11.insert(keypointsfil11.end(), keypointsfila[ i ]);
                        descriptorsfil11.push_back(descriptorsfila.row(i));
                        keypointsfil22.insert(keypointsfil22.end(), keypointsfilb[ i ]);
                        descriptorsfil22.push_back(descriptorsfilb.row(i));
                    }
            }
            matcher.match( descriptorsfil11, descriptorsfil22, matches );
            //drawMatches( imagem1, keypointsfil11,imagem2, keypointsfil22, matches, img_matches );  
            //imshow("asift Matches filtrado RANSAC .png", img_matches );    
            // imwrite("Matches_ASIFT_VANT_ransac3.png", img_matches ); 
            //cout<<matches.size()<<endl;
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

long ASIFT_VANT::tempo_1( )
{
  return tempo1;
}

long ASIFT_VANT::tempo_2( )
{
  return tempo2;
}

long ASIFT_VANT::tempo__casamento( )
{
  return tempo_casamento;
}

int ASIFT_VANT::pontos_1( )
{
  return pontos1;
}

int ASIFT_VANT::pontos_2( )
{
  return pontos2;
}

int ASIFT_VANT::pontos_casamento( )
{
  return pontos_matches; 
}

int ASIFT_VANT::pontos_casamento_total( )
{
  return pontos_matches_total;
}

vector<KeyPoint> ASIFT_VANT::key1( )
{
  return keypointsfil11;
}

vector<KeyPoint> ASIFT_VANT::key2( )
{
  return keypointsfil22;
}

Mat ASIFT_VANT::desc1( )
{
  return descriptorsfil11;
}

Mat ASIFT_VANT::desc2( )
{
  return descriptorsfil22;
}

vector< DMatch > ASIFT_VANT::keymatches( )
{
  return matches;
}

