//Neerajan Nepal (nepal1n@cmich.edu)
//date:- July 29, 2020
//this code converts the mo03 data to text file,
//which can be converted to xml input format
//of wn code


//NOTE: (1)SOME REACTION HAVE ZERO BRANCHING RATIO
//We included them too, assuming braching ratio 1e-10

//(2) For example Ne33, there are reactions in
//jina reaclib other than beta decay, in chapter 2 (or 3)
//will these affect branching ratio or rate if we use only this 
//dat file?

#include <fstream>
#include <string>
#include <iostream>

void mo03_reactions(){

  Double_t z, n, t12, p0, p1, p2, p3;
  Int_t m = 6000; 
  Double_t Z[m], N[m], T12[m], P0[m], P1[m], P2[m], P3[m];

  ifstream infile;
  infile.open("mo03_beta_minus.dat");

  if(!infile){
    cout<<"unable to open file"<<endl;
    exit(1);
  }
  

  Int_t i = 0;
  while(!infile.eof()){
    infile>>z>>n>>t12>>p0>>p1>>p2>>p3;
    Z[i] = z;
    N[i] = n;
    T12[i] = t12;
    P0[i] = p0;
    P1[i] = p1;
    P2[i] = p2;
    P3[i] = p3;
    i++;
  }
  cout<<i-1<<"is total number of lines"<<endl;
  Int_t lines = i-1;
  //load elements symbol
  Double_t el_Z[113] = {0};
  std::string el_symbol[113] = {""};

  ifstream ele_file;
  ele_file.open("elements.txt");
  if(!ele_file){
    cout<<"unable to open elements text file"<<endl;
    exit(1);
  }

  string str1, str2;
  Double_t atnum;
  for(int i=0; i<113; i++){
    ele_file>>str1>>str2>>atnum;
    el_symbol[i] = str2;
    el_Z[i] = atnum;
  }

  //output
  ofstream myfile;
  myfile.open("mo03_reactions.txt");

  for(Int_t j=0; j<lines; j++){
    if(P0[j]>0.0000){ //x.xxxx format in dat file
      Int_t at_num = Int_t ( Z[j] ) -1 ; //at number starts from 0 
      Int_t at_num_dau = at_num+1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass;
      myfile<<"single_rate"<<endl;
      myfile<<"mo03_cmu"<<endl;
      myfile<<"1"<<endl;
      myfile<<el_symbol[at_num]<<at_mass<<endl;
      myfile<<"3"<<endl;
      myfile<<el_symbol[at_num_dau]<<at_mass<<endl;
      myfile<<"electron"<<endl;
      myfile<<"anti-neutrino_e"<<endl;
      //FIND OUT WHAT EXACTLY IS RATE IN INPUT FILE?
      //Ans:- in jina reaclib, cofficients goes here
      //but in wn xml, rate = e^coffi. (coffi a_o from jina_reaclib)
      //or rate_p0 = (log(2)*p0/half-life), p0 is branching ratio
      Double_t rate = log(2) * P0[j]/T12[j];
      myfile<<rate<<"\n"<<endl;
    }
    if(P0[j]==0.0000){
      Int_t at_num = Int_t( Z[j] )- 1;
      Int_t at_num_dau = at_num + 1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass;
      Double_t rate = log(2)*0.0000000001/T12[j];
      myfile<<"single_rate \nmo03_cmu \n1 \n"<<el_symbol[at_num]<<at_mass<<"\n3\n";
      myfile<<el_symbol[at_num_dau]<<at_mass<<"\nelectron \nanti-neutrino_e\n"<<rate<<"\n\n";

      //      cout<<Z[j]<<"\t is at number "<<j<<endl;
    }
    


    if(P1[j]>0.000001){
      Int_t at_num = Int_t ( Z[j] ) -1 ; //at number starts from 0 
      Int_t at_num_dau = at_num+1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass - 1;
      myfile<<"single_rate"<<endl;
      myfile<<"mo03_cmu"<<endl;
      myfile<<"1"<<endl;
      myfile<<el_symbol[at_num]<<at_mass<<endl;
      myfile<<"4"<<endl;
      myfile<<"n"<<endl;
      myfile<<el_symbol[at_num_dau]<<at_mass<<endl;
      myfile<<"electron"<<endl;
      myfile<<"anti-neutrino_e"<<endl;
      Double_t rate = log(2) * P1[j]/T12[j];
      myfile<<rate<<"\n"<<endl;
    }
    if(P1[j]==0.0000){
      Int_t at_num = Int_t( Z[j] )- 1;
      Int_t at_num_dau = at_num + 1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass-1;
      Double_t rate = log(2)*0.0000000001/T12[j];
      myfile<<"single_rate \nmo03_cmu \n1 \n"<<el_symbol[at_num]<<at_mass<<"\n4 \nn \n";
      myfile<<el_symbol[at_num_dau]<<at_mass<<"\nelectron \nanti-neutrino_e\n"<<rate<<"\n\n";
    }


    if(P2[j]>0.000001){
      Int_t at_num = Int_t ( Z[j] ) -1 ; //at number starts from 0 
      Int_t at_num_dau = at_num+1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass - 2;
      myfile<<"single_rate"<<endl;
      myfile<<"mo03_cmu"<<endl;
      myfile<<"1"<<endl;
      myfile<<el_symbol[at_num]<<at_mass<<endl;
      myfile<<"5"<<endl;
      myfile<<"n"<<endl;
      myfile<<"n"<<endl;
      myfile<<el_symbol[at_num_dau]<<at_mass<<endl;
      myfile<<"electron"<<endl;
      myfile<<"anti-neutrino_e"<<endl;
      Double_t rate = log(2) * P2[j]/T12[j];
      myfile<<rate<<"\n"<<endl;
   }
    if(P2[j]==0.0000){
      Int_t at_num = Int_t( Z[j] )- 1;
      Int_t at_num_dau = at_num + 1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass-2;
      Double_t rate = log(2)*0.0000000001/T12[j];
      myfile<<"single_rate \nmo03_cmu \n1 \n"<<el_symbol[at_num]<<at_mass<<"\n5 \nn \nn \n";
      myfile<<el_symbol[at_num_dau]<<at_mass<<"\nelectron \nanti-neutrino_e\n"<<rate<<"\n\n";
    }


    if(P3[j]>0.000001){
      Int_t at_num = Int_t ( Z[j] ) -1 ; //at number starts from 0 
      Int_t at_num_dau = at_num+1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass - 3;
      myfile<<"single_rate"<<endl;
      myfile<<"mo03_cmu"<<endl;
      myfile<<"1"<<endl;
      myfile<<el_symbol[at_num]<<at_mass<<endl;
      myfile<<"6"<<endl;
      myfile<<"n"<<endl;
      myfile<<"n"<<endl;
      myfile<<"n"<<endl;
      myfile<<el_symbol[at_num_dau]<<at_mass<<endl;
      myfile<<"electron"<<endl;
      myfile<<"anti-neutrino_e"<<endl;
      Double_t rate = log(2) * P3[j]/T12[j];
      myfile<<rate<<"\n"<<endl;
    }
    if(P3[j]==0.0000){
      Int_t at_num = Int_t( Z[j] )- 1;
      Int_t at_num_dau = at_num + 1;
      Double_t at_mass = Z[j]+N[j];
      Double_t at_mass_dau = at_mass-3;
      Double_t rate = log(2)*0.0000000001/T12[j];
      myfile<<"single_rate \nmo03_cmu \n1 \n"<<el_symbol[at_num]<<at_mass<<"\n6 \nn \nn \nn\n";
      myfile<<el_symbol[at_num_dau]<<at_mass<<"\nelectron \nanti-neutrino_e\n"<<rate<<"\n\n";
    }

  }

  myfile.close();

}
