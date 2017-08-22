/*
Universidade Federal de Minas Gerais - Brazil 
LITC - Computational Intelligence and Technology Laboratory. http://litc.cpdee.ufmg.br. 
Code developed by: 
Juan Camilo Fonseca Galindo - juankmilofg@ufmg.br

Estudo de Métodos de Extração de Características Aplicados ao Problema da Estimação da Posição de um 
Vant em Navegação Autônoma com Visão Computacional - Dissertação de Mestrado - 2016
https://www.ppgee.ufmg.br/defesas/1343M.PDF

Fonseca-Galindo, J. C. and Lemos, A. P. (2016). Proposta de um método de extração de caracterı́sticas 
aplicado ao problema de estimação da posição de um vant em navegação autônoma,
Anais do XXI Congresso Brasileiro de Automática pp. 3139–3144.

Fonseca-Galindo, J. C., Lemos, A. P. and Shiguemori E. H. (2016). Novo extrator de caracrerísticas 
aplicado à estimação da posição de um VANT em navegação autonoma com visão computacional,
Anais do XIX ENMEC Encontro Nacional de Modelagem Computacional pp. 978–987. */


#include <opencv2/opencv.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>

#include "SIFT_VANT.h"
#include "SURF_VANT.h"
#include "ORB_VANT.h"
#include "AKAZE_VANT.h"
#include "ASIFT_VANT.h"
#include "ASURF_VANT.h"
#include "AAKAZE_VANT.h"
#include "ASIFT_ANGULOS_VANT.h"
#include "ASURF_ANGULOS_VANT.h"
#include "AAKAZE_ANGULOS_VANT.h"

#include "acuracia.h"

#include "iostream"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>


using namespace std;
using namespace cv;

