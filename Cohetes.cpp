#include <iostream>
#include <math.h>
#include <stdio.h>
//Prototipos y constantes
double E(double E0, double f);
double Fa(double y, double vy,double CD, double Ar);
double mc(double t, double tsfc, double E0, double mf0, double m0);
double g(double y);
double mf(double t, double tsfc, double E0, double mf0);
double rho(double y);
double acY(double E0, double tsfc, double CD, double Ar, double m0, double mf0, double y, double vy, double t, double f);
void guardar();

const double G = 6.693*pow(10, -11);
const double Rt = 6.378*pow(10, 6);
const double Mt = 5.9736*pow(10, 24);
const double R = 287.06;
const double To = 288.15;
const double Po = 101325; 
const double g0 = 9.81;
const double L = 0.0065;

static int numCohetes;

//clase Cohete
class Cohete
{
public:
    double E0;
    double tsfc;
    double CD;
    double Ar;
    double m0;
    double mf0;
    double y;
    double vy;
    void despegar();

    Cohete(double, double, double, double, double, double, double, double);

};
//constructor Cohete
Cohete::Cohete(double E0, double tsfc, double CD, double Ar, double m0, double mf0, double y, double vy)
{
    this->E0 = E0;
    this->tsfc = tsfc;
    this->CD = CD;
    this->Ar = Ar;
    this->m0 = m0;
    this->mf0 = mf0;
    this->y = y;
    this->vy = vy;
    numCohetes++;
}
//despegue de Cohete
void Cohete::despegar(){
    float delta = 0.25;
    y = 0;
    vy = 0;
    FILE * pf;
    FILE * pf2;
    float ymax = 0;
    float i = 0;
    // Esta parte es un asco pero no se como hacerlo de otra manera
    if(numCohetes==1){
        pf = fopen("posicion1.txt", "w");
    }
    if(numCohetes==2){
        pf = fopen("pisicion2.txt", "w");
    }if(numCohetes==3){
        pf = fopen("posicion3.txt", "w");
    }
    
    if(numCohetes==1){
        pf2 = fopen("velocidad1.txt", "w");
    }
    if(numCohetes==2){
        pf2 = fopen("velocidad2.txt", "w");
    }if(numCohetes==3){
        pf2 = fopen("velocidad3.txt", "w");
    }
    while(i < 1440 && y >= 0){
    if(y>ymax)ymax=y;
    //bool f = is0(mf(delta*i,tsfc,E0,mf0));
    fprintf(pf2,"%lf\t%lf\n",delta*i,vy);
    fprintf(pf,"%lf\t%lf\n",delta*i, y );
    vy += delta*acY(E0,tsfc,CD,Ar,m0,mf0,y,vy,delta*i, mf(delta*i,tsfc,E0,mf0) );
    y += delta*vy;
  //  printf("y(%.3f):%lf\ty'(%.3f):%lf\tmf:%lf\n",delta*i,y,delta*i,vy,mc(  delta*i ,   tsfc,   E0,   mf0,   m0) );
   // printf("Fa:%lf\t",Fa(y,vy,CD,Ar));
    
    i++;
        }
    fclose(pf);
    fclose(pf2);
    printf("Altura maxima de cohete %d: %lf\n",numCohetes,ymax);
}



int main(){
    
    Cohete coh1(3*pow(10,7),3.248*pow(10,-4),61.27 ,201.06 , 1.1*pow(10,5),1.5*pow(10,6),0,0);
    coh1.despegar();
  //  guardar();
    
    
    
    

    Cohete coh2(2.7*pow(10,7),2.248*pow(10,-4),61.27,201.06,1.1*pow(10,5),1.5*pow(10,6),0,0);
    coh2.despegar();
 //   guardar();
    
    
    Cohete coh3(2.5*pow(10,7), 2.248*pow(10,-4),70.25,215,1.8*pow(10,5),2.1*pow(10,6),0,0);
    coh3.despegar();
  //  guardar();
    system("gnuplot 'datos.gp'");
    system("gnuplot 'datos2.gp'");
    system("rm posicion1.txt pisicion2.txt posicion3.txt velocidad1.txt velocidad2.txt velocidad3.txt ");
    
    // system("rm datos.gp");
    // system("rm datos.txt");
    
    
}



//Definir funciones
double E(double E0,double mf){
    if(mf == 0){
        return 0; 
    }
    else{
        return E0;
    }
}

double rho(double y){
    return ((Po/(R*To)*pow( 1 - (L*y)/To , g0/(R*L) )));
}

double Fa(double y, double vy,double CD, double Ar){
    return 0.5*rho(y)*CD*Ar*vy*vy;
}

double g(double y){
    return ((G*Mt)/((Rt + y)*(Rt + y)));
}

double mf(double t, double tsfc, double E0, double mf0){
    double res = (mf0 - tsfc*E0*t);
    if(res < 0){
        return 0;
    }
    else{
        return res;
    }
}

double mc(double t, double tsfc, double E0, double mf0, double m0){
    return (m0 + mf(t, tsfc, E0, mf0));
}

double acY(double E0, double tsfc, double CD, double Ar, double m0, double mf0, double y, double vy, double t, double f){
    return ( E(E0, f)/mc(t,tsfc,E0,mf0,m0) - Fa(y,vy,CD,Ar)/mc(t,tsfc,E0,mf0,m0) - g(y));
}

void guardar(){
    FILE * pf;
    pf = fopen("datos.gp", "w");
    fprintf(pf,"plot 'datos%d.txt' w l\nset terminal png\nset output 'grafica%d.png'",numCohetes,numCohetes);
    fclose(pf);
}
