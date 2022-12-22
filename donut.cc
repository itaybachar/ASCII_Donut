#include <cstdio>
#include <cmath>
#include <string.h>

const int SCREEN_HEIGHT = 30;
const int SCREEN_WIDTH = 100;

char f_buff[SCREEN_HEIGHT][SCREEN_WIDTH];
float z_buff[SCREEN_HEIGHT][SCREEN_WIDTH];

float d_theta = 0.07;
float d_phi = 0.02;

int R1 = 1; //Radius of donut circle
int R2 = 2; //Radius of whole torus

int K2 = 5; //Distance of torus from screen(into 3d space)
int K1 = SCREEN_HEIGHT*K2*2/(8*(R1+R2)); //Arbitrary distance of eye from screen

//renders a frame of the donut with rotation A,B
void render_frame(float A,float B) {

  //Reset frame and z buffers
  memset(f_buff,' ', SCREEN_HEIGHT*SCREEN_WIDTH* sizeof(char));
  memset(z_buff,0,SCREEN_HEIGHT*SCREEN_WIDTH* sizeof(int));


  float cosA = cos(A), sinA = sin(A);
  float cosB = cos(B), sinB  = sin(B);

  //Loop over all theta(angle to create circle)
  for(float theta = 0; theta <2*M_PI;theta+=d_theta) {
    float costheta = cos(theta), sintheta = sin(theta);
    for(float phi = 0; phi < 2*M_PI;phi +=d_phi) {
      float cosphi = cos(phi), sinphi = sin(phi);

      float circle_x = R2 + R1*costheta;
      float circle_y = R1*sintheta;

      float x = sinB*(circle_y*cosA + sinA*sinphi*circle_x) + cosB*cosphi*circle_x;
      float y = cosB*(circle_y*cosA + sinA*sinphi*circle_x) - sinB*cosphi*circle_x;
      float z = K2 + cosA*sinphi*circle_x - circle_y*sinA;
      float z_inv = 1/z;

      //Finally get projected coordinates from middle of screen
      int x_proj = (int)(SCREEN_WIDTH/2 + K1*x*z_inv);
      int y_proj = (int)(SCREEN_HEIGHT/2 - K1*y*z_inv);


      //For this I am lighting the scene from (0,-1,-1)
      //We multiply the torus x,y,z calculation(with no tranlation this time by this matrix.
      float L = -cosA*costheta*sinphi + sinA*sintheta - cosB*cosA*sintheta - cosB*sinA*costheta*sinphi + sinB*costheta*cosphi;

      //If L is more than 0, then the object is facing us/light
      if(L > 0) {
        if(x_proj >= 0 && x_proj < SCREEN_WIDTH && y_proj >= 0 && y_proj < SCREEN_HEIGHT)
          if(z_inv > z_buff[y_proj][x_proj]) {
            int lumIndex = L * 8; //L is (0->sqrt(2))*8 -> 0-> ~11)

            z_buff[y_proj][x_proj] = z_inv;
            f_buff[y_proj][x_proj] = ".,-~:;=!*#$@"[lumIndex%11];
          }
      }
    }
  }

  //Loop over all phi(angle to rotate the circle around the y axis, creating a donut)

  //Calculate x,y for circle
  //Calculate final 3d x,y,z from matrix math
  //calculate projected x,y

  //Find luminace of the the pixel
  //If lit up,
  //Update the z buffer and framebuffer if it is closer to the camera.(1/z is larger)

  //Set the corresponding ascii character based on light

  printf("\x1b[H");
  for(int i = 0; i<SCREEN_HEIGHT; i++) {
    for(int j = 0; j<SCREEN_WIDTH; j++) {
      putchar(f_buff[i][j]);
    }
    putchar('\n');
  }
}

int main(){
  //Clear current screen
  printf("\033[2J");

  //Render loop
  float A = 990;
  float B = 0;
  for(;;){
    //Render the frame
    render_frame(A,B);
    A +=0.1;
    B += 0.05;
  }

  return 0;
}

