#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>
using namespace std;



int n_p_mos_counter = 1;
int wires_counter = 1;



void handler();
bool valid(string expression);
void generate (string valid_expression);
string NOT(string valid_expression, int location, string input_wire, string& output_wire);
string AND(string valid_expression, int location, string input_wire1, string input_wire2, string& output_wire);

int main()
{
    
    cout<<"\nHello, this program generates NetSpice code for your boolean function."<<endl;

    handler();
    
    return 0;
}

void handler(){
    string boolean_expression;
    cout<<"\nPlease, enter your boolean expression: ";
    getline(cin, boolean_expression);
   
    if( valid(boolean_expression) )
    {
        cout <<"You entred: " << boolean_expression << endl;
        generate(boolean_expression);
        return;
    }
   
    else
    {
        cout<<"\ninvalid input."<<endl;
        handler();
    }
}

bool valid(string expression){
    
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

string NOT(string valid_expression, int location, string input_wire, string & output_wire)
{
    string not_implementation; 
    
    not_implementation = "M"; // + n_p_mos_counter + " ";
    not_implementation += to_string(n_p_mos_counter); 
    not_implementation += " ";
    //drain
    not_implementation += "W";
    not_implementation += to_string (wires_counter);
    not_implementation += " ";
    //gate
    not_implementation += input_wire;
    not_implementation += " ";
    //source and body
    not_implementation += "vdd vdd";
    //type
    not_implementation +=" PMOS \n";
 
    n_p_mos_counter++;
 
    not_implementation += "M"; 
    not_implementation += to_string(n_p_mos_counter);
    not_implementation += " ";
    //drain
    not_implementation += "W";
    not_implementation += to_string (wires_counter);
    output_wire = "W";
    output_wire += to_string(wires_counter);

    wires_counter++;

   
    
    not_implementation += " ";
    //gate
    not_implementation += input_wire;
    not_implementation += " ";
    //source and body
    not_implementation += " 0   0 ";
    //type
    not_implementation += " NMOS \n";
 
    n_p_mos_counter++;

    return not_implementation; 
}

string AND(string valid_expression, int location, string input_wire1, string input_wire2, string& output_wire)
{
     
    //M1
    string and_implementation; 

    and_implementation = "M"; 
    and_implementation += to_string(n_p_mos_counter); 
        //drain

    and_implementation += " W"; 
    and_implementation += to_string(wires_counter); 
    and_implementation += " ";
        //gate
 
    and_implementation += input_wire1;
        //source and body
    and_implementation += " 0   0 ";

        //type
    and_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire1 = wires_counter;   //common wire
 
    wires_counter++;
   
 
    //M2
    //drain
   
    and_implementation += "M"; 
    and_implementation += to_string(n_p_mos_counter); 
        //drain

    and_implementation += " W"; 
    and_implementation += to_string(wires_counter); 
    and_implementation += " ";
 
    //gate
    and_implementation += input_wire2;

        //source and body
    and_implementation += " W";
    and_implementation += to_string (common_wire1);
    and_implementation += " W";
    and_implementation += to_string (common_wire1);
        //type
    and_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire2 = wires_counter;
    string to_not = "W"; 
    to_not += to_string(common_wire2);
 
    wires_counter++;
 
 
    //M3
    and_implementation += "M";
    and_implementation += to_string (n_p_mos_counter);
    and_implementation += " W";
    and_implementation += to_string (common_wire2);
    and_implementation += " ";
        //gate
    and_implementation += input_wire2;
    and_implementation += " vdd vdd ";
        //type
    and_implementation += "PMOS \n";
 
    n_p_mos_counter++;
 
    wires_counter++;
 
 
 
    //M4
    and_implementation += "M";
    and_implementation += to_string(n_p_mos_counter);      
    n_p_mos_counter++;
        //drain
    and_implementation += " W";
    and_implementation += to_string (common_wire2);
    and_implementation += " ";
        //gate
    and_implementation += input_wire1;
        //source and body
    and_implementation += " vdd vdd ";
        //type
    and_implementation += "PMOS \n";
 
    //call not gate
    //string valid_expression, int location, int wires_counter, int i

    string final_output; 
    wires_counter--;
    and_implementation += NOT(valid_expression, location, to_not, final_output);
    
    output_wire = final_output; 
   // cout << and_implementation;
   // replace(and_implementation.begin(), and_implementation.end(), '_', ' ');
    return and_implementation;

}


string OR(string valid_expression, int location, string input_wire1, string input_wire2, string& output_wire)
{
     
    //M1
    string or_implementation; 

    or_implementation = "M"; 
    or_implementation += to_string(n_p_mos_counter); 
        //drain

    or_implementation += " W"; 
    or_implementation += to_string(wires_counter); 
    or_implementation += " ";
        //gate
 
    or_implementation += input_wire1;
        //source or body
    or_implementation += " 0   0 ";

        //type
    or_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire1 = wires_counter;   //common wire
 
    wires_counter++;
   
 
    //M2
    //drain
   
    or_implementation += "M"; 
    or_implementation += to_string(n_p_mos_counter); 
        //drain

    or_implementation += " W"; 
    or_implementation += to_string(common_wire1); 
    or_implementation += " ";
 
    //gate
    or_implementation += input_wire2;
	or_implementation += " ";

        //source or body
    or_implementation += " 0   0 ";
        //type
    or_implementation += " NMOS \n";
 
    n_p_mos_counter++;
 
    int common_wire2 = wires_counter;
    string to_not = "W"; 
    to_not += to_string(common_wire1);
 
    wires_counter++;
 
 
    //M3
    or_implementation += "M";
    or_implementation += to_string (n_p_mos_counter);
    //drain
    or_implementation += " W";
    or_implementation += to_string (common_wire1);
    or_implementation += " ";
        //gate
    or_implementation += input_wire2;
    or_implementation += " ";

    //source
    or_implementation += "W";
	or_implementation += to_string(common_wire2);
	or_implementation += " W";
	or_implementation += to_string(common_wire2);

        //type
    or_implementation += " ";
    or_implementation += " PMOS \n";
 
    n_p_mos_counter++;
 
    wires_counter++;
 
 
 
    //M4
    or_implementation += "M";
    or_implementation += to_string(n_p_mos_counter);      
    n_p_mos_counter++;
        //drain
    or_implementation += " W";
    or_implementation += to_string (common_wire2);
    or_implementation += " ";
        //gate
    or_implementation += input_wire1;
    or_implementation += " ";
        //source or body
    or_implementation += "vdd vdd";
        //type
    or_implementation += " PMOS \n";
 
    //call not gate
    //string valid_expression, int location, int wires_counter, int i

    string final_output; 
    wires_counter--; 
    or_implementation += NOT(valid_expression, location, to_not, final_output);

    output_wire = final_output;

    replace(or_implementation.begin(), or_implementation.end(), '_', ' ');
    return or_implementation;

}



void generate(string valid_expression)
{
    ofstream NetList;
    NetList.open("netlist.txt", ios::out);
    int location = 0;
    location = valid_expression.find('=');
   
    NetList << valid_expression << '\n';
    string output_wire_name;

    //then we want to generate the not gate first. therefore, I loop over the string, if I found a not operation, we perform it 
    int expression_length = valid_expression.length();
	
    for (int i = location + 1; i < expression_length; i++)
    {
         if(valid_expression[i+1] == '`')       // if the next character is a not operation, we extract the current one we are wokring with
        {
            if(isalpha(valid_expression[i]))
            {
                //call NOT function
                string a;
                a = valid_expression[i];
                
                output_wire_name.clear();

                NetList << NOT(valid_expression, location, a, output_wire_name);
                valid_expression [i+1] = ' ';
                if(wires_counter > 10)
                {
                valid_expression [i] = output_wire_name[0];
                valid_expression [i+1] = output_wire_name[1];
                valid_expression.insert (i+1, 1, output_wire_name[2]);
                }
                else 
                {
                    valid_expression [i] = output_wire_name[0];
                    valid_expression [i+1] = '0';
                    valid_expression.insert (i+2, 1, output_wire_name[1]);
                }

           
            }
        }
    }

    // should replace not variables with their output wire names 

char *Arr = new char[100];
     for(int i = location + 1; i<expression_length; i++)
    {
        if(valid_expression[i+1] == '&')
        {
	
		/*
			NEW PART : REPLACE EVERY CHAR WITH THREE CHARS
		*/
		int my_length = valid_expression.length();
		valid_expression.insert(my_length, 1, '$');
		
		int counter = 0;
		for (int i = location + 1; i <= my_length; i++){
			if (valid_expression[i] != '&' && valid_expression[i] != '|' && valid_expression[i] != '$'){
				counter++;
			} else {
				if (counter == 3){
					counter = 0;
					continue;
				} else {
				valid_expression.insert(i - counter, 2, '_');
				i += 2 ;
				my_length += 2;
				counter = 0;
				}
			}
			
		}

		cout << "New Expression: "<< valid_expression << endl;
            
                //call AND function
                // we need a parser or a function that makes each argument or a wire 3 characters 
		        cout <<"i" << i<<endl;
                string a;
                a = valid_expression.substr(i, 3);
                string b;
                b = valid_expression.substr(i+4,3);
                cout<< b << endl; 
                output_wire_name.clear();
                NetList << AND(valid_expression, location, a, b, output_wire_name);
                valid_expression [i+1] = ' ';
                if(wires_counter > 10)
                {
                valid_expression [i] = output_wire_name[0];
                valid_expression [i+1] = output_wire_name[1];
                valid_expression.insert (i+1, 1, output_wire_name[2]);
                }
                else 
                {
                    valid_expression [i] = output_wire_name[0];
                    valid_expression [i+1] = '0';
                    valid_expression.insert (i+2, 1, output_wire_name[1]);
                }
       
            
        }

        if(valid_expression[i+1] == '|'){
            int my_length = valid_expression.length();
		    valid_expression.insert(my_length, 1, '$');
		
		    int counter = 0;
		    for (int i = location + 1; i <= my_length; i++){
			    if (valid_expression[i] != '&' && valid_expression[i] != '|' && valid_expression[i] != '$'){
				    counter++;
			    } else {
				    if (counter == 3){
				    	counter = 0;
				    	continue;
				    } else {
				    valid_expression.insert(i - counter, 2, '_');
				    i += 2 ;
				    my_length += 2;
				    counter = 0;
                    }
			    }   
            }

            cout << "New Expression: "<< valid_expression << endl;
            
                //call OR function
                // we need a parser or a function that makes each argument or a wire 3 characters 
		        cout <<"i" << i<<endl;
                string a;
                a = valid_expression.substr(i, 3);
                string b;
                b = valid_expression.substr(i+4,3);
                cout<< b << endl; 
                output_wire_name.clear();
                NetList << OR(valid_expression, location, a, b, output_wire_name);
                valid_expression [i+1] = ' ';
                if(wires_counter > 10)
                {
                valid_expression [i] = output_wire_name[0];
                valid_expression [i+1] = output_wire_name[1];
                valid_expression.insert (i+1, 1, output_wire_name[2]);
                }
                else 
                {
                    valid_expression [i] = output_wire_name[0];
                    valid_expression [i+1] = '0';
                    valid_expression.insert (i+2, 1, output_wire_name[1]);
                }
             }
        }

   


    NetList << "where " << valid_expression[0] << " = " << output_wire_name << endl; 
}
