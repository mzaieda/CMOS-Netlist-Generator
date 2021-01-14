
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
using namespace std; 

int n_p_mos_counter = 1;
int wires_counter = 1;



void generate(string valid_expression)
{
	ofstream NetList; 
	NetList.open("netlist.txt", ios::out);
	int location = 0; 
	location = valid_expression.find('=');
	
	NetList << valid_expression << '\n';

	// should detect the precednce 
	for(int i = location + 1; i<valid_expression.length(); i++)
	{
		if(valid_expression[i+1] == '`')
		{
			if(isalpha(valid_expression[i]))
			{
				//nmos first
				//id 
				NetList << "M" << n_p_mos_counter<< " "; 
				//drain
				NetList << valid_expression[0] << " "; 
				//gate
				NetList << valid_expression[i]<< " "; 
				//source and body
				NetList << "vdd" << " vdd";
				//type 
				NetList << " PMOS" << '\n';

				n_p_mos_counter++;

				NetList << "M" << n_p_mos_counter<< " "; 
				//drain
				NetList << valid_expression[0] << " "; 
				//gate
				NetList << valid_expression[i]<< " "; 
				//source and body
				NetList << "0" << " 0";
				//type 
				NetList << " NMOS" << '\n';

				n_p_mos_counter++;
			}
		}
		for (int i = location + 1; i < valid_expression.length(); i++){

		}
		if(valid_expression[i] == '&'){
			char a = valid_expression[i-1];
			char b = valid_expression[i+1];
			//M1
			NetList << "M" << n_p_mos_counter<< " "; 
				//drain
			NetList << "W" << wires_counter << " ";
				//gate

			NetList << a << " "; 
				//source and body
			NetList << "0" << " 0";
				//type 
			NetList << " NMOS" << '\n';

			n_p_mos_counter++;

			int common_wire1 = wires_counter;   //common wire

			wires_counter++;
			

			//M2
			NetList << "M" << n_p_mos_counter<< " "; 
				//drain
			NetList << "W" << wires_counter << " ";
				//gate

			NetList << b << " "; 
				//source and body
			NetList << "W" << common_wire1 << " "<< "W" <<  common_wire1;
				//type 
			NetList << " NMOS" << '\n';

			n_p_mos_counter++;

			int common_wire2 = wires_counter;

			wires_counter++;


			//M3
			NetList << "M" << n_p_mos_counter<< " "; 
				//drain
			NetList << "W" << common_wire2 << " "; 
				//gate
			NetList << b << " "; 
				//source and body
			NetList << "vdd" << " vdd ";
				//type 
			NetList << "PMOS" << '\n';

			n_p_mos_counter++;
		
			wires_counter++;



			//M4
			NetList << "M" << n_p_mos_counter<< " "; 		
				//drain
			NetList << "W" << common_wire2 << " "; 
				//gate
			NetList << a << " "; 
				//source and body
			NetList << "vdd" << " vdd ";
				//type 
			NetList << "PMOS" << '\n';

			//call not gate	
		}
	} for (int i = location + 1; i < valid_expression.length(); i++) {
		if(valid_expression[i] == '|'){
			char a = valid_expression[i-1];
			char b = valid_expression[i+1];

			//M1
			NetList << "M" << n_p_mos_counter<< " "; 
				//drain
			NetList << "W" << wires_counter << " ";
				//gate
			NetList << a << " "; 
				//source and body
			NetList << "0" << " 0";
				//type 
			NetList << " NMOS" << '\n';

			n_p_mos_counter++;

			int common_wire1 = wires_counter;   //common wire

			wires_counter++;
			

			//M2
			NetList << "M" << n_p_mos_counter<< " "; 
				//drain
			NetList << "W" << common_wire1 << " ";
				//gate
			NetList << b << " "; 
				//source and body
			NetList << "0" << " 0";
				//type 
			NetList << " NMOS" << '\n';

			n_p_mos_counter++;

			int common_wire2 = wires_counter;

			wires_counter++;


			//M3
			NetList << "M" << n_p_mos_counter<< " "; 
				//drain
			NetList << "W" << common_wire1 << " "; 
				//gate
			NetList << a << " "; 
				//source and body
			NetList << "W" << common_wire2 << " W" << common_wire2;
				//type 
			NetList << " PMOS" << '\n';

			n_p_mos_counter++;
		
			wires_counter++;



			//M4
			NetList << "M" << n_p_mos_counter<< " "; 		
				//drain
			NetList << "W" << common_wire2 << " "; 
				//gate
			NetList << b << " "; 
				//source and body
			NetList << "vdd" << " vdd ";
				//type 
			NetList << "PMOS" << '\n';

			//call inverter

		}
	}
		
	
	
	NetList.close();	
	return;
}


bool valid(string expression)
{
	char out = expression[0];
	int location = 0; 
	location = expression.find('=');

	for(int i = location+1; i<expression.length(); i++)
	{
		if(expression[i] == out)
		{
			return false;
		}
		
		if(expression[i] != '|' && expression[i] != '&' && expression[i] != '`' && expression[i]!=' ')
		{
			if(!isalpha(expression[i]))
			{		
				return false; 
			}
		}
	}
	
	return true;
}


void handler()
{
	string boolean_expression;
	cout<<"\nPlease, enter your boolean expression: ";
	getline(cin, boolean_expression);
	
	if( valid(boolean_expression) )
	{
		cout << boolean_expression<<endl; 
		generate(boolean_expression);
		return; 
	}
	
	else 
	{
		cout<<"\ninvalid input."<<endl;
		handler();
	}
}



int main()
{
	
	cout<<"\nHello, this program generates NetSpice code for your boolean function."<<endl;
	
 	handler();
	
	
	return 0;
}