int main()
{

    int theta, phi, psi, mosaico, imagem;    
    
   for(int campos = 0; campos<1; campos++)
    {    
        string  mosaico_tex;
        switch ( campos )
        {
            case 0: 
                mosaico_tex = "";
            break;
            case 1: 
                mosaico_tex = "google";
            break;
            case 2: 
                mosaico_tex = "urbano";
            break;
            case 3: 
                mosaico_tex = "rural";
            break;
        }
        for(mosaico=0;mosaico<1;mosaico++)//33
        {
            //CArregar imagem mosaico
            string  mosaico_dir = "imagenes_entrada/";
            string  mosaico_chave = "";
            string mosaiconum = "";
            mosaiconum = static_cast<ostringstream*>(&(ostringstream() << mosaico))->str();
            mosaico_chave = mosaico_dir + "mosaicos/" + mosaico_tex + "_" + mosaiconum + ".png";
            cout<<" "<<endl;
            cout<<"---------------------------------------------------------------"<<endl;
            cout<<"---------------------------------------------------------------"<<endl;
            cout<<"         mosaico: "<<mosaico_chave<<endl;
            cout<<"---------------------------------------------------------------"<<endl;
            cout<<"---------------------------------------------------------------"<<endl;
            cout<<" "<<endl;
            Mat imagem_mosaico;
            imagem_mosaico = imread( mosaico_chave, CV_LOAD_IMAGE_GRAYSCALE );
            //imshow(mosaico_chave,imagem_mosaico);
            for(theta=1;theta<2;theta++) //7
            {
                string thetanum = "";
                thetanum = static_cast<ostringstream*>(&(ostringstream() << theta))->str();
                
                string salvar_dados1 = "dados_SIFT/";
                salvar_dados1 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum  + ".txt";
                char *salvar_dadoss1 = new char[salvar_dados1.length() + 1];
                strcpy(salvar_dadoss1, salvar_dados1.c_str());
                
                string salvar_dados2 = "dados_ASIFT/";
                salvar_dados2 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss2 = new char[salvar_dados2.length() + 1];
                strcpy(salvar_dadoss2, salvar_dados2.c_str());
                        
                string salvar_dados3 = "dados_ASIFT_ANGULOS/";
                salvar_dados3 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss3 = new char[salvar_dados3.length() + 1];
                strcpy(salvar_dadoss3, salvar_dados3.c_str());
                
                string salvar_dados4 = "dados_SURF/";
                salvar_dados4 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss4 = new char[salvar_dados4.length() + 1];
                strcpy(salvar_dadoss4, salvar_dados4.c_str());
                
                string salvar_dados5 = "dados_ASURF/";
                salvar_dados5 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss5 = new char[salvar_dados5.length() + 1];
                strcpy(salvar_dadoss5, salvar_dados5.c_str());
                        
                string salvar_dados6 = "dados_ASURF_ANGULOS/"; + ".txt";//
                salvar_dados6 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss6 = new char[salvar_dados6.length() + 1];
                strcpy(salvar_dadoss6, salvar_dados6.c_str());
                
                string salvar_dados7 = "dados_ORB/";
                salvar_dados7 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss7 = new char[salvar_dados7.length() + 1];
                strcpy(salvar_dadoss7, salvar_dados7.c_str());
                
                string salvar_dados8 = "dados_AKAZE/";
                salvar_dados8 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss8 = new char[salvar_dados8.length() + 1];
                strcpy(salvar_dadoss8, salvar_dados8.c_str());
                
                string salvar_dados9 = "dados_AAKAZE/";
                salvar_dados9 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss9 = new char[salvar_dados9.length() + 1];
                strcpy(salvar_dadoss9, salvar_dados9.c_str());
                
                string salvar_dados10 = "dados_AAKAZE_ANGULOS/";
                salvar_dados10 += mosaico_tex + "_" + mosaiconum + ".txt";// + "_" + thetanum + ".txt";
                char *salvar_dadoss10 = new char[salvar_dados10.length() + 1];
                strcpy(salvar_dadoss10, salvar_dados10.c_str());
                
                for(phi=1;phi<2;phi++)  //4
                {
                    for(psi=1;psi<2;psi++)  //4Facebook
                    {
                        for(imagem=0;imagem<1;imagem++)//4
                        {
                        //carregar imagem busqueda                    
            
                        string imagem_chave = "";
                        string phinum = "";
                        phinum= static_cast<ostringstream*>(&(ostringstream() << phi))->str();
                        string psinum = "";
                        psinum= static_cast<ostringstream*>(&(ostringstream() << psi))->str();
                        string imagemnum = "";
                        imagemnum= static_cast<ostringstream*>(&(ostringstream() << imagem))->str();
                        imagem_chave = mosaico_dir +  mosaico_tex + "_" + mosaiconum +"/total_imagens/teste__" + thetanum + "_" + phinum  + "_" + psinum + "_" + imagemnum + ".png"; 
                        cout<<"---------------------------------------------------------------"<<endl;
                        cout<<"Imagem: "<< imagem_chave << endl;
                        cout<<"---------------------------------------------------------------"<<endl;
                        Mat imagem_busqueda;
                        imagem_busqueda = imread( imagem_chave, CV_LOAD_IMAGE_GRAYSCALE );
                        
                        vector<KeyPoint> keys1;
                        vector<KeyPoint> keys2;
                        vector< DMatch > Keysmateches;
                        
                        cout <<"--SIFT--"<<endl;
                        SIFT_VANT metodo1(&imagem_busqueda,&imagem_mosaico);    
                        keys1=metodo1.key1();
                        keys2=metodo1.key2();
                        Keysmateches = metodo1.keymatches();
                        acuracia metodo1a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo1a.distancia() <<endl;
                        
                        cout <<"--ASIFT--"<<endl;
                        ASIFT_VANT metodo2(&imagem_busqueda,&imagem_mosaico);   
                        keys1=metodo2.key1();
                        keys2=metodo2.key2();
                        Keysmateches = metodo2.keymatches();
                        acuracia metodo2a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo2a.distancia() <<endl;
                       
                        cout <<"--Proposta ASIFT--"<< endl;  
                        ASIFT_ANGULOS_VANT metodo3(&imagem_busqueda,&imagem_mosaico,theta,phi);     
                        keys1=metodo3.key1();
                        keys2=metodo3.key2();
                        Keysmateches = metodo3.keymatches();
                        acuracia metodo3a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo3a.distancia() <<endl;
                        
                        cout <<"--SURF--"<<endl;
                        SURF_VANT metodo4(&imagem_busqueda,&imagem_mosaico);    
                        keys1=metodo4.key1();
                        keys2=metodo4.key2();
                        Keysmateches = metodo4.keymatches();
                        acuracia metodo4a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo4a.distancia() <<endl;
                        
                        cout <<"--ASURF--"<<endl;
                        ASURF_VANT metodo5(&imagem_busqueda,&imagem_mosaico);   
                        keys1=metodo5.key1();
                        keys2=metodo5.key2();
                        Keysmateches = metodo5.keymatches();
                        acuracia metodo5a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo5a.distancia() <<endl;
                        
                        cout <<"--Proposta ASURF--"<<endl;
                        ASURF_ANGULOS_VANT metodo6(&imagem_busqueda,&imagem_mosaico,theta,phi);   
                        keys1=metodo6.key1();
                        keys2=metodo6.key2();
                        Keysmateches = metodo6.keymatches();
                        acuracia metodo6a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo6a.distancia() <<endl;
                        
                        cout <<"--ORB--"<<endl;
                        ORB_VANT metodo7(&imagem_busqueda,&imagem_mosaico);   
                        keys1=metodo7.key1();
                        keys2=metodo7.key2();
                        Keysmateches = metodo7.keymatches();
                        acuracia metodo7a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo7a.distancia() <<endl;
                        
                        cout <<"--AKAZE--"<<endl;
                        AKAZE_VANT metodo8(&imagem_busqueda,&imagem_mosaico);   
                        keys1=metodo8.key1();
                        keys2=metodo8.key2();
                        Keysmateches = metodo8.keymatches();
                        acuracia metodo8a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo8a.distancia() <<endl;
                        
                        cout <<"--AAKAZE--"<<endl;
                        AAKAZE_VANT metodo9(&imagem_busqueda,&imagem_mosaico);   
                        keys1=metodo9.key1();
                        keys2=metodo9.key2();
                        Keysmateches = metodo9.keymatches();
                        acuracia metodo9a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo9a.distancia() <<endl;
                        
                        cout <<"--Proposta AAKAZE--"<<endl;
                        AAKAZE_ANGULOS_VANT metodo10(&imagem_busqueda,&imagem_mosaico,theta,phi);   
                        keys1=metodo10.key1();
                        keys2=metodo10.key2();
                        Keysmateches = metodo10.keymatches();
                        acuracia metodo10a( &imagem_busqueda, &imagem_mosaico, &keys1, &keys2, &Keysmateches, theta, phi, psi, imagem);
                        //cout<< "distancia media "<< metodo10a.distancia() <<endl;
                        
                      
                        
                        ofstream file_keyteste1(salvar_dadoss1,ios::app);
                        file_keyteste1 << metodo1.pontos_1() <<";"<< metodo1.tempo_1() <<";"<<metodo1.pontos_2()<<";"<< metodo1.tempo_2()<<";"<< metodo1.pontos_casamento()<<";"<< metodo1.pontos_casamento_total()<< ";"<<metodo1.tempo__casamento();
                        if (metodo1.pontos_casamento()==0)
                            file_keyteste1<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste1<<";"<< metodo1a.distancia()<<endl; 
                        
                        ofstream file_keyteste2(salvar_dadoss2,ios::app);
                        file_keyteste2 << metodo2.pontos_1() <<";"<< metodo2.tempo_1() <<";"<<metodo2.pontos_2()<<";"<< metodo2.tempo_2()<<";"<< metodo2.pontos_casamento()<<";"<< metodo2.pontos_casamento_total()<<";"<< metodo2.tempo__casamento();
                        if (metodo2.pontos_casamento()==0)
                            file_keyteste2<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste2<<";"<< metodo2a.distancia()<<endl; 
                         
                        ofstream file_keyteste3(salvar_dadoss3,ios::app);
                        file_keyteste3 << metodo3.pontos_1() <<";"<< metodo3.tempo_1() <<";"<<metodo3.pontos_2()<<";"<< metodo3.tempo_2()<<";"<< metodo3.pontos_casamento()<<";"<< metodo3.pontos_casamento_total()<<";"<< metodo3.tempo__casamento();
                        if (metodo3.pontos_casamento()==0)
                            file_keyteste3<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste3<<";"<< metodo3a.distancia()<<endl; 
                        
                        ofstream file_keyteste4(salvar_dadoss4,ios::app);
                        file_keyteste4 << metodo4.pontos_1() <<";"<< metodo4.tempo_1() <<";"<<metodo4.pontos_2()<<";"<< metodo4.tempo_2()<<";"<< metodo4.pontos_casamento()<<";"<< metodo4.pontos_casamento_total()<<";"<< metodo4.tempo__casamento();
                        if (metodo4.pontos_casamento()==0)
                            file_keyteste4<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste4<<";"<< metodo4a.distancia()<<endl; 
                        
                         ofstream file_keyteste5(salvar_dadoss5,ios::app);
                        file_keyteste5 << metodo5.pontos_1() <<";"<< metodo5.tempo_1() <<";"<<metodo5.pontos_2()<<";"<< metodo5.tempo_2()<<";"<< metodo5.pontos_casamento()<<";"<< metodo5.pontos_casamento_total()<<";"<< metodo5.tempo__casamento();
                        if (metodo5.pontos_casamento()==0)
                            file_keyteste5<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste5<<";"<< metodo5a.distancia()<<endl; 
                        
                        ofstream file_keyteste6(salvar_dadoss6,ios::app);
                        file_keyteste6 << metodo6.pontos_1() <<";"<< metodo6.tempo_1() <<";"<<metodo6.pontos_2()<<";"<< metodo6.tempo_2()<<";"<< metodo6.pontos_casamento()<<";"<< metodo6.pontos_casamento_total()<<";"<< metodo6.tempo__casamento();
                        if (metodo6.pontos_casamento()==0)
                            file_keyteste6<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste6<<";"<< metodo6a.distancia()<<endl; 
                        
                        ofstream file_keyteste7(salvar_dadoss7,ios::app);
                        file_keyteste7 << metodo7.pontos_1() <<";"<< metodo7.tempo_1() <<";"<<metodo7.pontos_2()<<";"<< metodo7.tempo_2()<<";"<< metodo7.pontos_casamento()<<";"<< metodo7.pontos_casamento_total()<<";"<< metodo7.tempo__casamento();
                        if (metodo7.pontos_casamento()==0)
                            file_keyteste7<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste7<<";"<< metodo7a.distancia()<<endl; 
                        
                        ofstream file_keyteste8(salvar_dadoss8,ios::app);
                        file_keyteste8 << metodo8.pontos_1() <<";"<< metodo8.tempo_1() <<";"<<metodo8.pontos_2()<<";"<< metodo8.tempo_2()<<";"<< metodo8.pontos_casamento()<<";"<< metodo8.pontos_casamento_total()<<";"<< metodo8.tempo__casamento();
                        if (metodo8.pontos_casamento()==0)
                            file_keyteste8<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste8<<";"<< metodo8a.distancia()<<endl; 
                        
                        ofstream file_keyteste9(salvar_dadoss9,ios::app);
                        file_keyteste9 << metodo9.pontos_1() <<";"<< metodo9.tempo_1() <<";"<<metodo9.pontos_2()<<";"<< metodo9.tempo_2()<<";"<< metodo9.pontos_casamento()<<";"<< metodo9.pontos_casamento_total()<<";"<< metodo9.tempo__casamento();
                        if (metodo9.pontos_casamento()==0)
                            file_keyteste9<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste9<<";"<< metodo9a.distancia()<<endl;  
                        
                        ofstream file_keyteste10(salvar_dadoss10,ios::app);
                        file_keyteste10 << metodo10.pontos_1() <<";"<< metodo10.tempo_1() <<";"<<metodo10.pontos_2()<<";"<< metodo10.tempo_2()<<";"<< metodo10.pontos_casamento()<<";"<< metodo10.pontos_casamento_total()<<";"<< metodo10.tempo__casamento();
                        if (metodo10.pontos_casamento()==0)
                            file_keyteste10<<";"<< "Nao"<<endl; 
                        else
                            file_keyteste10<<";"<< metodo10a.distancia()<<endl; 
                    
                        imagem_busqueda.~Mat();
                        //waitKey(0);       
                        }
                    }
                }
                
                        ofstream file_keyteste1(salvar_dadoss1,ios::app);
                        file_keyteste1 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste2(salvar_dadoss2,ios::app);
                        file_keyteste2 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste3(salvar_dadoss3,ios::app);
                        file_keyteste3 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste4(salvar_dadoss4,ios::app);
                        file_keyteste4 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste5(salvar_dadoss5,ios::app);
                        file_keyteste5 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste6(salvar_dadoss6,ios::app);
                        file_keyteste6 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste7(salvar_dadoss7,ios::app);
                        file_keyteste7 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste8(salvar_dadoss8,ios::app);
                        file_keyteste8 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste9(salvar_dadoss9,ios::app);
                        file_keyteste9 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        ofstream file_keyteste10(salvar_dadoss10,ios::app);
                        file_keyteste10 << ";"<< ";"<<";"<< ";"<< ";"<< ";"<<endl; 
                        
                        
            }
            imagem_mosaico.~Mat();
        }
    }
    
    //waitKey(0);
    return EXIT_SUCCESS;
}
