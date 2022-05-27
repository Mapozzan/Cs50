#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float aux;
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            aux= round((image[i][j].rgbtBlue + image[i][j].rgbtRed + image[i][j].rgbtGreen)/3.000);
            
            image[i][j].rgbtBlue  = aux;
            image[i][j].rgbtRed   = aux;
            image[i][j].rgbtGreen = aux;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    int aux_color[3];

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width/2;j++)
        {
            aux_color[0] = image[i][j].rgbtBlue;   //aux_color[0] for blue
            aux_color[1] = image[i][j].rgbtRed;    //aux_color[1] for red
            aux_color[2] = image[i][j].rgbtGreen;  //aux_color[2] for green

            image[i][j].rgbtBlue  = image[i][width-j-1].rgbtBlue;
            image[i][j].rgbtRed   = image[i][width-j-1].rgbtRed;
            image[i][j].rgbtGreen = image[i][width-j-1].rgbtGreen;

            image[i][width-j-1].rgbtBlue  = aux_color[0];
            image[i][width-j-1].rgbtRed   = aux_color[1];
            image[i][width-j-1].rgbtGreen = aux_color[2];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    float    aux_blue,aux_red,aux_green,aux_count = 0;
    
    //Copy of the picture    
    RGBTRIPLE aux[height][width];
    

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            aux_blue = 0;    //Sum for all the blue in valid pixel
            aux_red = 0;     //Sum for all the red in valid pixel
            aux_green = 0;   //Sum for all the green in valid pixel
            aux_count = 0;   //Sum of how pixel is valid
            
            
            //function to sums values of the pixel if is valid, skips iteration if it goes outside.
            for(int m =-1;m<2;m++)
            {
                for(int n=-1;n<2;n++)
                {
                    if( (i+m >=0 && i+m<=(height-1)) && (j+n >=0 && j+n<=(width-1)) )
                    {
                        aux_blue  += image[i+m][j+n].rgbtBlue;
                        aux_red   += image[i+m][j+n].rgbtRed;
                        aux_green += image[i+m][j+n].rgbtGreen;
                        aux_count++;
                    }
                    else
                    {
                        continue;
                    }
                aux[i][j].rgbtBlue   =  round(aux_blue/aux_count);
                aux[i][j].rgbtRed    =  round(aux_red/aux_count);
                aux[i][j].rgbtGreen  =  round(aux_green/aux_count);
                }
            }
        }
    }
    
    //Transfer the blur effect of aux to the original picture 
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            image[i][j].rgbtBlue  = aux[i][j].rgbtBlue;   //aux_color[0] for blue
            image[i][j].rgbtRed   = aux[i][j].rgbtRed;    //aux_color[1] for red
            image[i][j].rgbtGreen = aux[i][j].rgbtGreen;  //aux_color[2] for green
        }
    }    
    
    return;
}


// function to limit the value of color
int limit(int x)
{
    if (x > 255)
    {
        x = 255;
    }
    return x;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    float X_blue,X_red,X_green;
    float Y_blue,Y_red,Y_green;
    int  matrix_X, matrix_Y;
    
    //Copy of the picture    
    RGBTRIPLE aux[height][width];
    

    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            X_blue = 0;    //Sum for all the blue in valid x pixel
            X_red = 0;     //Sum for all the red in valid x pixel
            X_green = 0;   //Sum for all the green in valid x pixel
            
            Y_blue = 0;    //Sum for all the blue in valid y pixel
            Y_red = 0;     //Sum for all the red in valid y pixel
            Y_green = 0;   //Sum for all the green in valid y pixel

            
            
            //function to sums values of the pixel if is valid, skips iteration if it goes outside.
            for(int m =-1;m<2;m++)
            {
                for(int n=-1;n<2;n++)
                {
                    if( (i+m >=0 && i+m<=(height-1)) && (j+n >=0 && j+n<=(width-1)) )
                    {
                        matrix_X = (n + 1 * n - n * abs(m));
                        matrix_Y = (m + 1 * m - m * abs(n));
                        
                        X_blue  += image[i+m][j+n].rgbtBlue*matrix_X ;
                        X_red   += image[i+m][j+n].rgbtRed*matrix_X ;
                        X_green += image[i+m][j+n].rgbtGreen*matrix_X ;
                        
                        Y_blue  += image[i+m][j+n].rgbtBlue*matrix_Y ;
                        Y_red   += image[i+m][j+n].rgbtRed*matrix_Y ;
                        Y_green += image[i+m][j+n].rgbtGreen*matrix_Y ;
                    }
                    else
                    {
                        continue;
                    }
                    aux[i][j].rgbtBlue   =  limit(round(sqrt(pow(X_blue,2)+pow(Y_blue,2))));
                    aux[i][j].rgbtRed    =  limit(round(sqrt(pow(X_red,2)+pow(Y_red,2))));
                    aux[i][j].rgbtGreen  =  limit(round(sqrt(pow(X_green,2)+pow(Y_green,2))));
                }    
            }
        }
    }
    
    //Transfer the blur effect of aux to the original picture 
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            image[i][j].rgbtBlue  = aux[i][j].rgbtBlue;   //aux_color[0] for blue
            image[i][j].rgbtRed   = aux[i][j].rgbtRed;    //aux_color[1] for red
            image[i][j].rgbtGreen = aux[i][j].rgbtGreen;  //aux_color[2] for green
        }
    }  
    
    return;
}
